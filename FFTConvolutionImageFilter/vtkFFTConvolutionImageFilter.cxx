/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkFFTConvolutionImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkFFTConvolutionImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkImageShiftScale.h"

#include "itkFFTConvolutionImageFilter.h"

// Warning: this is a hack!
#include "itkFFTWLock.cxx"


vtkStandardNewMacro(vtkFFTConvolutionImageFilter);

//----------------------------------------------------------------------------
vtkFFTConvolutionImageFilter::vtkFFTConvolutionImageFilter()
{
  this->SetNumberOfInputPorts(2);
  this->SetNumberOfOutputPorts(1);

  this->VTKExporter          = vtkImageExport::New();
  this->VTKKernelExporter    = vtkImageExport::New();
  this->ITKImporter          = ITKImageImportType::New();
  this->ITKKernelImporter    = ITKImageImportType::New();
  this->ITKConvolutionFilter = ITKFFTConvolutionFilterType::New();
  this->ITKExporter          = ITKImageExportType::New();
  this->VTKImporter          = vtkImageImport::New();

  this->InitializeITKImporters();
  this->InitializeITKExporter();
}

//----------------------------------------------------------------------------
vtkFFTConvolutionImageFilter::~vtkFFTConvolutionImageFilter()
{
  if (this->VTKExporter)
    {
    this->VTKExporter->Delete();
    }

  if (this->VTKKernelExporter)
    {
    this->VTKKernelExporter->Delete();
    }

  if (this->VTKImporter)
    {
    this->VTKImporter->Delete();
    }
}

//----------------------------------------------------------------------------
void vtkFFTConvolutionImageFilter::SetKernelImage(vtkAlgorithmOutput* image)
{
  this->SetInputConnection(1, image);
}

//----------------------------------------------------------------------------
void vtkFFTConvolutionImageFilter::InitializeITKImporters()
{
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

  // This call takes the place of the usual SetInput() method.
  this->ITKKernelImporter->SetCallbackUserData(this->VTKKernelExporter);

  // Set the rest of the callbacks
  this->ITKKernelImporter->SetBufferPointerCallback(this->VTKKernelExporter->GetBufferPointerCallback());
  this->ITKKernelImporter->SetDataExtentCallback(this->VTKKernelExporter->GetDataExtentCallback());
  this->ITKKernelImporter->SetOriginCallback(this->VTKKernelExporter->GetOriginCallback());
  this->ITKKernelImporter->SetSpacingCallback(this->VTKKernelExporter->GetSpacingCallback());
  this->ITKKernelImporter->SetNumberOfComponentsCallback(this->VTKKernelExporter->GetNumberOfComponentsCallback());
  this->ITKKernelImporter->SetPipelineModifiedCallback(this->VTKKernelExporter->GetPipelineModifiedCallback());
  this->ITKKernelImporter->SetPropagateUpdateExtentCallback(this->VTKKernelExporter->GetPropagateUpdateExtentCallback());
  this->ITKKernelImporter->SetScalarTypeCallback(this->VTKKernelExporter->GetScalarTypeCallback());
  this->ITKKernelImporter->SetUpdateDataCallback(this->VTKKernelExporter->GetUpdateDataCallback());
  this->ITKKernelImporter->SetUpdateInformationCallback(this->VTKKernelExporter->GetUpdateInformationCallback());
  this->ITKKernelImporter->SetWholeExtentCallback(this->VTKKernelExporter->GetWholeExtentCallback());
}

//----------------------------------------------------------------------------
void vtkFFTConvolutionImageFilter::InitializeITKExporter()
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
int vtkFFTConvolutionImageFilter::RequestData(vtkInformation *request,
					    vtkInformationVector **inputVector,
					    vtkInformationVector *outputVector)
{
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkImageData *input = vtkImageData::SafeDownCast
    (inInfo->Get(vtkDataObject::DATA_OBJECT()));
  if(!input)
    {
    vtkErrorMacro("Input is not of type vtkImageData");
    return 0;
    }
  vtkInformation *kernelInfo = inputVector[1]->GetInformationObject(0);
  vtkImageData *kernelInput = vtkImageData::SafeDownCast
    (kernelInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!kernelInfo)
    {
    vtkErrorMacro("Kernel is not of type vtkImageData");
    return 0;
    }

  vtkImageShiftScale* castInput = vtkImageShiftScale::New();
  castInput->SetOutputScalarTypeToFloat();
  castInput->SetInput(input);
  castInput->Update();

  vtkImageShiftScale* castKernel = vtkImageShiftScale::New();
  castKernel->SetOutputScalarTypeToFloat();
  castKernel->SetInput(kernelInput);
  castKernel->Update();

  // Hook up to the beginning of the ITK pipeline
  this->VTKExporter->SetInput(castInput->GetOutput());
  this->VTKKernelExporter->SetInput(castKernel->GetOutput());

  // Now connect the ITK pipeline output to the VTK output
  this->ITKConvolutionFilter->SetInput(this->ITKImporter->GetOutput());
  this->ITKConvolutionFilter->SetKernelImage(this->ITKKernelImporter->GetOutput());

  this->ITKConvolutionFilter->Update();

  this->ITKExporter->SetInput(this->ITKConvolutionFilter->GetOutput());

  this->ITKExporter->Update();

  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkImageData *output = vtkImageData::SafeDownCast
    (outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!output)
    {
      vtkErrorMacro("Output is not of type vtkImageData");
      return 0;
    }

  // For some reason, updates don't seem to go through unless you call
  // the following twice.
  this->VTKImporter->Update();
  this->VTKImporter->Update();

  output->DeepCopy(this->VTKImporter->GetOutput());

  return 1;
}

//----------------------------------------------------------------------------
int vtkFFTConvolutionImageFilter::FillInputPortInformation(int port, vtkInformation *info)
{
  info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkImageData");
  info->Set(vtkAlgorithm::INPUT_IS_REPEATABLE(), 0);
  info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 0);

  if (port == 0 || port == 1)
    {
    return 1;
    }

  return 0;
}

//----------------------------------------------------------------------------
void vtkFFTConvolutionImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
