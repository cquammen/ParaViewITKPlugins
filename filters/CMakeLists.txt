find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

set(classes
  vtkBinaryThinning3DFilter
  vtkBinaryThresholdImageFilter
  vtkConnectedComponentImageFilter
  vtkConnectedThresholdImageFilter
  vtkCoocurrenceTextureFeaturesImageFilter
)

vtk_module_add_module(ITK::vtkITKImageFilters
  CLASSES ${classes}
)

vtk_module_link(ITK::vtkITKImageFilters
  PUBLIC
    ${ITK_LIBRARIES}
)
