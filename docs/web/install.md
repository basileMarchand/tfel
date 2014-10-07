% Installing TFEL/MFront
% Helfer Thomas
% October 3, 2014

The latest version of this document is always available at
<http://tfel.sourceforge.net/install.html>.

It refers to the current development sources, instructions for
specific released versions are included with the sources and detailled
in the `INSTALL` and the `INSTALL-cmake` files located at the root
directory of the sources).

This document describes the generic installation procedure for
`TFEL`. The following topics were created:

- Installation on posix-compliant systems (including Linux)
- Cross-compiling `TFEL` (Linux host, Windows target)
- Compiling under Windows using [MINGW](http://www.mingw.org)

# Posix-compliant systems

This section describes the installation of `TFEL` on posix-compliant
operating systems. Please note that the main systems on which `TFEL`
was developed is `Linux`. Extensive testing on other posix-compliant
operating systems is, notably `FreeBSD`, is lacking, although
compilation and unit testing is known to work.

`TFEL` is known to work on standard architectures implemented by the
Intel and AMD processors, either 32 or 64 bits. As no specific
instructions relative to the underlying architecture is used in the
code, other architectures shall work as well.

The installation procedure itself is broken into five steps:

- Prerequisites
- Downloading the source
- Configuration
- Building
- Testing (optional)
- Final install

## Prerequisites

### Compilers 

`TFEL` version 2.0 requires a `C++-98` compliant compiler, a `C`
compiler and optionally a `fortran` compiler. The following compilers
suite are officially supported:

- [The GNU Compiler Collection](https://gcc.gnu.org/). This is the
  default on most Linux distributions and is used for the development
  of `TFEL`. Most versions starting from gcc 3.4 have been tested
  successfully, although versions prior to 4.4 are less and less used.
- [The `clang` C and C++ compilers](http://clang.llvm.org/). All
  versions starting from 3.3 have been tested successfully.
- The
  [Intel compilers suite](https://software.intel.com/en-us/c-compilers).
  All versions starting from 11.0 have been tested successfully.

### Third party libraries

`TFEL` has been designed to have no dependencies to third parties
libraries to the very exception of the
[Boost.Python](http://www.boost.org) library used to create the
optional bindings for the [`Python`](https://www.python.org/)
language.

### Third party tools


The better way to build the `TFEL` is to use the
[`cmake`](http://www.cmake.org/) build system (see this
[section](#using-the-cmake-build-system)).

To build `TFEL` documentation, one may need:

- a valid [LaTeX](http://www.latex-project.org/) installation
  (reference manual in pdf format). The author uses the
  [texlive](https://www.tug.org/texlive/) distribution available with
  major Linux distributions.
- the [doxygen](http://www.stack.nl/~dimitri/doxygen/) tool (code
  source documentation)

## Downloading the source

Official releases sources can be downloaded on
[`TFEL` sourceforge page](http://sourceforge.net/projects/tfel/files)
(see the Download entry of the navigation bar).

Sources of the development version are accessible through CEA svn
servers (see the
[Contributing](http://tfel.sourceforge.net/contributing.html) page for
details).

## Configuration

`TFEL` supports two build-systems:

- [`cmake`](http://www.cmake.org/) which is recommended.
- a `configure` script build upon the
  [GNU build system](http://www.gnu.org). This system is considered as
  deprecated and shall only be used if `cmake` is not available. Note
  that most unit tests will not be build.

### Creating a `build` directory

We highly recommend to use a separate directory to build the
sources.

In the following, we use the following convention:

- `$srcdir` points to the directory that actually contains the sources
  files.
- `$prefix` points to the final installation directory.

Both `$srcdir` and `$prefix` must contain an *absolute* path.

### Using the `cmake` build system

The use of the `cmake` build system is described in depth in the
`INSTALL-cmake` file that is located in the top directory of `TFEL`
sources.

A typical usage of `cmake` is the following:

~~~~ {#building .bash}
$ cmake $srcdir -DCMAKE_BUILD_TYPE=Release -Dlocal-castem-header=ON -Denable-fortran=ON -Denable-aster=ON -DCMAKE_INSTALL_PREFIX=$prefix
~~~~~~~~~~~~~~~~~~~~~~

This will build `TFEL` with the support of interfaces for the
[Cast3M](http://www-cast3m.cea.fr/) and
[Code-Aster](http://www.code-aster.org) finite element solvers.

Various other options can be passed to `cmake`:

- The option `-Denable-cyrano=ON` enables the interface for the
  `Cyrano3` fuel performance code.
- To enable the mechanical behaviour interface for the ZeBuLoN finite
  element solver, one may use the `-Denable-zmat=ON
  -DZSET_INSTALL_PATH=$ZSETPATH` option, where `$ZSETPATH` points to
  the installation directory of the ZeBuLoN finite element solver.
- To enable the material properties interface for the
  [`Python`](https://www.python.org/) language, one may use the
  `-Denable-python=ON` option.
- To enable bindings for the [`Python`](https://www.python.org/)
  language, notably for the `MTest` tool, one may use the
  `-Denable-python-bindings=ON` option. This requires the
  [Boost.Python](http://www.boost.org) to be available.

### Using the `configure` script

~~~~ {#building .bash}
$ ../$srcdir/configure --enable-fortran --enable-aster --enable-tests --enable-local-castem-header --prefix=${prefix}
~~~~~~~~~~~~~~~~~~~~~~

This will build `TFEL` with the support of interfaces for the
[Cast3M](http://www-cast3m.cea.fr/) and
[Code-Aster](http://www.code-aster.org) finite element solvers.

Various other options can be passed to the `configure` script:

- The option `--enable-cyrano` enables the interface for the
  `Cyrano3` fuel performance code.
- To enable the mechanical behaviour interface for the ZeBuLoN finite
  element solver, one may use the `--enable-zmat
  --with-zset=$ZSETPATH` option, where `$ZSETPATH` points to the
  installation directory of the ZeBuLoN finite element solver.
- To enable the material properties interface for the
  [`Python`](https://www.python.org/) language, one may use the
  `--enable-python` option.
- To enable bindings for the [`Python`](https://www.python.org/)
  language, notably for the `MTest` tool, one may use the
  `--enable-python-bindings` option. This requires the
  [Boost.Python](http://www.boost.org) to be available.

## Building

The configuration step created a compilation environment based on the
`make` tool.

To build `TFEL` libraries and binaries, just type:

~~~~ {#building .bash}
$ make
~~~~~~~~~~~~~~~~~~~~~~

To reduce compilation times, one may want to build `TFEL` libraries
and binaries in parallel. In this case, just type:

~~~~ {#building .bash}
$ make -j X
~~~~~~~~~~~~~~~~~~~~~~

`X` being the number of processors available.

## Testing (optional)

`TFEL` is delivered with many unit tests (more than 500 if you are
using the `cmake` build system and all the interfaces available). To
build them and execute them, just type:

~~~~ {#building .bash}
$ make check
~~~~~~~~~~~~~~~~~~~~~~

Again, compilation time can be reduced using the `-j` option of the
`make` command.

## Final install

To install `TFEL` binaries and libraries in the directory pointed by
`$prefix`, just type:

~~~~ {#building .bash}
$ make install
~~~~~~~~~~~~~~~~~~~~~~

### Installing the documentation

By default, documentation is not installed.

If a valid [LaTeX](http://www.latex-project.org) distribution is
available, reference manuals can be installed through:

~~~~ {#building .bash}
$ make doc-pdf-install
~~~~~~~~~~~~~~~~~~~~~~

If the [doxygen](http://www.stack.nl/~dimitri/doxygen/) tool is
available, code source documentation can be installed through:

~~~~ {#building .bash}
$ make doc-html-install
~~~~~~~~~~~~~~~~~~~~~~

# Cross compiling (Linux host, Windows target)

# Windows MINGW
