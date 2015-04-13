/*!
 * \file   mfront/include/MFront/BehaviourDSLBase.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   08 nov 2006
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONTPARSERBASE_HXX_
#define LIB_MFRONTPARSERBASE_HXX_ 

#include"MFront/BehaviourDSLCommon.hxx"

namespace mfront{

  template<typename Child>
  struct BehaviourDSLBase
    : public BehaviourDSLCommon
  {
    /*!
     * \brief analyse a file
     * \param[in] f     : file name
     * \param[in] ecmds : additionnal commands inserted treated before
     * the input file commands
     */
    virtual void
    importFile(const std::string&,
	       const std::vector<std::string>&) override;
    /*!
     * \brief return the list of keywords usable with this parser
     * \param[out] k : the list of keywords registred for this parser
     */
    virtual void
    getKeywordsList(std::vector<std::string>&) const;

  protected:
    
    typedef void (Child::* MemberFuncPtr)(void);
    typedef std::map<std::string,MemberFuncPtr> CallBackContainer;

    BehaviourDSLBase();

    void registerDefaultCallBacks(void);

    void treatDisabledCallBack(void);

    void registerNewCallBack(const std::string&,const MemberFuncPtr);

    void disableCallBack(const std::string&);

    virtual ~BehaviourDSLBase();

    CallBackContainer callBacks;
  };

} // end of namespace mfront  

#include"MFront/BehaviourDSLBase.ixx"

#endif /* LIB_MFRONTPARSERBASE_HXX_ */