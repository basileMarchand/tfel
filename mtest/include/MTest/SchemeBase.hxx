/*!
 * \file   mtest/include/MTest/SchemeBase.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   02 oct. 2015
 */

#ifndef _LIB_MTEST_SCHEMEBASE_HXX_
#define _LIB_MTEST_SCHEMEBASE_HXX_

#include<vector>
#include<string>

#include"TFEL/Material/OutOfBoundsPolicy.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"
#include"TFEL/Material/MechanicalBehaviour.hxx"

#include"MTest/Types.hxx"
#include"MTest/Config.hxx"
#include"MTest/Scheme.hxx"

namespace mtest{

  // forward declaration
  struct Behaviour;
  // forward declaration
  struct AccelerationAlgorithm;
  
  struct MTEST_VISIBILITY_EXPORT SchemeBase
    : public Scheme
  {
    /*!
     * \brief possible algorithms used for global convergence to
     * update the stiffness matrix
     */
    enum StiffnessUpdatingPolicy{
      /*!
       * the initial prediction matrix or the initial tangent coherent
       * matrix or the initial secant matrix (normally equal to the
       * elastic stiffness) is used for all iterations
       */
      CONSTANTSTIFFNESS,
      /*!
       * the prediction matrix or the first tangent coherent matrix or
       * the first secant matrix is used for all iterations and
       * updated and factorized at at periods
       */
      CONSTANTSTIFFNESSBYPERIOD,
      /*!
       * the tangent coherent matrix or the secant matrix is computed
       * and factorized at each iteration
       */
      UPDATEDSTIFFNESSMATRIX,
      //! default value
      UNSPECIFIEDSTIFFNESSUPDATINGPOLICY
    }; // end of enum StiffnessUpdatingPolicy
    /*!
     * \brief possible prediction policies
     */
    enum PredictionPolicy{
      NOPREDICTION,
      LINEARPREDICTION,
      ELASTICPREDICTION,
      ELASTICPREDICTIONFROMMATERIALPROPERTIES,
      SECANTOPERATORPREDICTION,
      TANGENTOPERATORPREDICTION,
      UNSPECIFIEDPREDICTIONPOLICY
    }; // end of enum PredictionPolicy
    /*!
     * \param[in] t : times
     */
    virtual void
    setTimes(const std::vector<real>&);
    /*!
     * \brief set the description
     * \param[in] d : description
     */
    virtual void setDescription(const std::string&);
    /*!
     * \brief set the author
     * \param[in] a : author
     */
    virtual void setAuthor(const std::string&);
    /*!
     * \brief set the out of bounds policy
     * \param[in] a : out of bounds policy
     */
    virtual void setOutOfBoundsPolicy(const tfel::material::OutOfBoundsPolicy);
    /*!
     * \brief set the date
     * \param[in] d : date
     */
    virtual void setDate(const std::string&);
    /*!
     * \brief set the behaviour
     * \param[in] i : interface
     * \param[in] l : library name
     * \param[in] f : function
     */
    virtual void
    setBehaviour(const std::string&,
		 const std::string&,
		 const std::string&);
    /*!
     * \param[in] h : modelling hypothesis
     */
    virtual void
    setModellingHypothesis(const std::string&);
    virtual void setMaximumNumberOfIterations(const unsigned int);
    virtual void setMaximumNumberOfSubSteps(const unsigned int);
    //! \return the dimension
    virtual unsigned short
    getDimension(void) const;
    //! \return the modelling hypothesis
    virtual tfel::material::ModellingHypothesis::Hypothesis
    getModellingHypothesis(void) const;
    //! \return the behaviour type
    virtual tfel::material::MechanicalBehaviourBase::BehaviourType
    getBehaviourType(void) const;
    //! \return the behaviour
    virtual std::shared_ptr<Behaviour>
    getBehaviour(void);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter value
     */
    virtual void
    setParameter(const std::string&,
		 const double);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter value
     */
    virtual void
    setIntegerParameter(const std::string&,
			const int);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter value
     */
    virtual void
    setUnsignedIntegerParameter(const std::string&,
				const unsigned int);
    /*!
     * \brief set the stiffness updating policy
     * \param[in] b : boolean
     */
    virtual void
    setStiffnessUpdatingPolicy(const StiffnessUpdatingPolicy);
    /*!
     * \brief set the prediction policy
     * \param[in] p : prediction policy
     */
    virtual void setPredictionPolicy(const PredictionPolicy);
    /*!
     * \brief set the stiffness matrix type
     * \param[in] k : stiffness matrix type
     */
    virtual void setStiffnessMatrixType(const StiffnessMatrixType::mtype);
    /*!
     * \brief set the use of the castem acceleration algorithm
     * \param[in] b : boolean
     */
    virtual void setUseCastemAccelerationAlgorithm(const bool);
    /*!
     * \brief set the acceleration algorithm to be used
     * \param[in] a : acceleration algorithm
     */
    virtual void setAccelerationAlgorithm(const std::string&);
    /*!
     * \brief set a parameter of the acceleration algorithm
     * \param[in] p : parameter name
     * \param[in] v : parameter value
     */
    virtual void setAccelerationAlgorithmParameter(const std::string&,
						   const std::string&);
    /*!
     * \brief set at which iteration the use of the castem
     * acceleration algorithm  will begin
     * \param[in] i : iteration number
     */
    virtual void setCastemAccelerationTrigger(const int);
    /*!
     * \brief set at which period the use of the castem
     * acceleration algorithm  will take place
     * \param[in] p : period
     */
    virtual void setCastemAccelerationPeriod(const int);
    //! destructor
    virtual ~SchemeBase();
  protected:
    /*!
     * \brief declare a new variable
     * \param[in] v : variable name
     */
    void declareVariable(const std::string&,
			 const bool);
    /*!
     * \brief declare a list of new variables
     * \param[in] v : variable names
     */
    void declareVariables(const std::vector<std::string>&,
			  const bool);
    //! \brief set the modelling hypothesis to the default one
    virtual void setDefaultHypothesis(void) = 0;
    //! times
    std::vector<real> times;
    //! list of internal variable names, including their suffixes
    std::vector<std::string> ivfullnames;
    //! declared variable names
    std::vector<std::string> vnames;
    //! the mechanical behaviour
    std::shared_ptr<Behaviour> b;
    //! description of the test
    std::string description;
    //! author
    std::string author;
    //! date
    std::string date;
    //! modelling hypothesis
    tfel::material::ModellingHypothesis::Hypothesis hypothesis = 
      tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    //! dimension used for the computation
    unsigned short dimension = 0u;
    //! stiffness updating policy
    StiffnessUpdatingPolicy ks = UNSPECIFIEDSTIFFNESSUPDATINGPOLICY;
    //! type of stifness matrix to use for the resolution
    StiffnessMatrixType::mtype ktype =
      StiffnessMatrixType::UNSPECIFIEDSTIFFNESSMATRIXTYPE;
    //! use a prediction matrix before beginning the resolution
    PredictionPolicy ppolicy = UNSPECIFIEDPREDICTIONPOLICY;
    //! acceleration algorithm
    std::shared_ptr<AccelerationAlgorithm> aa;
    //! maximum number of sub steps allowed
    int mSubSteps = -1;
    //! maximum number of iterations allowed in the Newton-Raphson algorithm
    int iterMax = -1;
    //! use castem acceleration
    bool useCastemAcceleration = false;
    //! initilisation stage
    bool initialisationFinished = false;
  }; // end of struct SchemeBase
  
} // end of namespace mtest

#endif /* _LIB_MTEST_SCHEMEBASE_HXX_ */
