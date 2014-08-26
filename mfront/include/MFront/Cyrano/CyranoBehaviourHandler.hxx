/*! 
 * \file  CyranoBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \date   21 fév 2014
 */

#ifndef _LIB_MFRONT_CYRANO_CYRANOBEHAVIOURHANDLER_H_
#define _LIB_MFRONT_CYRANO_CYRANOBEHAVIOURHANDLER_H_ 

#ifndef _LIB_MFRONT_CYRANO_CALL_H_
#error "This header shall not be called directly"
#endif

#include<algorithm>
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/Utilities/Name.hxx"

namespace cyrano
{
  
  /*!
   * structure in charge of calling the behaviour integrate method.
   * This structure handles two cases wether or not we shall handle
   * local substepping.
   */
  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,
		    typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL CyranoBehaviourHandler
    :public CyranoInterfaceBase
  {
    
    /*!
     * structure in charge of checking the thermal expansion
     * coefficient is zero
     */
    struct CheckThermalExpansionCoefficientIsNull
      : public CyranoInterfaceBase
    {
      /*!
       * \param[in] a : thermal expansion coefficient
       */
      TFEL_CYRANO_INLINE static
      void exe(const CyranoReal a)
      {
	using std::abs;
	using std::numeric_limits;
	using tfel::utilities::Name;
	typedef Behaviour<H,CyranoReal,false> BV;
	if(abs(a)>numeric_limits<CyranoReal>::min()){
	  CyranoInterfaceBase::throwThermalExpansionCoefficientShallBeNull(Name<BV>::getName());
	}
      }
    };
    /*!
     * structure in charge of not checking the thermal expansion
     * coefficient is zero
     */
    struct DontCheckThermalExpansionCoefficientIsNull
      : public CyranoInterfaceBase
    {
      /*!
       * \param[in] a : thermal expansion
       */
      TFEL_CYRANO_INLINE static
      void exe(const CyranoReal)
      {} // end of exe
    };
    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL DrivingVariableInitialiserWithStressFreeExpansion
      : public CyranoInterfaceBase
    {
      //! a simple alias
      typedef Behaviour<H,CyranoReal,false> BV;
      //! a simple alias for the behaviour data
      typedef typename BV::BehaviourData  BData;
      //! a simple alias for the integration data
      typedef typename BV::IntegrationData  IData;
      //! a simple alias
      typedef tfel::material::ModellingHypothesisToSpaceDimension<H> ModellingHypothesisToSpaceDimension;
      // spatial dimension
      static const unsigned short N = ModellingHypothesisToSpaceDimension::value;
      /*!
       * \param[out] bData  : behaviour data
       * \param[out] iData  : integration data
       * \param[in]  b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       */
      TFEL_CYRANO_INLINE static 
      void exe(BData& bData,
	       IData& iData,
	       const CyranoReal *const STRAN,
	       const CyranoReal *const DSTRAN)
      {
	using tfel::utilities::Name;
	using tfel::fsalgo::copy;
	using std::pair;
	typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
	CyranoReal eto[3];
	CyranoReal deto[3];
	// creating a fake behaviour to compute the stress-free expansion
	// this is not really elegant by can't do better
	pair<StressFreeExpansionType,StressFreeExpansionType> s;
	BV b(bData,iData);
	b.computeStressFreeExpansion(s);
	const StressFreeExpansionType& s0 = s.first;
	const StressFreeExpansionType& s1 = s.second;
	// convert to MFront conventions
	eto[0]  = STRAN[0]  - s0[0];
	eto[1]  = STRAN[2]  - s0[1];
	eto[2]  = STRAN[1]  - s0[2];
	deto[0] = DSTRAN[0] - (s1[0]-s0[0]);
	deto[1] = DSTRAN[2] - (s1[1]-s0[1]);
	deto[2] = DSTRAN[1] - (s1[2]-s0[2]);
	bData.setCYRANOBehaviourDataDrivingVariables(eto);
	iData.setCYRANOIntegrationDataDrivingVariables(deto);
      } // end of exe
      /*!
       * \param[out] b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       */
      TFEL_CYRANO_INLINE static 
      void exe(BV& b,
	       const CyranoReal *const STRAN,
	       const CyranoReal *const DSTRAN)
      {
	using tfel::utilities::Name;
	using tfel::fsalgo::copy;
	using std::pair;
	typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
	CyranoReal eto[3];
	CyranoReal deto[3];
	pair<StressFreeExpansionType,StressFreeExpansionType> s;
	b.computeStressFreeExpansion(s);
	const StressFreeExpansionType& s0 = s.first;
	const StressFreeExpansionType& s1 = s.second;
	// convert to MFront conventions
	eto[0]  = STRAN[0]  - s0[0];
	eto[1]  = STRAN[2]  - s0[1];
	eto[2]  = STRAN[1]  - s0[2];
	deto[0] = DSTRAN[0] - (s1[0]-s0[0]);
	deto[1] = DSTRAN[2] - (s1[1]-s0[1]);
	deto[2] = DSTRAN[1] - (s1[2]-s0[2]);
	b.setCYRANOBehaviourDataDrivingVariables(eto);
	b.setCYRANOIntegrationDataDrivingVariables(deto);
      } // end of exe

    }; // end of struct DrivingVariableInitialiserWithStressFreeExpansion

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL DrivingVariableInitialiserWithoutStressFreeExpansion
    {
      //! a simple alias
      typedef Behaviour<H,CyranoReal,false> BV;
      //! a simple alias for the behaviour data
      typedef typename BV::BehaviourData  BData;
      //! a simple alias for the integration data
      typedef typename BV::IntegrationData  IData;
      /*!
       * \param[out] bData  : behaviour data
       * \param[out] iData  : integration data
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       */
      TFEL_CYRANO_INLINE static 
      void exe(BData& bData,
	       IData& iData,
	       const CyranoReal *const STRAN,
	       const CyranoReal *const DSTRAN)
      {
	CyranoReal eto[3];
	CyranoReal deto[3];
	eto[0]=STRAN[0];
	eto[1]=STRAN[2];
	eto[2]=STRAN[1];
	deto[0]=DSTRAN[0];
	deto[1]=DSTRAN[2];
	deto[2]=DSTRAN[1];
	bData.setCYRANOBehaviourDataDrivingVariables(eto);
	iData.setCYRANOIntegrationDataDrivingVariables(deto);
      } // end of exe
      /*!
       * \param[out] b      : b
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                     time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       */
      TFEL_CYRANO_INLINE static 
      void exe(BV& b,
	       const CyranoReal *const STRAN,
	       const CyranoReal *const DSTRAN)
      {
	CyranoReal eto[3];
	CyranoReal deto[3];
	eto[0]=STRAN[0];
	eto[1]=STRAN[2];
	eto[2]=STRAN[1];
	deto[0]=DSTRAN[0];
	deto[1]=DSTRAN[2];
	deto[2]=DSTRAN[1];
	b.setCYRANOBehaviourDataDrivingVariables(eto);
	b.setCYRANOIntegrationDataDrivingVariables(deto);
      } // end of exe
    }; // end of struct DrivingVariableInitialiserWithoutStressFreeExpansion

    /*!
     * An helper structure which is used to compute the stiffness
     * tensor for the behaviour that requires it.
     */
    struct TFEL_VISIBILITY_LOCAL StiffnessOperatorInitializer
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_CYRANO_INLINE static void
	exe(BData& data,const CyranoReal * const props){
	CyranoComputeStiffnessOperator<H,CyranoTraits<BV>::stype>::exe(props,
								       data.getStiffnessOperator());
      } // end of exe
    }; // end of struct StiffnessOperatorInitializer
    
