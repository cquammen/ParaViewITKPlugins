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

// #include "vtkImagingGeneralModule.h" // For export macro
#include <itkVTKImageImport.h>
#include <itkVTKImageExport.h>
#include <itkImageToImageFilter.h>
#include <vtkImageImport.h>

class vtkImageExport;

class VTK_EXPORT vtkITKImageFilter : public vtkImageAlgorithm
{
public:

  //BTX
  typedef float                                                 PixelType;
  typedef itk::Image< PixelType, 3 >                            ITKImageType;
  typedef itk::ImageToImageFilter< ITKImageType, ITKImageType > ITKInternalFilterType;
  typedef itk::VTKImageImport< ITKImageType >                   ITKImageImportType;
  typedef itk::VTKImageExport< ITKImageType >                   ITKImageExportType;
  typedef itk::Image< PixelType, 3 >                            ITKFloatImageType;
  typedef int                                                   IntPixelType;
  typedef itk::Image< IntPixelType, 3 >                         ITKIntImageType;
  typedef itk::Vector< PixelType, 10 >                          ITKFloatVectorPixelType;
  typedef itk::Image< ITKFloatVectorPixelType, 3 >              ITKFloatVectorImageType;
  typedef itk::VTKImageExport< ITKFloatVectorImageType >        ITKVectorImageExportType;
  //ETX

  static vtkITKImageFilter* New();
  vtkTypeMacro(vtkITKImageFilter, vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkITKImageFilter();
  virtual ~vtkITKImageFilter();


  //BTX
  vtkImageCast**               VTKCasters;
  vtkImageExport**             VTKExporters;
  ITKImageImportType::Pointer* ITKImporters;
  ITKImageExportType::Pointer  ITKExporter;
  ITKVectorImageExportType::Pointer  ITKVectorImageExporter;
  vtkImageImport*              VTKImporter;
  //ETX

protected:
  void InitializeITKImporters();
  void InitializeVTKImporters();

  //BTX

  // DESCRIPTION:
  // Set the first filter in the ITK pipeline that receives input from
  // the VTK image passed into this filter. Note that this method
  // assigns the inputs to the ITK filter in the order they are given
  // by the input to the VTK class. If you need a different mapping
  // between the input to VTK and the input of the ITK filter,
  // override this method to set up the mapping.
  template< class T >
  void SetITKPipelineFirstFilter(T* filter)
  {
    int numPorts = this->GetNumberOfInputPorts();
    for (int i = 0; i < numPorts; i++)
      {
      filter->SetInput(i, this->ITKImporters[i]->GetOutput());
      }
  }

  // DESCRIPTION:
  // Set the laster filter in the ITK pipeline whose output should be
  // converted back to VTK.
  template< class T >
  void SetITKPipelineLastFilter(T* filter)
  {
    this->ITKExporter->SetInput(filter->GetOutput());
  }

  template< class T >
  void SetITKPipelineVectorOutputLastFilter(T* filter)
  {
    this->ITKVectorImageExporter->SetInput(filter->GetOutput());
  }

  template<class T>
  void InitializeSpecializedVTKImporters(T* exporter)
  {
    // This call takes the place of the usual SetInput() method.
    this->VTKImporter->SetCallbackUserData(exporter->GetCallbackUserData());

    // Set the rest of the callbacks
    this->VTKImporter->SetUpdateInformationCallback(
      exporter->GetUpdateInformationCallback());
    this->VTKImporter->SetPipelineModifiedCallback(
      exporter->GetPipelineModifiedCallback());
    this->VTKImporter->SetWholeExtentCallback(
      exporter->GetWholeExtentCallback());
    this->VTKImporter->SetSpacingCallback(exporter->GetSpacingCallback());
    this->VTKImporter->SetOriginCallback(exporter->GetOriginCallback());
    this->VTKImporter->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
    this->VTKImporter->SetNumberOfComponentsCallback(
      exporter->GetNumberOfComponentsCallback());
    this->VTKImporter->SetPropagateUpdateExtentCallback(
      exporter->GetPropagateUpdateExtentCallback());
    this->VTKImporter->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
    this->VTKImporter->SetDataExtentCallback(exporter->GetDataExtentCallback());
    this->VTKImporter->SetBufferPointerCallback(
      exporter->GetBufferPointerCallback());
  }

  // DESCRIPTION:
  // Set / Get whether the output is a vector image
  vtkSetMacro(VectorOutput, bool);
  vtkGetMacro(VectorOutput, bool);
  vtkBooleanMacro(VectorOutput, bool);

  //ETX

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

  int RequestData(vtkInformation* request,
                  vtkInformationVector** inputVector,
                  vtkInformationVector* outputVector) override;

  bool VectorOutput = false;

private:
  vtkITKImageFilter(const vtkITKImageFilter&);  // Not implemented.
  void operator=(const vtkITKImageFilter&);  // Not implemented.
};

#endif
