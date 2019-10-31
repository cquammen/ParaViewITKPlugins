paraview_add_plugin(ITKImageFilters
  VERSION "1.0"
  REQUIRED_ON_SERVER
  MODULES ITK::vtkITKImageFilters
  MODULE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/../filters/vtk.module"
    "${CMAKE_CURRENT_SOURCE_DIR}/../Common/vtk.module"
  SERVER_MANAGER_XML ParaViewITKPlugins.xml
)
