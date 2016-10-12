/*! 
 * \file  mfront/include/MFront/Abaqus/AbaqusException.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 24 janv. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_ABAQUSEXCEPTION_H_
#define LIB_MFRONT_ABAQUSEXCEPTION_H_ 

#include<string>
#include<exception>

#include"MFront/Abaqus/AbaqusConfig.hxx"

namespace abaqus {

  struct MFRONT_ABAQUS_VISIBILITY_EXPORT AbaqusException
    : public std::exception
  {

    AbaqusException(const std::string&);
    //! move constructor
    AbaqusException(AbaqusException&&);
    //! copy constructor
    AbaqusException(const AbaqusException&);

    virtual const char* 
    what (void) const noexcept override final;

    virtual std::string 
    getMsg(void) const noexcept final;
    //! destructor    
    virtual ~AbaqusException() noexcept;
  private:
    AbaqusException() = delete;
    AbaqusException& operator=(const AbaqusException&) = delete;
    AbaqusException& operator=(AbaqusException&&) = delete;
    //! error message
    const std::string msg;
  }; // end of struct AbaqusException

  struct MFRONT_ABAQUS_VISIBILITY_EXPORT AbaqusInvalidModellingHypothesis
    : public AbaqusException
  {
    /*!
     * \param[in] b : behaviour name
     */
    AbaqusInvalidModellingHypothesis(const char*);
    AbaqusInvalidModellingHypothesis(AbaqusInvalidModellingHypothesis&&);
    AbaqusInvalidModellingHypothesis(const AbaqusInvalidModellingHypothesis&);
    //! destructor
    virtual ~AbaqusInvalidModellingHypothesis() noexcept;
    AbaqusInvalidModellingHypothesis&
      operator=(const AbaqusInvalidModellingHypothesis&) = delete;
  }; // end of struct AbaqusInvalidModellingHypothesis
  
  struct MFRONT_ABAQUS_VISIBILITY_EXPORT AbaqusInvalidNTENSValue
    : public AbaqusException
  {
    AbaqusInvalidNTENSValue(const unsigned short);
    AbaqusInvalidNTENSValue(AbaqusInvalidNTENSValue&&);
    AbaqusInvalidNTENSValue(const AbaqusInvalidNTENSValue&);
    virtual ~AbaqusInvalidNTENSValue() noexcept;
  private:
    AbaqusInvalidNTENSValue();
    AbaqusInvalidNTENSValue&
    operator=(const AbaqusInvalidNTENSValue&);
  }; // end of struct AbaqusInvalidNTENSValue

  struct MFRONT_ABAQUS_VISIBILITY_EXPORT AbaqusInvalidDimension
    : public AbaqusException
  {
    AbaqusInvalidDimension(const std::string&,
			   const unsigned short);
    AbaqusInvalidDimension(AbaqusInvalidDimension&&);
    AbaqusInvalidDimension(const AbaqusInvalidDimension&);
    virtual ~AbaqusInvalidDimension() noexcept;
  private:
    AbaqusInvalidDimension();
    AbaqusInvalidDimension&
    operator=(const AbaqusInvalidDimension&);
  }; // end of struct AbaqusInvalidDimension
  
} // end of namespace abaqus

#endif /* LIB_MFRONT_ABAQUSEXCEPTION_H_ */
