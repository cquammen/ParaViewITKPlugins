SET(PLUGIN  WienerDeconvolutionImageFilter)
SET(XML     WienerDeconvolutionImageFilter.xml)
ADD_PARAVIEW_PLUGIN( WienerDeconvolutionImageFilter 1.0
  SERVER_MANAGER_XML ${XML}
  SERVER_MANAGER_SOURCES vtkWienerDeconvolutionImageFilter.cxx
  SOURCES ${XML}
)

INSTALL(TARGETS ${PLUGIN}
  DESTINATION ${ParaView_BINARY_DIR}\\plugins
)

INCLUDE_DIRECTORIES( ${ParaViewITKPlugins_SOURCE_DIR}/InsightJournal/deconv )
INCLUDE_DIRECTORIES( ${ParaViewITKPlugins_SOURCE_DIR}/InsightJournal/fftconv )

TARGET_LINK_LIBRARIES(${PLUGIN}
  itksys
  ITKCommon
  ITKBasicFilters
  ITKIO
  ${FFTWF_LIB}
  ${FFTWF_THREADS_LIB}
  ${FFTWD_LIB}
  ${FFTWD_THREADS_LIB}
  ParaViewITKPluginsCommon
)
