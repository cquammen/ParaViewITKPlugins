/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkDeconvolutionImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkDeconvolutionImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkStreamingDemandDrivenPipeline.h"


vtkStandardNewMacro(vtkDeconvolutionImageFilter);

//----------------------------------------------------------------------------
vtkDeconvolutionImageFilter::vtkDeconvolutionImageFilter()
{
  // Set up number of input and output ports
  this->SetNumberOfInputPorts(2);
  this->SetNumberOfOutputPorts(1);

  // Initialize the ITK-VTK glue.
  this->Init();
}

//----------------------------------------------------------------------------
vtkDeconvolutionImageFilter::~vtkDeconvolutionImageFilter()
{
}

//----------------------------------------------------------------------------
void vtkDeconvolutionImageFilter::SetKernelImage(vtkAlgorithmOutput* image)
{
  this->SetInputConnection(1, image);
}

//----------------------------------------------------------------------------
int vtkDeconvolutionImageFilter::RequestUpdateExtent (
  vtkInformation * vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation* inInfo1 = inputVector[0]->GetInformationObject(0);
  vtkInformation* inInfo2 = inputVector[1]->GetInformationObject(0);

  // Update the whole extent of both the input image and kernel image
  int inWExt1[6];
  inInfo1->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), inWExt1);
  inInfo1->Set(vtkStreamingDemandDrivenPipeline::UPDATE_EXTENT(), inWExt1, 6);

  // get the whole image for input 2
  int inWExt2[6];
  inInfo2->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), inWExt2);
  inInfo2->Set(vtkStreamingDemandDrivenPipeline::UPDATE_EXTENT(), inWExt2, 6);

  return 1;
}

//----------------------------------------------------------------------------
int vtkDeconvolutionImageFilter::FillInputPortInformation(int port, vtkInformation *info)
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
void vtkDeconvolutionImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
