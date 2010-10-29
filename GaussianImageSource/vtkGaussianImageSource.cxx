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

#include "vtkImageShiftScale.h"

vtkStandardNewMacro(vtkGaussianImageSource);

//----------------------------------------------------------------------------
vtkGaussianImageSource::vtkGaussianImageSource()
{
  this->SetNumberOfInputPorts(0);
  this->GaussianSource = ITKGaussianSourceType::New();
  this->ITKExporter = ITKImageExportType::New();
  this->VTKImporter = vtkImageImport::New();

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

    output->DeepCopy(this->VTKImporter->GetOutput());
    }
  catch(itk::ExceptionObject& error)
    {
    }

  return 1;
}

//----------------------------------------------------------------------------
void vtkGaussianImageSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
