#ifndef __vtkMultiScaleObjectnessFilter_h
#define __vtkMultiScaleObjectnessFilter_h

#include <vtkITKImageFilter.h>

#include <itkHessianToObjectnessMeasureImageFilter.h>
#include <itkMultiScaleHessianBasedMeasureImageFilter.h>

class VTK_EXPORT vtkMultiScaleObjectnessFilter : public vtkITKImageFilter
{
public:

  static vtkMultiScaleObjectnessFilter* New();
  vtkTypeMacro(vtkMultiScaleObjectnessFilter, vtkITKImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  //BTX
  typedef Superclass::PixelType             PixelType;
  typedef Superclass::ITKImageType          ITKImageType;
  typedef Superclass::ITKInternalFilterType ITKInternalFilterType;
  typedef Superclass::ITKImageImportType    ITKImageImportType;
  typedef Superclass::ITKImageExportType    ITKImageExportType;
  typedef itk::NumericTraits< PixelType >::RealType RealPixelType;
  typedef itk::SymmetricSecondRankTensor< RealPixelType, 3 > HessianPixelType;
  typedef itk::Image< HessianPixelType, 3 > HessianImageType;
  // Declare the type of enhancement filter
  typedef itk::HessianToObjectnessMeasureImageFilter< HessianImageType,ITKImageType > 
	ObjectnessFilterType;
  // Declare the type of multiscale enhancement filter
  typedef itk::MultiScaleHessianBasedMeasureImageFilter< ITKImageType,HessianImageType, ITKImageType > 
	MultiScaleEnhancementFilterType;
  //ETX

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

  //BTX
  ObjectnessFilterType::Pointer objectnessFilter;
  MultiScaleEnhancementFilterType::Pointer multiScaleEnhancementFilter;
  //ETX

protected:
	int UpdateInternalFilters();

private:
  vtkMultiScaleObjectnessFilter(const vtkMultiScaleObjectnessFilter&);  // Not implemented.
  void operator=(const vtkMultiScaleObjectnessFilter&);  // Not implemented.
};

#endif
