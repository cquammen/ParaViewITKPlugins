find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

set(classes
  vtkCoocurrenceTextureFeaturesImageFilter
)

vtk_module_add_module(ITK::vtkCoocurrenceTextureFeaturesImageFilter
  CLASSES ${classes}
)

vtk_module_link(ITK::vtkCoocurrenceTextureFeaturesImageFilter
  PRIVATE
    itksys
    ITKCommon
    ITKBasicFilters
    ITKIO
)
