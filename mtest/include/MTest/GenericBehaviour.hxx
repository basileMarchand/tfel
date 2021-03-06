/*! 
 * \file  mtest/include/MTest/GenericBehaviour.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 07 avril 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MTEST_GENERICBEHAVIOUR_HXX
#define LIB_MTEST_GENERICBEHAVIOUR_HXX 

#include"TFEL/System/ExternalFunctionsPrototypes.hxx"
#include"MTest/CastemStandardBehaviour.hxx"

namespace mtest
{

  /*!
   * \brief A class to handle mechanical beheaviours written using the
   * generic behaviour interface.
   */
  struct TFEL_VISIBILITY_LOCAL GenericBehaviour
    : public StandardBehaviourBase
  {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    GenericBehaviour(const Hypothesis,
			      const std::string&,
			      const std::string&);

    void getGradientsDefaultInitialValues(tfel::math::vector<real>&) const override;

    std::pair<bool,real>
    computePredictionOperator(BehaviourWorkSpace&,
			      const CurrentState&,
			      const StiffnessMatrixType) const override;

    std::pair<bool,real>
    integrate(CurrentState&,
	      BehaviourWorkSpace&,
	      const real,
	      const StiffnessMatrixType) const override;

    void setOptionalMaterialPropertiesDefaultValues(EvolutionManager&,
						    const EvolutionManager&) const override;

    StiffnessMatrixType getDefaultStiffnessMatrixType() const override;

    tfel::math::tmatrix<3u, 3u, real> getRotationMatrix(const tfel::math::vector<real>&, const tfel::math::tmatrix<3u, 3u, real>&) const override;

    void allocate(BehaviourWorkSpace&) const override;
    //! destructor
    ~GenericBehaviour() override;

  protected:

    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return a pair. The first member is true if the integration was
     * successfull, false otherwise. The second member contains a time
     * step scaling factor.
     * \param[out]    Kt:    tangent operator
     * \param[in,out] s :    current state
     * \param[out]    wk:    workspace
     * \param[in]     dt:    time increment
     * \param[in]     ktype: type of the stiffness matrix
     * \param[in]     b:     if true, integrate the behaviour over the time
     * step, if false compute a prediction of the stiffness matrix
     */
    std::pair<bool,real>
    call_behaviour(tfel::math::matrix<real>&,
		   CurrentState&,
		   BehaviourWorkSpace&,
		   const real,
		   const StiffnessMatrixType,
		   const bool) const;

    //! pointer to the function
    tfel::system::GenericBehaviourFctPtr fct;
  }; // end of struct GenericBehaviour
  
} // end of namespace mtest

#endif /* LIB_MTEST_GENERICBEHAVIOUR_HXX */
