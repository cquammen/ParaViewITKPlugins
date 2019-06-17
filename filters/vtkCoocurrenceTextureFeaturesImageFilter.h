
#ifndef __vtkCoocurrenceTextureFeaturesImageFilter_h
#define __vtkCoocurrenceTextureFeaturesImageFilter_h

#include <vtkITKImageFilter.h>

#include <itkCoocurrenceTextureFeaturesImageFilter.h>
#include <itkRelabelComponentImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkCastImageFilter.h>

class VTK_EXPORT vtkCoocurrenceTextureFeaturesImageFilter : public vtkITKImageFilter
{
public:

  static vtkCoocurrenceTextureFeaturesImageFilter* New();
  vtkTypeMacro(vtkCoocurrenceTextureFeaturesImageFilter, vtkITKImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //BTX
  typedef Superclass::PixelType                  PixelType;
  typedef Superclass::ITKFloatImageType          ITKFloatImageType;
  typedef Superclass::ITKFloatImageType          InputImageType;
  typedef Superclass::ITKIntImageType            ITKIntImageType;
  typedef Superclass::ITKFloatVectorImageType    OutputImageType;
  typedef itk::CastImageFilter<ITKFloatImageType, ITKIntImageType>
    CastingFilterType;

  typedef itk::Neighborhood<typename InputImageType::PixelType,
                            InputImageType::ImageDimension>
    NeighborhoodType;

  typedef itk::Statistics::
    CoocurrenceTextureFeaturesImageFilter<ITKIntImageType, OutputImageType>
      TextureFeaturesFilterType;
  //ETX

  void SetNumberOfBinsPerAxis(int val)
  {
    numberOfBinsPerAxis = val;
    this->Modified();
  }
  int GetNumberOfBinsPerAxis()
  {
    return numberOfBinsPerAxis;
  }
  void SetHistogramMinimum(double val)
  {
    histMinimum = val;
    this->Modified();
  }
  double GetHistogramMinimum()
  {
    return histMinimum;
  }
  void SetHistogramMaximum(double val)
  {
    histMaximum = val;
    this->Modified();
  }
  double GetHistogramMaximum()
  {
    return histMaximum;
  }
  void SetNeighborhoodRadius(int val)
  {
    neighborhoodRadius = val;
    this->Modified();
  }
  int GetNeighborhoodRadius()
  {
    return neighborhoodRadius;
  }

  vtkSetMacro(numberOfBinsPerAxis, int);
  vtkGetMacro(numberOfBinsPerAxis, int);
  vtkSetMacro(histMinimum, double);
  vtkGetMacro(histMinimum, double);
  vtkSetMacro(histMaximum, double);
  vtkGetMacro(histMaximum, double);
  vtkSetMacro(neighborhoodRadius, int);
  vtkGetMacro(neighborhoodRadius, int);

  void SetInputConnection(int port, vtkAlgorithmOutput* input) override
    {vtkImageAlgorithm::SetInputConnection(port, input);}
  void SetInputConnection(vtkAlgorithmOutput* input) override
    {vtkImageAlgorithm::SetInputConnection(input);}


protected:
  vtkCoocurrenceTextureFeaturesImageFilter();
  ~vtkCoocurrenceTextureFeaturesImageFilter();

  int numberOfBinsPerAxis;
  double histMinimum;
  double histMaximum;
  int neighborhoodRadius;

  //BTX
  NeighborhoodType Neighborhood;  // MJS FIXME Why can't this be a pointer?
  TextureFeaturesFilterType::Pointer TextureFeaturesFilter;
  CastingFilterType::Pointer caster;
  //ETX

protected:
  int UpdateInternalFilters() override;

private:
  vtkCoocurrenceTextureFeaturesImageFilter(
    const vtkCoocurrenceTextureFeaturesImageFilter&); // Not implemented.
  void operator=(
    const vtkCoocurrenceTextureFeaturesImageFilter&); // Not implemented.
};

#endif
