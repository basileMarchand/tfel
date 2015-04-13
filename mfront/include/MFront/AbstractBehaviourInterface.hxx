/*!
 * \file   mfront/include/MFront/AbstractBehaviourInterface.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   16 jan 2007
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTBEHAVIOURVIRTUALINTERFACE_HXX_
#define LIB_MFRONTBEHAVIOURVIRTUALINTERFACE_HXX_ 

#include<map>
#include<set>
#include<string>
#include<vector>
#include<memory>
#include<utility>

#include"MFront/MFrontConfig.hxx"
#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

namespace mfront{
  
  // forward declaration
  struct BehaviourDescription;
  // forward declaration
  struct FileDescription;
  // forward declartion
  struct TargetsDescription;

  /*!
   * This is the abstract base class of all behaviour interfaces
   */
  struct MFRONT_VISIBILITY_EXPORT AbstractBehaviourInterface
  {

    //! a simple alias
    typedef tfel::material::ModellingHypothesis ModellingHypothesis;
    //! a simple alias
    typedef ModellingHypothesis::Hypothesis Hypothesis;

    /*!
     * set if dynamically allocated arrays are allowed
     * \param[in] b : boolean
     */
    virtual void
    allowDynamicallyAllocatedArrays(const bool) = 0;

    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator) = 0;
    /*!
     * \return true if the interface handles the given modelling hypothesis
     * \param[in] h  : modelling hypothesis
     * \param[in] mb : mechanical behaviour description
     */
    virtual bool
    isModellingHypothesisHandled(const Hypothesis,
				 const BehaviourDescription&) const = 0;
    
    virtual std::set<Hypothesis>
    getModellingHypothesesToBeTreated(const BehaviourDescription&) const = 0;

    /*!
     * write interface specific includes
     * \param[in] out : output file
     * \param[in] mb  : mechanical behaviour description
     */
    virtual void 
    writeInterfaceSpecificIncludes(std::ofstream&,
				   const BehaviourDescription&) const = 0;

    virtual void 
    exportMechanicalData(std::ofstream&,
			 const Hypothesis,
			 const BehaviourDescription&) const = 0;
    /*!
     * write the behaviour constructor associated with the law
     * \param[in] behaviourFile           : output file
     * \param[in] mb                      : mechanical behaviour description
     * \param[in] initStateVarsIncrements : constructor part assigning
     *                                      default value (zero) to state
     *                                      variable increments
     */
    virtual void 
    writeBehaviourConstructor(std::ofstream&,
			      const BehaviourDescription&,
			      const std::string&) const = 0;
    
    virtual void
    writeBehaviourDataConstructor(std::ofstream&,
				  const Hypothesis,
				  const BehaviourDescription&) const = 0;
    /*!
     * write the behaviour constructor associated with the law
     * \param[in] behaviourFile : output file
     * \param[in] mb            : mechanical behaviour description
     */
    virtual void 
    writeBehaviourDataMainVariablesSetters(std::ofstream&,
					   const BehaviourDescription&) const = 0;
    
    virtual void 
    writeIntegrationDataConstructor(std::ofstream&,
				    const Hypothesis,
				    const BehaviourDescription&) const = 0;
    /*!
     * write the behaviour constructor associated with the law
     * \param[in] behaviourFile : output file
     * \param[in] mb            : mechanical behaviour description
     */
    virtual void 
    writeIntegrationDataMainVariablesSetters(std::ofstream&,
					     const BehaviourDescription&) const = 0;
    /*!
     * \brief write output files
     * \param[in] mb : mechanical behaviour description
     * \param[in] fd : mfront file description
     */
    virtual void
    endTreatement(const BehaviourDescription&,
		  const FileDescription&) const = 0;
    /*!
     * \param[out] d  : target description
     * \param[out] bd : behaviour description
     */
    virtual void getTargetsDescription(TargetsDescription&,
				       const BehaviourDescription&) = 0;
    //! destructor
    virtual ~AbstractBehaviourInterface();

  }; // end of AbstractBehaviourInterface

} // end of namespace mfront  

#endif /* LIB_MFRONTBEHAVIOURVIRTUALINTERFACE_HXX_ */