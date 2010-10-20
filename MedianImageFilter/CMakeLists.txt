SET(PLUGIN  MedianImageFilter)
PROJECT(${PLUGIN})

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

SET(XML MedianImageFilter.xml)

ADD_PARAVIEW_PLUGIN(${PLUGIN} 1.0
	SERVER_MANAGER_XML     ${XML}
	SERVER_MANAGER_SOURCES vtkMedianImageFilter.cxx
	SOURCES ${XML}
)

# This lets you build INSTALL and have it automatically drop the
# resulting DLLs in the directory where they will be autoloaded
# by ParaView.
INSTALL(TARGETS     ${PLUGIN} 
		DESTINATION ${ParaView_BINARY_DIR}\\plugins
)

TARGET_LINK_LIBRARIES(${PLUGIN} itksys ITKCommon ITKBasicFilters ITKIO)