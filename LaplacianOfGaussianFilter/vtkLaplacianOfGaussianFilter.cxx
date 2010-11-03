/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkLaplacianOfGaussianFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkLaplacianOfGaussianFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

vtkStandardNewMacro(vtkLaplacianOfGaussianFilter);

//----------------------------------------------------------------------------
vtkLaplacianOfGaussianFilter::vtkLaplacianOfGaussianFilter()
{
  // Set up number of input and output ports

  // Initialize the ITK-VTK glue.
  this->Init();

  // Set up the internal filter pipeline.
  this->LOGFilter = ITKLOGFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ITKInternalFilterType>(this->LOGFilter);
  this->SetITKPipelineLastFilter<ITKInternalFilterType>(this->LOGFilter);
}

//----------------------------------------------------------------------------
vtkLaplacianOfGaussianFilter::~vtkLaplacianOfGaussianFilter()
{
}

//----------------------------------------------------------------------------
int vtkLaplacianOfGaussianFilter::UpdateInternalFilters()
{
  this->LOGFilter->SetSigma(this->Sigma);
  this->LOGFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkLaplacianOfGaussianFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
