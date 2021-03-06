/*! 
 * \file  mfront/include/MFront/CodeBlock.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 18 mai 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MFRONT_CODEBLOCK_HXX
#define LIB_MFRONT_CODEBLOCK_HXX 

#include<set>
#include<string>

#include"MFront/MFrontConfig.hxx"

namespace mfront{

  /*!
   * Raw code block
   */
  struct MFRONT_VISIBILITY_EXPORT CodeBlock
  {
    CodeBlock();
    CodeBlock(CodeBlock&&);
    CodeBlock(const CodeBlock&);
    CodeBlock& operator=(CodeBlock&&);
    CodeBlock& operator=(const CodeBlock&);
    //! destructor
    ~CodeBlock();
    //! code
    std::string code;
    //! description
    std::string description;
    //! static members used in the block
    std::set<std::string> staticMembers;
      //! members used in the block
    std::set<std::string> members;
  }; // end of struct CodeBlock
  
} // end of namespace mfront

#endif /* LIB_MFRONT_CODEBLOCK_HXX */

