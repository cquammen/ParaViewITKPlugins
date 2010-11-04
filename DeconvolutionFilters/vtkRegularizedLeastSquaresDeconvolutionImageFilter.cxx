/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRegularizedLeastSquaresDeconvolutionImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkRegularizedLeastSquaresDeconvolutionImageFilter.h"

#include "vtkObjectFactory.h"

#include "itkRegularizedLeastSquaresDeconvolutionImageFilter.h"


vtkStandardNewMacro(vtkRegularizedLeastSquaresDeconvolutionImageFilter);

//----------------------------------------------------------------------------
vtkRegularizedLeastSquaresDeconvolutionImageFilter::vtkRegularizedLeastSquaresDeconvolutionImageFilter()
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
vtkRegularizedLeastSquaresDeconvolutionImageFilter::~vtkRegularizedLeastSquaresDeconvolutionImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkRegularizedLeastSquaresDeconvolutionImageFilter::UpdateInternalFilters()
{
  this->ITKDeconvolutionFilter->SetPadMethod(this->PaddingMethod);
  this->ITKDeconvolutionFilter->SetAlpha(this->Alpha);
  this->ITKDeconvolutionFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkRegularizedLeastSquaresDeconvolutionImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
