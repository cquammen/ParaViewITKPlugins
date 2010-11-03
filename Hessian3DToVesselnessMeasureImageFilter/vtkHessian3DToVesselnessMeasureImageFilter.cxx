
#include "vtkHessian3DToVesselnessMeasureImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

#include "vtkImageShiftScale.h"

vtkStandardNewMacro(vtkHessian3DToVesselnessMeasureImageFilter);

//----------------------------------------------------------------------------
vtkHessian3DToVesselnessMeasureImageFilter::vtkHessian3DToVesselnessMeasureImageFilter()
{
  // Set up number of input and output ports

  // Initialize the ITK-VTK glue.
  this->Init();

  // Set up the internal filter pipeline.
  this->HessianRGIFilter = ITKHessianRecursiveGaussianImageFilterType::New();
  this->Hessian3DTVMIFilter = ITKHessian3DToVesselnessMeasureImageFilterType::New();
  this->Hessian3DTVMIFilter->SetInput(HessianRGIFilter->GetOutput());

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ITKHessianRecursiveGaussianImageFilterType>(this->HessianRGIFilter);
  this->SetITKPipelineLastFilter<ITKHessian3DToVesselnessMeasureImageFilterType>(this->Hessian3DTVMIFilter);
}

//----------------------------------------------------------------------------
vtkHessian3DToVesselnessMeasureImageFilter::~vtkHessian3DToVesselnessMeasureImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkHessian3DToVesselnessMeasureImageFilter::UpdateInternalFilters()
{
  // Set ITK pipeline settings here
  this->HessianRGIFilter->SetSigma(this->Sigma);
  this->Hessian3DTVMIFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkHessian3DToVesselnessMeasureImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
