
#ifndef __vtkMeanImageFilter_h
#define __vtkMeanImageFilter_h

#include <vtkImageAlgorithm.h>

#include <itkVTKImageImport.h>
#include <itkVTKImageExport.h>
#include <itkMeanImageFilter.h>

class vtkImageExport;
class vtkImageImport;

class VTK_EXPORT vtkMeanImageFilter : public vtkImageAlgorithm
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
  typedef itk::MeanImageFilter< ITKImageType, ITKImageType >
    ITKMeanFilterType;
  //ETX

  static vtkMeanImageFilter* New();
  vtkTypeMacro(vtkMeanImageFilter, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkSetVector3Macro(NeighborhoodRadius, int);
  vtkGetVector3Macro(NeighborhoodRadius, int);

protected:
  vtkMeanImageFilter();
  ~vtkMeanImageFilter();

  int NeighborhoodRadius[3];

  vtkImageExport*                VTKExporter;
  //BTX
  ITKImageImportType::Pointer    ITKImporter;
  ITKMeanFilterType::Pointer MeanFilter;
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
  vtkMeanImageFilter(const vtkMeanImageFilter&);  // Not implemented.
  void operator=(const vtkMeanImageFilter&);  // Not implemented.
};

#endif
