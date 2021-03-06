/*!
 * \file  LibraryDescription.hxx
 * \brief
 * \author Thomas Helfer
 * \date   17 mars 2015
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_LIBRARYDESCRIPTION_H
#define LIB_MFRONT_LIBRARYDESCRIPTION_H

#include <string>
#include <vector>

#include "TFEL/Utilities/CxxTokenizer.hxx"
#include "MFront/MFrontConfig.hxx"
#include "MFront/MFrontUtilities.hxx"

namespace mfront {

  /*!
   * \brief This structure is used to build the compilation
   * rules for the library.
   * - the library name
   * - the generated sources
   * - the required preprocessor flags
   * - the linker flags of the generated library
   * - the generated entry points (function or class names)
   */
  struct MFRONT_VISIBILITY_EXPORT LibraryDescription {
    /*!
     * Supported target systems
     */
    enum TargetSystem {
      WINDOWS,  //<! Microsoft windows system
      CYGWIN,   //<! Cywgin environment
      MACOSX,   //<! Apple Mac Os
      UNIX      //<! General unix flavor (Linux, FreeBSD, Solaris, ...)
    };
    /*!
     * Types of library supported.
     * This difference is only relevant on Mac Os
     */
    enum LibraryType {
      SHARED_LIBRARY,  //<! a shared library meant to be used for linking
      MODULE           //<! a module is meant to be used as a plugin
    };
    /*!
     * \return the default library prefix for the given target system
     * and library type
     * \param[in] s : target system
     * \param[in] t : library type
     */
    static const char* getDefaultLibraryPrefix(const TargetSystem, const LibraryType) noexcept;
    /*!
      * \return the default library suffix for the given target system
      * and library type
      * \param[in] s : target system
      * \param[in] t : library type
      */
    static const char* getDefaultLibrarySuffix(const TargetSystem, const LibraryType) noexcept;
    /*!
     * Constructor
     * \param[in] n : name   of the library
     * \param[in] p : prefix of the library
     * \param[in] s : suffix of the library
     * \param[in] t : type   of the library
     */
    LibraryDescription(const std::string&,
                       const std::string&,
                       const std::string&,
                       const LibraryType);
    LibraryDescription(const LibraryDescription&);
    LibraryDescription(LibraryDescription&&);
    LibraryDescription& operator=(const LibraryDescription&) = delete;
    LibraryDescription& operator=(LibraryDescription&&) = delete;
    //! destructor
    ~LibraryDescription();
    //! library name
    const std::string name;
    //! libray suffix
    const std::string prefix;
    //! libray suffix
    const std::string suffix;
    //! libray type
    const LibraryType type;
    //! list of mfront files used to generate the C++ sources
    std::vector<std::string> mfront_sources;
    //! generated sources
    std::vector<std::string> sources;
    //! additional preprocessor flags
    std::vector<std::string> cppflags;
    //! path to the libraries to be linked
    std::vector<std::string> include_directories;
    //! path to the libraries to be linked
    std::vector<std::string> link_directories;
    //! libraries to be linked
    std::vector<std::string> link_libraries;
    //! dependencies to other generated libraries
    std::vector<std::string> deps;
    //! the linker flags
    std::vector<std::string> ldflags;
    //! generated entry points
    std::vector<std::string> epts;
  };  // end of struct LibraryDescription

  /*!
   * \return a string describing the library type
   * \param[in] t : library type
   */
  MFRONT_VISIBILITY_EXPORT std::string convert(const LibraryDescription::LibraryType);
  /*!
   * \brief merge two library description
   * \param[out] d : destination
   * \param[in]  s : source
   */
  MFRONT_VISIBILITY_EXPORT void mergeLibraryDescription(LibraryDescription&,
                                                        const LibraryDescription&);
  /*!
   * \brief write a library description to a stream
   * \param[out] os : output stream
   * \param[in]  t  : library description
   */
  MFRONT_VISIBILITY_EXPORT std::ostream& operator<<(std::ostream&, const LibraryDescription&);
  /*!
   * \brief read a LibraryDescription from a stream created by the
   * CxxTokenizer class
   * \param[in,out] p  : current position in the stream
   * \param[in]     pe : end of the stream
   * \return the library description read.
   * If this function succeed, p points past the last token treated.
   * If this function fails,   p is unchanged.
   */
  template <>
  MFRONT_VISIBILITY_EXPORT LibraryDescription
  read(tfel::utilities::CxxTokenizer::const_iterator&,
       const tfel::utilities::CxxTokenizer::const_iterator);

}  // end of namespace mfront

#endif /* LIB_MFRONT_LIBRARYDESCRIPTION_H */
