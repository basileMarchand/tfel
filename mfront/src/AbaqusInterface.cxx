/*!
 * \file   mfront/src/AbaqusInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   17 Jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdlib>
#include<stdexcept>

#include"TFEL/Config/GetInstallPath.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"

#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontLock.hxx"
#include"MFront/MFrontUtilities.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/AbaqusInterface.hxx"

namespace mfront{
  
  static void
  writeUMATArguments(std::ostream& out,
		     const BehaviourDescription::BehaviourType& t,
		     const bool f)
  {
    if(f){
      out << "(abaqus::AbaqusReal *const STRESS,\n"
	  << " abaqus::AbaqusReal *const STATEV,\n"
	  << " abaqus::AbaqusReal *const DDSDDE,\n"
	  << " abaqus::AbaqusReal *const SSE,\n"
	  << " abaqus::AbaqusReal *const SPD,\n"
	  << " abaqus::AbaqusReal *const SCD,\n"
	  << " abaqus::AbaqusReal *const RPL,\n"
	  << " abaqus::AbaqusReal *const DDSDDT,\n"
	  << " abaqus::AbaqusReal *const DRPLDE,\n"
	  << " abaqus::AbaqusReal *const DRPLDT,\n"
	  << " const abaqus::AbaqusReal *const STRAN,\n"
	  << " const abaqus::AbaqusReal *const DSTRAN,\n"
	  << " const abaqus::AbaqusReal *const TIME,\n"
	  << " const abaqus::AbaqusReal *const DTIME,\n"
	  << " const abaqus::AbaqusReal *const TEMP,\n"
	  << " const abaqus::AbaqusReal *const DTEMP,\n"
	  << " const abaqus::AbaqusReal *const PREDEF,\n"
	  << " const abaqus::AbaqusReal *const DPRED,\n"
	  << " const char           *const CMNAME,\n"
	  << " const abaqus::AbaqusInt  *const NDI,\n"
	  << " const abaqus::AbaqusInt  *const NSHR,\n"
	  << " const abaqus::AbaqusInt  *const NTENS,\n"
	  << " const abaqus::AbaqusInt  *const NSTATV,\n"
	  << " const abaqus::AbaqusReal *const PROPS,\n"
	  << " const abaqus::AbaqusInt  *const NPROPS,\n"
	  << " const abaqus::AbaqusReal *const COORDS,\n"
	  << " const abaqus::AbaqusReal *const DROT,\n"
	  << "       abaqus::AbaqusReal *const PNEWDT,\n"
	  << " const abaqus::AbaqusReal *const CELENT,\n"
	  << " const abaqus::AbaqusReal *const DFGRD0,\n"
	  << " const abaqus::AbaqusReal *const DFGRD1,\n"
	  << " const abaqus::AbaqusInt  *const NOEL,\n"
	  << " const abaqus::AbaqusInt  *const NPT,\n"
	  << " const abaqus::AbaqusInt  *const LAYER,\n"
	  << " const abaqus::AbaqusInt  *const KSPT,\n"
	  << " const abaqus::AbaqusInt  *const KSTEP,\n"
	  << "       abaqus::AbaqusInt  *const KINC,\n"
	  << "const int size)";
    } else {
      out << "(abaqus::AbaqusReal *const STRESS,\n"
	  << " abaqus::AbaqusReal *const STATEV,\n"
	  << " abaqus::AbaqusReal *const DDSDDE,\n"
	  << " abaqus::AbaqusReal *const,\n"
	  << " abaqus::AbaqusReal *const,\n"
	  << " abaqus::AbaqusReal *const,\n"
	  << " abaqus::AbaqusReal *const,\n"
	  << " abaqus::AbaqusReal *const,\n"
	  << " abaqus::AbaqusReal *const,\n"
	  << " abaqus::AbaqusReal *const,\n";
      if(t!=BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << " const abaqus::AbaqusReal *const STRAN,\n"
	    << " const abaqus::AbaqusReal *const DSTRAN,\n";
      } else {
	out << " const abaqus::AbaqusReal *const,\n"
	    << " const abaqus::AbaqusReal *const,\n";
      }
      out << " const abaqus::AbaqusReal *const,\n"
	  << " const abaqus::AbaqusReal *const DTIME,\n"
	  << " const abaqus::AbaqusReal *const TEMP,\n"
	  << " const abaqus::AbaqusReal *const DTEMP,\n"
	  << " const abaqus::AbaqusReal *const PREDEF,\n"
	  << " const abaqus::AbaqusReal *const DPRED,\n"
	  << " const char           *const,\n"
	  << " const abaqus::AbaqusInt  *const,\n"
	  << " const abaqus::AbaqusInt  *const,\n"
	  << " const abaqus::AbaqusInt  *const NTENS,\n"
	  << " const abaqus::AbaqusInt  *const NSTATV,\n"
	  << " const abaqus::AbaqusReal *const PROPS,\n"
	  << " const abaqus::AbaqusInt  *const NPROPS,\n"
	  << " const abaqus::AbaqusReal *const,\n"
	  << " const abaqus::AbaqusReal *const DROT,\n"
	  << "       abaqus::AbaqusReal *const PNEWDT,\n"
	  << " const abaqus::AbaqusReal *const,\n";
      if(t==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	out << " const abaqus::AbaqusReal *const F0,\n"
	    << " const abaqus::AbaqusReal *const F1,\n";
      } else {
	out << " const abaqus::AbaqusReal *const,\n"
	    << " const abaqus::AbaqusReal *const,\n";
      }
      out << " const abaqus::AbaqusInt  *const,\n"
	  << " const abaqus::AbaqusInt  *const,\n"
	  << " const abaqus::AbaqusInt  *const,\n"
	  << " const abaqus::AbaqusInt  *const,\n"
	  << " const abaqus::AbaqusInt  *const,\n"
	  << "       abaqus::AbaqusInt  *const,\n"
	  << "const int)";
    }
  } // end of writeUMATArguments

  static void
  writeUMATArguments(std::ostream& out)
  {
    out << "(abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const char           *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< "       abaqus::AbaqusReal *const,\n"
        << " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusReal *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< " const abaqus::AbaqusInt  *const,\n"
	<< "       abaqus::AbaqusInt  *const,\n"
	<< "const int)";
  } // end of writeUMATArguments
  
  std::string
  AbaqusInterface::getName(void)
  {
    return "abaqus";
  }
  
  std::string
  AbaqusInterface::getInterfaceName(void) const
  {
    return "Abaqus";
  } // end of AbaqusInterface::getInterfaceName


  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  AbaqusInterface::treatKeyword(const std::string& key,
			       tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
			       const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator end)
  {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("AbaqusInterface::treatKeyword: "+m));}
    };
    if (key=="@AbaqusGenerateMTestFileOnFailure"){
      this->generateMTestFile = this->readBooleanValue(key,current,end);
      return {true,current};      
    } else if(key=="@AbaqusCompareToNumericalTangentOperator"){
      this->compareToNumericalTangentOperator  = this->readBooleanValue(key,current,end);
      return make_pair(true,current);
    } else if ((key=="@AbaqusTangentOperatorComparisonCriterium")||
	       (key=="@AbaqusTangentOperatorComparisonCriterion")){
      throw_if(!this->compareToNumericalTangentOperator,
	       "comparison to tangent operator is not enabled at this stage.\n"
	       "Use the @AbaqusCompareToNumericalTangentOperator directive before "
	       "@AbaqusTangentOperatorComparisonCriterion");
      throw_if(current==end,"unexpected end of file");
      this->tangentOperatorComparisonCriterion = CxxTokenizer::readDouble(current,end);
      throw_if(current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+"'");
      ++(current);
      return {true,current};
    } else if (key=="@AbaqusStrainPerturbationValue"){
      throw_if(!this->compareToNumericalTangentOperator,
	       "time stepping is not enabled at this stage.\n"
	       "Use the @AbaqusUseTimeSubStepping directive before "
	       "@AbaqusStrainPerturbationValue");
      throw_if(current==end,"unexpected end of file");
      this->strainPerturbationValue = CxxTokenizer::readDouble(current,end);
      throw_if(current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+"'");
      ++(current);
      return {true,current};
    }
    return {false,current};
  } // end of treatKeyword

  void
  AbaqusInterface::endTreatment(const BehaviourDescription& mb,
				const FileDescription& fd) const
  {
    using namespace tfel::system;
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("AbaqusInterface::endTreatment: "+m));}
    };
    throw_if(!((mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	       (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)),
	     "the abaqus interface only supports small and "
	     "finite strain behaviours");
    if(this->compareToNumericalTangentOperator){
      throw_if(mb.getBehaviourType()!=BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR,
	       "unsupported feature @AbaqusSaveTangentOperator "
	       "and @AbaqusCompareToNumericalTangentOperator : "
	       "those are only valid for small strain beahviours");
    }
    // get the modelling hypotheses to be treated
    const auto& mh = this->getModellingHypothesesToBeTreated(mb);
    const auto name =  mb.getLibrary()+mb.getClassName();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/Abaqus");
    systemCall::mkdir("abaqus");

    std::ofstream out;
    {
      // copy umat.cpp locally
      MFrontLockGuard lock;
      out.open("abaqus/umat.cpp");
      if(out){
	const auto root = tfel::getInstallPath();
	const auto fn = root+"/share/doc/mfront/abaqus/umat.cpp";
	std::ifstream in{fn};
	if(in){
	  out << in.rdbuf();
	  in.close();
	} else {
	  std::cerr << "AbaqusInterface::endTreatment: "
		    << "could not open file '" << fn << "'" << std::endl;
	}
      } else {
	  std::cerr << "AbaqusInterface::endTreatment: "
		    << "could not open file 'abaqus/umat.cpp'" << std::endl;
      }
      out.close();
    }
    
    // header
    auto fname = "abaqus"+name+".hxx";
    out.open("include/MFront/Abaqus/"+fname);
    throw_if(!out,"could not open file '"+fname+"'");
    
    out << "/*!\n"
	<< "* \\file   "  << fname << '\n'
	<< "* \\brief  This file declares the abaqus interface for the " 
	<< mb.getClassName() << " behaviour law\n"
	<< "* \\author "  << fd.authorName << '\n'
	<< "* \\date   "  << fd.date       << '\n'
	<< "*/\n\n";

    const auto header = this->getHeaderDefine(mb);
    out << "#ifndef "<< header << "\n"
	<< "#define "<< header << "\n\n"
	<< "#include\"TFEL/Config/TFELConfig.hxx\"\n\n"
	<< "#include\"MFront/Abaqus/Abaqus.hxx\"\n"
	<< "#include\"MFront/Abaqus/AbaqusData.hxx\"\n\n";

    out << "#ifdef __cplusplus\n"
	<< "#include\"MFront/Abaqus/AbaqusTraits.hxx\"\n";
    if (mb.getSymmetryType()==mfront::ORTHOTROPIC){
      out << "#include\"MFront/Abaqus/AbaqusOrthotropicBehaviour.hxx\"\n";
    }
    out << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n"
	<< "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n"
	<< "namespace abaqus{\n\n";

    if(!mb.areAllMechanicalDataSpecialised(mh)){
      this->writeAbaqusBehaviourTraits(out,mb,ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    for(const auto & h : mh){
      if(mb.hasSpecialisedMechanicalData(h)){
	this->writeAbaqusBehaviourTraits(out,mb,h);
      }
    }

    out << "} // end of namespace abaqus\n\n"
	<< "#endif /* __cplusplus */\n\n"
	<< "#ifdef __cplusplus\n"
	<< "extern \"C\"{\n"
	<< "#endif /* __cplusplus */\n\n";

    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out,name);
    this->writeSetParametersFunctionsDeclarations(out,name,mb);

    out << "MFRONT_SHAREDOBJ void\n"
	<< getFunctionName(name);
    writeUMATArguments(out);
    out << ";\n\n";

    out << "#ifdef __cplusplus\n"
	<< "}\n"
	<< "#endif /* __cplusplus */\n\n"
	<< "#endif /* " << header << " */\n";

    out.close();

    fname  = "abaqus"+name+".cxx";
    out.open("src/"+fname);
    throw_if(!out,"could not open file '"+fname+"'");

    std::string sfeh;
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      sfeh = "abaqus::AbaqusStandardSmallStrainStressFreeExpansionHandler";
    } else if (mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      sfeh = "nullptr";
    } else {
      throw_if(true,"the abaqus interface only supports small and finite strain behaviours");
    }

    out << "/*!\n"
	<< "* \\file   "  << fname << '\n'
	<< "* \\brief  This file implements the abaqus interface for the " 
	<< mb.getClassName() << " behaviour law\n"
	<< "* \\author "  << fd.authorName << '\n'
	<< "* \\date   "  << fd.date       << '\n'
	<< "*/\n\n";

    this->getExtraSrcIncludes(out,mb);

    if(this->compareToNumericalTangentOperator){
      out << "#include<cmath>\n"
	  << "#include<vector>\n"
	  << "#include<algorithm>\n";
    }
    out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n"
	<< "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    out << "#include\"MFront/Abaqus/AbaqusStressFreeExpansionHandler.hxx\"\n\n"
	<< "#include\"MFront/Abaqus/AbaqusInterface.hxx\"\n\n"
	<< "#include\"MFront/Abaqus/abaqus" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out,name);
    
    out << "extern \"C\"{\n\n";
 
    this->generateUMATxxGeneralSymbols(out,name,mb,fd);
    if(!mb.areAllMechanicalDataSpecialised(mh)){
      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      this->generateUMATxxSymbols(out,name,uh,mb,fd);
    }
    for(const auto &h : mh){
      if(mb.hasSpecialisedMechanicalData(h)){
	this->generateUMATxxSymbols(out,name,h,mb,fd);
      }
    }
    
    this->writeSetParametersFunctionsImplementations(out,name,mb);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out,name);

    std::string dv0;
    std::string dv1;
    if(mb.getBehaviourType()==BehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
      dv0 = "STRAN";
      dv1 = "DSTRAN";
    } else if(mb.getBehaviourType()==BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      dv0 = "F0";
      dv1 = "F1";
    } else {
      throw_if(true,"the abaqus interface only supports small "
	       "and finite strain behaviours");
    }

    out << "MFRONT_SHAREDOBJ void\n" << getFunctionName(name);
    writeUMATArguments(out,mb.getBehaviourType(),false);
    out << "{\n";
    if(((getDebugMode())||(this->compareToNumericalTangentOperator))&&(!this->generateMTestFile)){
      out << "using namespace std;\n";
    }
    if(mb.getAttribute(BehaviourData::profiling,false)){
      out << "using mfront::BehaviourProfiler;\n"
	  << "using tfel::material::" << mb.getClassName() << "Profiler;\n"
	  << "BehaviourProfiler::Timer total_timer(" << mb.getClassName()
	  << "Profiler::getProfiler(),\n"
	  << "BehaviourProfiler::TOTALTIME);\n";
    }
    this->generateMTestFile1(out);
    if(this->compareToNumericalTangentOperator){
      out << "abaqus::AbaqusReal pnewdt0(*PNEWDT);\n"
	  << "vector<abaqus::AbaqusReal> deto0(*NTENS);\n"
	  << "vector<abaqus::AbaqusReal> sig0(*NTENS);\n"
	  << "vector<abaqus::AbaqusReal> sv0(*NSTATV);\n"
	  << "copy(DSTRAN,DSTRAN+*(NTENS),deto0.begin());\n"
	  << "copy(STRESS,STRESS+*(NTENS),sig0.begin());\n"
	  << "copy(STATEV,STATEV+*(NSTATV),sv0.begin());\n";
    }
    out << "abaqus::AbaqusData d = {STRESS,PNEWDT,DDSDDE,STATEV,\n"
	<< "                        *NTENS,*NPROPS,*NSTATV,*DTIME,\n"
	<< "                        DROT," << dv0 << "," << dv1 << ",TEMP,DTEMP,\n"
      	<< "                        PROPS,PREDEF,DPRED,\n"
	<< getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh << "};\n"
	<< "if(abaqus::AbaqusInterface<tfel::material::" << mb.getClassName() 
	<< ">::exe(d)!=0){\n";
    this->generateMTestFile2(out,mb.getBehaviourType(),
			     name,"",mb);
    out << "*PNEWDT = -1.;\n"
	<< "return;\n"
	<< "}\n";
    if(getDebugMode()){
      out << "cout << \"Dt :\" << endl;\n"
	  << "for(abaqus::AbaqusInt i=0;i!=*NTENS;++i){\n"
	  << "for(abaqus::AbaqusInt j=0;j!=*NTENS;++j){\n"
	  << "cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n"
	  << "}\n"
	  << "cout << endl;\n"
	  << "}\n"
	  << "cout << endl;\n";
    }
    if(this->compareToNumericalTangentOperator){
      out << "// computing the tangent operator by pertubation\n"
	  << "vector<abaqus::AbaqusReal> nD((*NTENS)*(*NTENS));\n"
	  << "vector<abaqus::AbaqusReal> deto(*NTENS);\n"
	  << "vector<abaqus::AbaqusReal> sigf(*NTENS);\n"
	  << "vector<abaqus::AbaqusReal> sigb(*NTENS);\n"
	  << "vector<abaqus::AbaqusReal> sv(*NSTATV);\n"
	  << "vector<abaqus::AbaqusReal> D((*NTENS)*(*NTENS));\n"
	  << "abaqus::AbaqusReal pnewdt(pnewdt0);\n"
	  << "abaqus::AbaqusReal m;\n"
	  << "abaqus::AbaqusReal mDt;\n"
	  << "abaqus::AbaqusReal mnDt;\n"
	  << "for(abaqus::AbaqusInt i=0;i!=*NTENS;++i){\n"
	  << "copy(deto0.begin(),deto0.end(),deto.begin());\n"
	  << "copy(sig0.begin(),sig0.end(),sigf.begin());\n"
	  << "copy(sv0.begin(),sv0.end(),sv.begin());\n"
	  << "deto[i] += " << this->strainPerturbationValue << ";\n"
	  << "D[0] = 0.;\n"
	  << "abaqus::AbaqusData d2 = {&sigf[0],&pnewdt0,&D[0],&sv[0],\n"
	  << "                         *NTENS,*NPROPS,*NSTATV,*DTIME,\n"
	  << "                         DROT,STRAN,&deto[0],TEMP,DTEMP,\n"
	  << "                         PROPS,PREDEF,DPRED,\n"
	  << getFunctionName(name) << "_getOutOfBoundsPolicy()," << sfeh << "};\n"
	  << "if(abaqus::AbaqusInterface<tfel::material::" << mb.getClassName() << ">::exe(d2)!=0){\n"
	  << "return;\n"
	  << "}\n"
	  << "abaqus::AbaqusReal pnewdt(pnewdt0);\n"
	  << "copy(deto0.begin(),deto0.end(),deto.begin());\n"
	  << "copy(sig0.begin(),sig0.end(),sigb.begin());\n"
	  << "copy(sv0.begin(),sv0.end(),sv.begin());\n"
	  << "deto[i] -= " << this->strainPerturbationValue << ";\n"
	  << "D[0] = 0.;\n"
	  << "abaqus::AbaqusData d3 = {&sigf[0],&pnewdt0,&D[0],&sv[0],\n"
	  << "                         *NTENS,*NPROPS,*NSTATV,*DTIME,\n"
	  << "                         DROT,STRAN,&deto[0],TEMP,DTEMP,\n"
	  << "                         PROPS,PREDEF,DPRED,\n"
	  << "if(abaqus::AbaqusInterface<tfel::material::" << mb.getClassName() << ">::exe(d3)!=0){\n"
	  << "return;\n"
	  << "}\n"
	  << "for(abaqus::AbaqusInt j=0;j!=*NTENS;++j){\n"
	  << "nD[j*(*NTENS)+i] = (sigf[j]-sigb[j])/(2.*" << this->strainPerturbationValue << ");\n"
	  << "}\n"
	  << "}\n"
	  << "// comparison\n"
	  << "m=0.;\n"
	  << "mDt=0.;\n"
	  << "mnDt=0.;\n"
	  << "for(i=0;i!=(*NTENS)*(*NTENS);++i){\n"
	  << "mDt=max(mDt,*(DDSDDE+i));\n"
	  << "mnDt=max(mnDt,nD[i]);\n"
	  << "m=max(m,abs(nD[i]-*(DDSDDE+i)));\n"
	  << "}\n"
	  << "if(m>" << this->tangentOperatorComparisonCriterion << "){\n"
	  << "cout << \"||nDt-Dt|| = \" << m << \" (\" << 100.*m/(0.5*(mDt+mnDt)) << \"%)\"<< endl;\n"
	  << "cout << \"Dt :\" << endl;\n"
	  << "for(abaqus::AbaqusInt i=0;i!=*NTENS;++i){\n"
	  << "for(abaqus::AbaqusInt j=0;j!=*NTENS;++j){\n"
	  << "cout << *(DDSDDE+j*(*NTENS)+i) << \" \";\n"
	  << "}\n"
	  << "cout << endl;\n"
	  << "}\n"
	  << "cout << \"nDt :\" << endl;\n"
	  << "for(abaqus::AbaqusInt i=0;i!=*NTENS;++i){\n"
	  << "for(abaqus::AbaqusInt j=0;j!=*NTENS;++j){\n"
	  << "cout << nD[j*(*NTENS)+i] << \" \";\n"
	  << "}\n"
	  << "cout << endl;\n"
	  << "}\n"
	  << "cout << endl;\n"
	  << "}\n";
    }
    out << "}\n"
	<< "} // end of extern \"C\"\n";
    out.close();
    this->writeInputFileExample(mb,fd,true);
  } // end of AbaqusInterface::endTreatment
  
  void 
  AbaqusInterface::writeInterfaceSpecificIncludes(std::ostream& out,
						  const BehaviourDescription&) const
  {
    out << "#include\"MFront/Abaqus/Abaqus.hxx\"\n"
	<< "#include\"MFront/Abaqus/AbaqusConvert.hxx\"\n\n";
  } // end of AbaqusInterface::writeInterfaceSpecificIncludes

  std::vector<std::pair<std::string,std::string>>
  AbaqusInterface::getBehaviourDataConstructorAdditionalVariables(void) const{
    return {{"DR","increment of rigid body rotation"}};
  } // end of AbaqusInterface::getBehaviourDataConstructorAdditionalVariables

  void 
  AbaqusInterface::writeBehaviourDataMainVariablesSetters(std::ostream& os,
							  const BehaviourDescription& mb) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    SupportedTypes::TypeSize ov,of;
    os << "void set"
       << iprefix << "BehaviourDataDrivingVariables(const Type* const " << iprefix << "stran)\n"
       << "{\n";
    for(const auto& v : mb.getMainVariables()){
      this->writeBehaviourDataDrivingVariableSetter(os,v.first,ov);
      ov += this->getTypeSize(v.first.type,1u);
    }
    os << "}\n\n";
    os << "void set"
       << iprefix << "BehaviourDataThermodynamicForces(const Type* const " << iprefix << "stress_,\n"
       << "                                                        const Type* const )\n"
       << "{\n";
    for(const auto& v : mb.getMainVariables()){
      this->writeBehaviourDataThermodynamicForceSetter(os,v.second,of);
      of += this->getTypeSize(v.second.type,1u);
    }
    os << "}\n\n";
  } // end of AbaqusInterface::writeBehaviourDataMainVariablesSetters

  void 
  AbaqusInterface::writeBehaviourDataDrivingVariableSetter(std::ostream& os,
							   const DrivingVariable& v,
							   const SupportedTypes::TypeSize o) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if(!o.isNull()){
      throw(std::runtime_error("AbaqusInterface::writeBehaviourDataMainVariablesSetter : "
			       "only one driving variable supported"));
    }
    if(v.increment_known){
      os << "abaqus::UMATImportDrivingVariables<hypothesis>::exe(this->" << v.name << ","
	 << iprefix << "stran);\n";
    } else {
      os << "abaqus::UMATImportDrivingVariables<hypothesis>::exe(this->" << v.name << "0,"
	 << iprefix << "stran);\n";
    }
  } // end of AbaqusInterface::writeBehaviourDataDrivingVariableSetter

  void 
  AbaqusInterface::writeIntegrationDataDrivingVariableSetter(std::ostream& os,
							   const DrivingVariable& v,
							   const SupportedTypes::TypeSize o) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if(!o.isNull()){
      throw(std::runtime_error("AbaqusInterface::writeIntegrationDataMainVariablesSetter : "
			       "only one driving variable supported"));
    }
    if(v.increment_known){
      os << "abaqus::UMATImportDrivingVariables<hypothesis>::exe(this->d" << v.name << ","
	 << iprefix << "dstran);\n";
    } else {
      os << "abaqus::UMATImportDrivingVariables<hypothesis>::exe(this->" << v.name << "1,"
	 << iprefix << "dstran);\n";
    }
  } // end of AbaqusInterface::writeIntegrationDataDrivingVariableSetter
  
  void 
  AbaqusInterface::writeBehaviourDataThermodynamicForceSetter(std::ostream& os,
							      const ThermodynamicForce& f,
							      const SupportedTypes::TypeSize o) const
  {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if(this->getTypeFlag(f.type)==SupportedTypes::Stensor){
      os << "abaqus::UMATImportThermodynamicForces<hypothesis>::exe(this->" << f.name << ",";
      if(!o.isNull()){
	os << iprefix << "stress_+" << o << ");\n";
      } else {
	os << iprefix << "stress_);\n";
      }
    } else {
      throw(std::runtime_error("AbaqusInterface::writeBehaviourDataMainVariablesSetters : "
			       "unsupported forces type"));
    }
  } // end of AbaqusInterface::writeBehaviourDataThermodynamicForceSetter
  
  void 
  AbaqusInterface::completeBehaviourDataConstructor(std::ostream& out,
						    const Hypothesis h,
						    const BehaviourDescription& mb) const
  {
    auto do_nothing = [&out](){
      out << "static_cast<void>(ABAQUSDR);\n";
    };
    if((mb.getSymmetryType()==mfront::ORTHOTROPIC)||
       (mb.getBehaviourType()!=BehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      do_nothing();
      return;
    }
    // checking if there are variables that need to be rotated
    const auto& d = mb.getBehaviourData(h);
    bool b = false; // have persistent variables that have to be updated
    for(const auto& v:d.getPersistentVariables()){
      const auto flag = this->getTypeFlag(v.type);
      if((flag==SupportedTypes::Stensor)||
	 (flag==SupportedTypes::Tensor)){
	b = true;
	break;
      }
    }
    if(!b){
      do_nothing();
      return;
    }
    // rotate variables
    out << "const tfel::math::tmatrix<3u,3u,real> abaqus_dr = {ABAQUSDR[0],ABAQUSDR[1],ABAQUSDR[2],\n"
      "                                                   ABAQUSDR[3],ABAQUSDR[4],ABAQUSDR[5],\n"
      "                                                   ABAQUSDR[6],ABAQUSDR[7],ABAQUSDR[8]};\n";
    for(const auto& v:d.getPersistentVariables()){
      const auto flag = this->getTypeFlag(v.type);
      if((flag==SupportedTypes::Stensor)||
	 (flag==SupportedTypes::Tensor)){
	if(v.arraySize==1u){
	  out << "this->" << v.name << ".changeBasis(abaqus_dr);\n";
	} else {
	  for(unsigned short i=0;i!=v.arraySize;++i){
	    out << "this->" << v.name << "[" << i << "].changeBasis(abaqus_dr);\n";
	  }
	}
      }
    }
  } // end of UMATInterfaceBase::completeBehaviourDataConstructor

  void 
  AbaqusInterface::exportThermodynamicForce(std::ostream& out,
					    const std::string& a,
					    const ThermodynamicForce& f,
					    const SupportedTypes::TypeSize o) const
   {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto flag = this->getTypeFlag(f.type);
    if(flag==SupportedTypes::Stensor){
      if(!o.isNull()){
	out << "abaqus::UMATExportThermodynamicForces<hypothesis>::exe("
	    << a << "+" << o << ",this->sig);\n";
      } else {
	out << "abaqus::UMATExportThermodynamicForces<hypothesis>::exe(" << a << ",this->sig);\n";
      }
    } else {
      throw(std::runtime_error("AbaqusInterface::exportThermodynamicForce: "
			       "unsupported forces type"));
    }
  } // end of AbaqusInterface::exportThermodynamicForce

  void
  AbaqusInterface::getTargetsDescription(TargetsDescription& d,
					const BehaviourDescription& bd)
  {
    const auto lib  = this->getLibraryName(bd);
    const auto name = bd.getLibrary()+bd.getClassName(); 
#ifdef _WIN32
    const std::string tfel_config = "tfel-config.exe";
#else /* WIN32 */
    const std::string tfel_config = "tfel-config";
#endif /* WIN32 */
    insert_if(d[lib].cppflags,"$(shell "+tfel_config+" --includes)");
    insert_if(d[lib].sources,"abaqus"+name+".cxx");
    d.headers.push_back("MFront/Abaqus/abaqus"+name+".hxx");
    insert_if(d[lib].ldflags,"-lAbaqusInterface");
    if(this->generateMTestFile){
      insert_if(d[lib].ldflags,"-lMTestFileGenerator");
    }
    insert_if(d[lib].ldflags,"$(shell "+tfel_config+" --libs --material --mfront-profiling)");
    insert_if(d[lib].epts,this->getFunctionName(name));
  } // end of AbaqusInterface::getTargetsDescription

  AbaqusInterface::~AbaqusInterface() = default;
  
} // end of namespace mfront