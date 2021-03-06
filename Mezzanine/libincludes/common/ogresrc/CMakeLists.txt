cmake_minimum_required (VERSION 2.8)
project (Ogre)
message ( "${PROJECT_NAME} - Begin" )

#Choose the destination directory
#set ( Ogre_BINARY_DIR "${ProjectBinaryDir}/ogre/" )
#set ( OGREDEPS_BINARY_DIR "${Ogre_BINARY_DIR}deps" )
set ( Ogre_BINARY_DIR ${LIBRARY_OUTPUT_PATH} )
set ( OgreDeps_BINARY_DIR ${LIBRARY_OUTPUT_PATH} )
message ( STATUS "Putting Ogre binaries in: ${Ogre_BINARY_DIR}" )
message ( STATUS "Putting Ogre Dependancy binaries in: ${OgreDeps_BINARY_DIR}" )

# Long list of changes necessary to get ogre properly merged with our project.
# One quick thing to note is that all I applied these changes from top to bottom noting their line number as I went.  If you try to use the mentioned
# line numbers in an unaltered version of the file you will have a bad time.  I try to explain the area of the file by their settings where I could to allieviate this.
#
# Before you get started it's good to know that the Dependancies folder referenced here has to be created and placed manually, and the contents of the dependancy source 
# repo placed in it minus the hg config files.
#
# The root /CMakeLists.txt the version number should be manually entered around line 251
#
# In ogresvnsrc\CMake\Utils\OgreConfigTargets.cmake:
# Have to change the output dirs listed near the top to instead use the binary dir defined above.
# Also have to update the additional paths listed below the output dirs to be empty, since we want to put it all in one place regardless of build.
# There is one exception to this, the variable "OGRE_PLUGIN_PATH" should be left as "/" or ".", but never as "".  Possibly others but in my tests
# it did not complain.
#
# --In ogresvnsrc\CMake\PrepareThreadingOptions.cmake:
# --Only thing here is have to comment out line 19, where it sets boost as the threading option if/when it finds boost.
#
# In ogresvnsrc\CMake\Dependencies.cmake:
# Need to comment out lines 56 to 72 that try to find the 4 core ogre dependencies: Zlip, Zziplib, FreeImage, and Freetype.
# Next, comment out lines 177 and 178 where it tries to find OIS.
#
# --In ogresvnsrc\CMake\InstallDependencies.cmake:
# --On line 23, mark the options OGRE_INSTALL_DEPENDENCIES and OGRE_COPY_DEPENDENCIES as advanced.
#
# In ogresvnsrc\cmakelists.txt:
# Set the commonbuild flags and force the GL rendersystem to be compiled aroun line 46.
## --At around line 102, comment out the line that adds definitions and add these two lines below it (wrapped in an if to target just mingw):
## --  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse -march=i686")
## --  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -march=i686")
# At around line 202 change the archive, output and library directory to be the same output directory as before.
# -- Around line 324 disable building of demos and RPATH usage. comment out the 4 lines around line 281 that say to build the samples, tools, xsi export, and tests.  These need to be set to false.
# Lines 311 and 315 were changed from "cmake_dependant_option" to "option", and removed the extra conditions to change the default.  
# --Have to comment out the line under "# Dependancies" at around line 180 that includes the Dependancies package. this entry is responsible for
# -- finding OpenGL, it a proxy for it must exist. I left this one and remarked out the one on line 421 
#-- Also have to change the output directories just below the dependancies line to use the binary dir defined above.
# remark out the compilation of samples on 403 and 420
# --Next, have to mark all the cmake options that are listed starting around line 256 as advanced until the allocator options at around line 314.
# --Next, the install options have to all be set to false, and disabled.  These options start around line 351.
# --Next, mark the Unity options at around line 368 as advanced.
# --Next, where the sub-directories are being added for the actual libs, add the subdirectory for Dependancies above OgreMain.  Around line 421. This
# one was remarked out in favor of the original on line 180ish
# Next, comment out the lines where the additional sub-directories are being added for the Samples and Media.  Starting around line 436.
# --set(OGRE_BUILD_RENDERSYSTEM_GL TRUE) was added near the add_definitions that we already added neat the beggining of the file.
# --OGRE_DEPENDENCIES_DIR was marked as advanced
# --Lastly, at the end of the file add "mark_as_advanced(Boost_LIB_DIAGNOSTIC_DEFINITIONS)"
# --near line 169 a check for big endianess is performed, but isn't required on 64bit systems.
#
# In ogresvnsrc\Dependencies\CMakeLists.txt:
# --Add a section to define the output directory similar to the one at the top of this file.  Put it in the same dir listed above plus "deps".
# -- I commented out the section mentioned above and instead moved it to this file, so that it could be used in the other dependencies setup.
#
# --In ogresvnsrc\Dependancies\cmake\FindDirectX.cmake:
# --At line 79 they mark a bunch of options and paths as advanced, but missed the variable "DirectX_DINPUT8_LIBRARY".  Add it to the list of things being marked as advanced.
#
# In ogresvnsrc\Dependencies\src\CMakeLists.txt:
# At the top of the file all the options to build the various dependancies must be set to true with the exception of OIS, which must be set to false.
# Next, "OGREDEPS_LIB_DIRECTORY" needs to be set to the OGREDEPS_BINARY_DIR.
# Next, Comment out the lines starting around line 22 that refer to release and debug paths, and runtime output.  Replace the runtime output with the output directory set in the above file.
#  Also replace the release and debug paths with just a "/"
# Next, Add two lines inside the "install_dep" function, near the top.  One for the Archive and another for the Library output.  Set them both to OGREDEPS_BINARY_DIR.
#
# --In ogresvnsrc\Dependencies\src\Cg\CMakeLists.txt:
# --At the top of the file, around line 4 just after the option "Cg_INCLUDE_DIR", need to add a line marking it as advanced.
# --Also, at line 43, you must mark the options "Cg_LIBRARY_REL" and "Cg_LIBRARY_DBG" as advanced.
#
# in ogresvnsrc/Dependencies/src/Cg/CMakeLists.txt
# surround the bulk of the file, after the "project(Cg)" if(MINGW OR MSVC) and endif(MINGW OR MSVC)
#
# In ogresvnsrc\Dependencies\src\zziplib\CMakeLists.txt:
# At line 38, remove the "zzip/" portion of the filename.  Also move the actual file to it's parent folder to properly account for the other change.
# The above change also necessitates removal of the "zzip/" portion of the includes in each file of the zziplib that needs it.  The files that need it are below:
# 1. lib.h
# 2. plugin.h
#
# In ogresvnsrc/Dependencies/src/FreeImage/CMakeLists.txt
# Add a line that explicitly tells cmake we need to link against winsock2 if we are on windows.  Add an "if(WIN32)" block with "target_link_libraries(FreeImage ws2_32)" to around line 536.
#
# --ogresvnsrc/Dependencies/src/FreeImage/Source/LibMNG/libmng_conf.h
# --remark out line 80 to disable the definition of MNG_OPTIMIZE_CHUNKREADER to allow compilation
#
# In ogresvnsrc\OgreMain\CMakeLists.txt:
# At around line 707, add include directories for all the dependancies of OgreMain.  Separately in an "if" statement put the Cg include directory.
# Also, just after the include directories, add a line to declare the link directory.
# After the ogremain target is created at around line 733, add a line to define the dependancies of OgreMain, with an "if" statement to exclude Cg on iOS.
# Next, in the same if statement above, add lines for declaring the link libraries for the ogre dependancies.
#
# In ogresvnsrc\OgreMain\Src\OgreZip.cpp:
# At around line 37 change the include '#include <zzip/zzip.h>' to '#include <zzip.h>'


# To get -std=c++11
# in dcraw_common.cpp 
# around 9125 this: tiff_set (&th->ntag, 270, , 512, TOFF(th->t_desc));
# should change to: tiff_set (&th->ntag, 270, 0, 512, TOFF(th->t_desc));
# and 
# around 9125 this: strcpy (th->soft, "dcraw v"DCRAW_VERSION);
# should change to: strcpy (th->soft, "dcraw v9.12");


add_subdirectory ( ogresvnsrc )

message ("${PROJECT_NAME} - End")

