/*!
 * \file   include/TFEL/Math/Parser/EvaluatorFunction.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   15 jan 2009
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATH_EVALUATORFUNCTION_HXX_
#define LIB_TFEL_MATH_EVALUATORFUNCTION_HXX_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/Evaluator.hxx"

namespace tfel
{

  namespace math
  {

    namespace parser
    {

#ifdef __SUNPRO_CC
      extern "C" {
#endif /*__SUNPRO_CC */
	typedef double (*EvaluatorProxyFunctionPtr1V)(double); 
	typedef double (*EvaluatorProxyFunctionPtr2V)(double,double); 
	typedef double (*EvaluatorProxyFunctionPtr1P1V)(int,double); 
	typedef double (*EvaluatorProxyFunctionPtr1P2V)(int,double,double);
	typedef double (*EvaluatorProxyFunctionPtr2P1V)(int,int,double);
	typedef double (*EvaluatorProxyFunctionPtr2P2V)(int,int,double,double);
#ifdef __SUNPRO_CC
     }
#endif /* __SUNPRO_CC */

      struct EvaluatorFunctionBase
	: public Function
      {
	EvaluatorFunctionBase();
	virtual std::shared_ptr<Expr>
	differentiate(const std::vector<double>::size_type,
		      const std::vector<double>&) const override;
	virtual ~EvaluatorFunctionBase();
      }; // end of struct EvaluatorFunctionBase

      struct EvaluatorFunction1VBase
	: public EvaluatorFunctionBase
      {
	EvaluatorFunction1VBase(const std::shared_ptr<Expr>);
	virtual void
	checkCyclicDependency(std::vector<std::string>&) const override;
	virtual ~EvaluatorFunction1VBase();
      protected:
	const std::shared_ptr<Expr> expr;
      }; // end of struct EvaluatorFunction1V

      struct EvaluatorFunction2VBase
	: public EvaluatorFunctionBase
      {
	EvaluatorFunction2VBase(const std::shared_ptr<Expr>,
				const std::shared_ptr<Expr>);
	virtual void
	checkCyclicDependency(std::vector<std::string>&) const override;
	virtual ~EvaluatorFunction2VBase();
      protected:
	const std::shared_ptr<Expr> e1;
	const std::shared_ptr<Expr> e2;
      }; // end of struct EvaluatorFunction2V

      struct EvaluatorFunction1P1V final
	: public EvaluatorFunction1VBase
      {
	EvaluatorFunction1P1V(const EvaluatorProxyFunctionPtr1P1V,
			      const int,
			      const std::shared_ptr<Expr>);
	virtual double getValue(void) const override;
	virtual std::shared_ptr<Expr>
	resolveDependencies(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	clone(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const override;	
	virtual void
	getParametersNames(std::set<std::string>&) const override;
	virtual ~EvaluatorFunction1P1V();
      private:
	EvaluatorProxyFunctionPtr1P1V f;
	const int n;
      }; // end of struct EvaluatorFunction1P1V

      struct EvaluatorFunction2P1V final
	: public EvaluatorFunction1VBase
      {
	EvaluatorFunction2P1V(const EvaluatorProxyFunctionPtr2P1V,
			      const int,
			      const int,
			      const std::shared_ptr<Expr>);
	virtual double getValue(void) const override;
	virtual std::shared_ptr<Expr>
	resolveDependencies(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	clone(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const override;
	virtual void
	getParametersNames(std::set<std::string>&) const override;
	virtual ~EvaluatorFunction2P1V();
      private:
	EvaluatorProxyFunctionPtr2P1V f;
	const int n;
	const int m;
      }; // end of struct EvaluatorFunction2P1V

      struct EvaluatorFunction1P2V final
	: public EvaluatorFunction2VBase
      {
	EvaluatorFunction1P2V(const EvaluatorProxyFunctionPtr1P2V,
			      const int,
			      const std::shared_ptr<Expr>,
			      const std::shared_ptr<Expr>);
	virtual double getValue(void) const override;
	virtual std::shared_ptr<Expr>
	resolveDependencies(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	clone(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const override;	
	virtual void
	getParametersNames(std::set<std::string>&) const override;
	virtual ~EvaluatorFunction1P2V();
      private:
	EvaluatorProxyFunctionPtr1P2V f;
	const int n;
      }; // end of struct EvaluatorFunction1P2V

      struct EvaluatorFunction2P2V final
	: public EvaluatorFunction2VBase
      {
	EvaluatorFunction2P2V(const EvaluatorProxyFunctionPtr2P2V,
			      const int,
			      const int,
			      const std::shared_ptr<Expr>,
			      const std::shared_ptr<Expr>);
	virtual double getValue(void) const override;
	virtual std::shared_ptr<Expr>
	resolveDependencies(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	clone(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const override;	
	virtual void
	getParametersNames(std::set<std::string>&) const override;
	virtual ~EvaluatorFunction2P2V();
      private:
	EvaluatorProxyFunctionPtr2P2V f;
	const int n;
	const int m;
      }; // end of struct EvaluatorFunction2P2V

      template<unsigned short N>
      struct EvaluatorFunctionWrapper;

      template<unsigned short N>
      struct TFEL_VISIBILITY_LOCAL EvaluatorFunctionNV final
	: public EvaluatorFunctionBase
      {
	EvaluatorFunctionNV(typename EvaluatorFunctionWrapper<N>::type,
			    const std::vector<std::shared_ptr<Expr> >&);
	virtual double getValue(void) const override;
	virtual std::shared_ptr<Expr>
	resolveDependencies(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	clone(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const override;	
	virtual void
	checkCyclicDependency(std::vector<std::string>&) const override;
	virtual void
	getParametersNames(std::set<std::string>&) const override;
	virtual ~EvaluatorFunctionNV();
      protected:
	typename EvaluatorFunctionWrapper<N>::type f;
	const std::vector<std::shared_ptr<Expr> > args;
      }; // end of struct EvaluatorFunctionNV

      template<unsigned short N>
      struct TFEL_VISIBILITY_LOCAL EvaluatorFunction1PNV final
	: public EvaluatorFunctionBase
      {
	EvaluatorFunction1PNV(typename EvaluatorFunctionWrapper<N>::type1P,
			      const int,
			      const std::vector<std::shared_ptr<Expr> >&);
	virtual double getValue(void) const override;
	virtual std::shared_ptr<Expr>
	resolveDependencies(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	clone(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const override;	
	virtual void
	checkCyclicDependency(std::vector<std::string>&) const override;
	virtual void
	getParametersNames(std::set<std::string>&) const override;
	virtual ~EvaluatorFunction1PNV();
      protected:
	typename EvaluatorFunctionWrapper<N>::type1P f;
	const int n;
	const std::vector<std::shared_ptr<Expr> > args;
      }; // end of struct EvaluatorFunction1PNV

      template<unsigned short N>
      struct TFEL_VISIBILITY_LOCAL EvaluatorFunction2PNV final
	: public EvaluatorFunctionBase
      {
	EvaluatorFunction2PNV(typename EvaluatorFunctionWrapper<N>::type2P,
			      const int,const int,
			      const std::vector<std::shared_ptr<Expr> >&);
	virtual double getValue(void) const override;
	virtual std::shared_ptr<Expr>
	resolveDependencies(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	clone(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const override;
	virtual void
	checkCyclicDependency(std::vector<std::string>&) const override;
	virtual ~EvaluatorFunction2PNV();
      protected:
	typename EvaluatorFunctionWrapper<N>::type2P f;
	const int n;
	const int m;
	const std::vector<std::shared_ptr<Expr> > args;
      }; // end of struct EvaluatorFunction2PNV

      template<unsigned short N>
      struct TFEL_VISIBILITY_LOCAL EvaluatorFunction1UPNV final
	: public EvaluatorFunctionBase
      {
	EvaluatorFunction1UPNV(typename EvaluatorFunctionWrapper<N>::type1UP,
			       const unsigned int,
			       const std::vector<std::shared_ptr<Expr> >&);
	virtual double getValue(void) const override;
	virtual std::shared_ptr<Expr>
	resolveDependencies(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	clone(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const override;	
	virtual void
	checkCyclicDependency(std::vector<std::string>&) const override;
	virtual void
	getParametersNames(std::set<std::string>&) const override;
	virtual ~EvaluatorFunction1UPNV();
      protected:
	typename EvaluatorFunctionWrapper<N>::type1UP f;
	const unsigned int n;
	const std::vector<std::shared_ptr<Expr> > args;
      }; // end of struct EvaluatorFunction1UPNV

      template<unsigned short N>
      struct TFEL_VISIBILITY_LOCAL EvaluatorFunction2UPNV final
	: public EvaluatorFunctionBase
      {
	EvaluatorFunction2UPNV(typename EvaluatorFunctionWrapper<N>::type2UP,
			       const unsigned int n,
			       const unsigned int m,
			       const std::vector<std::shared_ptr<Expr> >&);
	virtual double getValue(void) const override;
	virtual std::shared_ptr<Expr>
	resolveDependencies(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	clone(const std::vector<double>&) const override;
	virtual std::shared_ptr<Expr>
	createFunctionByChangingParametersIntoVariables(const std::vector<double>&,
							const std::vector<std::string>&,
							const std::map<std::string,
							std::vector<double>::size_type>&) const override;	
	virtual void
	checkCyclicDependency(std::vector<std::string>&) const override;
	virtual void
	getParametersNames(std::set<std::string>&) const override;
	virtual ~EvaluatorFunction2UPNV();
      protected:
	typename EvaluatorFunctionWrapper<N>::type2UP f;
	const unsigned int n;
	const unsigned int m;
	const std::vector<std::shared_ptr<Expr> > args;
      }; // end of struct EvaluatorFunction2UPNV

    } // end of namespace parser

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Parser/EvaluatorFunction.ixx"

#endif /* LIB_TFEL_MATH_EVALUATORFUNCTION_HXX_ */
