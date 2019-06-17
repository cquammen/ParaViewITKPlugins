
#ifndef __vtkBinaryThresholdImageFilter_h
#define __vtkBinaryThresholdImageFilter_h

#include <vtkITKImageFilter.h>

#include <itkBinaryThresholdImageFilter.h>

class VTK_EXPORT vtkBinaryThresholdImageFilter : public vtkITKImageFilter
{
public:

  static vtkBinaryThresholdImageFilter* New();
  vtkTypeMacro(vtkBinaryThresholdImageFilter, vtkITKImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  //BTX
  typedef Superclass::PixelType             PixelType;
  typedef Superclass::ITKImageType          ITKImageType;
  typedef Superclass::ITKInternalFilterType ITKInternalFilterType;
  typedef Superclass::ITKImageImportType    ITKImageImportType;
  typedef Superclass::ITKImageExportType    ITKImageExportType;
  typedef itk::BinaryThresholdImageFilter< ITKImageType, ITKImageType >
    ITKBinaryThresholdImageFilterType;
  //ETX

  vtkSetMacro(outsideValue, int);
  vtkGetMacro(outsideValue, int);
  vtkSetMacro(insideValue, int);
  vtkGetMacro(insideValue, int);
  vtkSetMacro(lowerThreshold, int);
  vtkGetMacro(lowerThreshold, int);
  vtkSetMacro(upperThreshold, int);
  vtkGetMacro(upperThreshold, int);

  void SetInputConnection(int port, vtkAlgorithmOutput* input) override
    {vtkImageAlgorithm::SetInputConnection(port, input);}
  void SetInputConnection(vtkAlgorithmOutput* input) override
    {vtkImageAlgorithm::SetInputConnection(input);}

protected:
  vtkBinaryThresholdImageFilter();
  ~vtkBinaryThresholdImageFilter();

  int outsideValue;
  int insideValue;
  int lowerThreshold;
  int upperThreshold;

  //BTX
  ITKBinaryThresholdImageFilterType::Pointer		 BinaryThresholdImageFilter;
  //ETX

protected:
  int UpdateInternalFilters();

private:
  vtkBinaryThresholdImageFilter(const vtkBinaryThresholdImageFilter&);  // Not implemented.
  void operator=(const vtkBinaryThresholdImageFilter&);  // Not implemented.
};

#endif
