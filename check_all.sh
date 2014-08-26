#! /usr/bin/env bash
# Exit if any error detected
set -e

pbuild=no
while getopts ":j:" opt; do
  case $opt in
    j) pbuild=yes;
       nbproc="$OPTARG";
      ;;
    \?)
      echo "$0 : invalid option '$OPTARG'" >&2
      echo "bailing out." >&2
      exit 1
      ;;
    :)
      echo "$0 : option '-$OPTARG' requires an argument." >&2
      echo "bailing out." >&2
      exit 1
      ;;
  esac
done

make_exec=make
if test "x$pbuild" == "xyes" ;
then
    make_exec="$make_exec -j $nbproc"
fi

# remove previous temporary
if [ -d build-check ];
then
    chmod +w -R build-check
    rm -fr build-check
fi

# source directory
src=$(dirname $(realpath $0))
# current directory
build=$(pwd)

# get the package name
pkg_name=$(cat $src/configure.ac|grep AC_INIT|awk 'BEGIN{FS=","} {print $2}')

mkdir build-check
pushd build-check
mkdir autotools
pushd autotools
mkdir install-autotools
mkdir install-autotools-debug
mkdir build-autotools
pushd build-autotools
$src/configure --enable-python --enable-python-bindings --enable-fortran --enable-aster --enable-tests --enable-local-castem-header --enable-cyrano --prefix=$build/build-check/autotools/install-autotools 
$make_exec
$make_exec check
$make_exec distcheck
$make_exec doc
$make_exec install
$make_exec doc-install
popd # from build-autotools
mkdir build-autotools-debug
pushd build-autotools-debug
$src/configure --enable-python --enable-python-bindings --enable-fortran --enable-aster --enable-tests --enable-local-castem-header --enable-cyrano --prefix=$build/build-check/autotools/install-autotools-debug 
$make_exec
$make_exec check
$make_exec distcheck
$make_exec doc
$make_exec install
$make_exec doc-install
popd # from build-autotools-debug
popd # from autotools
mkdir cmake
pushd cmake
tar -xvjf $build/build-check/autotools/build-autotools/tfel-$pkg_name.tar.bz2
mkdir install-cmake
mkdir build-cmake
mkdir install-cmake-release
mkdir build-cmake-release
mkdir install-cmake-debug
mkdir build-cmake-debug
pushd build-cmake
cmake ../tfel-$pkg_name/ -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=ON -Denable-python-bindings=ON -Denable-aster=ON -Denable-cyrano=ON -DCMAKE_INSTALL_PREFIX=$build/build-check/cmake/install-cmake
$make_exec
if [ test "x$pbuild" == "xyes" ];
then
    make check ARGS="-j $nbproc"
else
    $make_exec check 
fi
$make_exec install
$make_exec doc-install
$make_exec tests-install
popd #from build-cmake
pushd build-cmake-release
cmake ../tfel-$pkg_name/ -DCMAKE_BUILD_TYPE=Release -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=ON -Denable-python-bindings=ON -Denable-aster=ON -Denable-cyrano=ON -DCMAKE_INSTALL_PREFIX=$build/build-check/cmake/install-cmake-release
$make_exec
if [ test "x$pbuild" == "xyes" ];
then
    make check ARGS="-j $nbproc"
else
    $make_exec check 
fi
$make_exec install
$make_exec doc-install
$make_exec tests-install
popd #from build-cmake-release
pushd build-cmake-debug
cmake ../tfel-$pkg_name/ -DCMAKE_BUILD_TYPE=Debug -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-python=ON -Denable-python-bindings=ON -Denable-aster=ON -Denable-cyrano=ON -DCMAKE_INSTALL_PREFIX=$build/build-check/cmake/install-cmake-debug
make
if [ test "x$pbuild" == "xyes" ];
then
    make check ARGS="-j $nbproc"
else
    $make_exec check 
fi
$make_exec install
$make_exec doc-install
$make_exec tests-install
popd #from build-cmake-debug
popd #from cmake
popd #from build-check