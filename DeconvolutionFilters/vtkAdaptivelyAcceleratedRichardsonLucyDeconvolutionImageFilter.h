/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter - peforms Gaussian blurring of an input image
// .SECTION Description
// vtkMyImageShiftFilter is a filter to generate scalar values from a
// dataset.  The scalar values lie within a user specified range, and
// are generated by computing a projection of each dataset point onto
// a line. The line can be oriented arbitrarily. A typical example is
// to generate scalars based on elevation or height above a plane.

#ifndef __vtkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter_h
#define __vtkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter_h

#include <vtkIterativeDeconvolutionImageFilter.h>

#include <itkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter.h>

class VTK_EXPORT vtkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter : public vtkIterativeDeconvolutionImageFilter
{
public:

  static vtkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter* New();
  vtkTypeMacro(vtkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter, vtkIterativeDeconvolutionImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  //BTX
  typedef Superclass::PixelType             PixelType;
  typedef Superclass::ITKImageType          ITKImageType;
  typedef Superclass::ITKInternalFilterType ITKInternalFilterType;
  typedef Superclass::ITKImageImportType    ITKImageImportType;
  typedef Superclass::ITKImageExportType    ITKImageExportType;
  typedef itk::AdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter< ITKImageType, ITKImageType, ITKImageType >
    ITKDeconvolutionFilterType;
  //ETX

protected:
  vtkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter();
  ~vtkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter();

  //BTX
  ITKDeconvolutionFilterType::Pointer ITKDeconvolutionFilter;
  //ETX

protected:
  int UpdateInternalFilters();

private:
  vtkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter(const vtkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter&);  // Not implemented.
  void operator=(const vtkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter&);  // Not implemented.
};

#endif