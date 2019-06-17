# Create a supporting library for base classes
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

SET(classes
  vtkITKImageFilter
)

VTK_MODULE_ADD_MODULE(ITK::ParaViewITKPluginsCommon
  CLASSES ${classes}
)

VTK_MODULE_LINK(ITK::ParaViewITKPluginsCommon
  PRIVATE
    ${ITK_LIBRARIES}
)
