/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkVanCittertDeconvolutionImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkVanCittertDeconvolutionImageFilter.h"

#include "vtkObjectFactory.h"

#include "itkVanCittertDeconvolutionImageFilter.h"


vtkStandardNewMacro(vtkVanCittertDeconvolutionImageFilter);

//----------------------------------------------------------------------------
vtkVanCittertDeconvolutionImageFilter::vtkVanCittertDeconvolutionImageFilter()
{
  // Set up the internal filter pipeline.
  this->ITKDeconvolutionFilter = ITKDeconvolutionFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ITKInternalFilterType>(this->ITKDeconvolutionFilter);
  this->SetITKPipelineLastFilter<ITKInternalFilterType>(this->ITKDeconvolutionFilter);
}

//----------------------------------------------------------------------------
vtkVanCittertDeconvolutionImageFilter::~vtkVanCittertDeconvolutionImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkVanCittertDeconvolutionImageFilter::UpdateInternalFilters()
{
  this->ITKDeconvolutionFilter->SetPadMethod(this->PaddingMethod);
  this->ITKDeconvolutionFilter->SetNumberOfIterations(this->NumberOfIterations);
  this->ITKDeconvolutionFilter->SetAlpha(this->Alpha);
  this->ITKDeconvolutionFilter->SetNonNegativity(this->NonNegativity);
  this->ITKDeconvolutionFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkVanCittertDeconvolutionImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
