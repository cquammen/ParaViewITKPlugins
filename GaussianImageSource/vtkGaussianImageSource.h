/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkGaussianImageSource.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkGaussianImageSource - peforms Gaussian blurring of an input image
// .SECTION Description
// vtkMyImageShiftFilter is a filter to generate scalar values from a
// dataset.  The scalar values lie within a user specified range, and
// are generated by computing a projection of each dataset point onto
// a line. The line can be oriented arbitrarily. A typical example is
// to generate scalars based on elevation or height above a plane.

#ifndef __vtkGaussianImageSource_h
#define __vtkGaussianImageSource_h

#include <vtkITKImageFilter.h>

#include <itkGaussianImageSource.h>

class VTK_EXPORT vtkGaussianImageSource : public vtkITKImageFilter
{
public:

  static vtkGaussianImageSource* New();
  vtkTypeMacro(vtkGaussianImageSource, vtkITKImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  //BTX
  typedef Superclass::PixelType             PixelType;
  typedef Superclass::ITKImageType          ITKImageType;
  typedef Superclass::ITKInternalFilterType ITKInternalFilterType;
  typedef Superclass::ITKImageImportType    ITKImageImportType;
  typedef Superclass::ITKImageExportType    ITKImageExportType;
  typedef itk::GaussianImageSource< ITKImageType >
    ITKGaussianSourceType;
  //ETX

  // Set/get image size.
  vtkSetVector3Macro(Size, int);
  vtkGetVector3Macro(Size, int);

  // Set/get origin.
  vtkSetVector3Macro(Origin, double);
  vtkGetVector3Macro(Origin, double);

  // Set/get mean (center of Gaussian)
  vtkSetVector3Macro(Mean, double);
  vtkGetVector3Macro(Mean, double);

  // Set/get standard deviation of the Gaussian.
  vtkSetVector3Macro(StandardDeviation, double);
  vtkGetVector3Macro(StandardDeviation, double);

protected:
  vtkGaussianImageSource();
  ~vtkGaussianImageSource();

  int    Size[3];
  double Origin[3];
  double Mean[3];
  double StandardDeviation[3];

  //BTX
  ITKGaussianSourceType::Pointer GaussianSource;
  //ETX

protected:
  virtual int RequestInformation (vtkInformation *, vtkInformationVector**, vtkInformationVector *);
  //virtual int RequestData(vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector);

  int UpdateInternalFilters();

private:
  vtkGaussianImageSource(const vtkGaussianImageSource&);  // Not implemented.
  void operator=(const vtkGaussianImageSource&);  // Not implemented.
};

#endif
