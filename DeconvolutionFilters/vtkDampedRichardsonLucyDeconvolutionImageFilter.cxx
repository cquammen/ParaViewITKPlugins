/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkDampedRichardsonLucyDeconvolutionImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkDampedRichardsonLucyDeconvolutionImageFilter.h"

#include "vtkObjectFactory.h"

#include "itkDampedRichardsonLucyDeconvolutionImageFilter.h"


vtkStandardNewMacro(vtkDampedRichardsonLucyDeconvolutionImageFilter);

//----------------------------------------------------------------------------
vtkDampedRichardsonLucyDeconvolutionImageFilter::vtkDampedRichardsonLucyDeconvolutionImageFilter()
{
  // Set up the internal filter pipeline.
  this->ITKDeconvolutionFilter = ITKDeconvolutionFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ITKInternalFilterType>(this->ITKDeconvolutionFilter);
  this->SetITKPipelineLastFilter<ITKInternalFilterType>(this->ITKDeconvolutionFilter);
}

//----------------------------------------------------------------------------
vtkDampedRichardsonLucyDeconvolutionImageFilter::~vtkDampedRichardsonLucyDeconvolutionImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkDampedRichardsonLucyDeconvolutionImageFilter::UpdateInternalFilters()
{
  this->ITKDeconvolutionFilter->SetPadMethod(this->PaddingMethod);
  this->ITKDeconvolutionFilter->SetNumberOfIterations(this->NumberOfIterations);
  this->ITKDeconvolutionFilter->SetThreshold(this->Threshold);
  this->ITKDeconvolutionFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkDampedRichardsonLucyDeconvolutionImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
