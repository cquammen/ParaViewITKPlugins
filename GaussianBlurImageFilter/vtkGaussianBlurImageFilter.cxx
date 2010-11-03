/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkGaussianBlurImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkGaussianBlurImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

vtkStandardNewMacro(vtkGaussianBlurImageFilter);

//----------------------------------------------------------------------------
vtkGaussianBlurImageFilter::vtkGaussianBlurImageFilter()
{
  // Set up number of input and output ports

  // Initialize the ITK-VTK glue.
  this->Init();

  // Set up the internal filter pipeline.
  this->GaussianFilter = ITKGaussianFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ITKInternalFilterType>(this->GaussianFilter);
  this->SetITKPipelineLastFilter<ITKInternalFilterType>(this->GaussianFilter);
}

//----------------------------------------------------------------------------
vtkGaussianBlurImageFilter::~vtkGaussianBlurImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkGaussianBlurImageFilter::UpdateInternalFilters()
{
  double variance[3];
  for (unsigned int i = 0; i < 3; i++)
    {
      variance[i] = this->StandardDeviation[i] * this->StandardDeviation[i];
    }
  this->GaussianFilter->SetVariance(variance);
  this->GaussianFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkGaussianBlurImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
