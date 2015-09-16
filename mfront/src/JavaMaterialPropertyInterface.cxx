/*!
 * \file   mfront/src/JavaMaterialPropertyInterface.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<algorithm>
#include<sstream>
#include<stdexcept>
#include<iterator>
#include<string>
#include<set>

#include<sys/types.h>
#include<sys/stat.h>
#if ! (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
#include<unistd.h>
#else
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include<io.h>
#define F_OK    0       /* Test for existence.  */
#ifndef S_ISREG
#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#endif
#endif

#include"TFEL/System/System.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"

#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontLock.hxx"
#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontUtilities.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/CMaterialPropertyInterface.hxx"
#include"MFront/JavaMaterialPropertyInterface.hxx"

#ifndef _MSC_VER
static const char * const constexpr_c = "constexpr";
#else
static const char * const constexpr_c = "const";
#endif

namespace mfront
{

  static std::string
  getJavaClassName(const MaterialPropertyDescription& mpd){
    if(mpd.library.empty()){
      if(!mpd.material.empty()){
	return mpd.material;
      } else {
	return "UnknownMaterial";
      }
    }
    return mpd.library;
  } // end of getJavaClassName

  static std::string
  getJavaClassFileName(const MaterialPropertyDescription& mpd,
		       const std::string& package){
    if(package.empty()){
      return getJavaClassName(mpd);
    }
    return package+"/"+getJavaClassName(mpd);
  } // end of getJavaClassFileName
  
  std::string
  JavaMaterialPropertyInterface::getName(void)
  {
    return "java";
  }

  JavaMaterialPropertyInterface::JavaMaterialPropertyInterface()
  {}

  std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
  JavaMaterialPropertyInterface::treatKeyword(const std::string& key,
					      tfel::utilities::CxxTokenizer::TokensContainer::const_iterator current,
					      const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator end)
  {
    auto throw_if = [](const bool b,const std::string& m){
      if(b){throw(std::runtime_error("Fortran03MaterialPropertyInterface::treatKeyword : "+m));}
    };
    if(key=="@Package"){
      throw_if(!this->package.empty(),"package name already defined");
      throw_if(current==end,"unexpected end of file");
      const auto p = current->value;
      throw_if(!tfel::utilities::CxxTokenizer::isValidIdentifier(p,true),
	       "invalid package name '"+p+"'");
      for(const auto c : p){
	if(isalpha(c)){
	  throw_if(!islower(c),"invalid package name '"+p+"'");
	}
      }
      throw_if(++current==end,"unexpected end of file");
      throw_if(current->value!=";","expected ';', read '"+current->value+"'");
      ++(current);
      this->package = p;
      return make_pair(true,current);      
    }
    return {false,current};
  } // end of treatKeyword

  JavaMaterialPropertyInterface::~JavaMaterialPropertyInterface()
  {}

  void
  JavaMaterialPropertyInterface::getTargetsDescription(TargetsDescription& d,
						       const MaterialPropertyDescription& mpd)
  {
    const auto lib = "lib"+getMaterialLawLibraryNameBase(mpd.library,mpd.material)+"-java";
    const auto name = (mpd.material.empty()) ? mpd.className : mpd.material+"_"+mpd.className;
    insert_if(d[lib].ldflags,"-lm");    
    // the jni part
    insert_if(d[lib].cppflags,TFEL_JAVA_INCLUDES);
    insert_if(d[lib].sources,name+"-java.cxx");
    if(this->package.empty()){
      insert_if(d[lib].epts,getJavaClassName(mpd)+"."+mpd.law);
    } else {
      insert_if(d[lib].epts,this->package+"."+getJavaClassName(mpd)+"."+mpd.law);
    }
    // the java class
    const auto jfname = getJavaClassFileName(mpd,this->package);
    const auto src    = "../java/"+jfname+".java";
    const auto target = "../java/"+jfname+".class";
    auto cmd = std::string{};
    const char * java = ::getenv("JAVAC");
    if(java==nullptr){
      cmd += "javac";
    } else {
      cmd += java;
    }
    cmd += " "+src;
    d.specific_targets[target].first.clear();
    d.specific_targets[target].first.push_back(src);
    d.specific_targets[target].second.clear();
    d.specific_targets[target].second.push_back(cmd);
    insert_if(d.specific_targets["all"].first,target);
  } // end of JavaMaterialPropertyInterface::getTargetsDescription

