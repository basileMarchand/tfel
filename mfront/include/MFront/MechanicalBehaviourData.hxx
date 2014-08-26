/*!
 * \file   MechanicalBehaviourData.hxx
 * \brief  This file declares the MechanicalBehaviourData class
 * \author Helfer Thomas
 * \date   21 Mars 2014
 */

#ifndef _LIB_MFRONT_MECHANICALBEHAVIOURDATA_H_
#define _LIB_MFRONT_MECHANICALBEHAVIOURDATA_H_ 

#include<set>
#include<map>
#include<vector>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/SmartPtr.hxx"

#include"MFront/CodeBlock.hxx"
#include"MFront/VariableDescription.hxx"
#include"MFront/DrivingVariable.hxx"
#include"MFront/BoundsDescription.hxx"
#include"MFront/ThermodynamicForce.hxx"
#include"MFront/SupportedTypes.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/StaticVariableDescription.hxx"
#include"MFront/MechanicalBehaviourAttribute.hxx"

namespace mfront{

  /*!
   * This structure gathers various behaviour characteristic
   */
  struct TFEL_VISIBILITY_EXPORT MechanicalBehaviourData
    : private SupportedTypes
  {
    /*
     * normalised code block names
     * \note code block name begins with an upper case
     */
    //! standard code name
    static const std::string FlowRule;
    //! standard code name
    static const std::string BeforeInitializeLocalVariables;
    //! standard code name
    static const std::string InitializeLocalVariables;
    //! standard code name
    static const std::string AfterInitializeLocalVariables;
    //! standard code name
    static const std::string ComputePredictor;
    //! standard code name
    static const std::string ComputeStress;
    //! standard code name
    /* Most behaviours will only rely the @ComputeStress keyword to
     * estimate stresses at the middle of the time step and at the
     * end.  However, some finite strains behaviours must compute a
     * stress measure during the iterations which is not the Cauchy
     * stress. Thus, @ComputeStress also defines an
     * ComputeFinalStressCandidate code block which will be used if
     * the user does not provide an alternative through the
     * @ComputeFinalStress
     */
    static const std::string ComputeFinalStressCandidate;
    //! standard code name
    static const std::string ComputeFinalStress;
    //! standard code name
    static const std::string DefaultComputeFinalStress;
    //! standard code name
    static const std::string Integrator;
    //! standard code name
    static const std::string ComputeDerivative;
    //! standard code name
    static const std::string ComputePredictionOperator;
    //! standard code name
    static const std::string ComputeTangentOperator;
    //! standard code name
    static const std::string UpdateAuxiliaryStateVariables;
    //! standard code name
    static const std::string InitializeJacobian;

    /*
     * normalised attribute names
     * \note properties name begins with a lower case
     */
    //! if this attribute is true, the behaviour can compute a tangent
    //! operator
    static const std::string hasConsistentTangentOperator;
    //! if this attribute is true, the tangent operator can be
    //  considered symmetric
    static const std::string isConsistentTangentOperatorSymmetric;
    //! if this attribute is true, the behaviour can compute a
    //! furnish a time step scaling factor
    static const std::string hasTimeStepScalingFactor;
    //! if this attribute is true, the behaviour can compute a
    //! prediction operator
    static const std::string hasPredictionOperator;
    //! if this attribute is true, the implicit algorithm may use the
    //! Cast3M acceleration algorithm
    static const std::string useCast3MAccelerationAlgorithm;
    //! if this attribute is true, the implicit algorithm may use the
    //! Cast3M acceleration algorithm
    static const std::string useRelaxationAlgorithm;
    //! if this attribute is true, the implicit algorithm compares the
    //! analytical jacobian to the numeric one
    static const std::string compareToNumericalJacobian;
    //! a boolean attribute telling if the additionnal data can be declared
    static const std::string allowsNewUserDefinedVariables;
    //! a boolean attribute telling if profiling information shall be collected
    static const std::string profiling;
    //! algorithm used
    static const std::string algorithm;
    //! number of evaluation of the computeDerivative method 
    //  of a Runge-Kutta algorithm
    static const std::string numberOfEvaluations;

    /*!
     * mode used when inserting code
     */
    enum Mode
    {
      CREATE,               /*!< create a code block. Return an errur
			      if if fails */
      CREATEORREPLACE,      /*!< create a code block or replace 
			     *   it if it already exists. If the
			     *   code is to be replaced, all 
			     *   positions are cleard. This applies
			     *   to the whole code block and not only
			     *   to specific positions */
      CREATEORAPPEND,       /*!< create a code block or append 
			     *   a code block to an existing one */
      CREATEBUTDONTREPLACE  /*!< create a code block but don't replace 
			     *   it if it already exists. This applies
			     *   to the whole code block and not only
			     *   to specific positions. This option
			     *   is meant to be used internally and
			     *   is not accessible from a mfront file*/
    }; // end of enum Mode
    /*!
     * position of the inserted code
     */
    enum Position
    {
      AT_BEGINNING,
      BODY,
      AT_END   
    }; // end of enum Mode
    /*!
     * constructor
     */
    MechanicalBehaviourData();
    /*!
     * This method has been introduced to optimize the mechanical
     * resolution in licos : a purely implicit resolution only
     * evaluate external loadings at the end of the time step. This
     * implies that no increment of an external loading is used in the
     * behaviour law.
     */
    bool
    isUsableInPurelyImplicitResolution(void) const;
    /*!
     * \brief set if this behaviour can be used in a purely implicit
     * resolution.
     * \param[in] b : new value of the
     * usableInPurelyImplicitResolution member
     * \see isUsableInPurelyImplicitResolution for details about
     * purely implicit resolution.
     */
    void
    setUsableInPurelyImplicitResolution(const bool);
    /*!
     * \brief declares an external state variable to be probably
     * unusable in a purely implicit resolution. This means that its
     * increment is used somewhere.
     *
     * The term "probably" refers to the fact that it may happens that
     * the increment is only used to get the value of the external
     * state variable at a given date. For example, in a standard
     * resolution (non implicit), the value of \(T+\theta\,\Delta\,T\)
     * gives the temperature value at
     * \(t+\theta\,\Delta\,t\). However, this expression is still
     * valid in a purely implicit resolution.
     */
    void
    declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const std::string&);
    /*!
     * \brief get all the external state variables found to be
     * probably unusable in a purely implicit resolution.
     */
    const std::set<std::string>&
    getExternalStateVariablesDeclaredProbablyUnusableInPurelyImplicitResolution(void) const;

