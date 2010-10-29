/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkGaussianImageSource.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkGaussianImageSource.h"


#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkPointData.h"
#include "vtkStreamingDemandDrivenPipeline.h"

#include "vtkImageShiftScale.h"

vtkStandardNewMacro(vtkGaussianImageSource);

//----------------------------------------------------------------------------
vtkGaussianImageSource::vtkGaussianImageSource()
{
  this->SetNumberOfInputPorts(0);
  this->SetNumberOfOutputPorts(1);

  for (unsigned int i = 0; i < 3; i++)
    {
    this->Size[i] = 5;
    this->Origin[i] = -(0.5 * (this->Size[i]-1));
    this->Mean[i] = 0.0;
    this->StandardDeviation[i] = 1.0;
    }

  this->GaussianSource = ITKGaussianSourceType::New();
  this->ITKExporter = ITKImageExportType::New();
  this->VTKImporter = vtkImageImport::New();
  this->VTKImporter->SetScalarArrayName("GaussianFunction");

  this->InitializeITKExporter();
}

//----------------------------------------------------------------------------
vtkGaussianImageSource::~vtkGaussianImageSource()
{
  if (this->VTKImporter)
    {
      this->VTKImporter->Delete();
    }
}

//----------------------------------------------------------------------------
void vtkGaussianImageSource::InitializeITKExporter()
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
int vtkGaussianImageSource::RequestInformation (
  vtkInformation * vtkNotUsed(request),
  vtkInformationVector** vtkNotUsed( inputVector ),
  vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  int wholeExtent[6];
  wholeExtent[0] = wholeExtent[2] = wholeExtent[4] = 0;
  wholeExtent[1] = this->Size[0] - 1;
  wholeExtent[3] = this->Size[1] - 1;
  wholeExtent[5] = this->Size[2] - 1;

  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
               wholeExtent,6);
  outInfo->Set(vtkDataObject::SPACING(), 1.0, 1.0, 1.0);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, 1);
  return 1;
}

//----------------------------------------------------------------------------
int vtkGaussianImageSource::RequestData(vtkInformation *request,
                                        vtkInformationVector **inputVector,
                                        vtkInformationVector *outputVector)
{
  try
    {

    // Set up ITK pipeline settings here
    ITKGaussianSourceType::SizeType size;
    ITKGaussianSourceType::ArrayType sigma;
    for (unsigned int i = 0; i < 3; i++)
      {
      size[i]  = static_cast<ITKGaussianSourceType::SizeValueType>
        (this->Size[i]);
      sigma[i] = static_cast<ITKGaussianSourceType::ArrayType::ValueType>
        (this->StandardDeviation[i]);
      }
    this->GaussianSource->SetSize(size);
    this->GaussianSource->SetOrigin(this->Origin);
    this->GaussianSource->SetMean(this->Mean);
    this->GaussianSource->SetSigma(sigma);
    this->GaussianSource->Update();

    // Now connect the ITK pipeline output to the VTK output
    this->ITKExporter->SetInput(this->GaussianSource->GetOutput());
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
    }
  catch(itk::ExceptionObject& error)
    {
    vtkErrorMacro(<< "Exception caught when running ITK filter." );
    }

  return 1;
}

//----------------------------------------------------------------------------
void vtkGaussianImageSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
