#ifndef __vtkHessian3DToVesselnessMeasureImageFilter_h
#define __vtkHessian3DToVesselnessMeasureImageFilter_h

#include <vtkITKImageFilter.h>

#include <itkHessianRecursiveGaussianImageFilter.h>
#include <itkHessian3DToVesselnessMeasureImageFilter.h>

class VTK_EXPORT vtkHessian3DToVesselnessMeasureImageFilter : public vtkITKImageFilter
{
public:

  static vtkHessian3DToVesselnessMeasureImageFilter* New();
  vtkTypeMacro(vtkHessian3DToVesselnessMeasureImageFilter, vtkITKImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  //BTX
  typedef Superclass::PixelType             PixelType;
  typedef Superclass::ITKImageType          ITKImageType;
  typedef Superclass::ITKInternalFilterType ITKInternalFilterType;
  typedef Superclass::ITKImageImportType    ITKImageImportType;
  typedef Superclass::ITKImageExportType    ITKImageExportType;
  typedef itk::HessianRecursiveGaussianImageFilter< ITKImageType >
    ITKHessianRecursiveGaussianImageFilterType;
  typedef itk::Hessian3DToVesselnessMeasureImageFilter< float >
    ITKHessian3DToVesselnessMeasureImageFilterType;
  //ETX

  vtkSetMacro(Sigma, double);
  vtkGetMacro(Sigma, double);


protected:
  vtkHessian3DToVesselnessMeasureImageFilter();
  ~vtkHessian3DToVesselnessMeasureImageFilter();

  double Sigma;

  //BTX
  ITKHessian3DToVesselnessMeasureImageFilterType::Pointer   Hessian3DTVMIFilter;
  ITKHessianRecursiveGaussianImageFilterType::Pointer HessianRGIFilter;
  //ETX

protected:
  int UpdateInternalFilters();

private:
  vtkHessian3DToVesselnessMeasureImageFilter(const vtkHessian3DToVesselnessMeasureImageFilter&);  // Not implemented.
  void operator=(const vtkHessian3DToVesselnessMeasureImageFilter&);  // Not implemented.
};

#endif
