#ifndef __vtkOtsuThresholdImageFilter_h
#define __vtkOtsuThresholdImageFilter_h

#include <vtkImageAlgorithm.h>

#include <itkVTKImageImport.h>
#include <itkVTKImageExport.h>
#include <itkOtsuThresholdImageFilter.h>

class vtkImageExport;
class vtkImageImport;

class VTK_EXPORT vtkOtsuThresholdImageFilter : public vtkImageAlgorithm
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
  typedef itk::OtsuThresholdImageFilter< ITKImageType, ITKImageType >
    ITKOtsuThresholdImageFilterType;
  //ETX

  static vtkOtsuThresholdImageFilter* New();
  vtkTypeMacro(vtkOtsuThresholdImageFilter, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);


protected:
  vtkOtsuThresholdImageFilter();
  ~vtkOtsuThresholdImageFilter();


  vtkImageExport*                VTKExporter;
  //BTX
  ITKImageImportType::Pointer    ITKImporter;
  ITKOtsuThresholdImageFilterType::Pointer OtsuFilter;
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
  vtkOtsuThresholdImageFilter(const vtkOtsuThresholdImageFilter&);  // Not implemented.
  void operator=(const vtkOtsuThresholdImageFilter&);  // Not implemented.
};

#endif
