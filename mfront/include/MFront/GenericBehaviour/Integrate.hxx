/*!
 * \file   MFront/GenericBehaviour/Integrate.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   03/07/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_GENERICBEHAVIOUR_INTEGRATE_HXX
#define LIB_MFRONT_GENERICBEHAVIOUR_INTEGRATE_HXX

#include <algorithm>
#include "TFEL/Math/st2tost2.hxx"
#include "TFEL/Math/t2tost2.hxx"
#include "TFEL/Material/OutOfBoundsPolicy.hxx"
#include "TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"
#include "MFront/GenericBehaviour/BehaviourData.h"

namespace mfront {

  namespace gb {

    template <typename real,unsigned short N>
    void exportTangentOperator(real* const v,
			       const tfel::math::st2tost2<N,real>& K){
      std::copy(K.begin(), K.end(), v);
    } // end of exportTangentOperator

    template <typename real,unsigned short N>
    void exportTangentOperator(real* const v,
			       const tfel::material::FiniteStrainBehaviourTangentOperator<N,real>& K){
      if(K.template is<tfel::math::t2tost2<N,real>>()){
	const auto& k = K.template get<tfel::math::t2tost2<N,real>>();
	std::copy(k.begin(), k.end(), v);
      } else if(K.template is<tfel::math::t2tost2<N,real>*>()){
	const auto& k = *(K.template get<tfel::math::t2tost2<N,real>*>());
	std::copy(k.begin(), k.end(), v);
      } else if(K.template is<tfel::math::st2tost2<N,real>>()){
	const auto& k = K.template get<tfel::math::st2tost2<N,real>>();
	std::copy(k.begin(), k.end(), v);
      } else if(K.template is<tfel::math::st2tost2<N,real>*>()){
	const auto& k = *(K.template get<tfel::math::st2tost2<N,real>*>());
	std::copy(k.begin(), k.end(), v);
      } else {
	tfel::raise("mfront::gb::exportTangentOperator: "
		    "unsupported tangent operator type");
      }
    } // end of exportTangentOperator

    template <typename real,unsigned short N>
    void exportTangentOperator(real* const v,
			       const tfel::math::tmatrix<N,N,real>& K){
      std::copy(K.begin(), K.end(), v);
    } // end of exportTangentOperator
    
    /*!
     * \brief compute the prediction operator
     * \tparam Behaviour: class implementing the behaviour
     * \param[in,out] b: behaviour 
     * \param[in,out] d: behaviour data
     * \param[in,out] smflag: consistent tangent operator expected
     */
    template <typename Behaviour>
    int computePredictionOperator(Behaviour& b,
				  MFront_GB_BehaviourData& d,
				  const typename Behaviour::SMFlag f) {
      const auto smt =  [&d]{
	if (-1.5 < d.K[0]) {
	  return Behaviour::ELASTIC;
	} else if ((-2.5 < d.K[0]) && (d.K[0] < -1.5)) {
	  return Behaviour::SECANTOPERATOR;
	}
	return Behaviour::TANGENTOPERATOR;
      }();
      b.computePredictionOperator(f, smt);
      exportTangentOperator(d.K,b.getTangentOperator());
      return 1;
    } // end of computePredictionOperator
    
    /*!
     * \brief integrate the behaviour over a time step
     * \tparam Behaviour: class implementing the behaviour
     * \param[in,out] d: behaviour data
     * \param[in] f: choice of consistent tangent operator
     * \param[in] p: out of bounds policy
     */
    template <typename Behaviour>
    int integrate(MFront_GB_BehaviourData& d,
		  const typename Behaviour::SMFlag f,
		  const tfel::material::OutOfBoundsPolicy p) {
      Behaviour b(d);
      b.setOutOfBoundsPolicy(p);
      b.initialize();
      try{
	b.checkBounds();
	if(d.K[0]<-0.25){
	  return computePredictionOperator(b, d, f);
	}
	const auto smt =  [&d]{
	  if (d.K[0] < 0.5) {
	    return Behaviour::NOSTIFFNESSREQUESTED;
	  } else if ((0.5 < d.K[0]) && (d.K[0] < 1.5)) {
	    return Behaviour::ELASTIC;
	  } else if ((1.5 < d.K[0]) && (d.K[0] < 2.5)) {
	    return Behaviour::SECANTOPERATOR;
	  } else if ((2.5 < d.K[0]) && (d.K[0] < 3.5)) {
	    return Behaviour::TANGENTOPERATOR;
	  }
	  return Behaviour::CONSISTENTTANGENTOPERATOR;
	}();
	auto tsf = b.computeAPrioriTimeStepScalingFactor(d.rdt);
	d.rdt = tsf.second;
	if (!tsf.first) {
	  return -1;
	}
	const auto r = b.integrate(f,smt);
	if (r == Behaviour::FAILURE) {
	  d.rdt = b.getMinimalTimeStepScalingFactor();
	  return -1;
	}
	const auto atsf = b.computeAPosterioriTimeStepScalingFactor(d.rdt);
	d.rdt = std::min(atsf.second, d.rdt);
	if (!atsf.first) {
	  return -1;
	}
	b.exportStateData(d.s1);
	if(d.K[0]>0.5){
	  exportTangentOperator(d.K,b.getTangentOperator());
	}
      } catch(...){
	d.rdt = b.getMinimalTimeStepScalingFactor();
	return -1;
      }
      return d.rdt < 0.99 ? 0 : 1;
    } // end of integrate

  }  // end of namespace gb

}  // end of namespace mfront

#endif /* LIB_MFRONT_GENERICBEHAVIOUR_INTEGRATE_HXX */
