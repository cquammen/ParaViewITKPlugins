
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
  typedef Superclass::FloatPixelType                PixelType;
  typedef Superclass::ITKFloatImageType          ITKFloatImageType;
  typedef Superclass::ITKFloatImageType          InputImageType;
  // typedef Superclass::ITKVectorPixelType       VectorPixelType;
  typedef Superclass::ITKIntImageType             ITKIntImageType;
  typedef Superclass::ITKFloatVectorImageType       OutputImageType;
  // typedef Superclass::ITKInternalFilterType ITKInternalFilterType;
  // typedef Superclass::ITKImageImportType    ITKImageImportType;
  // typedef Superclass::ITKImageExportType    ITKImageExportType;
  typedef itk::CastImageFilter< ITKFloatImageType, ITKIntImageType > CastingFilterType;

  // typedef itk::Image< float , 3 >  InputImageType;
  // typedef itk::Image< itk::Vector< float, 10 > , 3 >  OutputImageType;

  typedef itk::Neighborhood<typename InputImageType::PixelType, InputImageType::ImageDimension >
    NeighborhoodType;
    // using NeighborhoodType = itk::Neighborhood<typename InputImageType::PixelType,
    //   InputImageType::ImageDimension>;

  // typedef itk::CastImageFilter< InternalImageType, ITKImageType > CastingFilterType;
  // typedef itk::BinaryThresholdImageFilter< ITKImageType, ITKImageType > ThresholdFilterType;
  typedef itk::Statistics::CoocurrenceTextureFeaturesImageFilter< ITKIntImageType, OutputImageType >
    TextureFeaturesFilterType;
  // typedef itk::RelabelComponentImageFilter< InternalImageType, ITKImageType > RelabelType;
  //ETX

  // void SetLowerThreshold(double val) { LowerThreshold = val; this->Modified(); }
  // double GetLowerThreshold() { return LowerThreshold; }
  // void SetUpperThreshold(double val) { UpperThreshold = val; this->Modified(); }
  // double GetUpperThreshold() { return UpperThreshold; }
  // void SetFullyConnected(int ival) { FullyConnected = ival; this->Modified();}
  // int GetFullyConnected() { return FullyConnected; }
  // void SetMinimumObjectSize(int ival) { MinimumObjectSize = ival; this->Modified();}
  // int GetMinimumObjectSize() { return MinimumObjectSize; }
  void SetNumberOfBinsPerAxis(int val) { numberOfBinsPerAxis = val; this->Modified(); }
  int GetNumberOfBinsPerAxis() { return numberOfBinsPerAxis; }
  void SetHistogramMinimum(double val) { histMinimum = val; this->Modified(); }
  double GetHistogramMinimum() { return histMinimum; }
  void SetHistogramMaximum(double val) { histMaximum = val; this->Modified(); }
  double GetHistogramMaximum() { return histMaximum; }
  void SetNeighborhoodRadius(int val) { neighborhoodRadius = val; this->Modified(); }
  int GetNeighborhoodRadius() { return neighborhoodRadius; }

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
  vtkCoocurrenceTextureFeaturesImageFilter(const vtkCoocurrenceTextureFeaturesImageFilter&);  // Not implemented.
  void operator=(const vtkCoocurrenceTextureFeaturesImageFilter&);  // Not implemented.
};

#endif
