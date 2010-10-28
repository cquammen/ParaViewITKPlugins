#Change PROJECT_NAME to the name of your project
#PROJECT(fftconv)

# required to avoid warnings while running cmake
#CMAKE_MINIMUM_REQUIRED(VERSION 2.4.2)

# cpack stuff
#INCLUDE(CPack)

# set the name of the input image used to run the test
#SET(INPUT_IMAGE ${CMAKE_CURRENT_SOURCE_DIR}/images/cthead1.tif)

#include some macros from another file...
#INCLUDE(IJMacros.txt)

#The following lines are required to use Dart
#ENABLE_TESTING()
#INCLUDE(Dart)
#FIND_PROGRAM(ITK_TEST_DRIVER itkTestDriver)
#SET(TEST_COMMAND ${ITK_TEST_DRIVER} --add-before-env PATH ${CMAKE_CURRENT_BINARY_DIR})

#Declare any external dependencies that your project may have here.
#examples include: ITK, VTK, JPEG, PNG, OpenGL, ZLIB, Perl, Java
#If you're not sure what name to use, look in the Modules directory of your
#cmake install and check that a file named Find(Package).cmake exists
#SET(Required_Packages
#  ITK
#)

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
#FOREACH(Package ${Required_Packages})
#  LOADPACKAGE(${Package})
#ENDFOREACH(Package)

#Set any libraries that your project depends on.
#examples: ITKCommon, VTKRendering, etc
SET(Libraries
  ITKAlgorithms
  ITKCommon
  ITKBasicFilters
  ITKIO
)


# install devel files
#OPTION(INSTALL_DEVEL_FILES "Install C++ headers" ON)
#IF(INSTALL_DEVEL_FILES)
#FILE(GLOB develFiles *.h *.txx)
#FOREACH(f ${develFiles})
#  INSTALL_FILES(/include/InsightToolkit/BasicFilters FILES ${f})
#ENDFOREACH(f)
#ENDIF(INSTALL_DEVEL_FILES)



# option for wrapping
#OPTION(BUILD_WRAPPERS "Wrap library" OFF)
#IF(BUILD_WRAPPERS)
#   SUBDIRS(Wrapping)
#ENDIF(BUILD_WRAPPERS)



#the following block of code is an example of how to build an executable in
#cmake.  Unmodified, it will add an executable called "MyExe" to the project.
#MyExe will be built using the files MyClass.h and MyClass.cxx, and it will
#be linked to all the libraries you specified above.
#You can build more than one executable per project

#IF(BUILD_TESTING)

#FOREACH(CurrentExe ref pad pad1 norm fftconv conv perf2D)
#  ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx itkFFTWLock.cxx)
#  TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})
#ENDFOREACH(CurrentExe)

#ENDIF(BUILD_TESTING)

#the following line is an example of how to add a test to your project.
#Testname is the title for this particular test.  ExecutableToRun is the
#program which will be running this test.  It can either be a part of this
#project or an external executable.  After that list any args that are needed
#for this test.  Include as many tests as you like.  If your project doesn't have
#any tests you can comment out or delete the following line.
# ADD_TEST(Testname ExecutableToRun arg1 arg2 arg3)

