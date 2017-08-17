#include <string>
#include <sstream>

#include "TFEL/Check/Comparison.hxx"

namespace tfel{

  namespace check{

    Comparison::Comparison()
      : success(true),
	prec(0.),
	precision2(0.),
	interpolationIsConform(false) {
    }

    Comparison::Comparison(const std::shared_ptr<Column> c1_,
			   const std::shared_ptr<Column> c2_,
			   const double prec_,
			   const double precision2_,
			   const std::shared_ptr<Column> ci_,
			   const std::string it_,
			   const bool ic_,
			   const std::shared_ptr<Column> cii_,
			   const std::shared_ptr<Interpolation> ii_)
      : success(true),
	c1(c1_),
	c2(c2_),
	prec(prec_),
	precision2(precision2_),
	ci(ci_),
	interpolationType(it_),
	interpolationIsConform(ic_),
	colIntegralInterpolated(cii_),
	integralInterpolation(ii_) {
    }

    bool Comparison::hasSucceed() const {
      return this->success;
    }

    std::string Comparison::getMsgLog() const {
      return this->msgLog;
    }

    void Comparison::setParameters(const std::shared_ptr<Column> c1_,
				   const std::shared_ptr<Column> c2_,
				   const double prec_,
				   const double precision2_,
				   const std::shared_ptr<Column> ci_,
				   const std::string it_,
				   const bool ic_,
				   const std::shared_ptr<Column> cii_,
				   const std::shared_ptr<Interpolation> ii_) {
      this->c1 = c1_;
      this->c2 = c2_;
      this->prec = prec_;
      this->precision2 = precision2_;
      this->ci = ci_;
      this->interpolationType       = it_;
      this->interpolationIsConform  = ic_;
      this->colIntegralInterpolated = cii_;
      this->integralInterpolation   = ii_;
    }

    const std::string& Comparison::getName() const {
      return this->name;
    }

    Comparison::~Comparison() = default;
  
  } // end of namespace check

} // end of namespace tfel
