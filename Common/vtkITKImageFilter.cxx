/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkITKImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef __vtkITKImageFilter_cxx
#define __vtkITKImageFilter_cxx

#include "vtkITKImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

vtkStandardNewMacro(vtkITKImageFilter);

//----------------------------------------------------------------------------
vtkITKImageFilter::vtkITKImageFilter()
{
  this->VTKCaster   = vtkImageCast::New();
  this->VTKCaster->SetOutputScalarTypeToFloat();

  this->VTKExporter = vtkImageExport::New();
  this->ITKImporter = ITKImageImportType::New();
  this->ITKExporter = ITKImageExportType::New();
  this->VTKImporter = vtkImageImport::New();

  this->InitializeITKImporters();
  this->InitializeVTKImporters();
}

//----------------------------------------------------------------------------
vtkITKImageFilter::~vtkITKImageFilter()
{
  if (this->VTKCaster)
    {
    this->VTKCaster->Delete();
    }

  if (this->VTKExporter)
    {
    this->VTKExporter->Delete();
    }

  if (this->VTKImporter)
    {
    this->VTKImporter->Delete();
    }
}

//----------------------------------------------------------------------------
void vtkITKImageFilter::InitializeITKImporters()
{
  // Connect the vtkImageCast that converts the input to the
  // floating-point type we want to pass to ITK filters.
  this->VTKExporter->SetInputConnection(this->VTKCaster->GetOutputPort());

  // This call takes the place of the usual SetInput() method.
  this->ITKImporter->SetCallbackUserData(this->VTKExporter);

  // Set the rest of the callbacks
  this->ITKImporter->SetBufferPointerCallback(this->VTKExporter->GetBufferPointerCallback());
  this->ITKImporter->SetDataExtentCallback(this->VTKExporter->GetDataExtentCallback());
  this->ITKImporter->SetOriginCallback(this->VTKExporter->GetOriginCallback());
  this->ITKImporter->SetSpacingCallback(this->VTKExporter->GetSpacingCallback());
  this->ITKImporter->SetNumberOfComponentsCallback(this->VTKExporter->GetNumberOfComponentsCallback());
  this->ITKImporter->SetPipelineModifiedCallback(this->VTKExporter->GetPipelineModifiedCallback());
  this->ITKImporter->SetPropagateUpdateExtentCallback(this->VTKExporter->GetPropagateUpdateExtentCallback());
  this->ITKImporter->SetScalarTypeCallback(this->VTKExporter->GetScalarTypeCallback());
  this->ITKImporter->SetUpdateDataCallback(this->VTKExporter->GetUpdateDataCallback());
  this->ITKImporter->SetUpdateInformationCallback(this->VTKExporter->GetUpdateInformationCallback());
  this->ITKImporter->SetWholeExtentCallback(this->VTKExporter->GetWholeExtentCallback());
}

//----------------------------------------------------------------------------
void vtkITKImageFilter::InitializeVTKImporters()
{
  // This call takes the place of the usual SetInput() method.
  this->VTKImporter->SetCallbackUserData(ITKExporter->GetCallbackUserData());

  // Set the rest of the callbacks
  this->VTKImporter->SetUpdateInformationCallback(ITKExporter->GetUpdateInformationCallback());
  this->VTKImporter->SetPipelineModifiedCallback(ITKExporter->GetPipelineModifiedCallback());
  this->VTKImporter->SetWholeExtentCallback(ITKExporter->GetWholeExtentCallback());
  this->VTKImporter->SetSpacingCallback(ITKExporter->GetSpacingCallback());
  this->VTKImporter->SetOriginCallback(ITKExporter->GetOriginCallback());
  this->VTKImporter->SetScalarTypeCallback(ITKExporter->GetScalarTypeCallback());
  this->VTKImporter->SetNumberOfComponentsCallback(ITKExporter->GetNumberOfComponentsCallback());
  this->VTKImporter->SetPropagateUpdateExtentCallback(ITKExporter->GetPropagateUpdateExtentCallback());
  this->VTKImporter->SetUpdateDataCallback(ITKExporter->GetUpdateDataCallback());
  this->VTKImporter->SetDataExtentCallback(ITKExporter->GetDataExtentCallback());
  this->VTKImporter->SetBufferPointerCallback(ITKExporter->GetBufferPointerCallback());
}

//----------------------------------------------------------------------------
void vtkITKImageFilter::Init()
{
  this->InitializeITKImporters();
  this->InitializeVTKImporters();
}

//----------------------------------------------------------------------------
int vtkITKImageFilter::BeforeUpdateInternalFilters(vtkInformationVector** inputVector)
{
  // Check that the inputs are of type vtkImageData
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkImageData *input = vtkImageData::SafeDownCast
    (inInfo->Get(vtkDataObject::DATA_OBJECT()));
  if(!input)
    {
    vtkErrorMacro("An input is not of type vtkImageData");
    return 0;
    }

  this->VTKCaster->SetInput(input);
  this->VTKCaster->Update();

  // Hook up to the beginning of the ITK pipeline
  this->VTKExporter->SetInput(this->VTKCaster->GetOutput());

  return 1;
}

//----------------------------------------------------------------------------
int vtkITKImageFilter::AfterUpdateInternalFilters(vtkInformationVector* outputVector)
{
  // Now connect the ITK pipeline output to the VTK output
  this->ITKExporter->Update();

  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkImageData *output = vtkImageData::SafeDownCast
    (outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!output)
    {
    vtkErrorMacro("Output is not of type vtkImageData");
    return 0;
    }

  this->VTKImporter->Update();
  this->VTKImporter->Update();

  output->ShallowCopy(this->VTKImporter->GetOutput());

  return 1;
}

//----------------------------------------------------------------------------
int vtkITKImageFilter::RequestData(vtkInformation *request,
                                   vtkInformationVector **inputVector,
                                   vtkInformationVector *outputVector)
{
  try
    {
    if (!this->BeforeUpdateInternalFilters(inputVector))
      {
      return 0;
      }

    this->UpdateInternalFilters();

    if (!this->AfterUpdateInternalFilters(outputVector))
      {
      return 0;
      }
    }
  catch(itk::ExceptionObject& error)
    {
    return 0;
    }

  return 1;
}

//----------------------------------------------------------------------------
void vtkITKImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

#endif // __vtkITKImageFilter_cxx
