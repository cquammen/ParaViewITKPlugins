
#include "vtkConnectedComponentImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

vtkStandardNewMacro(vtkConnectedComponentImageFilter);

//----------------------------------------------------------------------------
vtkConnectedComponentImageFilter::vtkConnectedComponentImageFilter()
{
  // Set up number of input and output ports

  // Initialize the ITK-VTK glue.
  this->Init();

  // Set up the internal filter pipeline.
  this->ThresholdFilter = ThresholdFilterType::New();
  this->ConnectedFilter = ConnectedFilterType::New();
  this->RelabelFilter = RelabelType::New();
  this->caster = CastingFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ThresholdFilterType>(this->ThresholdFilter);
  this->SetITKPipelineLastFilter<RelabelType>(this->RelabelFilter);
}

//----------------------------------------------------------------------------
vtkConnectedComponentImageFilter::~vtkConnectedComponentImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkConnectedComponentImageFilter::UpdateInternalFilters()
{
  this->ThresholdFilter->SetInsideValue(itk::NumericTraits<PixelType>::One);
  this->ThresholdFilter->SetOutsideValue(itk::NumericTraits<PixelType>::Zero);
  this->ThresholdFilter->SetLowerThreshold(this->LowerThreshold);
  this->ThresholdFilter->SetUpperThreshold(this->UpperThreshold);
  this->ThresholdFilter->Update();
  
  this->ConnectedFilter->SetInput (this->ThresholdFilter->GetOutput());
  this->ConnectedFilter->SetFullyConnected( this->FullyConnected );
  this->ConnectedFilter->Update();
  this->caster->SetInput( ConnectedFilter->GetOutput() );
  this->RelabelFilter->SetInput( this->ConnectedFilter->GetOutput() );
  this->RelabelFilter->SetMinimumObjectSize( this->MinimumObjectSize );
  this->RelabelFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkConnectedComponentImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
