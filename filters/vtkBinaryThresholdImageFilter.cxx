
#include "vtkBinaryThresholdImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

vtkStandardNewMacro(vtkBinaryThresholdImageFilter);

//----------------------------------------------------------------------------
vtkBinaryThresholdImageFilter::vtkBinaryThresholdImageFilter()
{
  // Set up number of input and output ports

  // Initialize the ITK-VTK glue.
  this->Init();

  // Set up the internal filter pipeline.
  this->BinaryThresholdImageFilter = ITKBinaryThresholdImageFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ITKInternalFilterType>(this->BinaryThresholdImageFilter);
  this->SetITKPipelineLastFilter<ITKInternalFilterType>(this->BinaryThresholdImageFilter);
}

//----------------------------------------------------------------------------
vtkBinaryThresholdImageFilter::~vtkBinaryThresholdImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkBinaryThresholdImageFilter::UpdateInternalFilters()
{
  this->BinaryThresholdImageFilter->SetOutsideValue( this->outsideValue );
  this->BinaryThresholdImageFilter->SetInsideValue( this->insideValue );

  this->BinaryThresholdImageFilter->SetLowerThreshold( this->lowerThreshold );
  this->BinaryThresholdImageFilter->SetUpperThreshold( this->upperThreshold );

  this->BinaryThresholdImageFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkBinaryThresholdImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