    /*!
     * An helper structure which is used to compute the thermal
     * expansion tensor for the behaviour that requires it.
     */
    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_CYRANO_INLINE static void
	exe(BData& data,const CyranoReal * const props){
	CyranoComputeThermalExpansionCoefficientTensor<H,CyranoTraits<BV>::stype>::exe(props,
										       data.getThermalExpansionCoefficientTensor());
      } // end of exe
    }; // end of struct ThermalExpansionCoefficientTensorInitializer
    
    /*!
     * an helper class which don't do any initialisation
     */
    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_CYRANO_INLINE static void
	exe(BData&,const CyranoReal * const)
	{}
    }; // end of struct DoNothingInitializer
    
    /*!
     * A helper structure used to handle the case where the behaviour
     * is not defined.
     *
     * For example, if the behaviour is only defined in 1D, this
     * structure will be used if the user tries to use it in 2D or 3D.
     */
    struct TFEL_VISIBILITY_LOCAL Error
    {
      
      TFEL_CYRANO_INLINE Error(const CyranoReal *const,
			       const CyranoReal *const,
			       const CyranoReal *const,
			       const CyranoReal *const,
			       const CyranoReal *const,
			       const CyranoReal *const,
			       const CyranoReal *const,
			       const CyranoReal *const,
			       const CyranoReal *const,
			       const CyranoReal *const)
      {} // end of Error
      
      TFEL_CYRANO_INLINE void exe(CyranoReal *const,
				  CyranoReal *const,
				  CyranoReal *const)
	throw(CyranoException)
	{
	  using namespace std;
	  using namespace tfel::material;
	  using namespace tfel::utilities;
	  throw(CyranoInvalidDimension(Name<Behaviour<H,CyranoReal,false> >::getName(),1u));
	  return;
	} // end of Error::exe
      
    }; // end of struct Error

    template<const bool bs,     // requires StiffnessOperator
	     const bool ba>     // requires ThermalExpansionCoefficientTensor
      struct TFEL_VISIBILITY_LOCAL IntegratorWithTimeStepping
    {
      //! A simple alias
      typedef typename tfel::meta::IF<bs,
	StiffnessOperatorInitializer,
	DoNothingInitializer>::type SInitializer;
      //! A simple alias
      typedef typename tfel::meta::IF<ba,
				      ThermalExpansionCoefficientTensorInitializer,
				      DoNothingInitializer>::type AInitializer;
      
      TFEL_CYRANO_INLINE
	IntegratorWithTimeStepping(const CyranoReal *const DTIME ,
				   const CyranoReal *const STRAN ,
				   const CyranoReal *const DSTRAN,
				   const CyranoReal *const TEMP  ,
				   const CyranoReal *const DTEMP,
				   const CyranoReal *const PROPS ,
				   const CyranoReal *const PREDEF,
				   const CyranoReal *const DPRED,
				   CyranoReal *const STATEV,
				   CyranoReal *const STRESS)
	: bData(TEMP,PROPS+CyranoTraits<BV>::propertiesOffset,
		STATEV,PREDEF),
	iData(DTIME,DTEMP,DPRED),
	dt(*DTIME)
	{
	  using namespace tfel::material;
	  typedef MechanicalBehaviourTraits<BV> Traits;
	  typedef typename tfel::meta::IF<
	    Traits::hasStressFreeExpansion,
	    DrivingVariableInitialiserWithStressFreeExpansion,
	    DrivingVariableInitialiserWithoutStressFreeExpansion>::type DVInitializer;
	  SInitializer::exe(this->bData,PROPS);
	  AInitializer::exe(this->bData,PROPS);
	  SInitializer::exe(this->bData,PROPS);
	  AInitializer::exe(this->bData,PROPS);
	  DVInitializer::exe(this->bData,this->iData,STRAN,DSTRAN);
	  // initial stress
	  CyranoReal sig[3];
	  // turning to MFront conventions
	  sig[0]=STRESS[0];
	  sig[1]=STRESS[2];
	  sig[2]=STRESS[1];
	  this->bData.setCYRANOBehaviourDataThermodynamicForces(sig);
	} // end of IntegratorWithTimeStepping
      
      TFEL_CYRANO_INLINE2 void
	exe(CyranoReal *const DDSOE,
	    CyranoReal *const STRESS,
	    CyranoReal *const STATEV)
	{
	  using namespace tfel::utilities;
	  using namespace tfel::material;
	  typedef MechanicalBehaviourTraits<BV> Traits;
	  typedef typename tfel::meta::IF<
	    Traits::hasConsistantTangentOperator,
	    typename tfel::meta::IF<
	    Traits::isConsistantTangentOperatorSymmetric,
	    SymmetricConsistantTangentOperatorComputer,
	    GeneralConsistantTangentOperatorComputer>::type,
	    ConsistantTangentOperatorIsNotAvalaible
	    >::type ConsistantTangentOperatorHandler;
	  typedef typename tfel::meta::IF<
	    Traits::hasPredictionOperator,
	    StandardPredictionOperatorComputer,
	    PredictionOperatorIsNotAvalaible
	    >::type PredictionOperatorComputer;
	  const CyranoOutOfBoundsPolicy& up = CyranoOutOfBoundsPolicy::getCyranoOutOfBoundsPolicy();
	  CyranoReal sig[3];
	  unsigned short subSteps   = 0u;
	  unsigned short iterations = 1u;
	  if(this->dt<0.){
	    throwNegativeTimeStepException(Name<Behaviour<H,CyranoReal,false> >::getName());
	  }
	  while((iterations!=0)&&
		(subSteps!=CyranoTraits<BV>::maximumSubStepping)){
	    BV behaviour(this->bData,this->iData);
	    behaviour.initialize();
	    behaviour.setOutOfBoundsPolicy(up.getOutOfBoundsPolicy());
	    behaviour.checkBounds();
	    typename BV::IntegrationResult r = BV::SUCCESS;
	    try{
	      if((-3.25<*DDSOE)&&(*DDSOE<-2.75)){
		r = PredictionOperatorComputer::exe(behaviour,BV::TANGENTOPERATOR);
	      } else if((-2.25<*DDSOE)&&(*DDSOE<-1.75)){
		r = PredictionOperatorComputer::exe(behaviour,BV::SECANTOPERATOR);
	      } else if((-1.25<*DDSOE)&&(*DDSOE<-0.75)){
		r = PredictionOperatorComputer::exe(behaviour,BV::ELASTIC);
	      } else if((-0.25<*DDSOE)&&(*DDSOE<0.25)){
		r = behaviour.integrate(BV::NOSTIFFNESSREQUESTED);
	      } else if((0.75<*DDSOE)&&(*DDSOE<1.25)){
		r = behaviour.integrate(BV::ELASTIC);
	      } else if((1.75<*DDSOE)&&(*DDSOE<2.25)){
		r = behaviour.integrate(BV::SECANTOPERATOR);
	      } else if((2.75<*DDSOE)&&(*DDSOE<3.25)){
		r = behaviour.integrate(BV::TANGENTOPERATOR);
	      } else if((3.75<*DDSOE)&&(*DDSOE<4.25)){
		r = behaviour.integrate(BV::CONSISTANTTANGENTOPERATOR);
	      } else {
		throwInvalidDDSOEException(Name<BV>::getName(),*DDSOE);
	      }
	      if(r==BV::FAILURE){
		if(*DDSOE<-0.5){
		  throwPredictionComputationFailedException(Name<BV>::getName());
		}
	      }
	    }
	    catch(const tfel::material::DivergenceException& e){
#ifdef MFRONT_CYRANO_VERBOSE
	      std::cerr << "no convergence : " << e.what() << std::endl;
#endif
	      r = BV::FAILURE;
	    }
	    if((r==BV::SUCCESS)||((r==BV::UNRELIABLE_RESULTS)&&
				  (!CyranoTraits<BV>::doSubSteppingOnInvalidResults))){
	      --(iterations);
	      behaviour.checkBounds();
	      behaviour.updateExternalStateVariables();
	      this->bData = static_cast<const BData&>(behaviour);
	      if(iterations==0){
		if((*DDSOE>0.5)||(*DDSOE<-0.5)){
		  ConsistantTangentOperatorHandler::exe(behaviour,DDSOE);
		}
	      }
	    } else if ((r==BV::UNRELIABLE_RESULTS)&&
		       (CyranoTraits<BV>::doSubSteppingOnInvalidResults)){
	      iterations = static_cast<unsigned short>(iterations*2u);
	      this->iData *= 0.5;
	    } else {
	      ++subSteps;
	      iterations = static_cast<unsigned short>(iterations*2u);
	      this->iData *= 0.5;
	    }
	  }
	  if((subSteps==CyranoTraits<BV>::maximumSubStepping)&&(iterations!=0)){
	    throwMaximumNumberOfSubSteppingReachedException(Name<Behaviour<H,CyranoReal,false> >::getName());
	  }
	  this->bData.CYRANOexportStateData(STRESS,STATEV);
	  STRESS[0]=sig[0];
	  STRESS[1]=sig[2];
	  STRESS[2]=sig[1];
	} // end of IntegratorWithTimeStepping::exe

    private:
      typedef Behaviour<H,CyranoReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      typedef typename BV::IntegrationData  IData;

      BData bData;
      IData iData;
      CyranoReal dt;
      tfel::material::ModellingHypothesis::Hypothesis hypothesis;
	
    }; // end of struct IntegratorWithTimeStepping

    template<const bool bs,     // requires StiffnessOperator
	     const bool ba>     // requires ThermalExpansionCoefficientTensor
      struct TFEL_VISIBILITY_LOCAL Integrator
    {
      typedef typename tfel::meta::IF<bs,
				      StiffnessOperatorInitializer,
				      DoNothingInitializer>::type SInitializer;

      typedef typename tfel::meta::IF<ba,
				      ThermalExpansionCoefficientTensorInitializer,
				      DoNothingInitializer>::type AInitializer;

      TFEL_CYRANO_INLINE Integrator(const CyranoReal *const DTIME ,
				    const CyranoReal *const STRAN ,
				    const CyranoReal *const DSTRAN,
				    const CyranoReal *const TEMP,
				    const CyranoReal *const DTEMP,
				    const CyranoReal *const PROPS ,
				    const CyranoReal *const PREDEF,
				    const CyranoReal *const DPRED,
				    const CyranoReal *const STATEV,
				    const CyranoReal *const STRESS)
	: behaviour(DTIME,TEMP,DTEMP,
		    PROPS+CyranoTraits<BV>::propertiesOffset,
		    STATEV,PREDEF,DPRED),
	dt(*DTIME)
	{
	  using namespace tfel::material;
	  typedef MechanicalBehaviourTraits<BV> Traits;
	  typedef typename tfel::meta::IF<
	    Traits::hasStressFreeExpansion,
	    DrivingVariableInitialiserWithStressFreeExpansion,
	    DrivingVariableInitialiserWithoutStressFreeExpansion>::type DVInitializer;
	  const CyranoOutOfBoundsPolicy& up = CyranoOutOfBoundsPolicy::getCyranoOutOfBoundsPolicy();
	  this->behaviour.setOutOfBoundsPolicy(up.getOutOfBoundsPolicy());
	  // elastic tensor
	  SInitializer::exe(this->behaviour,PROPS);
	  // thermal expansion coefficient
	  AInitializer::exe(this->behaviour,PROPS);
	  // initialising driving variables
	  DVInitializer::exe(this->behaviour,STRAN,DSTRAN);
	  // initalising stresses
	  CyranoReal sig[3];
	  // convert to MFront conventions
	  sig[0]=STRESS[0];
	  sig[1]=STRESS[2];
	  sig[2]=STRESS[1];
	  this->behaviour.setCYRANOBehaviourDataThermodynamicForces(sig);
	  // initialise the behaviour
	  this->behaviour.initialize();
	} // end of Integrator::Integrator
	
      TFEL_CYRANO_INLINE2
	void exe(CyranoReal *const DDSOE,
		 CyranoReal *const STRESS,
		 CyranoReal *const STATEV)
	{
	  using namespace tfel::utilities;
	  using namespace tfel::material;
	  typedef MechanicalBehaviourTraits<BV> Traits;
	  typedef typename tfel::meta::IF<
	    Traits::hasConsistantTangentOperator,
	    typename tfel::meta::IF<
	      Traits::isConsistantTangentOperatorSymmetric,
	      SymmetricConsistantTangentOperatorComputer,
	      GeneralConsistantTangentOperatorComputer>::type,
	    ConsistantTangentOperatorIsNotAvalaible
	    >::type ConsistantTangentOperatorHandler;
	  typedef typename tfel::meta::IF<
	    Traits::hasPredictionOperator,
	    StandardPredictionOperatorComputer,
	    PredictionOperatorIsNotAvalaible
	    >::type PredictionOperatorComputer;
	  CyranoReal sig[3];
	  if(this->dt<0.){
	    throwNegativeTimeStepException(Name<BV>::getName());
	  }
	  behaviour.checkBounds();
	  typename BV::IntegrationResult r = BV::SUCCESS;
	  if((-3.25<*DDSOE)&&(*DDSOE<-2.75)){
	    r = PredictionOperatorComputer::exe(this->behaviour,BV::TANGENTOPERATOR);
	  } else if((-2.25<*DDSOE)&&(*DDSOE<-1.75)){
	    r = PredictionOperatorComputer::exe(this->behaviour,BV::SECANTOPERATOR);
	  } else if((-1.25<*DDSOE)&&(*DDSOE<-0.75)){
	    r = PredictionOperatorComputer::exe(this->behaviour,BV::ELASTIC);
	  } else if((-0.25<*DDSOE)&&(*DDSOE<0.25)){
	    r = this->behaviour.integrate(BV::NOSTIFFNESSREQUESTED);
	  } else if((0.75<*DDSOE)&&(*DDSOE<1.25)){
	    r = this->behaviour.integrate(BV::ELASTIC);
	  } else if((1.75<*DDSOE)&&(*DDSOE<2.25)){
	    r = this->behaviour.integrate(BV::SECANTOPERATOR);
	  } else if((2.75<*DDSOE)&&(*DDSOE<3.25)){
	    r = this->behaviour.integrate(BV::TANGENTOPERATOR);
	  } else if((3.75<*DDSOE)&&(*DDSOE<4.25)){
	    r = this->behaviour.integrate(BV::CONSISTANTTANGENTOPERATOR);
	  } else {
	    throwInvalidDDSOEException(Name<BV>::getName(),*DDSOE);
	  }
	  if(r==BV::FAILURE){
	    // Il manque un vraie gestion locale de résultats imprécis
	    if(*DDSOE<-0.5){
	      throwPredictionComputationFailedException(Name<BV>::getName());
	    } else {
	      throwBehaviourIntegrationFailedException(Name<BV>::getName());
	    }
	  }
	  // if(this->behaviour.integrate(BV::NOSTIFFNESSREQUESTED)==BV::FAILURE){
	  //   throwBehaviourIntegrationFailedException(Name<BV>::getName());
	  // }
	  behaviour.checkBounds();
	  this->behaviour.CYRANOexportStateData(sig,STATEV);
	  STRESS[0]=sig[0];
	  STRESS[1]=sig[2];
	  STRESS[2]=sig[1];
	  if((*DDSOE>0.5)||(*DDSOE<-0.5)){
	    ConsistantTangentOperatorHandler::exe(this->behaviour,DDSOE);
	  }
	} // end of Integrator::exe
	
    private:
      typedef Behaviour<H,CyranoReal,false> BV;
      BV behaviour;
      CyranoReal dt;
    }; // end of struct Integrator

    struct StandardPredictionOperatorComputer
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      static typename BV::IntegrationResult
      exe(BV& b,const typename BV::SMType smt)
      {
	return b.computePredictionOperator(smt);
      } // end of exe	  
    };

    struct PredictionOperatorIsNotAvalaible
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      static typename BV::IntegrationResult
      exe(BV&,const typename BV::SMType)
      {
	using namespace tfel::utilities;
	throwPredictionOperatorIsNotAvalaible(Name<BV>::getName());
	return BV::FAILURE;
      } // end of exe	  
    };
      
    struct ConsistantTangentOperatorIsNotAvalaible
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      static void exe(BV&,CyranoReal *const)
      {
	using namespace tfel::utilities;
	throwConsistantTangentOperatorIsNotAvalaible(Name<BV>::getName());
      } // end of exe	  
    };

    struct SymmetricConsistantTangentOperatorComputer
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      static void exe(const BV& bv,CyranoReal *const DDSOE)
      {
	using namespace tfel::math;
	st2tost2<1u,CyranoReal>& Kt = *(reinterpret_cast<st2tost2<1u,CyranoReal>*>(DDSOE));
	const st2tost2<1u,CyranoReal>& Dt = bv.getTangentOperator();
	// conversion vers les conventions cyrano
	Kt(0,0)=Dt(0,0);
	Kt(1,0)=Dt(2,0);
	Kt(2,0)=Dt(1,0);
	Kt(0,1)=Dt(0,2);
	Kt(1,1)=Dt(2,2);
	Kt(2,1)=Dt(1,2);
	Kt(0,2)=Dt(0,1);
	Kt(1,2)=Dt(2,1);
	Kt(2,2)=Dt(1,1);
      } // end of exe	  
    };

    struct GeneralConsistantTangentOperatorComputer
    {
      typedef Behaviour<H,CyranoReal,false> BV;
      static void exe(const BV& bv,CyranoReal *const DDSOE)
      {
	using namespace tfel::math;
	st2tost2<1u,CyranoReal>& Kt = *(reinterpret_cast<st2tost2<1u,CyranoReal>*>(DDSOE));
	const st2tost2<1u,CyranoReal>& Dt = bv.getTangentOperator();
	// conversion vers les conventions cyrano
	Kt(0,0)=Dt(0,0);
	Kt(1,0)=Dt(2,0);
	Kt(2,0)=Dt(1,0);
	Kt(0,1)=Dt(0,2);
	Kt(1,1)=Dt(2,2);
	Kt(2,1)=Dt(1,2);
	Kt(0,2)=Dt(0,1);
	Kt(1,2)=Dt(2,1);
	Kt(2,2)=Dt(1,1);
	// les conventions fortran....
	std::swap(Kt(0,1),Kt(1,0));
	std::swap(Kt(0,2),Kt(2,0));
	std::swap(Kt(1,2),Kt(2,1));
      } // end of exe	  
    };

    TFEL_CYRANO_INLINE2 static void
      checkNPROPS(const CyranoInt NPROPS)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::material;
      typedef Behaviour<H,CyranoReal,false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      const unsigned short offset  = CyranoTraits<BV>::propertiesOffset;
      const unsigned short nprops  = Traits::material_properties_nb;
      const unsigned short NPROPS_ = offset+nprops == 0 ? 1u : offset+nprops; 
      const bool is_defined_       = Traits::is_defined;
      //Test if the nb of properties matches Behaviour requirements
      if((NPROPS!=NPROPS_)&&is_defined_){
	throwUnMatchedNumberOfMaterialProperties(Name<Behaviour<H,CyranoReal,false> >::getName(),
						 NPROPS_,NPROPS);
      }
    } // end of checkNPROPS
      
    TFEL_CYRANO_INLINE2 static void
      checkNSTATV(const CyranoInt NSTATV)
    {
      using namespace tfel::utilities;
      typedef Behaviour<H,CyranoReal,false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short nstatv  = Traits::internal_variables_nb;
      const unsigned short NSTATV_ = nstatv == 0 ? 1u : nstatv;
      const bool is_defined_       = Traits::is_defined;
      //Test if the nb of state variables matches Behaviour requirements
      if((NSTATV_!=NSTATV)&&is_defined_){
	throwUnMatchedNumberOfStateVariables(Name<Behaviour<H,CyranoReal,false> >::getName(),
					     NSTATV_,NSTATV);
      }
    } // end of checkNSTATV
      
  }; // end of struct CyranoBehaviourHandler
  
} // end of namespace cyrano  

#include"MFront/Cyrano/CyranoIsotropicBehaviourHandler.hxx"
#include"MFront/Cyrano/CyranoOrthotropicBehaviourHandler.hxx"

#endif /* _LIB_MFRONT_CYRANO_CYRANOBEHAVIOURHANDLER_H */