    const VariableDescriptionContainer&
    getMaterialProperties(void) const;

    const VariableDescriptionContainer&
    getPersistentVariables(void) const;

    const VariableDescriptionContainer&
    getIntegrationVariables(void) const;

    const VariableDescriptionContainer&
    getStateVariables(void) const;

    const VariableDescriptionContainer&
    getAuxiliaryStateVariables(void) const;

    const VariableDescriptionContainer&
    getExternalStateVariables(void) const;

    const VariableDescriptionContainer&
    getLocalVariables(void) const;

    const VariableDescriptionContainer&
    getParameters(void) const;

    const VariableDescription&
    getPersistentVariableHandler(const std::string&) const;

    const VariableDescription&
    getIntegrationVariableHandler(const std::string&) const;

    const VariableDescription&
    getStateVariableHandler(const std::string&) const;

    const VariableDescription&
    getVariableHandler(const VariableDescriptionContainer&,
		       const std::string&) const;
    /*!
     * \return true if the given variable is used in a code block
     * \param[in] h : modelling hypothesis
     * \param[in] n : name
     */
    bool
    isVariableUsedInCodeBlocks(const std::string&) const;

    bool
    isMaterialPropertyName(const std::string&) const;

    bool
    isLocalVariableName(const std::string&) const;

