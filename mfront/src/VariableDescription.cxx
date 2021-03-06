/*!
 * \file   mfront/src/VariableDescription.cxx
 * 
 * \brief    
 * \author Thomas Helfer
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>
#include"TFEL/Raise.hxx"
#include"TFEL/Glossary/Glossary.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"
#include"MFront/SupportedTypes.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/VariableDescription.hxx"

namespace mfront{

  const char *const VariableDescription::depth = "depth";
  const char *const VariableDescription::initialValue = "initialValue";
  const char *const VariableDescription::defaultValue = "defaultValue";
  const char *const
  VariableDescription::errorNormalisationFactor = "errorNormalisationFactor";
  
  VariableDescription::VariableDescription() = default;

  VariableDescription::VariableDescription(const std::string& t,
					   const std::string& n,
					   const unsigned short s,
					   const size_t l)
    : VariableDescriptionBase(t,n,s,l)
  {} // end of VariableDescription::VariableDescription

  VariableDescription::VariableDescription(const VariableDescription&) = default;
  VariableDescription::VariableDescription(VariableDescription&&) = default;
  VariableDescription&
  VariableDescription::operator=(VariableDescription&&) = default;
  VariableDescription&
  VariableDescription::operator=(const VariableDescription&) = default;

  SupportedTypes::TypeFlag VariableDescription::getTypeFlag() const {
    return SupportedTypes::getTypeFlag(this->type);
  }

  SupportedTypes::TypeSize VariableDescription::getTypeSize() const {
    return SupportedTypes::getTypeSize(this->type,this->arraySize);
  }

  void VariableDescription::setGlossaryName(const std::string& g) {
    using tfel::glossary::Glossary;
    auto throw_if = [](const bool b,const std::string& m){
      tfel::raise_if(b,"VariableDescription::setGlossaryName: "+m);
    };
    const auto& glossary = Glossary::getGlossary();
    throw_if(!glossary.contains(g),"'"+g+"' is not a glossary name");
    throw_if(this->hasGlossaryName(),"the glossary name has already been defined "
	     "for variable '"+this->name+"'");
    throw_if(this->hasEntryName(),"an entry name has already been defined "
	     "for variable '"+this->name+"'");
    this->glossaryName = glossary.getGlossaryEntry(g).getKey();
  } // end of VariableDescription::setGlossaryName

  void VariableDescription::setEntryName(const std::string& e) {
    using tfel::glossary::Glossary;
    auto throw_if = [](const bool b,const std::string& m){
      tfel::raise_if(b,"VariableDescription::setEntryName: "+m);
    };
    const auto& glossary = Glossary::getGlossary();
    throw_if(glossary.contains(e),"'"+e+"' is a glossary name");
    throw_if(this->hasGlossaryName(),"the glossary name has already been defined "
	     "for variable '"+this->name+"'");
    throw_if(this->hasEntryName(),"an entry name has already been defined "
	     "for variable '"+this->name+"'");
    this->entryName = e;    
  } // end of VariableDescription::setEntryName
  
  bool VariableDescription::hasGlossaryName() const{
    return this->glossaryName.is<std::string>();
  }

  bool VariableDescription::hasEntryName() const {
    return this->entryName.is<std::string>();
  }

  const std::string& VariableDescription::getExternalName() const {
    if(this->hasGlossaryName()){
      return this->glossaryName.get<std::string>();
    }
    if(this->hasEntryName()){
      return this->entryName.get<std::string>();
    }
    return this->name;
  } // end of VariableDescription::getExternalName

  void VariableDescription::throwUndefinedAttribute(const std::string& n) {
    tfel::raise("VariableDescription::getAttribute : "
		"no attribute named '"+n+"'");
  } // end of VariableDescription::throwUndefinedAttribute

  void VariableDescription::setAttribute(const std::string& n,
                                         const VariableAttribute& a,
                                         const bool b) {
    auto throw_if = [](const bool c, const std::string& m){
      tfel::raise_if(c,"VariableDescription::setAttribute: "+m);
    };    
    auto p=this->attributes.find(n);
    if(p!=this->attributes.end()){
      throw_if(a.getTypeIndex()!=p->second.getTypeIndex(),
	       "attribute already exists with a different type");
    }
    if(!this->attributes.insert({n,a}).second){
      throw_if(!b,"attribute '"+n+"' already declared");
    }
  } // end of VariableDescription::setAttribute

  bool VariableDescription::hasAttribute(const std::string& n) const {
    return this->attributes.count(n)!=0u;
  } // end of VariableDescription::hasAttribute

  const std::map<std::string, VariableAttribute>&
  VariableDescription::getAttributes() const {
    return this->attributes;
  } // end of VariableDescription::getAttributes

  static void checkBoundsCompatibility(const VariableDescription& v,
                                       const VariableBoundsDescription& b) {
    auto throw_if = [](const bool c,const std::string& m){
      tfel::raise_if(c,"mfront::checkBoundsCompatibility: "+m);
    };    
    SupportedTypes st;
    throw_if(!st.isSupportedType(v.type),
	     "can't assign bounds to parameter '"+v.name+"' (invalid type)");
    if((b.component!=-1)&&(b.component!=0)){
      const auto f = SupportedTypes::getTypeFlag(v.type);
      throw_if(f!=SupportedTypes::SCALAR,
	       "invalid component value for a scalar "
	       "("+std::to_string(b.component)+")");
    }
    if(b.boundsType==VariableBoundsDescription::LOWERANDUPPER){
      throw_if(b.lowerBound>b.upperBound,
	       "invalid bounds value");
    }
  } // end of checkBoundsCompatibility

  /*!
   * \param[in] b1: standard bounds
   * \param[in] b2: physical bounds
   */
  static void checkBoundsCompatibility(const VariableBoundsDescription& b1,
                                       const VariableBoundsDescription& b2,
                                       const std::string& n) {
    auto throw_if = [](const bool c,const std::string& m){
      tfel::raise_if(c,"mfront::checkBoundsCompatibility: "+m);
    };
    throw_if((!b1.hasLowerBound())&&(b2.hasLowerBound()),
	     "standard bounds for variable '"+n+"' has no lower bounds "
	     "whereas the physical bounds has. This is inconsistent.");
    if(b1.hasLowerBound()){
      throw_if(b1.lowerBound<b2.lowerBound,
	       "standard bounds for variable '"+n+"' are not contained  "
	       "in the physical bounds.");
    }
    throw_if((!b1.hasUpperBound())&&(b2.hasUpperBound()),
	     "standard bounds for variable '"+n+"' has no lower bounds "
	     "whereas the physical bounds has. This is inconsistent.");
    if(b1.hasUpperBound()){
      throw_if(b1.upperBound>b2.upperBound,
	       "standard bounds for variable '"+n+"' are not contained  "
	       "in the physical bounds.");
    }
  } // end of checkBoundsCompatibility

  bool VariableDescription::hasBounds() const {
    return this->bounds.is<VariableBoundsDescription>();
  } // end of VariableDescription::hasBounds

  bool VariableDescription::hasBounds(const unsigned short i) const {
    tfel::raise_if(this->arraySize==1u,
		   "VariableDescription::hasBounds: "
		   "invalid call on scalar variable '"+
		   this->name+"'");
    if(this->bounds.is<VariableBoundsDescription>()){
      return true;
    } else if (this->bounds.is<std::map<unsigned short,
	                                VariableBoundsDescription>>()){
      const auto& m = this->bounds.get<std::map<unsigned short,
					       VariableBoundsDescription>>();
      return m.count(i)!=0;
    }
    return false;
  } // end of VariableDescription::hasBounds

  void VariableDescription::setBounds(const VariableBoundsDescription& b) {
    tfel::raise_if(!this->bounds.empty(),
		   "VariableDescription::setBounds: "
		   "bounds have already been set on variable "
		   "'"+this->name+"'");
    mfront::checkBoundsCompatibility(*this,b);
    if(this->arraySize==1u){
      if(this->hasPhysicalBounds()){
	mfront::checkBoundsCompatibility(b,this->getPhysicalBounds(),
					 this->name);
      }
    } else {
      for(unsigned short i=0;i!=this->arraySize;++i){
	if(this->hasPhysicalBounds(i)){
	  mfront::checkBoundsCompatibility(b,this->getPhysicalBounds(i),
					 this->name);
	}
      }
    }
    this->bounds = b;
  } // end of VariableDescription::setBounds

  void VariableDescription::setBounds(const VariableBoundsDescription& b,
                                      const unsigned short i) {
    auto throw_if = [](const bool c,const std::string& m){
      tfel::raise_if(c,"VariableDescription::setBounds: "+m);
    };    
    throw_if(this->arraySize==1u,"invalid call on scalar variable");
    throw_if(i>this->arraySize,"invalid index");
    throw_if(this->bounds.is<VariableBoundsDescription>(),
	     "bounds have already been set on variable "
	     "'"+this->name+"'");
    if(!this->bounds.empty()){
      tfel::raise_if(this->hasBounds(i),
		     "VariableDescription::setBounds: "
		     "bounds have already been set on variable "
		     "'"+this->name+"'");
    }
    for(unsigned short j=0;j!=this->arraySize;++j){
      if(this->hasPhysicalBounds(j)){
        mfront::checkBoundsCompatibility(b, this->getPhysicalBounds(j),
                                         this->name);
      }
    }
    if(this->bounds.empty()){
      this->bounds = std::map<unsigned short,VariableBoundsDescription>();
    }
    mfront::checkBoundsCompatibility(*this,b);
    if(this->hasPhysicalBounds(i)){
      mfront::checkBoundsCompatibility(b,this->getPhysicalBounds(i),
				       this->name);
    }
    auto& m = this->bounds.get<std::map<unsigned short,
					VariableBoundsDescription>>();
    m.insert({i,b});
  } // end of VariableDescription::setBounds

  const VariableBoundsDescription& VariableDescription::getBounds() const {
    tfel::raise_if(!this->hasBounds(),
		   "VariableDescription::getBounds: "
		   "no bounds set on variable "
		   "'"+this->name+"'");
    return this->bounds.get<VariableBoundsDescription>();
  } // end of VariableDescription::getBounds

  const VariableBoundsDescription&
  VariableDescription::getBounds(const unsigned short i) const
  {
    tfel::raise_if(!this->hasBounds(i),
		   "VariableDescription::getBounds: "
		   "no bounds set on variable "
		   "'"+this->name+"'");
    if(this->bounds.is<VariableBoundsDescription>()){
      return this->bounds.get<VariableBoundsDescription>();
    }
    const auto& m = this->bounds.get<std::map<unsigned short,
					      VariableBoundsDescription>>();
    return m.at(i);
  } // end of VariableDescription::getBounds

  bool VariableDescription::hasPhysicalBounds() const
  {
    return this->physicalBounds.is<VariableBoundsDescription>();
  } // end of VariableDescription::hasPhysicalBounds

  bool VariableDescription::hasPhysicalBounds(const unsigned short i) const
  {
    tfel::raise_if(this->arraySize==1u,
		   "VariableDescription::hasPhysicalBounds: "
		   "invalid call on scalar variable '"+
		   this->name+"'");
    if(this->physicalBounds.is<VariableBoundsDescription>()){
      return true;
    } else if (this->physicalBounds.is<std::map<unsigned short,
	                                VariableBoundsDescription>>()){
      const auto& m = this->physicalBounds.get<std::map<unsigned short,
							VariableBoundsDescription>>();
      return m.count(i)!=0;
    }
    return false;
  } // end of VariableDescription::hasPhysicalBounds
  
  void VariableDescription::setPhysicalBounds(const VariableBoundsDescription& b)
  {
    tfel::raise_if(!this->physicalBounds.empty(),
		   "VariableDescription::setPhysicalBounds: "
		   "bounds have already been set on variable "
		   "'"+this->name+"'");
    mfront::checkBoundsCompatibility(*this,b);
    if(this->arraySize==1u){
      if(this->hasBounds()){
	mfront::checkBoundsCompatibility(this->getBounds(),b,
					 this->name);
      }
    } else {
      for(unsigned short i=0;i!=this->arraySize;++i){
	if(this->hasBounds(i)){
	  mfront::checkBoundsCompatibility(this->getBounds(i),b,
					   this->name);
	}
      }
    }
    this->physicalBounds = b;
  } // end of VariableDescription::setPhysicalBounds

  void VariableDescription::setPhysicalBounds(const VariableBoundsDescription& b,
					      const unsigned short i)
  {
    auto throw_if = [](const bool c, const std::string& m) {
      if (c) {
        tfel::raise("VariableDescription::setPhysicalBounds: " + m);
      }
    };
    throw_if(this->arraySize==1u,"invalid call on scalar variable");
    throw_if(i>this->arraySize,"invalid index");
    throw_if(this->physicalBounds.is<VariableBoundsDescription>(),
	     "physical bounds have already been set on variable "
	     "'"+this->name+"'");
    tfel::raise_if(this->hasPhysicalBounds(i),
		   "VariableDescription::setPhysicalBounds: "
		   "bounds have already been set on variable "
		   "'"+this->name+"'");
    mfront::checkBoundsCompatibility(*this,b);
    for(unsigned short j=0;j!=this->arraySize;++j){
      if(this->hasBounds(j)){
	mfront::checkBoundsCompatibility(this->getBounds(j),b,
					 this->name);
      }
    }
    if(this->physicalBounds.empty()){
      this->physicalBounds = std::map<unsigned short,VariableBoundsDescription>();
    }
    auto& m = this->physicalBounds.get<std::map<unsigned short,
					VariableBoundsDescription>>();
    m.insert({i,b});
  } // end of VariableDescription::setPhysicalBounds
  
  const VariableBoundsDescription& VariableDescription::getPhysicalBounds() const
  {
    tfel::raise_if(!this->hasPhysicalBounds(),
		   "VariableDescription::setPhysicalBounds: "
		   "no bounds set on variable "
		   "'"+this->name+"'");
    return this->physicalBounds.get<VariableBoundsDescription>();
  } // end of VariableDescription::getPhysicalBounds

  const VariableBoundsDescription&
  VariableDescription::getPhysicalBounds(const unsigned short i) const
  {
    tfel::raise_if(!this->hasPhysicalBounds(i),
		   "VariableDescription::setPhysicalBounds: "
		   "no bounds set on variable "
		   "'"+this->name+"'");
    if(this->physicalBounds.is<VariableBoundsDescription>()){
      return this->physicalBounds.get<VariableBoundsDescription>();
    }
    const auto& m = this->physicalBounds.get<std::map<unsigned short,
					      VariableBoundsDescription>>();
    return m.at(i);
  } // end of VariableDescription::getBounds
  
  VariableDescription::~VariableDescription() = default;

  bool hasBounds(const VariableDescription& v){
    return v.hasBounds();
  } // end of hasBounds

  bool hasBounds(const VariableDescription& v,
		 const unsigned short i)
  {
    return v.hasBounds(i);
  } // end of hasBounds
  
  bool hasPhysicalBounds(const VariableDescription& v){
    return v.hasPhysicalBounds();
  } // end of hasPhysicalBounds

  bool hasPhysicalBounds(const VariableDescription& v,
			 const unsigned short i){
    return v.hasPhysicalBounds(i);
  } // end of hasPhysicalBounds
  
  VariableDescriptionContainer::VariableDescriptionContainer() = default;
  
  VariableDescriptionContainer::VariableDescriptionContainer(const std::initializer_list<VariableDescription>& l)
	  : std::vector<VariableDescription>(l)
  {}

  VariableDescriptionContainer::VariableDescriptionContainer(VariableDescriptionContainer&&) = default;
  VariableDescriptionContainer::VariableDescriptionContainer(const VariableDescriptionContainer&) = default;
  VariableDescriptionContainer&
  VariableDescriptionContainer::operator=(VariableDescriptionContainer&&) = default;
  VariableDescriptionContainer&
  VariableDescriptionContainer::operator=(const VariableDescriptionContainer&) = default;

  bool VariableDescriptionContainer::contains(const std::string& n) const
  {
    for(const auto& v : *this){
      if(v.name==n){
	return true;
      }
    }
    return false;
  } // end of VariableDescriptionContainer::contains

  std::vector<std::string> VariableDescriptionContainer::getExternalNames() const
  {
    std::vector<std::string> n;
    this->appendExternalNames(n);
    return n;
  }

  void VariableDescriptionContainer::getExternalNames(std::vector<std::string>& n) const
  {
    n.clear();
    this->appendExternalNames(n);
  }

  void VariableDescriptionContainer::appendExternalNames(std::vector<std::string>& n) const
  {
    for(const auto& v : *this){
      const auto name = v.getExternalName();
      if(v.arraySize==1u){
	n.push_back(name);
      } else {
	for(unsigned short i=0;i!=v.arraySize;++i){
	  std::ostringstream nb;
	  nb << '[' << i << ']';
	  n.push_back(name+nb.str());
	}
      }
    }
  } // end of VariableDescriptionContainer::appendExternalNames

  VariableDescription&
  VariableDescriptionContainer::getVariable(const std::string& n)
  {
    for(auto& v : *this){
      if(v.name==n){
	return v;
      }
    }
    tfel::raise("VariableDescriptionContainer::getVariable : "
		"no variable named '"+n+"'");
  }

  const VariableDescription&
  VariableDescriptionContainer::getVariableByExternalName(const std::string& n) const
  {
    for(auto& v : *this){
      if(v.getExternalName()==n){
	return v;
      }
    }
    tfel::raise("VariableDescriptionContainer::getVariableByExternalName : "
		"no variable with external name '"+n+"'");
  } // end of VariableDescriptionContainer::getVariableByExternalName
  
  const VariableDescription&
  VariableDescriptionContainer::getVariable(const std::string& n) const
  {
    for(const auto& v : *this){
      if(v.name==n){
	return v;
      }
    }
    tfel::raise("VariableDescriptionContainer::getVariable : "
		"no variable named '"+n+"'");
  }

  SupportedTypes::TypeSize VariableDescriptionContainer::getTypeSize() const
  {
    auto s = SupportedTypes::TypeSize{};
    for(const auto& v:*this){
      s += v.getTypeSize();
    }
    return s;
  }

  unsigned short VariableDescriptionContainer::getNumberOfVariables() const
  {
    size_t n = 0u;
    for(const auto& v : *this){
      n = n + v.arraySize;
    }
    return n;
  } // end of SupportedTypes::getNumberOfVariables
  
  VariableDescriptionContainer::~VariableDescriptionContainer() = default;
  
  bool hasBounds(const VariableDescriptionContainer& c)
  {
    for(const auto& v:c){
      if(v.arraySize==1u){
	if(v.hasBounds()){
	  return true;
	}
      } else {
	for(unsigned short i=0;i!=v.arraySize;++i){
	  if(v.hasBounds(i)){
	    return true;
	  }
	}
      }
    }
    return false;
  } // end of hasBounds

  bool hasPhysicalBounds(const VariableDescriptionContainer& c)
  {
    for(const auto& v:c){
      if(v.arraySize==1u){
	if(v.hasPhysicalBounds()){
	  return true;
	}
      } else {
	for(unsigned short i=0;i!=v.arraySize;++i){
	  if(v.hasPhysicalBounds(i)){
	    return true;
	  }
	}
      }
    }
    return false;
  } // end of hasPhysicalBounds

} // end of namespace mfront
