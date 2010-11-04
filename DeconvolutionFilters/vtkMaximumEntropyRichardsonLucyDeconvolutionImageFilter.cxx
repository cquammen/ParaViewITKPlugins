/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMaximumEntropyRichardsonLucyDeconvolutionImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkMaximumEntropyRichardsonLucyDeconvolutionImageFilter.h"

#include "vtkObjectFactory.h"

#include "itkMaximumEntropyRichardsonLucyDeconvolutionImageFilter.h"


vtkStandardNewMacro(vtkMaximumEntropyRichardsonLucyDeconvolutionImageFilter);

//----------------------------------------------------------------------------
vtkMaximumEntropyRichardsonLucyDeconvolutionImageFilter::vtkMaximumEntropyRichardsonLucyDeconvolutionImageFilter()
{
  // Set up the internal filter pipeline.
  this->ITKDeconvolutionFilter = ITKDeconvolutionFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ITKInternalFilterType>(this->ITKDeconvolutionFilter);
  this->SetITKPipelineLastFilter<ITKInternalFilterType>(this->ITKDeconvolutionFilter);
}

//----------------------------------------------------------------------------
vtkMaximumEntropyRichardsonLucyDeconvolutionImageFilter::~vtkMaximumEntropyRichardsonLucyDeconvolutionImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkMaximumEntropyRichardsonLucyDeconvolutionImageFilter::UpdateInternalFilters()
{
  this->ITKDeconvolutionFilter->SetPadMethod(this->PaddingMethod);
  this->ITKDeconvolutionFilter->SetNumberOfIterations(this->NumberOfIterations);
  this->ITKDeconvolutionFilter->SetLambda(this->Lambda);
  this->ITKDeconvolutionFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkMaximumEntropyRichardsonLucyDeconvolutionImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