#ADD_TEST(FFTPad0 ${TEST_COMMAND}
#   pad ${CMAKE_CURRENT_SOURCE_DIR}/images/cthead1.tif ${CMAKE_CURRENT_SOURCE_DIR}/images/kernel.png
#   pad-image-0.png pad-kernel-0.png
#   2 0
#   --compare pad-image-0.png ${CMAKE_CURRENT_SOURCE_DIR}/images/pad-image-0.png
#   --compare pad-kernel-0.png ${CMAKE_CURRENT_SOURCE_DIR}/images/pad-kernel-0.png
#)
#ADD_TEST(FFTPad2 ${TEST_COMMAND}
#   pad ${CMAKE_CURRENT_SOURCE_DIR}/images/cthead1.tif ${CMAKE_CURRENT_SOURCE_DIR}/images/kernel.png
#   pad-image-2.png pad-kernel-2.png
#   2 2
#   --compare pad-image-2.png ${CMAKE_CURRENT_SOURCE_DIR}/images/pad-image-2.png
#   --compare pad-kernel-2.png ${CMAKE_CURRENT_SOURCE_DIR}/images/pad-kernel-2.png
#)
#ADD_TEST(FFTPad13 ${TEST_COMMAND}
#   pad ${CMAKE_CURRENT_SOURCE_DIR}/images/cthead1.tif ${CMAKE_CURRENT_SOURCE_DIR}/images/kernel.png
#   pad-image-13.png pad-kernel-13.png
#   2 13
#   --compare pad-image-13.png ${CMAKE_CURRENT_SOURCE_DIR}/images/pad-image-13.png
#   --compare pad-kernel-13.png ${CMAKE_CURRENT_SOURCE_DIR}/images/pad-kernel-13.png
#)
#ADD_TEST(FFTPadZeroFluxNeumann ${TEST_COMMAND}
#   pad ${CMAKE_CURRENT_SOURCE_DIR}/images/cthead1.tif ${CMAKE_CURRENT_SOURCE_DIR}/images/kernel.png
#   pad-image-zero-flux.png pad-kernel-zero-flux.png
#   1 2
#   --compare pad-image-zero-flux.png ${CMAKE_CURRENT_SOURCE_DIR}/images/pad-image-zero-flux.png
#   --compare pad-kernel-zero-flux.png ${CMAKE_CURRENT_SOURCE_DIR}/images/pad-kernel-2.png
#)
#ADD_TEST(FFTPadMirror ${TEST_COMMAND}
#   pad ${CMAKE_CURRENT_SOURCE_DIR}/images/cthead1.tif ${CMAKE_CURRENT_SOURCE_DIR}/images/kernel.png
#   pad-image-mirror.png pad-kernel-mirror.png
#   3 2
#   --compare pad-image-mirror.png ${CMAKE_CURRENT_SOURCE_DIR}/images/pad-image-mirror.png
#   --compare pad-kernel-mirror.png ${CMAKE_CURRENT_SOURCE_DIR}/images/pad-kernel-2.png
#)
#ADD_TEST(FFTPadWrap ${TEST_COMMAND}
#   pad ${CMAKE_CURRENT_SOURCE_DIR}/images/cthead1.tif ${CMAKE_CURRENT_SOURCE_DIR}/images/kernel.png
#   pad-image-wrap.png pad-kernel-wrap.png
#   4 2
#   --compare pad-image-wrap.png ${CMAKE_CURRENT_SOURCE_DIR}/images/pad-image-wrap.png
#   --compare pad-kernel-wrap.png ${CMAKE_CURRENT_SOURCE_DIR}/images/pad-kernel-2.png
#)
#ADD_TEST(FFTPadNoPadding ${TEST_COMMAND}
#   pad ${CMAKE_CURRENT_SOURCE_DIR}/images/cthead1.tif ${CMAKE_CURRENT_SOURCE_DIR}/images/kernel.png
#   pad-image-no-padding.png pad-kernel-no-padding.png
#   0 2
#   --compare pad-image-no-padding.png ${CMAKE_CURRENT_SOURCE_DIR}/images/cthead1.tif
#   --compare pad-kernel-no-padding.png ${CMAKE_CURRENT_SOURCE_DIR}/images/pad-kernel-no-padding.png
#)
#ADD_TEST(Norm ${TEST_COMMAND}
#   norm ${CMAKE_CURRENT_SOURCE_DIR}/images/kernel.png
#   norm.png
#   10000
#   --compare norm.png ${CMAKE_CURRENT_SOURCE_DIR}/images/norm.png
#)
#ADD_TEST(Ref ${TEST_COMMAND}
#   ref ${CMAKE_CURRENT_SOURCE_DIR}/images/cthead1.tif
#   ref.png
#   --compare ref.png ${CMAKE_CURRENT_SOURCE_DIR}/images/ref.png
#)
#ADD_TEST(FFTConv0 ${TEST_COMMAND}
#   fftconv ${CMAKE_CURRENT_SOURCE_DIR}/images/cthead1.tif ${CMAKE_CURRENT_SOURCE_DIR}/images/kernel.png
#   fftconv-0.png
#   1 0
#   --compare fftconv-0.png ${CMAKE_CURRENT_SOURCE_DIR}/images/fftconv.png
#)
#ADD_TEST(FFTConv1 ${TEST_COMMAND}
#   fftconv ${CMAKE_CURRENT_SOURCE_DIR}/images/cthead1.tif ${CMAKE_CURRENT_SOURCE_DIR}/images/kernel.png
#   fftconv-1.png
#   1 1
#   --compare fftconv-1.png ${CMAKE_CURRENT_SOURCE_DIR}/images/fftconv.png
#)
#ADD_TEST(FFTConv2 ${TEST_COMMAND}
#   fftconv ${CMAKE_CURRENT_SOURCE_DIR}/images/cthead1.tif ${CMAKE_CURRENT_SOURCE_DIR}/images/kernel.png
#   fftconv-2.png
#   1 2
#   --compare fftconv-2.png ${CMAKE_CURRENT_SOURCE_DIR}/images/fftconv.png
#)
#ADD_TEST(FFTConv13 ${TEST_COMMAND}
#   fftconv ${CMAKE_CURRENT_SOURCE_DIR}/images/cthead1.tif ${CMAKE_CURRENT_SOURCE_DIR}/images/kernel.png
#   fftconv-13.png
#   1 13
#   --compare fftconv-13.png ${CMAKE_CURRENT_SOURCE_DIR}/images/fftconv.png
#)
