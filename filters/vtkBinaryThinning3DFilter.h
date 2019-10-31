
#ifndef __vtkBinaryThinning3DFilter_h
#define __vtkBinaryThinning3DFilter_h

#include <vtkITKImageFilter.h>

#include <itkConnectedThresholdImageFilter.h>
#include <itkImageRegionIterator.h>
#include "itkBinaryThinningImageFilter3D.h"

class VTK_EXPORT vtkBinaryThinning3DFilter : public vtkITKImageFilter
{
public:

  static vtkBinaryThinning3DFilter* New();
  vtkTypeMacro(vtkBinaryThinning3DFilter, vtkITKImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  //BTX
  typedef Superclass::PixelType             PixelType;
  typedef Superclass::ITKImageType          ITKImageType;
  typedef Superclass::ITKInternalFilterType ITKInternalFilterType;
  typedef Superclass::ITKImageImportType    ITKImageImportType;
  typedef Superclass::ITKImageExportType    ITKImageExportType;
  typedef itk::BinaryThinningImageFilter3D< ITKImageType, ITKImageType >
    ITKBinaryThinning3DFilterType;
  //ETX

//  vtkSetMacro(Sigma, double);
//  vtkGetMacro(Sigma, double);

  void SetInputConnection(int port, vtkAlgorithmOutput* input) override
    {vtkImageAlgorithm::SetInputConnection(port, input);}
  void SetInputConnection(vtkAlgorithmOutput* input) override
    {vtkImageAlgorithm::SetInputConnection(input);}

protected:
  vtkBinaryThinning3DFilter();
  ~vtkBinaryThinning3DFilter();

//  double Sigma;

  //BTX
  ITKBinaryThinning3DFilterType::Pointer		 BinaryThinning3DFilter;
  //ETX

protected:
  int UpdateInternalFilters();

private:
  vtkBinaryThinning3DFilter(const vtkBinaryThinning3DFilter&);  // Not implemented.
  void operator=(const vtkBinaryThinning3DFilter&);  // Not implemented.
};

#endif
