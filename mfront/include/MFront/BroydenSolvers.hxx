/*! 
 * \file  mfront/include/MFront/BroydenSolvers.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 22 août 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_MFRONTBROYDENSOLVERS_HXX
#define LIB_MFRONT_MFRONTBROYDENSOLVERS_HXX 

#include "MFront/NonLinearSystemSolverBase.hxx"
#include "MFront/PowellDogLegAlgorithmBase.hxx"

namespace mfront{

  /*!
   * base class for non linear system solver based on the
   * Broyden algorithm.
   * The Broyden algorithm can be coupled with the
   * PowellDogLegAlgorithm for increased robustness.
   */
  struct MFrontBroydenSolverBase
    : public    NonLinearSystemSolverBase,
      protected PowellDogLegAlgorithmBase
  {
    //! a simple alias
    using NonLinearSystemSolverBase::tokens_iterator;
    //! a simple alias
    using Hypothesis = NonLinearSystemSolverBase::Hypothesis;
    /*!
     * \return the reserved names
     */
    std::vector<std::string> getReservedNames() const override;
    /*!
     * \return true if the solver uses the jacobian of the system
     * (Newton-Raphson solvers) or an approximation of it (Broyden
     * method).
     */
    bool usesJacobian() const override;
    /*!
     * \return true if the solver uses the invert of the jacobian of
     * the system or an approximation of it (second Broyden method).
     */
    bool usesJacobianInvert() const override;
    /*!
     * \return true if the solver requires a numerical jacobian
     */
    bool requiresNumericalJacobian() const override;
    /*!
     * \return true if the solver allows the user to initialize the
     * jacobian
     */
    bool allowsJacobianInitialisation() const override;
    /*!
     * \return true if the solver allows the user to initialize the
     * invert of the jacobian.
     * \note If the solver does not provide such initialisation, the
     * invert of the jacobian could for example be initialised to
     * identity in the behaviour constructor.
     */
    bool allowsJacobianInvertInitialisation() const override;
    /*!
     * \return true if the solver requires the jacobian to be
     * initialized to the identity matrix at the beginning of the
     * computeFdF method.
     */
    bool requiresJacobianToBeReinitialisedToIdentityAtEachIterations() const override;
    /*!
     * \brief write the algorithm specific members
     * \param[in,out] bd: behaviour description
     * \param[in] key   : keyword
     * \param[in] p     : current position in file (after keyword)
     * \param[in] pe    : end of file
     */
    std::pair<bool,tokens_iterator>
    treatSpecificKeywords(BehaviourDescription&,
			  const std::string&,
			  const tokens_iterator,
			  const tokens_iterator) override;
    /*!
     * \brief method called when all the user defined variables have been set.
     * \param[in,out] md  : mechanical description
     */
    void completeVariableDeclaration(BehaviourDescription&) const override;
    /*!
     * \brief write algorithm specific initialisations
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    void writeSpecificInitializeMethodPart(std::ostream&,
					   const BehaviourDescription&,
					   const Hypothesis) const override;
    /*!
     * \brief write the algorithm specific members
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    void writeSpecificMembers(std::ostream&,
			      const BehaviourDescription&,
			      const Hypothesis) const override;
    /*!
     * \brief write the resolution algorithm
     * \param[in] out : output file
     * \param[in] md  : mechanical description
     * \param[in] h   : modelling hypotheis
     */
    void writeResolutionAlgorithm(std::ostream&,
				  const BehaviourDescription&,
				  const Hypothesis) const override;
    //! destructor
    ~MFrontBroydenSolverBase() override;

  protected:

    /*!
     * The derived class shall return true if the NewtonAlgorithm
     * shall be combined with the powell dog leg algorithm
     */
    virtual bool
    usesPowellDogLegAlgorithm() const = 0;

  }; // end of struct MFrontBroydenSolverBase

  /*!
   * The standard Broyden Solver
   */
  struct MFrontBroydenSolver
    : public MFrontBroydenSolverBase
  {
    //! destructor
    ~MFrontBroydenSolver() override;
  protected:
    /*!
     * The derived class shall return true if the NewtonAlgorithm
     * shall be combined with the powell dog leg algorithm
     */
    bool usesPowellDogLegAlgorithm() const override;
  };

  /*!
   * The standard Broyden Solver coupled with the Powell dog leg
   * algorithm
   */
  struct MFrontPowellDogLegBroydenSolver
    : public MFrontBroydenSolverBase
  {
    //! destructor
    ~MFrontPowellDogLegBroydenSolver() override;
  protected:
    /*!
     * The derived class shall return true if the PowellDogLegNewtonAlgorithm
     * shall be combined with the powell dog leg algorithm
     */
    bool usesPowellDogLegAlgorithm() const override;
  };

} // end of namespace mfront

#endif /* LIB_MFRONT_MFRONTBROYDENSOLVERS_HXX */

