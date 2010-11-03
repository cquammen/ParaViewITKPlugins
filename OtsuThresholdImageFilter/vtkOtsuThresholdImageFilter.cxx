/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkOtsuThresholdImageFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "vtkOtsuThresholdImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

vtkStandardNewMacro(vtkOtsuThresholdImageFilter);

//----------------------------------------------------------------------------
vtkOtsuThresholdImageFilter::vtkOtsuThresholdImageFilter()
{
  // Set up number of input and output ports

  // Initialize the ITK-VTK glue.
  this->Init();

  // Set up the internal filter pipeline.
  this->OtsuFilter = ITKOtsuThresholdImageFilterType::New();
  this->OtsuFilter->SetInsideValue(0.0);
  this->OtsuFilter->SetOutsideValue(1.0);

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter<ITKInternalFilterType>(this->OtsuFilter);
  this->SetITKPipelineLastFilter<ITKInternalFilterType>(this->OtsuFilter);
}

//----------------------------------------------------------------------------
vtkOtsuThresholdImageFilter::~vtkOtsuThresholdImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkOtsuThresholdImageFilter::UpdateInternalFilters()
{
  this->OtsuFilter->Update();

  return 1;
}

//----------------------------------------------------------------------------
void vtkOtsuThresholdImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
