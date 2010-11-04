/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRichardsonLucyDeconvolutionImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkRichardsonLucyDeconvolutionImageFilter.h"

#include "vtkObjectFactory.h"

#include "itkRichardsonLucyDeconvolutionImageFilter.h"


vtkStandardNewMacro(vtkRichardsonLucyDeconvolutionImageFilter);

//----------------------------------------------------------------------------
vtkRichardsonLucyDeconvolutionImageFilter::vtkRichardsonLucyDeconvolutionImageFilter()
{
  // Set up the internal filter pipeline.
  this->ITKDeconvolutionFilter = ITKDeconvolutionFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ITKInternalFilterType>(this->ITKDeconvolutionFilter);
  this->SetITKPipelineLastFilter<ITKInternalFilterType>(this->ITKDeconvolutionFilter);
}

//----------------------------------------------------------------------------
vtkRichardsonLucyDeconvolutionImageFilter::~vtkRichardsonLucyDeconvolutionImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkRichardsonLucyDeconvolutionImageFilter::UpdateInternalFilters()
{
  this->ITKDeconvolutionFilter->SetPadMethod(this->PaddingMethod);
  this->ITKDeconvolutionFilter->SetNumberOfIterations(this->NumberOfIterations);
  this->ITKDeconvolutionFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkRichardsonLucyDeconvolutionImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
