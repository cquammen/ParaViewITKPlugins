# Create a supporting library for base classes
SET( ParaViewITKPluginsCommonSrcs
  vtkITKImageFilter.cxx
)
VTK_WRAP_ClientServer("ITKImageFilter"
  ParaViewITKPluginsWrappedSrcs "${ParaViewITKPluginsCommonSrcs}")
ADD_LIBRARY( ParaViewITKPluginsCommon
  ${ParaViewITKPluginsCommonSrcs}
  ${ParaViewITKPluginsWrappedSrcs}
)
TARGET_LINK_LIBRARIES(ParaViewITKPluginsCommon
  vtkClientServer
  vtkPVServerManager
  itksys
  ITKCommon
  ITKBasicFilters
  ITKIO
)
