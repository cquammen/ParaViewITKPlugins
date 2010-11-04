/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkJanssonVanCittertDeconvolutionImageFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkJanssonVanCittertDeconvolutionImageFilter - peforms Gaussian blurring of an input image
// .SECTION Description
// vtkMyImageShiftFilter is a filter to generate scalar values from a
// dataset.  The scalar values lie within a user specified range, and
// are generated by computing a projection of each dataset point onto
// a line. The line can be oriented arbitrarily. A typical example is
// to generate scalars based on elevation or height above a plane.

#ifndef __vtkJanssonVanCittertDeconvolutionImageFilter_h
#define __vtkJanssonVanCittertDeconvolutionImageFilter_h

#include <vtkIterativeDeconvolutionImageFilter.h>

#include <itkJanssonVanCittertDeconvolutionImageFilter.h>

class VTK_EXPORT vtkJanssonVanCittertDeconvolutionImageFilter : public vtkIterativeDeconvolutionImageFilter
{
public:

  static vtkJanssonVanCittertDeconvolutionImageFilter* New();
  vtkTypeMacro(vtkJanssonVanCittertDeconvolutionImageFilter, vtkIterativeDeconvolutionImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  //BTX
  typedef Superclass::PixelType             PixelType;
  typedef Superclass::ITKImageType          ITKImageType;
  typedef Superclass::ITKInternalFilterType ITKInternalFilterType;
  typedef Superclass::ITKImageImportType    ITKImageImportType;
  typedef Superclass::ITKImageExportType    ITKImageExportType;
  typedef itk::JanssonVanCittertDeconvolutionImageFilter< ITKImageType, ITKImageType, ITKImageType >
    ITKDeconvolutionFilterType;
  //ETX

protected:
  vtkJanssonVanCittertDeconvolutionImageFilter();
  ~vtkJanssonVanCittertDeconvolutionImageFilter();

  //BTX
  ITKDeconvolutionFilterType::Pointer ITKDeconvolutionFilter;
  //ETX

protected:
  int UpdateInternalFilters();

private:
  vtkJanssonVanCittertDeconvolutionImageFilter(const vtkJanssonVanCittertDeconvolutionImageFilter&);  // Not implemented.
  void operator=(const vtkJanssonVanCittertDeconvolutionImageFilter&);  // Not implemented.
};

#endif