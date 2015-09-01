/*!
 * \file   include/TFEL/Math/LambdaExpression/ScalarLambdaExpr.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 d�c 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/Math/LambdaExpression/LambdaConcept.hxx"

#ifndef _LIB_TFEL_SCALARLAMBDAEXPR_HXX_
#define _LIB_TFEL_SCALARLAMBDAEXPR_HXX_ 

namespace tfel{

  namespace math{

    template<typename Scalar,typename Lambda,typename Op>
    class ScalarLambdaExpr;

    template<typename Scalar,typename Lambda,typename Op>
    struct LambdaTraits<ScalarLambdaExpr<Scalar,Lambda,Op> >{
      /*
       * minimal dimension of the argument.
       */
      static const unsigned short dimension = LambdaTraits<Lambda>::dimension; 
      /*
       * variables list
       */
      typedef typename LambdaTraits<Lambda>::Variables Variables; 
    }; // end of LambdaTraits

    template<unsigned short N,typename Scalar,typename Lambda,typename Op>
    class ScalarLambdaExprImpl;

    template<typename Scalar,typename Lambda,typename Op>
    class ScalarLambdaExpr
      : public LambdaConcept<ScalarLambdaExpr<Scalar,Lambda,Op> >,
	public ScalarLambdaExprImpl<LambdaTraits<ScalarLambdaExpr<Scalar,Lambda,Op> >::dimension,
				    Scalar,Lambda,Op>
    {

      ScalarLambdaExpr();

      static const unsigned short 
      dimension =LambdaTraits<ScalarLambdaExpr<Scalar,Lambda,Op> >::dimension;
	
      typedef ScalarLambdaExprImpl<dimension,Scalar,Lambda,Op> Impl;


    public:


      ScalarLambdaExpr(const Scalar a_, const Lambda b_)
	: Impl(a_,b_)
      {}
	
      using Impl::operator();
    };

  } // end of namespace math

} // end of namespace tfel  

#include"TFEL/Math/LambdaExpression/ScalarLambdaExpr.ixx"  

#endif /* _LIB_TFEL_SCALARLAMBDAEXPR_HXX */
