/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkTikhonovMillerDeconvolutionImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkTikhonovMillerDeconvolutionImageFilter.h"

#include "vtkObjectFactory.h"

#include "itkTikhonovMillerDeconvolutionImageFilter.h"


vtkStandardNewMacro(vtkTikhonovMillerDeconvolutionImageFilter);

//----------------------------------------------------------------------------
vtkTikhonovMillerDeconvolutionImageFilter::vtkTikhonovMillerDeconvolutionImageFilter()
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
vtkTikhonovMillerDeconvolutionImageFilter::~vtkTikhonovMillerDeconvolutionImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkTikhonovMillerDeconvolutionImageFilter::UpdateInternalFilters()
{
  this->ITKDeconvolutionFilter->SetPadMethod(this->PaddingMethod);
  this->ITKDeconvolutionFilter->SetGamma(this->Gamma);
  this->ITKDeconvolutionFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkTikhonovMillerDeconvolutionImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
