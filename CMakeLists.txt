PROJECT(ParaViewITKPlugins)

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

# Add supporting directories below
ADD_SUBDIRECTORY( InsightJournal )

# Add plugins directories below
ADD_SUBDIRECTORY( GaussianBlurImageFilter )
ADD_SUBDIRECTORY( LaplacianOfGaussianFilter )
ADD_SUBDIRECTORY( MedianImageFilter )
ADD_SUBDIRECTORY( NoiseImageFilter )
