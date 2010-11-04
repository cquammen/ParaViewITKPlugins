SET( DECONVOLUTION_PLUGINS
  AdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter
  DampedRichardsonLucyDeconvolutionImageFilter
  JanssonVanCittertDeconvolutionImageFilter
  LandweberDeconvolutionImageFilter
  MaximumAPosterioriLeastSquaresDeconvolutionImageFilter
  MaximumEntropyRichardsonLucyDeconvolutionImageFilter
  PoissonMaximumAPosterioriDeconvolutionImageFilter
  RegularizedLeastSquaresDeconvolutionImageFilter
  RichardsonLucyDeconvolutionImageFilter
  TikhonovMillerDeconvolutionImageFilter
  TikhonovMillerRichardsonLucyDeconvolutionImageFilter
  VanCittertDeconvolutionImageFilter
  WienerDeconvolutionImageFilter
)


INCLUDE_DIRECTORIES( ${ParaViewITKPlugins_SOURCE_DIR}/InsightJournal/deconv )
INCLUDE_DIRECTORIES( ${ParaViewITKPlugins_SOURCE_DIR}/InsightJournal/fftconv )

SET( ParaViewITKPluginsDeconvolutionSrcs
  vtkDeconvolutionImageFilter.cxx
  vtkIterativeDeconvolutionImageFilter.cxx
)
VTK_WRAP_ClientServer("DeconvolutionImageFilter"
  WrappedSrcs "${ParaViewITKPluginsDeconvolutionSrcs}")
ADD_LIBRARY( ParaViewITKPluginsDeconvolution
  ${ParaViewITKPluginsDeconvolutionSrcs}
  ${WrappedSrcs}
  ../InsightJournal/fftconv/itkFFTWLock.cxx
)
TARGET_LINK_LIBRARIES( ParaViewITKPluginsDeconvolution
  vtkClientServer
  vtkPVServerManager
  itksys
  ITKCommon
  ITKBasicFilters
  ITKIO
)

SET( LINK_LIBRARIES
  itksys
  ITKCommon
  ITKBasicFilters
  ITKIO
  ${FFTWF_LIB}
  ${FFTWF_THREADS_LIB}
  ${FFTWD_LIB}
  ${FFTWD_THREADS_LIB}
  ParaViewITKPluginsCommon
  ParaViewITKPluginsDeconvolution
)

FOREACH( PLUGIN ${DECONVOLUTION_PLUGINS} )

  SET(XML "${PLUGIN}.xml")
  ADD_PARAVIEW_PLUGIN( "${PLUGIN}" 1.0
    SERVER_MANAGER_XML ${XML}
    SERVER_MANAGER_SOURCES "vtk${PLUGIN}.cxx"
    SOURCES ${XML}
    )

  INSTALL(TARGETS ${PLUGIN}
    DESTINATION ${ParaView_BINARY_DIR}\\plugins
    )

  TARGET_LINK_LIBRARIES(${PLUGIN}
    ${LINK_LIBRARIES}
    )

ENDFOREACH( PLUGIN )
