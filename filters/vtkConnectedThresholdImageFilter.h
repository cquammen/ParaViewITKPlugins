
#ifndef __vtkConnectedThresholdImageFilter_h
#define __vtkConnectedThresholdImageFilter_h

#include <vtkITKImageFilter.h>

#include <itkConnectedThresholdImageFilter.h>
#include <itkCurvatureFlowImageFilter.h>

class VTK_EXPORT vtkConnectedThresholdImageFilter : public vtkITKImageFilter
{
public:

  static vtkConnectedThresholdImageFilter* New();
  vtkTypeMacro(vtkConnectedThresholdImageFilter, vtkITKImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //BTX
  typedef Superclass::PixelType             PixelType;
  typedef Superclass::ITKImageType          ITKImageType;
  typedef Superclass::ITKInternalFilterType ITKInternalFilterType;
  typedef Superclass::ITKImageImportType    ITKImageImportType;
  typedef Superclass::ITKImageExportType    ITKImageExportType;
  typedef itk::CurvatureFlowImageFilter< ITKImageType, ITKImageType > CurvatureFlowImageFilterType;
  typedef itk::ConnectedThresholdImageFilter< ITKImageType, ITKImageType > ConnectedFilterType;
  //ETX

  vtkSetMacro(NumberofIterations, int);
  vtkGetMacro(NumberofIterations, int);
  vtkSetMacro(TimeStep, double);
  vtkGetMacro(TimeStep, double);
  vtkSetMacro(LowerThreshold, double);
  vtkGetMacro(LowerThreshold, double);
  vtkSetMacro(UpperThreshold, double);
  vtkGetMacro(UpperThreshold, double);
  vtkSetVector3Macro(Seed, int);
  vtkGetVector3Macro(Seed, int);

  void SetInputConnection(int port, vtkAlgorithmOutput* input) override
    {vtkImageAlgorithm::SetInputConnection(port, input);}
  void SetInputConnection(vtkAlgorithmOutput* input) override
    {vtkImageAlgorithm::SetInputConnection(input);}

protected:
  vtkConnectedThresholdImageFilter();
  ~vtkConnectedThresholdImageFilter();

  int NumberofIterations;
  double TimeStep;
  double LowerThreshold;
  double UpperThreshold;
  int Seed[3];

  //BTX
  CurvatureFlowImageFilterType::Pointer SmoothingFilter;
  ConnectedFilterType::Pointer ConnectedThresholdFilter;
  //ETX

protected:
  int UpdateInternalFilters() override;

private:
  vtkConnectedThresholdImageFilter(const vtkConnectedThresholdImageFilter&);  // Not implemented.
  void operator=(const vtkConnectedThresholdImageFilter&);  // Not implemented.
};

#endif
