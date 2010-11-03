
#include "vtkMeanImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

vtkStandardNewMacro(vtkMeanImageFilter);

//----------------------------------------------------------------------------
vtkMeanImageFilter::vtkMeanImageFilter()
{
  // Set up number of input and output ports

  // Initialize the ITK-VTK glue.
  this->Init();

  // Set up the internal filter pipeline.
  this->MeanFilter = ITKMeanFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ITKInternalFilterType>(this->MeanFilter);
  this->SetITKPipelineLastFilter<ITKInternalFilterType>(this->MeanFilter);
}

//----------------------------------------------------------------------------
vtkMeanImageFilter::~vtkMeanImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkMeanImageFilter::UpdateInternalFilters()
{
  ITKImageType::SizeType radius;
  for (unsigned int i = 0; i < 3; i++)
    {
    radius[i] = this->NeighborhoodRadius[i];
    }
  this->MeanFilter->SetRadius(radius);
  this->MeanFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkMeanImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