    bool
    isPersistentVariableName(const std::string&) const;

    bool
    isIntegrationVariableName(const std::string&) const;

    bool
    isIntegrationVariableIncrementName(const std::string&) const;

    bool
    isStateVariableName(const std::string&) const;

    bool
    isStateVariableIncrementName(const std::string&) const;

    bool
    isAuxiliaryStateVariableName(const std::string&) const;

    bool
    isExternalStateVariableName(const std::string&) const;

    bool
    isExternalStateVariableIncrementName(const std::string&) const;

    bool
    isParameterName(const std::string&) const;
    /*!
     * \return true if the given name is the one of a static variable
     * \param[in] n : name
     */
    bool
    isStaticVariableName(const std::string& n) const;

    const std::vector<BoundsDescription>&
    getBounds(void) const;
    /*!
     * \brief set a bound on a variable
     * \param[in] d : bounds description
     */
    void
    setBounds(const BoundsDescription&);
    /*!
     * \return the static variables defined
     */
    const StaticVariableDescriptionContainer&
    getStaticVariables(void) const;
    /*!
     * \brief add a material property
     * \param[in] v : variable description
     */
    void
    addMaterialProperty(const VariableDescription&);
    /*!
     * \brief add a integration variable
     * \param[in] v : variable description
     */
    void
    addIntegrationVariable(const VariableDescription&);
    /*!
     * \brief add a state variable
     * \param[in] v : variable description
     */
    void
    addStateVariable(const VariableDescription&);
    /*!
     * \brief add an auxiliary state variable
     * \param[in] v : variable description
     */
    void
    addAuxiliaryStateVariable(const VariableDescription&);
    /*!
     * \brief add an external state variable
     * \param[in] v : variable description
     */
    void
    addExternalStateVariable(const VariableDescription&);
    /*!
     * \brief add a : variable
     * \param[in] v : variable description
     */
    void
    addLocalVariable(const VariableDescription&);
    /*!
     * \brief add a parameter
     * \param[in] v : variable description
     */
    void
    addParameter(const VariableDescription&);
    /*!
     * \return true if a parameter with the given name exists
     * \param[in] n : name
     */
    bool
    hasParameter(const std::string&) const;
    /*!
     * \return true if at least one parameter is defined
     */
    bool
    hasParameters() const;
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter default value
     */
    void
    setParameterDefaultValue(const std::string&,
			     const double);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter default value
     */
    void
    setParameterDefaultValue(const std::string&,
			     const int);
    /*!
     * \param[in] n : parameter name
     * \param[in] v : parameter default value
     */
    void
    setParameterDefaultValue(const std::string&,
			     const unsigned short);
    /*!
     * \return the default value of a paramater
     * \param[in] v : parameter default value
     */
    double
    getFloattingPointParameterDefaultValue(const std::string&) const;
    /*!
     * \return the default value of a paramater
     * \param[in] v : parameter default value
     */
    int
    getIntegerParameterDefaultValue(const std::string&) const;
    /*!
     * \return the default value of a paramater
     * \param[in] v : parameter default value
     */
    unsigned short
    getUnsignedShortParameterDefaultValue(const std::string&) const;
    /*!
     * \brief add a static variable description
     * \param[in] v : variable description
     */
    void
    addStaticVariable(const StaticVariableDescription&);
    /*!
     * \brief check if one has to include tvector.hxx or vector.hxx
     * \param[in] b1 : requires true if one has to include tvector.hxx
     * \param[in] b2 : requires true if one has to include vector.hxx
     */
    void
    requiresTVectorOrVectorIncludes(bool&,
				    bool&) const;   
    /*!
     * \brief return the name of the code blocks defined so far
     * \param[in] h : modelling hypothesis
     */
    std::vector<std::string>
    getCodeBlockNames(void) const;
    /*!
     * \brief declare or update a code block
     * \param[in] n : name
     * \param[in] c : code
     * \param[in] m : mode
     * \param[in] p : position
     */
    void setCode(const std::string&,
		 const CodeBlock&,
		 const Mode,
		 const Position);
    /*!
     * \return the code block associated with the given name
     * \param[in] n : name
     */
    const CodeBlock&
    getCode(const std::string&) const;
    /*!
     * \return true if a code block associated with the given name has
     * been defined
     * \param[in] n : name
     */
    bool
    hasCode(const std::string&) const;
    /*!
     * \brief insert a new attribute
     * \param[in] n : name
     * \param[in] a : attribute
     * \param[in] b : don't throw if the the
     *                attribute already exists.
     *                The attribute is left unchanged.
     *                However the type of the attribute is checked.
     */
    void
    setAttribute(const std::string&,
		 const MechanicalBehaviourAttribute&,
		 const bool);
    /*!
     * \return true if an attribute with the given name as been registred
     * \param[in] n : name
     */
    bool
    hasAttribute(const std::string&) const;
    /*!
     * \return the glossary or entry names associated with the given
     * variable.
     * \param[in] v  : variable name
     */
    std::string
    getGlossaryName(const std::string&) const;
    /*!
     * \return the glossary or entry names associated with the
     * variables contained in the given container
     * \param[in] v  : variables for which glossary names are requested
     */
    std::vector<std::string>
    getGlossaryNames(const VarContainer&) const;
    /*!
     * get the glossary names associated with the variables
     * contained in the given container
     * \param[out] n : names
     * \param[in] v  : variables for which glossary names are requested
     */
    void
    getGlossaryNames(std::vector<std::string>&,
		     const VarContainer&) const;
    /*!
     * get the glossary names associated with the variables
     * contained in the given container
     * \param[out] n : names
     * \param[in]  v : variables for which glossary names are requested
     */
    void
    appendGlossaryNames(std::vector<std::string>&,
			const VarContainer&) const;
    /*!
     * associate a glossary name to a variable
     * \param[in] n : variable name
     * \param[in] g : glossary name
     */
    void
    setGlossaryName(const std::string&,
		    const std::string&);
    /*!
     * look if the given name is a glossary name
     * \param[in] n : variable name
     */
    bool
    isGlossaryName(const std::string&) const;
    /*!
     * associate an entry name to a variable
     * \param[in] n : variable name
     * \param[in] e : entry name
     */
    void
    setEntryName(const std::string&,
		 const std::string&);
    /*!
     * look if the given name is a entry name
     * \param[in] n : variable name
     */
    bool
    isEntryName(const std::string&) const;
    /*!
     * \return the name of the variable associated with the given
     * glossary or entry name
     * \param[in] n : name
     */
    std::string
    getVariableNameFromGlossaryNameOrEntryName(const std::string&) const;
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template<typename T>
    typename tfel::meta::EnableIf<
      tfel::meta::TLCountNbrOfT<T,MechanicalBehaviourAttributeTypes>::value==1, 
      T&>::type
    getAttribute(const std::string&);
    /*!
     * \return the attribute with the given name
     * \param[in] n : name
     */
    template<typename T>
    typename tfel::meta::EnableIf<
      tfel::meta::TLCountNbrOfT<T,MechanicalBehaviourAttributeTypes>::value==1, 
      const T&>::type
    getAttribute(const std::string&) const;
    /*!
     * \return all the attribute registred
     * \param[in] n : name
     */
    const std::map<std::string,MechanicalBehaviourAttribute>&
    getAttributes(void) const;
    /*!
     * register a new variable name
     * \param[in] n : variable name
     * \note an exception is thrown is the given name has already been
     * registred
     */
    void registerVariable(const std::string&);
  private:
    /*!
     * structure used to handle a blocks of code
     */
    struct CodeBlocksAggregator
    {
      //! a simple alias
      typedef MechanicalBehaviourData::Position Position;
      //! a simple alias
      typedef MechanicalBehaviourData::Mode Mode;
      //! constructor
      CodeBlocksAggregator();
      /*!
     * setter
     * \param[in] c : code
     * \param[in] p : position
     */
      void set(const CodeBlock&,
	       const Position);
      /*!
       * setter
       * \param[in] c : code
       * \param[in] p : position
       */
      void replace(const CodeBlock&,
		   const Position);
      /*!
       * \return the code block
       */
      const CodeBlock&
      get(void) const;
    private:
      /*! 
       * \brief update the code block
       */
      void update(void);
      /*!
       * check that the get function was not already called.
       */ 
      void check(void) const;
      //! beginnig of the block
      std::string cblock_begin;
      //! block body
      std::string cblock_body;
      //! end of the block
      std::string cblock_end;
      /*!
       * resulting code block
       */
      CodeBlock cblock;
      //! get already called
      mutable bool get_already_called;
    };
    /*!
     * \brief throw an exception saying that no attribute with the given name exists
     */
    static void throwUndefinedAttribute(const std::string&);
    //! assignement operator constructor (disabled)
    MechanicalBehaviourData&
    operator = (const MechanicalBehaviourData&);
    /*!
     * check that the given name has been registred as a variable name
     * \param[in] n : variable name
     * \note an exception is thrown is the given name is not found
     */
    void checkVariableName(const std::string&) const;
    /*!
     * \brief add a variable to a container
     * \param[in] c : container
     * \param[in] v : variable to be added
     * \param[in] b : if false, don't register variable name
     */
    void
    addVariable(VariableDescriptionContainer& v,
		const VariableDescription&,
		const bool = true);
    /*!
     * variables names
     */
    std::set<std::string> vnames;
    /*!
     * variables flagged as probably unusable in purely implicit
     * resolutions
     */
    std::set<std::string> pupirv;
    /*!
     * boolean stating if this behaviour can be used in a purely
     * implicit resolution.
     * \see isUsableInPurelyImplicitResolution for details about
     * purely implicit resolution.
     */
    bool usableInPurelyImplicitResolution;
    /*!
     * registred code blocks
     */
    std::map<std::string,CodeBlocksAggregator> cblocks;
    /*!
     * registred material properties
     */
    VariableDescriptionContainer materialProperties;
    /*!
     * registred persistent variables
     */
    VariableDescriptionContainer persistentVariables;
    /*!
     * registred integration variables
     */
    VariableDescriptionContainer integrationVariables;
    /*!
     * registred state variables
     */
    VariableDescriptionContainer stateVariables;
    /*!
     * registred auxiliary state variables
     */
    VariableDescriptionContainer auxiliaryStateVariables;
    /*!
     * registred external state variables
     */
    VariableDescriptionContainer externalStateVariables;
    /*!
     * registred local variables
     */
    VariableDescriptionContainer localVariables;
    /*!
     * parameters of the behaviour
     */
    VariableDescriptionContainer parameters;
    //! default value for floatting point paramerters
    std::map<std::string,double>         parametersDefaultValues;
    //! default value for signed integer paramerters
    std::map<std::string,int>            iParametersDefaultValues;
    //! default value for short unsigned integer paramerters
    std::map<std::string,unsigned short> uParametersDefaultValues;
    /*!
     * static variables
     */
    StaticVariableDescriptionContainer staticVars;
    /*!
     * bounds of the registred variables
     */
    std::vector<BoundsDescription> bounds;
    /*!
     * behaviour attributes
     */
    std::map<std::string,
	     MechanicalBehaviourAttribute> attributes;
    /*!
     * list of glossary names used by the behaviour
     */
    std::map<std::string,std::string> glossaryNames;
    /*!
     * list of entry names used by the behaviour
     */
    std::map<std::string,std::string> entryNames;
  }; // end of struct MechanicalBehaviourData

} // end of namespace mfront

#include"MFront/MechanicalBehaviourData.ixx"

#endif /* _LIB_MFRONT_MECHANICALBEHAVIOURDATA_H */
