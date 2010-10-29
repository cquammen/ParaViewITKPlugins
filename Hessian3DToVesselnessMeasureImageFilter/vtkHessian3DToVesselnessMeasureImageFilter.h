#ifndef __vtkHessian3DToVesselnessMeasureImageFilter_h
#define __vtkHessian3DToVesselnessMeasureImageFilter_h

#include <vtkImageAlgorithm.h>

#include <itkVTKImageImport.h>
#include <itkVTKImageExport.h>
#include <itkImage.h>
#include <itkHessianRecursiveGaussianImageFilter.h>
#include <itkHessian3DToVesselnessMeasureImageFilter.h>
#include <itkImageRegionIteratorWithIndex.h>

class vtkImageExport;
class vtkImageImport;

class VTK_EXPORT vtkHessian3DToVesselnessMeasureImageFilter : public vtkImageAlgorithm
{
public:

  //BTX
  typedef float
    PixelType;
  typedef itk::Image< PixelType, 3 >
    ITKImageType;
  typedef itk::VTKImageImport< ITKImageType >
    ITKImageImportType;
  typedef itk::VTKImageExport< ITKImageType >
    ITKImageExportType;
  typedef itk::Hessian3DToVesselnessMeasureImageFilter< float >
    ITKHessian3DToVesselnessMeasureImageFilterType;
  typedef itk::HessianRecursiveGaussianImageFilter< ITKImageType >
	ITKHessianRecursiveGaussianImageFilterType;
  //ETX

  static vtkHessian3DToVesselnessMeasureImageFilter* New();
  vtkTypeMacro(vtkHessian3DToVesselnessMeasureImageFilter, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  //void SetStandardDeviation(double sigma0, double sigma1, double sigma2);
  //void SetStandardDeviation(double sigma[3]);

  vtkSetMacro(SigmaValue, double);
  vtkGetMacro(SigmaValue, double);


protected:
  vtkHessian3DToVesselnessMeasureImageFilter();
  ~vtkHessian3DToVesselnessMeasureImageFilter();

  double SigmaValue;

  vtkImageExport*                VTKExporter;
  //BTX
  ITKImageImportType::Pointer    ITKImporter;
  ITKHessian3DToVesselnessMeasureImageFilterType::Pointer   Hessian3DTVMIFilter;
  ITKHessianRecursiveGaussianImageFilterType::Pointer HessianRGIFilter;
  ITKImageExportType::Pointer    ITKExporter;
  //ETX
  vtkImageImport*                VTKImporter;

protected:
  void InitializeITKImporter();
  void InitializeITKExporter();

  int RequestData(vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector);

  //BTX
  template <class T>
  void RunITKPipeline(const T *input);
  //ETX

private:
  vtkHessian3DToVesselnessMeasureImageFilter(const vtkHessian3DToVesselnessMeasureImageFilter&);  // Not implemented.
  void operator=(const vtkHessian3DToVesselnessMeasureImageFilter&);  // Not implemented.
};

#endif
