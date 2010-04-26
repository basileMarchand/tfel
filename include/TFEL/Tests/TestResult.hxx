/*!
 * \file   TestResult.hxx
 * \author Helfer Thomas
 * \date   10 Apr 10
 * \brief  
 */

#ifndef _LIB_TFEL_TESTS_TESTRESULT_HXX
#define _LIB_TFEL_TESTS_TESTRESULT_HXX 1

#include<ostream>
#include<string>
#include<vector>

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel
{

  namespace tests
  {
    
   /*! 
     * result of test
     */
    struct TFEL_VISIBILITY_EXPORT TestResult
      : protected std::vector<TestResult>
    {
      using std::vector<TestResult>::const_iterator;
      /*! 
       * Default onstructor
       */
      TestResult();
      /*! 
       * Constructor
       * \param b : tells if the test is a success
       */
      TestResult(const bool);
      /*! 
       * Constructor
       * \param b : tells if the test is a success
       * \param c : detail
       */
      TestResult(const bool,
		 const char* const);
      /*! 
       * Constructor
       * \param b : tells if the test is a success
       * \param c : detail
       */
      TestResult(const bool,
		 const std::string&);
      /*!
       * copy constructor
       * \param src : object to be copied
       */
      TestResult(const TestResult&);
      /*!
       * assignement operator
       * \param src : object to be assigned
       * \return a reference to this object
       */
      TestResult&
      operator = (const TestResult&);
      /*! 
       * report success
       */
      bool success() const;
      /*! 
       * get result details
       */
      const std::string&
      details() const;
      /*!
       *  \return a read-only (constant) iterator that points to the
       *  first element in the vector.
       */
      const_iterator
      begin() const;
      /*!
       *  \return an iterator that points one past
       *  the last element in the vector
       */
      const_iterator
      end() const;
      /*!
       * This is a typical stack operation.  The function creates an
       * element at the end of the vector and assigns the given data
       * to it.
       * Due to the nature of a vector this operation can be
       * done in constant time if the vector has preallocated space
       * available.
       * \param r : result to be appended
       */
      void
      append(const TestResult&);
    private:
      //! detail
      std::string d;
      //! success of the test
      bool s;
    }; // end of struct 
    
    std::ostream&
    operator << (std::ostream&,const TestResult&);

  } // end of namespace tests

} // end of namespace tfel

#endif  /*_LIB_TFEL_TESTS_TESTRESULT_HXX */