# INSTALL
1. Windows
2. Mac OS X
3. Linux
 
## Windows

* If using Cygwin:

  ./configure
  make
  make install
    
* If using Visual Studio:

You can use CMake to generate the VS project files

Follow the directions at the link for running CMake on Windows:
http://www.opentissue.org/wikitissue/index.php/Using_CMake

NOTE: Select the "build" folder as the location to build the binaries.

*If using MinGW:

You can use CMake to generate the MinGW Makefiles

  cmake -G "MinGW Makefiles"
  or if you are using MSYS
  cmake -G "MSYS Makefiles"

    
## Mac OS X

  ./configure
  make
  sudo make install

## Linux

  ./configure
  make
  sudo make install
