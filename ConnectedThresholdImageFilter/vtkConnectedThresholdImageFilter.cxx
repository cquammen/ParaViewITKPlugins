
#include "vtkConnectedThresholdImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

vtkStandardNewMacro(vtkConnectedThresholdImageFilter);

//----------------------------------------------------------------------------
vtkConnectedThresholdImageFilter::vtkConnectedThresholdImageFilter()
{
  // Set up number of input and output ports

  // Initialize the ITK-VTK glue.
  this->Init();

  // Set up the internal filter pipeline.
  this->SmoothingFilter = CurvatureFlowImageFilterType::New();
  this->ConnectedThresholdFilter = ConnectedFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ITKInternalFilterType>(this->SmoothingFilter);
  this->SetITKPipelineLastFilter<ITKInternalFilterType>(this->ConnectedThresholdFilter);
}

//----------------------------------------------------------------------------
vtkConnectedThresholdImageFilter::~vtkConnectedThresholdImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkConnectedThresholdImageFilter::UpdateInternalFilters()
{
  this->ConnectedThresholdFilter->SetInput( this->SmoothingFilter->GetOutput() );
  this->SmoothingFilter->SetNumberOfIterations( this->NumberofIterations );
  this->SmoothingFilter->SetTimeStep( this->TimeStep );
  this->ConnectedThresholdFilter->SetLower( this->LowerThreshold );
  this->ConnectedThresholdFilter->SetUpper( this->UpperThreshold );
  ITKImageType::IndexType index;
  for (unsigned int i = 0; i < 3; i++)
  {
     index[i] = this->Seed[i];
  }
  this->ConnectedThresholdFilter->SetSeed( index );
  this->ConnectedThresholdFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkConnectedThresholdImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
