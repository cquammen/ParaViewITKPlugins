/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkWienerDeconvolutionImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkWienerDeconvolutionImageFilter.h"

#include "vtkObjectFactory.h"

#include "itkWienerDeconvolutionImageFilter.h"


vtkStandardNewMacro(vtkWienerDeconvolutionImageFilter);

//----------------------------------------------------------------------------
vtkWienerDeconvolutionImageFilter::vtkWienerDeconvolutionImageFilter()
{
  // Set up number of input and output ports
  this->SetNumberOfInputPorts(2);
  this->SetNumberOfOutputPorts(1);

  // Initialize the ITK-VTK glue.
  this->Init();

  // Set up the internal filter pipeline.
  this->ITKDeconvolutionFilter = ITKDeconvolutionFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ITKInternalFilterType>(this->ITKDeconvolutionFilter);
  this->SetITKPipelineLastFilter<ITKInternalFilterType>(this->ITKDeconvolutionFilter);
}

//----------------------------------------------------------------------------
vtkWienerDeconvolutionImageFilter::~vtkWienerDeconvolutionImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkWienerDeconvolutionImageFilter::UpdateInternalFilters()
{
  this->ITKDeconvolutionFilter->SetPadMethod(this->PaddingMethod);
  this->ITKDeconvolutionFilter->SetGamma(this->Gamma);
  this->ITKDeconvolutionFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkWienerDeconvolutionImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
