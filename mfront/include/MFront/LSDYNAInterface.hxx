/*!
 * \file   mfront/include/MFront/LSDYNAInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \date   16 mars 2016
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_LSDYNAEXPLICITINTERFACE_HXX
#define LIB_MFRONT_LSDYNAEXPLICITINTERFACE_HXX

#include "MFront/UMATInterfaceBase.hxx"

namespace mfront {

  /*!
   * Interface dedicated to the LS-DYNA finite element solver
   * \see{http://www.lstc.com/products/ls-dyna}
   */
  struct LSDYNAInterface : public UMATInterfaceBase {
    //! \return the name of the interface
    static std::string getName();
    /*!
     * \brief name of the attribute used to store the orthotropic management
     * policy.
     */
    static const char* const orthotropyManagementPolicy;
    /*!
     * \brief name of the attribute used to store the finite strain strategy.
     */
    static const char* const finiteStrainStrategy;

    std::pair<bool, tokens_iterator> treatKeyword(
        BehaviourDescription&,
        const std::string&,
        const std::vector<std::string>&,
        tokens_iterator,
        const tokens_iterator) override;
    std::set<Hypothesis> getModellingHypothesesToBeTreated(
        const BehaviourDescription&) const override;
    void endTreatment(const BehaviourDescription&,
                      const FileDescription&) const override;
    void writeInterfaceSpecificIncludes(
        std::ostream&, const BehaviourDescription&) const override;
    void getTargetsDescription(TargetsDescription&,
                               const BehaviourDescription&) override;
    std::string getLibraryName(const BehaviourDescription&) const override;
    //! destructor
    ~LSDYNAInterface() override;

   protected:
    std::string getFunctionNameBasis(const std::string&) const override;
    /*!
     * \return the name of the function generated by the interface
     * \param[in] n: name of the behaviour as defined by interface
     *               (generally taking into account the material
     *               and the behaviour name)
     * \param[in] h: modelling hypothesis
     */
    virtual std::string getFunctionNameForHypothesis(const std::string&,
                                                     const Hypothesis) const;
    std::string getInterfaceName() const override;
    std::string getModellingHypothesisTest(const Hypothesis) const override;
    void writeBehaviourConstructorHeader(std::ostream&,
					 const BehaviourDescription&,
					 const Hypothesis,
					 const std::string&) const override;
    void writeBehaviourDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    void writeBehaviourDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    void writeBehaviourDataDrivingVariableSetter(
        std::ostream&,
        const DrivingVariable&,
        const SupportedTypes::TypeSize) const override;
    void writeBehaviourDataThermodynamicForceSetter(
        std::ostream&,
        const ThermodynamicForce&,
        const SupportedTypes::TypeSize) const override;
    void writeIntegrationDataConstructor(
        std::ostream&,
        const Hypothesis,
        const BehaviourDescription&) const override;
    void writeIntegrationDataMainVariablesSetters(
        std::ostream&, const BehaviourDescription&) const override;
    void writeIntegrationDataDrivingVariableSetter(
        std::ostream&,
        const DrivingVariable&,
        const SupportedTypes::TypeSize) const override;
    void exportMechanicalData(std::ostream&,
                              const Hypothesis,
                              const BehaviourDescription&) const override;
    void writeMTestFileGeneratorSetModellingHypothesis(
        std::ostream&) const override;
  };

}  // end of namespace mfront

#endif /* LIB_MFRONT_LSDYNAEXPLICITINTERFACE_HXX */
