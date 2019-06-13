# Create a supporting library for base classes
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

SET( classes
  vtkITKImageFilter
)

VTK_MODULE_ADD_MODULE(ITK::ParaViewITKPluginsCommon
  CLASSES ${classes}
)

VTK_MODULE_LINK(ITK::ParaViewITKPluginsCommon
  PRIVATE
    ${ITK_LIBRARIES}
)
# VTK_WRAP_ClientServer("ITKImageFilter"
#   ParaViewITKPluginsWrappedSrcs "${ParaViewITKPluginsCommonSrcs}")
# ADD_LIBRARY( ParaViewITKPluginsCommon
#   ${ParaViewITKPluginsCommonSrcs}
#   ${ParaViewITKPluginsWrappedSrcs}
# )
# TARGET_LINK_LIBRARIES(ParaViewITKPluginsCommon
#   PRIVATE
#     ${VTK_LIBRARIES}
#     # vtkClientServer
#     # vtkPVServerManager
#     itksys
#     ITKCommon
#     ITKBasicFilters
#     ITKIO
# )
