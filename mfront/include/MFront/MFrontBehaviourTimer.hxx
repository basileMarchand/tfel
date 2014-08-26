/*! 
 * \file  MFrontBehaviourTimer.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 mai 2014
 */

#ifndef _LIB_MFRONT_MFRONTBEHAVIOURTIMER_H_
#define _LIB_MFRONT_MFRONTBEHAVIOURTIMER_H_ 

#include"MFront/MFrontConfig.hxx"

#include<array>
#include<atomic>

namespace mfront{

  /*!
   * structure in charge of performance measurements in mechanical
   * behaviour
   */
  struct MFRONTTIMER_VISIBILITY_EXPORT MFrontBehaviourTimer
  {
    /*!
     * \see MechanialBehaviourData for a description
     */
    enum CodeBlock{
      FLOWRULE = 0,
      BEFOREINITIALIZELOCALVARIABLES = 1,
      INITIALIZELOCALVARIABLES = 2,
      AFTERINITIALIZELOCALVARIABLES = 3,
      COMPUTEPREDICTOR = 4,
      COMPUTESTRESS = 5,
      COMPUTEFINALSTRESS = 6,
      DEFAULTCOMPUTEFINALSTRESS = 7,
      INTEGRATOR = 8,
      COMPUTEDERIVATIVE = 9,
      COMPUTEPREDICTIONOPERATOR = 10,
      COMPUTETANGENTOPERATOR = 11,
      UPDATEAUXILIARYSTATEVARIABLES = 12,
      INITIALIZEJACOBIAN = 13
    }; // end of CodeBlock
    //! destructor
    ~MFrontBehaviourTimer();
    /*!
     * a timer for a specicied code block.
     * This descructor will increase the time count for the code block.
     */
    struct Timer
    {
      /*!
       * \param[in,out] t  : global timer
       * \param[in]     cn : code block 
       */
      Timer(MFrontBehaviourTimer&,
	    const CodeBlock);
      //! destructor
      ~Timer();
    protected:
      //! timer to which the results are reported
      MFrontBehaviourTimer& gtimer;
      //! code block associated with the timer
      const CodeBlock c;
    }; // end of struct Timer
    //!  time spend in each code block (nanoseconds)
    std::array<std::atomic<intmax_t>,14> measures;
  }; // end of MFrontBehaviourTimer

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTBEHAVIOURTIMER_H */

