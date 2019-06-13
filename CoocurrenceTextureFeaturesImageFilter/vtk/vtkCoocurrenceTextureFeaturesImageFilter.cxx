
#include "vtkCoocurrenceTextureFeaturesImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

vtkStandardNewMacro(vtkCoocurrenceTextureFeaturesImageFilter);

//----------------------------------------------------------------------------
vtkCoocurrenceTextureFeaturesImageFilter::vtkCoocurrenceTextureFeaturesImageFilter()
{
  // Set up number of input and output ports

  // Initialize the ITK-VTK glue.
  this->Init();

  // Set up the internal filter pipeline.
  this->TextureFeaturesFilter = TextureFeaturesFilterType::New();
  // this->RelabelFilter = RelabelType::New();
  this->caster = CastingFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<CastingFilterType>(this->caster);
  this->SetITKPipelineLastFilter<TextureFeaturesFilterType>(this->TextureFeaturesFilter);
}

//----------------------------------------------------------------------------
vtkCoocurrenceTextureFeaturesImageFilter::~vtkCoocurrenceTextureFeaturesImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkCoocurrenceTextureFeaturesImageFilter::UpdateInternalFilters()
{
  this->TextureFeaturesFilter->SetInput(caster->GetOutput());
  this->TextureFeaturesFilter->SetNumberOfBinsPerAxis(this->numberOfBinsPerAxis);
  this->TextureFeaturesFilter->SetHistogramMinimum(this->histMinimum);
  this->TextureFeaturesFilter->SetHistogramMaximum(this->histMaximum);
  this->Neighborhood.SetRadius(neighborhoodRadius);
  this->TextureFeaturesFilter->SetNeighborhoodRadius(Neighborhood.GetRadius());
  this->TextureFeaturesFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkCoocurrenceTextureFeaturesImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
