/*!
 * \file   mfront/src/DianaFEAInterface.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08/01/2019
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>

#include "TFEL/Raise.hxx"
#include "TFEL/Config/GetInstallPath.hxx"
#include "TFEL/Utilities/StringAlgorithms.hxx"
#include "TFEL/System/System.hxx"

#include "MFront/DSLUtilities.hxx"
#include "MFront/MFrontLock.hxx"
#include "MFront/MFrontUtilities.hxx"
#include "MFront/MFrontLogStream.hxx"
#include "MFront/MFrontDebugMode.hxx"
#include "MFront/FileDescription.hxx"
#include "MFront/TargetsDescription.hxx"
#include "MFront/DianaFEASymbolsGenerator.hxx"
#include "MFront/DianaFEAInterface.hxx"

#ifndef _MSC_VER
static const char* const constexpr_c = "constexpr";
#else
static const char* const constexpr_c = "const";
#endif

namespace mfront {

  static void checkFiniteStrainStrategyDefinitionConsistency(
      const BehaviourDescription& bd) {
    if (bd.getBehaviourType() !=
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      tfel::raise(
          "checkFiniteStrainStrategyDefinitionConsistency: "
          "only standard strain based behaviours are supported by the "
          "Diana FEA interface");
    }
    if ((bd.isStrainMeasureDefined()) &&
        (bd.getStrainMeasure() != BehaviourDescription::LINEARISED)) {
      tfel::raise(
          "checkFiniteStrainStrategyDefinitionConsistency: "
          "the strain measure used by the behaviour "
          "is not supported by the Diana FEA interface");
    }
  }  // end of checkFiniteStrainStrategyDefinitionConsistency

  static void writeArguments(std::ostream& out, const BehaviourDescription&) {
    out << "(dianafea::DianaFEAReal* const sig,\n"
        << "dianafea::DianaFEAReal* const ddsdde,\n"
        << "dianafea::DianaFEAReal* const statev,\n"
        << "const dianafea::DianaFEAInt*  const ntens,\n "
        << "const dianafea::DianaFEAInt*  const nprops,\n"
        << "const dianafea::DianaFEAInt*  const nstatv,\n"
        << "const dianafea::DianaFEAReal* const eto,\n"
        << "const dianafea::DianaFEAReal* const deto,\n"
        << "const dianafea::DianaFEAReal* const dt,\n"
        << "const dianafea::DianaFEAReal* const props,\n"
        << "const dianafea::DianaFEAReal* const T,\n"
        << "const dianafea::DianaFEAReal* const dT)";
  }  // end of writeArguments

  static void writeArguments(std::ostream& out) {
    out << "(dianafea::DianaFEAReal* const,\n"        // sig
        << "dianafea::DianaFEAReal* const,\n"         // ddsdde
        << "dianafea::DianaFEAReal* const,\n"         // statev
        << "const dianafea::DianaFEAInt*  const,\n "  // ntens
        << "const dianafea::DianaFEAInt*  const,\n"   // nprops
        << "const dianafea::DianaFEAInt*  const,\n"   // nstatv
        << "const dianafea::DianaFEAReal* const,\n"   // eto
        << "const dianafea::DianaFEAReal* const,\n"   // deto
        << "const dianafea::DianaFEAReal* const,\n"   // dt
        << "const dianafea::DianaFEAReal* const,\n"   // props
        << "const dianafea::DianaFEAReal* const,\n"   // T
        << "const dianafea::DianaFEAReal* const)"     // dT
        ;
   }  // end of writeArguments

  std::string DianaFEAInterface::getName() { return "dianafea"; }

  std::string DianaFEAInterface::getInterfaceName() const {
    return "DianaFEA";
  }  // end of DianaFEAInterface::getInterfaceName

  std::pair<bool, DianaFEAInterface::tokens_iterator>
  DianaFEAInterface::treatKeyword(BehaviourDescription& bd,
                                  const std::string& k,
                                  const std::vector<std::string>& i,
                                  tokens_iterator current,
                                  const tokens_iterator end) {
    using tfel::utilities::CxxTokenizer;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "DianaFEAInterface::treatKeyword: " + m);
    };
    if (!i.empty()) {
      if (std::find(i.begin(), i.end(), this->getName()) != i.end()) {
        const auto keys =
            std::vector<std::string>{"@DianaFEAGenerateMTestFileOnFailure",
                                     "@DianaFEAStrainPerturbationValue"};
        throw_if(std::find(keys.begin(), keys.end(), k) == keys.end(),
                 "unsupported key '" + k + "'");
      } else {
        return {false, current};
      }
    }
    if (k == "@DianaFEAGenerateMTestFileOnFailure") {
      this->setGenerateMTestFileOnFailureAttribute(
          bd, this->readBooleanValue(k, current, end));
      return {true, current};
    }
    return {false, current};
  }  // end of DianaFEAInterface::treatKeyword

  void DianaFEAInterface::endTreatment(const BehaviourDescription& mb,
                                       const FileDescription& fd) const {
    using namespace tfel::system;
    auto throw_if = [](const bool b, const std::string& m) {
      tfel::raise_if(b, "DianaFEAInterface::endTreatment: " + m);
    };
    throw_if(!((mb.getBehaviourType() ==
                BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) ||
               (mb.getBehaviourType() ==
                BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR)),
             "the dianafea interface only supports small and "
             "finite strain behaviours");
    checkFiniteStrainStrategyDefinitionConsistency(mb);
    // the only supported modelling hypothesis
    constexpr const auto h = ModellingHypothesis::TRIDIMENSIONAL;
    const auto& d = mb.getBehaviourData(h);
    throw_if(d.getExternalStateVariables().size() != 1u,
             "external state variables are not supported "
             "by Diana FEA's native interface");
    // get the modelling hypotheses to be treated
    const auto name = mb.getLibrary() + mb.getClassName();
    // output directories
    systemCall::mkdir("include/MFront");
    systemCall::mkdir("include/MFront/DianaFEA");
    systemCall::mkdir("dianafea");

    std::ofstream out;

    // header
    auto fname = "DianaFEA" + name + ".hxx";
    out.open("include/MFront/DianaFEA/" + fname);
    throw_if(!out, "could not open file '" + fname + "'");

    out << "/*!\n"
        << "* \\file   " << fname << '\n'
        << "* \\brief  This file declares the dianafea interface for the "
        << mb.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    const auto header = this->getHeaderGuard(mb);
    out << "#ifndef " << header << "\n"
        << "#define " << header << "\n\n"
        << "#include\"TFEL/Config/TFELConfig.hxx\"\n\n"
        << "#include\"MFront/DianaFEA/DianaFEA.hxx\"\n"
        << "#include\"MFront/DianaFEA/DianaFEAData.hxx\"\n\n"
        << "#ifdef __cplusplus\n"
        << "#include\"MFront/DianaFEA/DianaFEATraits.hxx\"\n"
        << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n\n"
        << "#endif /* __cplusplus */\n\n";

    this->writeVisibilityDefines(out);

    out << "#ifdef __cplusplus\n\n"
        << "namespace dianafea{\n\n";

    this->writeBehaviourTraits(out, mb);

    out << "} // end of namespace dianafea\n\n"
        << "#endif /* __cplusplus */\n\n"
        << "#ifdef __cplusplus\n"
        << "extern \"C\"{\n"
        << "#endif /* __cplusplus */\n\n";

    this->writeSetOutOfBoundsPolicyFunctionDeclaration(out, name);
    this->writeSetParametersFunctionsDeclarations(out, mb, name);

    out << "MFRONT_SHAREDOBJ void\n" << this->getFunctionNameBasis(name);
    writeArguments(out);
    out << ";\n\n";

    out << "#ifdef __cplusplus\n"
        << "}\n"
        << "#endif /* __cplusplus */\n\n"
        << "#endif /* " << header << " */\n";

    out.close();

    fname = "DianaFEA" + name + ".cxx";
    out.open("src/" + fname);
    throw_if(!out, "could not open file '" + fname + "'");

    out << "/*!\n"
        << "* \\file   " << fname << '\n'
        << "* \\brief  This file implements the DianaFEA interface for the "
        << mb.getClassName() << " behaviour law\n"
        << "* \\author " << fd.authorName << '\n'
        << "* \\date   " << fd.date << '\n'
        << "*/\n\n";

    this->getExtraSrcIncludes(out, mb);

    out << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"\n"
        << "#include\"TFEL/Material/" << mb.getClassName() << ".hxx\"\n";
    if (mb.getAttribute(BehaviourData::profiling, false)) {
      out << "#include\"MFront/BehaviourProfiler.hxx\"\n\n";
    }
    if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "#include\"MFront/DianaFEA/DianaFEARotationMatrix.hxx\"\n";
    }
    out << "#include\"MFront/DianaFEA/"
           "DianaFEAStressFreeExpansionHandler.hxx\"\n\n"
        << "#include\"MFront/DianaFEA/DianaFEAInterface.hxx\"\n\n"
        << "#include\"MFront/DianaFEA/DianaFEA" << name << ".hxx\"\n\n";

    this->writeGetOutOfBoundsPolicyFunctionImplementation(out, name);

    out << "extern \"C\"{\n\n";

    DianaFEASymbolsGenerator sg;
    sg.generateGeneralSymbols(out, *this, mb, fd, {h}, name);
    sg.generateSymbols(out, *this, mb, fd, name, h);

    this->writeSetParametersFunctionsImplementations(out, mb, name);
    this->writeSetOutOfBoundsPolicyFunctionImplementation(out, name);

    // implementation body

    out << "} // end of extern \"C\"\n";
    out.close();
    this->writeInputFileExample(mb, fd, true);
  }  // end of DianaFEAInterface::endTreatment

  void DianaFEAInterface::writeInterfaceSpecificIncludes(
      std::ostream& out, const BehaviourDescription&) const {
    out << "#include\"MFront/DianaFEA/DianaFEA.hxx\"\n"
        << "#include\"MFront/DianaFEA/DianaFEAConvert.hxx\"\n\n";
  }  // end of DianaFEAInterface::writeInterfaceSpecificIncludes

  void DianaFEAInterface::writeBehaviourDataGradientSetter(
      std::ostream& os,
      const Gradient& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    tfel::raise_if(!o.isNull(),
                   "DianaFEAInterface::writeBehaviourDataMainVariablesSetter : "
                   "only one driving variable supported");
    if (Gradient::isIncrementKnown(v)) {
      os << "dianafea::ImportGradients::exe(this->" << v.name << ","
         << iprefix << "stran);\n";
    } else {
      os << "dianafea::ImportGradients::exe(this->" << v.name << "0,"
         << iprefix << "stran);\n";
    }
  }  // end of DianaFEAInterface::writeBehaviourDataGradientSetter

  void DianaFEAInterface::writeIntegrationDataGradientSetter(
      std::ostream& os,
      const Gradient& v,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    tfel::raise_if(
        !o.isNull(),
        "DianaFEAInterface::writeIntegrationDataMainVariablesSetter : "
        "only one driving variable supported");
    if (Gradient::isIncrementKnown(v)) {
      os << "dianafea::ImportGradients::exe(this->d" << v.name << ","
         << iprefix << "dstran);\n";
    } else {
      os << "dianafea::ImportGradients::exe(this->" << v.name << "1,"
         << iprefix << "dstran);\n";
    }
  }  // end of DianaFEAInterface::writeIntegrationDataGradientSetter

  void DianaFEAInterface::writeBehaviourDataThermodynamicForceSetter(
      std::ostream& os,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    if (SupportedTypes::getTypeFlag(f.type) == SupportedTypes::STENSOR) {
      os << "dianafea::ImportThermodynamicForces::exe(this->" << f.name << ",";
      if (!o.isNull()) {
        os << iprefix << "stress_+" << o << ");\n";
      } else {
        os << iprefix << "stress_);\n";
      }
    } else {
      tfel::raise(
          "DianaFEAInterface::writeBehaviourDataMainVariablesSetters : "
          "unsupported forces type");
    }
  }  // end of DianaFEAInterface::writeBehaviourDataThermodynamicForceSetter

  void DianaFEAInterface::exportThermodynamicForce(
      std::ostream& out,
      const std::string& a,
      const ThermodynamicForce& f,
      const SupportedTypes::TypeSize o) const {
    const auto iprefix = makeUpperCase(this->getInterfaceName());
    const auto flag = SupportedTypes::getTypeFlag(f.type);
    if (flag == SupportedTypes::STENSOR) {
      if (!o.isNull()) {
        out << "dianafea::ExportThermodynamicForces::exe(" << a << "+" << o
            << ",this->sig);\n";
      } else {
        out << "dianafea::ExportThermodynamicForces::exe(" << a
            << ",this->sig);\n";
      }
    } else {
      tfel::raise(
          "DianaFEAInterface::exportThermodynamicForce: "
          "unsupported forces type");
    }
  }  // end of DianaFEAInterface::exportThermodynamicForce

  void DianaFEAInterface::getTargetsDescription(
      TargetsDescription& d, const BehaviourDescription& bd) {
    const auto lib = this->getLibraryName(bd);
    const auto name = bd.getLibrary() + bd.getClassName();
    const auto tfel_config = tfel::getTFELConfigExecutableName();
    insert_if(d[lib].cppflags,
              "$(shell " + tfel_config + " --cppflags --compiler-flags)");
    insert_if(d[lib].include_directories,
              "$(shell " + tfel_config + " --include-path)");
    insert_if(d[lib].sources, "DianaFEA" + name + ".cxx");
    d.headers.push_back("MFront/DianaFEA/DianaFEA" + name + ".hxx");
    insert_if(d[lib].link_directories,
              "$(shell " + tfel_config + " --library-path)");
    insert_if(d[lib].link_libraries,
              tfel::getLibraryInstallName("DianaFEAInterface"));
    if (this->shallGenerateMTestFileOnFailure(bd)) {
      insert_if(d[lib].link_libraries,
                tfel::getLibraryInstallName("MTestFileGenerator"));
    }
#if __cplusplus >= 201703L
    insert_if(d[lib].link_libraries, "$(shell " + tfel_config +
                                         " --library-dependency "
                                         "--material --mfront-profiling)");
#else  /* __cplusplus < 201703L */
    insert_if(d[lib].link_libraries,
              "$(shell " + tfel_config +
                  " --library-dependency "
                  "--material --mfront-profiling --physical-constants)");
#endif /* __cplusplus < 201703L */
    insert_if(d[lib].epts, this->getFunctionNameBasis(name));
  }  // end of DianaFEAInterface::getTargetsDescription

  std::string DianaFEAInterface::getLibraryName(
      const BehaviourDescription& mb) const {
    if (mb.getLibrary().empty()) {
      if (!mb.getMaterialName().empty()) {
        return this->getInterfaceName() + mb.getMaterialName();
      } else {
        return this->getInterfaceName() + "Behaviour";
      }
    }
    return this->getInterfaceName() + mb.getLibrary();
  }  // end of DianaFEAInterface::getLibraryName

  std::string DianaFEAInterface::getFunctionNameBasis(
      const std::string& name) const {
    return makeUpperCase(name);
  }  // end of DianaFEAInterface::getFunctionName

  std::set<DianaFEAInterface::Hypothesis>
  DianaFEAInterface::getModellingHypothesesToBeTreated(
      const BehaviourDescription& mb) const {
    const auto& bh = mb.getModellingHypotheses();
    tfel::raise_if(bh.find(ModellingHypothesis::TRIDIMENSIONAL) == bh.end(),
                   "DianaFEAInterface::getModellingHypothesesToBeTreated : "
                   "the 'Tridimensional' hypothesis is not supported, "
                   "which is required for the DianaFEA interface");
    return {ModellingHypothesis::TRIDIMENSIONAL};
  }  // end of DianaFEAInterface::getModellingHypothesesToBeTreated

  void DianaFEAInterface::writeBehaviourTraits(
      std::ostream& out, const BehaviourDescription& mb) const {
    constexpr const auto h = ModellingHypothesis::TRIDIMENSIONAL;
    const auto mvs = mb.getMainVariablesSize();
    const auto mprops = this->buildMaterialPropertiesList(mb, h);
    out << "template<typename Type";
    if (mb.useQt()) {
      out << ",bool use_qt";
    }
    out << ">\n"
        << "struct DianaFEATraits<tfel::material::" << mb.getClassName()
        << "<tfel::material::ModellingHypothesis::TRIDIMENSIONAL,";
    out << "Type,";
    if (mb.useQt()) {
      out << "use_qt";
    } else {
      out << "false";
    }
    out << ">>\n{\n"
        << "//! behaviour type\n";
    if (mb.getBehaviourType() ==
        BehaviourDescription::STANDARDSTRAINBASEDBEHAVIOUR) {
      out << "static " << constexpr_c << " DianaFEABehaviourType btype = "
                                         "dianafea::"
                                         "STANDARDSTRAINBASEDBEHAVIOUR;\n";
    } else if (mb.getBehaviourType() ==
               BehaviourDescription::STANDARDFINITESTRAINBEHAVIOUR) {
      out << "static " << constexpr_c << " DianaFEABehaviourType btype = "
                                         "dianafea::"
                                         "STANDARDFINITESTRAINBEHAVIOUR;\n";
    } else {
      tfel::raise(
          "DianaFEAInterface::writeBehaviourTraits : "
          "unsupported behaviour type");
    }
    out << "//! space dimension\n"
        << "static " << constexpr_c << " unsigned short N "
        << "= "
           "tfel::material::ModellingHypothesisToSpaceDimension<tfel::material:"
           ":ModellingHypothesis::TRIDIMENSIONAL>::value;\n"
        << "// tiny vector size\n"
        << "static " << constexpr_c << " unsigned short TVectorSize = N;\n"
        << "// symmetric tensor size\n"
        << "static " << constexpr_c
        << " unsigned short StensorSize = "
           "tfel::math::StensorDimeToSize<N>::value;\n"
        << "// tensor size\n"
        << "static " << constexpr_c
        << " unsigned short TensorSize  = "
           "tfel::math::TensorDimeToSize<N>::value;\n"
        << "// size of the driving variable array\n"
        << "static " << constexpr_c
        << " unsigned short GradientSize = " << mvs.first << ";\n"
        << "// size of the thermodynamic force variable array (STRESS)\n"
        << "static " << constexpr_c
        << " unsigned short ThermodynamicForceVariableSize = " << mvs.second
        << ";\n";
    if (mb.getAttribute(BehaviourDescription::requiresUnAlteredStiffnessTensor,
                        false)) {
      out << "static " << constexpr_c
          << " bool requiresUnAlteredStiffnessTensor = true;\n";
    } else {
      out << "static " << constexpr_c
          << " bool requiresUnAlteredStiffnessTensor = false;\n";
    }
    if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor, false)) {
      out << "static " << constexpr_c
          << " bool requiresStiffnessTensor = true;\n";
    } else {
      out << "static " << constexpr_c
          << " bool requiresStiffnessTensor = false;\n";
    }
    if (mb.getAttribute(
            BehaviourDescription::requiresThermalExpansionCoefficientTensor,
            false)) {
      out << "static " << constexpr_c
          << " bool requiresThermalExpansionCoefficientTensor = true;\n";
    } else {
      out << "static " << constexpr_c
          << " bool requiresThermalExpansionCoefficientTensor = false;\n";
    }
    if (mb.getSymmetryType() == mfront::ISOTROPIC) {
      out << "static " << constexpr_c
          << " DianaFEASymmetryType type = dianafea::ISOTROPIC;\n";
    } else if (mb.getSymmetryType() == mfront::ORTHOTROPIC) {
      out << "static " << constexpr_c
          << " DianaFEASymmetryType type = dianafea::ORTHOTROPIC;\n";
    } else {
      tfel::raise(
          "DianaFEAInterface::writeBehaviourTraits: "
          "unsupported behaviour type.\n"
          "The DianaFEA interface only support isotropic or orthotropic "
          "behaviour at this time.");
    }
    // computing material properties size
    auto msize = SupportedTypes::TypeSize{};
    if (!mprops.first.empty()) {
      const auto& m = mprops.first.back();
      msize = m.offset;
      msize += SupportedTypes::getTypeSize(m.type, m.arraySize);
      msize -= mprops.second;
    }
    out << "static " << constexpr_c
        << " unsigned short material_properties_nb = " << msize << ";\n";
    if (mb.getElasticSymmetryType() == mfront::ISOTROPIC) {
      out << "static " << constexpr_c
          << " DianaFEASymmetryType etype = dianafea::ISOTROPIC;\n";
      if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset = 2u;\n";
      } else {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if (mb.getAttribute(
              BehaviourDescription::requiresThermalExpansionCoefficientTensor,
              false)) {
        out << "static " << constexpr_c
            << " unsigned short thermalExpansionPropertiesOffset = 1u;\n";
      } else {
        out << "static " << constexpr_c
            << " unsigned short thermalExpansionPropertiesOffset = 0u;\n";
      }
    } else if (mb.getElasticSymmetryType() == mfront::ORTHOTROPIC) {
      out << "static " << constexpr_c
          << " DianaFEASymmetryType etype = dianafea::ORTHOTROPIC;\n";
      if (mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset "
            << "= 9u;\n";
      } else {
        out << "static " << constexpr_c
            << " unsigned short elasticPropertiesOffset = 0u;\n";
      }
      if (mb.getAttribute(
              BehaviourDescription::requiresThermalExpansionCoefficientTensor,
              false)) {
        out << "static " << constexpr_c
            << " unsigned short thermalExpansionPropertiesOffset = 3u;\n";
      } else {
        out << "static " << constexpr_c
            << " unsigned short thermalExpansionPropertiesOffset = 0u;\n";
      }
    } else {
      tfel::raise(
          "DianaFEAInterface::writeBehaviourTraits: "
          "unsupported behaviour type.\n"
          "The DianaFEA interface only support isotropic or "
          "orthotropic behaviour at this time.");
    }
    out << "}; // end of class DianaFEATraits\n\n";
  }

  std::map<UMATInterfaceBase::Hypothesis, std::string>
  DianaFEAInterface::gatherModellingHypothesesAndTests(
      const BehaviourDescription& mb) const {
    auto res = std::map<Hypothesis, std::string>{};
    if ((mb.getSymmetryType() == mfront::ORTHOTROPIC) &&
        ((mb.getAttribute(BehaviourDescription::requiresStiffnessTensor,
                          false)) ||
         (mb.getAttribute(
             BehaviourDescription::requiresThermalExpansionCoefficientTensor,
             false)))) {
      for (const auto& h : this->getModellingHypothesesToBeTreated(mb)) {
        res.insert({h, this->getModellingHypothesisTest(h)});
      }
      return res;
    }
    return UMATInterfaceBase::gatherModellingHypothesesAndTests(mb);
  }  // end of DianaFEAInterface::gatherModellingHypothesesAndTests

  std::string DianaFEAInterface::getModellingHypothesisTest(
      const Hypothesis h) const {
    if (h == ModellingHypothesis::TRIDIMENSIONAL) {
      return "true";
    }
    tfel::raise(
        "DianaFEAInterface::getModellingHypothesisTest : "
        "unsupported modelling hypothesis");
  }  // end of DianaFEAInterface::gatherModellingHypothesesAndTests

  bool DianaFEAInterface::areExternalStateVariablesSupported() const {
    return false;
  }  // end of DianaFEAInterface::areExternalStateVariablesSupported()

  bool DianaFEAInterface::isTemperatureIncrementSupported() const {
    return false;
  }  // end of DianaFEAInterface::isTemperatureIncrementSupported()

  void DianaFEAInterface::writeMTestFileGeneratorSetModellingHypothesis(
      std::ostream& out) const {
    out << "mg.setModellingHypothesis(ModellingHypothesis::TRIDIMENSIONAL);\n";
  }  // end of DianaFEAInterface::writeMTestFileGeneratorSetModellingHypothesis

  void DianaFEAInterface::writeInputFileExample(const BehaviourDescription&,
                                                const FileDescription&,
                                                const bool) const {
  }  // end of DianaFEAInterface::writeInputFileExample

  DianaFEAInterface::~DianaFEAInterface() = default;

}  // end of namespace mfront
