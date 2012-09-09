## How to build MARMEAS ##

### You need: ### 
   - C++ tool chain (compiler, linker, ..
   - QT4 libraries 
   - QT4 Utilities (like to create help, etc)
   - cmake version 2.8.6 or higher
   - cppunit 0.11 or higher
   - doxygen 
   - NullSoft Installer toolkit (windows only)
   - MinGW environment (windows only)

### Build Procedure (for outband build) ###
   - get source and extract it into a directory e.g. 'marmeas'
   - create a 'build' directory
   - change to the build directory
   - call cmake 'marmeas' 
     you can modify some of the cmake default settings using ccmake or cmakegui
   - (on linux) call make all ; make install ; make package 

## Help/Documentation ##

   - will be created during the build procedure 
   - user documentation is done facilitating the QT Help system
   - developer documentation is done by using doxygen; the result are html 
     documents stored into the source-docs directory

# Plugins #
   - MARMEAS uses a plugin concept and encapsulates objects to be measured
     (like line, circle, polygone, etc into libraries that are loaded at 
     runtime.
   - Primitive objects are created during the build, still it is possible to 
     develop plugins independently upon the development of the application

# UnitTests #
   - MARMEAS assures major portions of the desired behavior by unittests.
   - you can execute the unit tests after you build/installed all by calling the
    'marmeas-xxx-testrunner' application. All tests must successfully pass.

# Directories and important Iiles #
===============================
   - ./src                       the source code
   - ./src/resources             resources like icons, data, etc
   - ./include                   public include files required for plugin development
   - ./plugins                   folder for base plugins
   - ./plugins/<name>            source of the plugin <name>
   - ./plugins/<name>/resources  resources for the plugin <name> 
   - ./doc                       the user and project documentation
   - ./tests                     the cppunit test code

   - ./license.txt               license terms
   - ./README.md                 this file
   - ./CMakeLists.txt            build definitions (required by cmake)                


