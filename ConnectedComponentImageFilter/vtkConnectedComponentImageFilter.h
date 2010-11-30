
#ifndef __vtkConnectedComponentImageFilter_h
#define __vtkConnectedComponentImageFilter_h

#include <vtkITKImageFilter.h>

#include <itkConnectedComponentImageFilter.h>
#include <itkRelabelComponentImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkCastImageFilter.h>

class VTK_EXPORT vtkConnectedComponentImageFilter : public vtkITKImageFilter
{
public:

  static vtkConnectedComponentImageFilter* New();
  vtkTypeMacro(vtkConnectedComponentImageFilter, vtkITKImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  //BTX
  typedef Superclass::PixelType             PixelType;
  typedef Superclass::ITKImageType          ITKImageType;
  typedef Superclass::ITKInternalFilterType ITKInternalFilterType;
  typedef Superclass::ITKImageImportType    ITKImageImportType;
  typedef Superclass::ITKImageExportType    ITKImageExportType;
//  typedef itk::Image<unsigned short,3> OutputImageType;

  typedef   unsigned short  InternalPixelType;
  typedef itk::Image< InternalPixelType, 3 >  InternalImageType;

  typedef itk::CastImageFilter< InternalImageType, ITKImageType > CastingFilterType;
  typedef itk::BinaryThresholdImageFilter< ITKImageType, ITKImageType > ThresholdFilterType;
  typedef itk::ConnectedComponentImageFilter< ITKImageType, InternalImageType > ConnectedFilterType;
  typedef itk::RelabelComponentImageFilter< InternalImageType, ITKImageType > RelabelType;
  //ETX

  vtkSetMacro(LowerThreshold, double);
  vtkGetMacro(LowerThreshold, double);
  vtkSetMacro(UpperThreshold, double);
  vtkGetMacro(UpperThreshold, double);
  vtkSetMacro(FullyConnected, int);
  vtkGetMacro(FullyConnected, int);
  vtkSetMacro(MinimumObjectSize, int);
  vtkGetMacro(MinimumObjectSize, int);

protected:
  vtkConnectedComponentImageFilter();
  ~vtkConnectedComponentImageFilter();

  double LowerThreshold;
  double UpperThreshold;
  int FullyConnected;
  int MinimumObjectSize;

  //BTX
  ThresholdFilterType::Pointer ThresholdFilter;
  ConnectedFilterType::Pointer ConnectedFilter;
  CastingFilterType::Pointer caster;
  RelabelType::Pointer RelabelFilter;
  //ETX

protected:
  int UpdateInternalFilters();

private:
  vtkConnectedComponentImageFilter(const vtkConnectedComponentImageFilter&);  // Not implemented.
  void operator=(const vtkConnectedComponentImageFilter&);  // Not implemented.
};

#endif
