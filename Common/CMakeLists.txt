# Create a supporting library for base classes
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

SET( classes
  vtkITKImageFilter
)

vtk_module_add_module(ITK::ParaViewITKPluginsCommon
  CLASSES ${classes}
)

vtk_module_link(ITK::ParaViewITKPluginsCommon
  PRIVATE
    ${VTK_LIBRARIES}
    itksys
    ITKCommon
    ITKBasicFilters
    ITKIO
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
