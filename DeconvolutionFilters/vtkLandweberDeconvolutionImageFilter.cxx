/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkLandweberDeconvolutionImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkLandweberDeconvolutionImageFilter.h"

#include "vtkObjectFactory.h"

#include "itkLandweberDeconvolutionImageFilter.h"


vtkStandardNewMacro(vtkLandweberDeconvolutionImageFilter);

//----------------------------------------------------------------------------
vtkLandweberDeconvolutionImageFilter::vtkLandweberDeconvolutionImageFilter()
{
  // Set up the internal filter pipeline.
  this->ITKDeconvolutionFilter = ITKDeconvolutionFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ITKInternalFilterType>(this->ITKDeconvolutionFilter);
  this->SetITKPipelineLastFilter<ITKInternalFilterType>(this->ITKDeconvolutionFilter);
}

//----------------------------------------------------------------------------
vtkLandweberDeconvolutionImageFilter::~vtkLandweberDeconvolutionImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkLandweberDeconvolutionImageFilter::UpdateInternalFilters()
{
  this->ITKDeconvolutionFilter->SetPadMethod(this->PaddingMethod);
  this->ITKDeconvolutionFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkLandweberDeconvolutionImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
