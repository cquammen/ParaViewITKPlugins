
#ifndef __vtkLaplacianOfGaussianFilter_h
#define __vtkLaplacianOfGaussianFilter_h

#include <vtkImageAlgorithm.h>

#include <itkVTKImageImport.h>
#include <itkVTKImageExport.h>
#include <itkLaplacianRecursiveGaussianImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>

class vtkImageExport;
class vtkImageImport;

class VTK_EXPORT vtkLaplacianOfGaussianFilter : public vtkImageAlgorithm
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
  typedef itk::LaplacianRecursiveGaussianImageFilter< ITKImageType, ITKImageType >
    ITKLOGFilterType;
  typedef itk::RescaleIntensityImageFilter<ITKImageType,ITKImageType>  RescaleIntensityType;
  //ETX

  static vtkLaplacianOfGaussianFilter* New();
  vtkTypeMacro(vtkLaplacianOfGaussianFilter, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  //void SetStandardDeviation(double sigma0, double sigma1, double sigma2);
  //void SetStandardDeviation(double sigma[3]);

  vtkSetMacro(Sigma, double);
  vtkGetMacro(Sigma, double);

protected:
  vtkLaplacianOfGaussianFilter();
  ~vtkLaplacianOfGaussianFilter();

  double Sigma;

  vtkImageExport*                VTKExporter;
  //BTX
  ITKImageImportType::Pointer    ITKImporter;
  ITKLOGFilterType::Pointer		 LOGFilter;
  RescaleIntensityType::Pointer  RIIFFilter;
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
  vtkLaplacianOfGaussianFilter(const vtkLaplacianOfGaussianFilter&);  // Not implemented.
  void operator=(const vtkLaplacianOfGaussianFilter&);  // Not implemented.
};

#endif
