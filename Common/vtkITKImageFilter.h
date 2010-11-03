/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkITKImageFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkITKImageFilter - base class for VTK classes that wrap ITK filters
//
// .SECTION Description

#ifndef __vtkITKImageFilter_h
#define __vtkITKImageFilter_h

#include <vtkImageCast.h>

#include <itkImageToImageFilter.h>
#include <itkVTKImageImport.h>
#include <itkVTKImageExport.h>

class vtkImageExport;
class vtkImageImport;

class VTK_EXPORT vtkITKImageFilter : public vtkImageAlgorithm
{
public:

  //BTX
  typedef float                                                 PixelType;
  typedef itk::Image< PixelType, 3 >                            ITKImageType;
  typedef itk::ImageToImageFilter< ITKImageType, ITKImageType > ITKInternalFilterType;
  typedef itk::VTKImageImport< ITKImageType >                   ITKImageImportType;
  typedef itk::VTKImageExport< ITKImageType >                   ITKImageExportType;
  //ETX

  static vtkITKImageFilter* New();
  vtkTypeMacro(vtkITKImageFilter, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

protected:
  vtkITKImageFilter();
  virtual ~vtkITKImageFilter();


  //BTX
  vtkImageCast*               VTKCaster;
  vtkImageExport*             VTKExporter;
  ITKImageImportType::Pointer ITKImporter;
  ITKImageExportType::Pointer ITKExporter;
  vtkImageImport*             VTKImporter;
  //ETX

protected:
  void InitializeITKImporters();
  void InitializeVTKImporters();

  // DESCRIPTION:
  // Set the first filter in the ITK pipeline that receives input from
  // the VTK image passed into this filter.
  template< class T >
  void SetITKPipelineFirstFilter(T* filter)
  {
    filter->SetInput(this->ITKImporter->GetOutput());
  }

  // DESCRIPTION:
  // Set the laster filter in the ITK pipeline whose output should be
  // converted back to VTK.
  template< class T >
  void SetITKPipelineLastFilter(T* filter)
  {
    this->ITKExporter->SetInput(filter->GetOutput());
  }

  // DESCRIPTION:
  // Subclasses must call this method *after* specifying the number of
  // input and output ports of the VTK filter.
  void Init();

  // DESCRIPTION:
  // This is called prior to the UpdateInternalFilters method.
  virtual int BeforeUpdateInternalFilters(vtkInformationVector** inputVector);

  // DESCRIPTION:
  // This is called after the UpdateInternalFilters method.
  virtual int AfterUpdateInternalFilters(vtkInformationVector* outputVector);

  // DESCRIPTION:
  // This is where all internal filter parameters should be updated.
  virtual int UpdateInternalFilters() {return 1;};

  int RequestData(vtkInformation *request, vtkInformationVector **inputVector, vtkInformationVector *outputVector);

private:
  vtkITKImageFilter(const vtkITKImageFilter&);  // Not implemented.
  void operator=(const vtkITKImageFilter&);  // Not implemented.
};

#endif
