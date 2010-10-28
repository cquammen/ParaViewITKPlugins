PROJECT(ParaViewITKPlugins)

SET(CMAKE_MODULE_PATH ${ParaViewITKPlugins_SOURCE_DIR}/CMake)

FIND_PACKAGE(ParaView REQUIRED)
IF(ParaView_FOUND)
  INCLUDE(${PARAVIEW_USE_FILE})
  INCLUDE(${ParaView_CMAKE_DIR}/ParaViewPlugins.cmake)
ELSE(ParaView_FOUND)
  MESSAGE(FATAL_ERROR "Cannot locate ParaView directory.")
ENDIF(ParaView_FOUND)

FIND_PACKAGE(ITK REQUIRED)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR "Cannot locate ITK directory.")
ENDIF(ITK_FOUND)

MARK_AS_ADVANCED(USE_FFTWD)
OPTION(USE_FFTWD "Use double precision FFTW if found" ON)
MARK_AS_ADVANCED(USE_FFTWF)
OPTION(USE_FFTWF "Use single precision FFTW if found" ON)
FIND_PACKAGE(FFTW REQUIRED)

# Add supporting directories below
ADD_SUBDIRECTORY( InsightJournal )

# Add plugins directories below
ADD_SUBDIRECTORY( FFTConvolutionImageFilter )
ADD_SUBDIRECTORY( GaussianBlurImageFilter )
ADD_SUBDIRECTORY( LaplacianOfGaussianFilter )
ADD_SUBDIRECTORY( MedianImageFilter )
ADD_SUBDIRECTORY( NoiseImageFilter )