  void
  JavaMaterialPropertyInterface::writeOutputFiles(const MaterialPropertyDescription& mpd,
						  const FileDescription& fd)
  {
    auto throw_if = [](const bool b, const std::string& m){
      if(b){throw(std::runtime_error("JavaMaterialPropertyInterface::writeOutputFiles : "+m));}
    };
    using tfel::utilities::replace_all;
    tfel::system::systemCall::mkdir("java");
    if(!this->package.empty()){
      tfel::system::systemCall::mkdir("java/"+this->package);
    }
    const auto lib   = getMaterialLawLibraryNameBase(mpd.library,mpd.material)+"-java";
    const auto cname = (!mpd.material.empty()) ? mpd.material : "UnknowMaterial";
    const auto name  = (!mpd.material.empty()) ? mpd.material+"_"+mpd.law : mpd.law;
    const auto srcFileName     = "src/" + name + "-java.cxx";
    std::ofstream srcFile{srcFileName};
    throw_if(!srcFile,"unable to open '"+ srcFileName+"' for writing output file.");
    srcFile.exceptions(std::ios::badbit|std::ios::failbit);
    // writing source
    srcFile << "/*!\n"
	    << "* \\file   " << srcFileName  << '\n'
	    << "* \\brief  " << "this file implements the java interface "
	    << "for the " << name << " material law.\n"
	    << "*         File generated by "
	    << MFrontHeader::getVersionName() << " "
	    << "version " << MFrontHeader::getVersionNumber() << '\n';
    if(!fd.authorName.empty()){
      srcFile << "* \\author " << fd.authorName << '\n';
    }
    if(!fd.date.empty()){
      srcFile << "* \\date   " << fd.date       << '\n';
    }
    srcFile << " */\n\n";
    // headers 
    srcFile << "#include<algorithm>\n"
	    << "#include<iostream>\n"
	    << "#include<sstream>\n"
      	    << "#include<cstring>\n"
	    << "#include<cstdlib>\n"
	    << "#include<string>\n"
	    << "#include<cmath>\n\n"
	    << "#include <jni.h>\n\n";
    if(!mpd.includes.empty()){
      srcFile << mpd.includes << "\n\n";
    }
    srcFile << "#ifdef __cplusplus\n";
    srcFile << "extern \"C\"{\n";
    srcFile << "#endif /* __cplusplus */\n\n";
    srcFile << "JNIEXPORT jdouble JNICALL\n";
    if(this->package.empty()){
      srcFile << "Java_" << replace_all(getJavaClassName(mpd),"_","_1");
    } else {
      srcFile << "Java_" << replace_all(this->package,"_","_1") << "_"
	      << replace_all(getJavaClassName(mpd),"_","_1");
    }
    srcFile << "_" <<  replace_all(mpd.law,"_","_1") << "(";
    srcFile << "JNIEnv *java_env,jclass";
    for(const auto i:mpd.inputs){
      srcFile << ", const jdouble " << i.name;
    }
    srcFile << ")\n{\n";
    srcFile << "using namespace std;\n";
    srcFile << "using real = jdouble;\n";
    // handle java exceptions
    srcFile << "auto throwJavaRuntimeException = [java_env](const string& msg){\n"
	    << "  auto jexcept = java_env->FindClass(\"java/lang/RuntimeException\");\n"
	    << "  if (jexcept==nullptr){\n"
	    << "    cerr << \"Internal error : can't find the java \"\n"
	    << "         << \"RuntimeException class\" << endl;\n"
	    << "    ::exit(EXIT_FAILURE);\n"
	    << "  }\n"
	    << "  java_env->ThrowNew(jexcept,msg.c_str());\n"
	    << "  java_env->DeleteLocalRef(jexcept);\n"
	    << "  return jdouble{};\n"
	    << "};\n";
    // material laws
    writeMaterialLaws("JavaMaterialPropertyInterface::writeOutputFile",
		      srcFile,mpd.materialLaws);
    // static variables
    writeStaticVariables("JavaMaterialPropertyInterface::writeOutputFile",
			 srcFile,mpd.staticVars,fd.fileName);
    // parameters
    for(const auto& p : mpd.parameters){
      const auto p6 = mpd.parametersValues.find(p);
      throw_if(p6==mpd.parametersValues.end(),
	       "internal error (can't find value of parameter '"+p+"')");
      srcFile << "static " << constexpr_c << " double " << p << " = " << p6->second << ";\n";
    }
    if((!mpd.physicalBoundsDescriptions.empty())||(!mpd.boundsDescriptions.empty())){
      srcFile << "#ifndef JAVA_NO_BOUNDS_CHECK\n";
      if(!mpd.physicalBoundsDescriptions.empty()){
	srcFile << "// treating physical bounds\n";
	for(const auto& b : mpd.physicalBoundsDescriptions){
	  if(b.boundsType==VariableBoundsDescription::Lower){
	    srcFile << "if(" << b.varName<< " < "<< b.lowerBound << "){\n";
	    srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
	    	    << b.varName << " is below its physical lower bound (\"\n << "
	    	    << b.varName << " << \"<" << b.lowerBound << ").\";\n";
	    srcFile << "return throwJavaRuntimeException(msg.str());\n";
	    srcFile << "}\n";
	  } else if(b.boundsType==VariableBoundsDescription::Upper){
	    srcFile << "if(" << b.varName<< " > "<< b.upperBound << "){\n";
	    srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
	    	    << b.varName << " is beyond its physical upper bound (\"\n << "
	    	    << b.varName << " << \">" << b.upperBound << ").\";\n";
	    srcFile << "return throwJavaRuntimeException(msg.str());\n";
	    srcFile << "}\n";
	  } else {
	    srcFile << "if((" << b.varName<< " < "<< b.lowerBound << ")||"
	    	    << "(" << b.varName<< " > "<< b.upperBound << ")){\n";
	    srcFile << "if(" << b.varName<< " < " << b.lowerBound << "){\n";
	    srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
	    	    << b.varName << " is below its physical lower bound (\"\n << "
	    	    << b.varName << " << \"<" << b.lowerBound << ").\";\n";
	    srcFile << "return throwJavaRuntimeException(msg.str());\n";
	    srcFile << "} else {\n";
	    srcFile << "ostringstream msg;\nmsg << \"" << name << " : "
	    	    << b.varName << " is beyond its physical upper bound (\"\n << "
	    	    << b.varName << " << \">" << b.upperBound << ").\";\n";
	    srcFile << "return throwJavaRuntimeException(msg.str());\n";
	    srcFile << "}\n";
	    srcFile << "}\n";
	  }
	}
	if(!mpd.boundsDescriptions.empty()){
	  srcFile << "// treating standard bounds\n";
	  for(const auto& b : mpd.boundsDescriptions){
	    if((b.boundsType==VariableBoundsDescription::Lower)||
	       (b.boundsType==VariableBoundsDescription::LowerAndUpper)){
	      srcFile << "if(" << b.varName<< " < "<< b.lowerBound << "){\n";
	      srcFile << "auto policy = "
		      << "::getenv(\"JAVA_OUT_OF_BOUNDS_POLICY\");\n";
	      srcFile << "if(policy!=nullptr){\n";
	      srcFile << "if((strcmp(policy,\"STRICT\")==0)||"
		      << "(strcmp(policy,\"WARNING\")==0)){\n";
	      srcFile << "ostringstream msg;\n";
	      srcFile << "msg << \"" << name << " : "
		      << b.varName << " is below its lower bound (\"\n << "
		      << b.varName << " << \"<" << b.lowerBound << ").\";\n";
	      srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	      srcFile << "return throwJavaRuntimeException(msg.str());\n";
	      srcFile << "} else {\n";
	      srcFile << "fprintf(stderr,\"%s\\n\",msg.str().c_str());\n";
	      srcFile << "}\n";
	      srcFile << "}\n";
	      srcFile << "}\n";
	      srcFile << "}\n";
	    }
	    if((b.boundsType==VariableBoundsDescription::Upper)||
	       (b.boundsType==VariableBoundsDescription::LowerAndUpper)){
	      srcFile << "if(" << b.varName<< " > "<< b.upperBound << "){\n";
	      srcFile << "auto policy = "
		      << "::getenv(\"JAVA_OUT_OF_BOUNDS_POLICY\");\n";
	      srcFile << "if(policy!=nullptr){\n";
	      srcFile << "if((strcmp(policy,\"STRICT\")==0)||"
		      << "(strcmp(policy,\"WARNING\")==0)){\n";
	      srcFile << "ostringstream msg;\n";
	      srcFile << "msg << \"" << name << " : "
		      << b.varName << " is over its upper bound (\"\n << "
		      << b.varName << " << \">" << b.upperBound << ").\";\n";
	      srcFile << "if(strcmp(policy,\"STRICT\")==0){\n";
	      srcFile << "return throwJavaRuntimeException(msg.str());\n";
	      srcFile << "} else {\n";
	      srcFile << "fprintf(stderr,\"%s\\n\",msg.str().c_str());\n";
	      srcFile << "}\n";
	      srcFile << "}\n";
	      srcFile << "}\n";
	      srcFile << "}\n";
	    }
	  }
	}
	srcFile << "#endif /* JAVA_NO_BOUNDS_CHECK */\n";
      }
    }
    srcFile << "jdouble " << mpd.output << ";\n"
	    << "try{\n"
	    << mpd.f.body
	    << "} catch(exception& cpp_except){\n"
	    << "  return throwJavaRuntimeException(cpp_except.what());\n"
	    << "} catch(...){\n"
      	    << "  return throwJavaRuntimeException(\"unknown C++ exception\");\n"
	    << "}\n"
	    << "return " << mpd.output << ";\n"
	    << "} // end of " << name << "\n\n"
	    << "#ifdef __cplusplus\n"
	    << "} // end of extern \"C\"\n"
	    << "#endif /* __cplusplus */\n\n";
    srcFile.close();
    // writing java class
    MFrontLockGuard lock;
    auto methods = std::map<std::string,decltype(mpd.inputs.size())>{};
    auto jcname = getJavaClassName(mpd);
    auto jcmlst = "src/"+jcname+"-java.lst";
    struct stat buffer; // for call to stat
    if(access(jcmlst.c_str(),F_OK)==0){
      stat(jcmlst.c_str(),&buffer);
      throw_if(!S_ISREG(buffer.st_mode),"'"+jcmlst+"' is not a regular file");
      std::ifstream f(jcmlst);
      throw_if(!f,"unable to open '"+jcmlst+"'");
      auto line = std::string();
      std::getline(f,line);
      throw_if(line!=this->package,
	       "inconsistent package name for class '"+jcname+"' "
	       "(read '"+line+"')");
      while (std::getline(f, line)){
	std::istringstream iss{line};
	iss.exceptions(std::ios::badbit|std::ios::failbit);
	auto m = std::string{}; // method name
	auto n = int{};    // number of arguments
	iss >> m >> n;
	methods.insert({m,n});
      }
      f.close();
    }
    methods[mpd.law] = mpd.inputs.size();
    std::ofstream mf{jcmlst};
    throw_if(!mf,"unable to open '"+jcmlst+"'");
    mf.exceptions(std::ios::badbit|std::ios::failbit);
    mf << this->package << '\n';
    for(const auto& m:methods){
      mf << m.first << " " << m.second << "\n";
    }
    mf.close();
    std::ofstream jc{"java/"+getJavaClassFileName(mpd,this->package)+".java"};
    throw_if(!jc,"unable to open '"+jcname+".java'");
    jc.exceptions(std::ios::badbit|std::ios::failbit);
    if(!this->package.empty()){
      jc << "package " << this->package << ";\n\n";
    }
    jc << "public class " << jcname <<"{\n"
       << "static {System.loadLibrary(\"" << lib  << "\");}\n";
    for(const auto& m:methods){
      jc << "public static native double " << m.first << "(";
      for(decltype(m.second) i=0;i!=m.second;++i){
	if(i!=0){
	  jc << ",\n";
	}
	jc << "double x" << i;
      }
      jc << ");\n";
    }
    jc << "}\n";
    jc.close();
  } // end of JavaMaterialPropertyInterface::writeSrcFile(void)

} // end of namespace mfront