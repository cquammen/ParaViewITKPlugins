SET(PLUGIN  NoiseImageFilter)
SET(XML     NoiseImageFilter.xml)

ADD_PARAVIEW_PLUGIN(${PLUGIN} 1.0
  SERVER_MANAGER_XML ${XML}
  SERVER_MANAGER_SOURCES vtkNoiseImageFilter.cxx
  SOURCES ${XML}
)

# This lets you build INSTALL and have it automatically drop the
# resulting DLLs in the directory where they will be autoloaded
# by ParaView.
INSTALL(TARGETS ${PLUGIN}
  DESTINATION ${ParaView_BINARY_DIR}\\plugins
)

INCLUDE_DIRECTORIES( ${ParaViewITKPlugins_SOURCE_DIR}/InsightJournal/noise )

TARGET_LINK_LIBRARIES(${PLUGIN}
  itksys
  ITKCommon
  ITKBasicFilters
  ITKIO
)
