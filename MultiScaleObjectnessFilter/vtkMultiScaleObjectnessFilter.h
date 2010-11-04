#ifndef __vtkMultiScaleObjectnessFilter_h
#define __vtkMultiScaleObjectnessFilter_h

#include <vtkImageAlgorithm.h>

#include <itkVTKImageImport.h>
#include <itkVTKImageExport.h>
#include <itkHessianToObjectnessMeasureImageFilter.h>
#include <itkMultiScaleHessianBasedMeasureImageFilter.h>

class vtkImageExport;
class vtkImageImport;

class VTK_EXPORT vtkMultiScaleObjectnessFilter : public vtkImageAlgorithm
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
  typedef itk::NumericTraits< PixelType >::RealType 
	RealPixelType;
  typedef itk::SymmetricSecondRankTensor< RealPixelType, 3 >
	HessianPixelType;
  typedef itk::Image< HessianPixelType, 3 >
	HessianImageType;
  // Declare the type of enhancement filter
  typedef itk::HessianToObjectnessMeasureImageFilter< HessianImageType,ITKImageType > 
	ObjectnessFilterType;
  // Declare the type of multiscale enhancement filter
  typedef itk::MultiScaleHessianBasedMeasureImageFilter< ITKImageType,HessianImageType, ITKImageType > 
	MultiScaleEnhancementFilterType;
  //ETX

  static vtkMultiScaleObjectnessFilter* New();
  vtkTypeMacro(vtkMultiScaleObjectnessFilter, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  //void SetStandardDeviation(double sigma0, double sigma1, double sigma2);
  //void SetStandardDeviation(double sigma[3]);

  vtkSetMacro(Alpha, double);
  vtkGetMacro(Alpha, double);
  vtkSetMacro(Beta, double);
  vtkGetMacro(Beta, double);
  vtkSetMacro(Gamma, double);
  vtkGetMacro(Gamma, double);
  vtkSetMacro(ObjectDim, int);
  vtkGetMacro(ObjectDim, int);
  vtkSetMacro(SigmaMax, double);
  vtkGetMacro(SigmaMax, double);
  vtkSetMacro(SigmaMin, double);
  vtkGetMacro(SigmaMin, double);
  vtkSetMacro(NoOfSigmaSteps, int);
  vtkGetMacro(NoOfSigmaSteps, int);

protected:
  vtkMultiScaleObjectnessFilter();
  ~vtkMultiScaleObjectnessFilter();

  double Alpha;
  double Beta;
  double Gamma;
  int ObjectDim;
  double SigmaMax;
  double SigmaMin;
  int NoOfSigmaSteps;

  vtkImageExport*                VTKExporter;
  //BTX
  ITKImageImportType::Pointer    ITKImporter;
  ITKImageExportType::Pointer    ITKExporter;
  ObjectnessFilterType::Pointer objectnessFilter;
  MultiScaleEnhancementFilterType::Pointer multiScaleEnhancementFilter;
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
  vtkMultiScaleObjectnessFilter(const vtkMultiScaleObjectnessFilter&);  // Not implemented.
  void operator=(const vtkMultiScaleObjectnessFilter&);  // Not implemented.
};

#endif
