/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkNoiseImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkNoiseImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkImageShiftScale.h"

#include "vtkPointData.h"

// Warning: this is really a hack to avoid creating a library just for
// the following class.
#include "itkThreadSafeMersenneTwisterRandomVariateGenerator.cxx"


vtkStandardNewMacro(vtkNoiseImageFilter);

//----------------------------------------------------------------------------
vtkNoiseImageFilter::vtkNoiseImageFilter()
{
  this->NoiseType = GAUSSIAN_NOISE;
  this->VTKExporter = vtkImageExport::New();
  this->ITKImporter = ITKImageImportType::New();
  this->GaussianNoiseFilter = ITKGaussianNoiseFilterType::New();
  this->PoissonNoiseFilter  = ITKPoissonNoiseFilterType::New();
  this->ITKExporter = ITKImageExportType::New();
  this->VTKImporter = vtkImageImport::New();
  this->VTKImporter->SetScalarArrayName("NoisyScalars");

  this->InitializeITKImporter();
  this->InitializeITKExporter();
}

//----------------------------------------------------------------------------
vtkNoiseImageFilter::~vtkNoiseImageFilter()
{
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
void vtkNoiseImageFilter::InitializeITKImporter()
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
}

//----------------------------------------------------------------------------
void vtkNoiseImageFilter::InitializeITKExporter()
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
int vtkNoiseImageFilter::RequestData(vtkInformation *request,
					    vtkInformationVector **inputVector,
					    vtkInformationVector *outputVector)
{
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkImageData *input = vtkImageData::SafeDownCast
    (inInfo->Get(vtkDataObject::DATA_OBJECT()));
  if(!input)
    {
      vtkErrorMacro("Output is not of type vtkImageData");
      return 0;
    }

  vtkImageShiftScale* castSource = vtkImageShiftScale::New();
  castSource->SetOutputScalarTypeToFloat();
  castSource->SetInput(input);
  castSource->Update();

  // Hook up to the beginning of the ITK pipeline
  this->VTKExporter->SetInput(castSource->GetOutput());

  this->GaussianNoiseFilter->SetInput(NULL);
  this->PoissonNoiseFilter->SetInput(NULL);

  // Now connect the ITK pipeline output to the VTK output
  if (NoiseType == GAUSSIAN_NOISE)
    {
    this->GaussianNoiseFilter->SetInput(this->ITKImporter->GetOutput());
    this->GaussianNoiseFilter->SetMean(this->Mean);
    this->GaussianNoiseFilter->SetStandardDeviation(this->StandardDeviation);

    this->GaussianNoiseFilter->Update();

    this->ITKExporter->SetInput(this->GaussianNoiseFilter->GetOutput());
    }
  else if (NoiseType == POISSON_NOISE)
    {
    this->PoissonNoiseFilter->SetInput(this->ITKImporter->GetOutput());
    this->PoissonNoiseFilter->Update();

    this->ITKExporter->SetInput(this->PoissonNoiseFilter->GetOutput());
    }

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

  output->ShallowCopy(this->VTKImporter->GetOutput());

  return 1;
}

//----------------------------------------------------------------------------
void vtkNoiseImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
