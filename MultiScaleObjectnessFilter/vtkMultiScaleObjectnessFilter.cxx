#include "vtkMultiScaleObjectnessFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

#include "vtkImageShiftScale.h"

vtkStandardNewMacro(vtkMultiScaleObjectnessFilter);

//----------------------------------------------------------------------------
vtkMultiScaleObjectnessFilter::vtkMultiScaleObjectnessFilter()
{
 // Set up number of input and output ports

  // Initialize the ITK-VTK glue.
  this->Init();

  // Set up the internal filter pipeline.
  this->objectnessFilter = ObjectnessFilterType::New();
  this->multiScaleEnhancementFilter = MultiScaleEnhancementFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<MultiScaleEnhancementFilterType>(this->multiScaleEnhancementFilter);
  this->SetITKPipelineLastFilter<MultiScaleEnhancementFilterType>(this->multiScaleEnhancementFilter);
}

//----------------------------------------------------------------------------
vtkMultiScaleObjectnessFilter::~vtkMultiScaleObjectnessFilter()
{
}

//----------------------------------------------------------------------------
int vtkMultiScaleObjectnessFilter::UpdateInternalFilters()
{
  this->objectnessFilter->SetScaleObjectnessMeasure(false);
  this->objectnessFilter->SetBrightObject(true);
  this->objectnessFilter->SetAlpha(this->Alpha);
  this->objectnessFilter->SetBeta(this->Beta);
  this->objectnessFilter->SetGamma(this->Gamma);
  this->objectnessFilter->SetObjectDimension(this->ObjectDim);

  this->multiScaleEnhancementFilter->SetHessianToMeasureFilter( this->objectnessFilter );
  this->multiScaleEnhancementFilter->SetSigmaStepMethodToLogarithmic();
  this->multiScaleEnhancementFilter->SetSigmaMinimum(this->SigmaMin);
  this->multiScaleEnhancementFilter->SetSigmaMaximum(this->SigmaMax);
  this->multiScaleEnhancementFilter->SetNumberOfSigmaSteps(this->NoOfSigmaSteps);

  this->multiScaleEnhancementFilter->Modified();
  this->multiScaleEnhancementFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkMultiScaleObjectnessFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
