CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

PROJECT(ParaViewITKPlugins)

INCLUDE(GNUInstallDirs)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

OPTION(${PROJECT_NAME}_BUILD_SHARED_LIBS
  "Build ${PROJECT_NAME}  with shared libraries" ON)
SET(BUILD_SHARED_LIBS "${${PROJECT_NAME}_BUILD_SHARED_LIBS}")

SET(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/CMake
  ${CMAKE_MODULE_PATH}
)

# ParaView
FIND_PACKAGE(ParaView REQUIRED)

# Plugins
# Find paraview.plugin files in subdirectories
PARAVIEW_PLUGIN_FIND_PLUGINS(
  ${PROJECT_NAME}Files
  ${CMAKE_SOURCE_DIR}
)

# Add the found plugins to the build system
PARAVIEW_PLUGIN_SCAN(
  PLUGIN_FILES ${${PROJECT_NAME}Files}
  PROVIDES_PLUGINS plugins
  REQUIRES_MODULES required_modules
  ENABLE_BY_DEFAULT ON
)

FOREACH(module IN LISTS required_modules)
  IF(NOT TARGET "${module}")
    MESSAGE("Missing required module: ${module}")
    RETURN()
  ENDIF()
ENDFOREACH()

PARAVIEW_PLUGIN_BUILD(
  RUNTIME_DESTINATION "${CMAKE_INSTALL_BINDIR}"
  LIBRARY_DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  LIBRARY_SUBDIRECTORY "${PARAVIEW_PLUGIN_SUBDIR}"
  PLUGINS ${plugins}
)

# # ITK
# FIND_PACKAGE(ITK REQUIRED)
# INCLUDE(${ITK_USE_FILE})
# 
# MARK_AS_ADVANCED(USE_FFTWD)
# OPTION(USE_FFTWD "Use double precision FFTW if found" ON)
# MARK_AS_ADVANCED(USE_FFTWF)
# OPTION(USE_FFTWF "Use single precision FFTW if found" ON)
# FIND_PACKAGE(FFTW REQUIRED)
# 
# INCLUDE_DIRECTORIES(
#   ${ParaViewITKPlugins_SOURCE_DIR}
#   ${ParaViewITKPlugins_SOURCE_DIR}/Common
# )
# 
# 
# # Add supporting directories below
# ADD_SUBDIRECTORY( Common )
# ADD_SUBDIRECTORY( InsightJournal )
# 
# # Add plugins directories below
# ADD_SUBDIRECTORY( GaussianBlurImageFilter )
# ADD_SUBDIRECTORY( GaussianImageSource )
# ADD_SUBDIRECTORY( LaplacianOfGaussianFilter )
# ADD_SUBDIRECTORY( MedianImageFilter )
# ADD_SUBDIRECTORY( MeanImageFilter )
# ADD_SUBDIRECTORY( OtsuThresholdImageFilter )
# ADD_SUBDIRECTORY( Hessian3DToVesselnessMeasureImageFilter)
# ADD_SUBDIRECTORY( BinaryThinningImageFilter )
# ADD_SUBDIRECTORY( MultiScaleObjectnessFilter )
# ADD_SUBDIRECTORY( BinaryThresholdImageFilter )
# ADD_SUBDIRECTORY( ConnectedThresholdImageFilter )
# ADD_SUBDIRECTORY( ConnectedComponentImageFilter )
# 
# # These plugin does not currently compile on Windows
# IF( NOT WIN32 )
#   ADD_SUBDIRECTORY( NoiseImageFilter )
#   ADD_SUBDIRECTORY( FFTConvolutionImageFilter )
#   ADD_SUBDIRECTORY( DeconvolutionFilters )
# ENDIF( NOT WIN32 )
