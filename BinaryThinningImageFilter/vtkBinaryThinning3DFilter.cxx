
#include "vtkBinaryThinning3DFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

vtkStandardNewMacro(vtkBinaryThinning3DFilter);

//----------------------------------------------------------------------------
vtkBinaryThinning3DFilter::vtkBinaryThinning3DFilter()
{
  // Set up number of input and output ports

  // Initialize the ITK-VTK glue.
  this->Init();

  // Set up the internal filter pipeline.
  this->BinaryThinning3DFilter = ITKBinaryThinning3DFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ITKInternalFilterType>(this->BinaryThinning3DFilter);
  this->SetITKPipelineLastFilter<ITKInternalFilterType>(this->BinaryThinning3DFilter);
}

//----------------------------------------------------------------------------
vtkBinaryThinning3DFilter::~vtkBinaryThinning3DFilter()
{
}

//----------------------------------------------------------------------------
int vtkBinaryThinning3DFilter::UpdateInternalFilters()
{
//  this->BinaryThinning3DFilter->SetSigma(this->Sigma);
  this->BinaryThinning3DFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkBinaryThinning3DFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
