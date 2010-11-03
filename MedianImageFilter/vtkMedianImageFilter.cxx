/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkMedianImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkMedianImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

#include "vtkImageShiftScale.h"

vtkStandardNewMacro(vtkMedianImageFilter);

//----------------------------------------------------------------------------
vtkMedianImageFilter::vtkMedianImageFilter()
{
  // Set up number of input and output ports

  // Initialize the ITK-VTK glue.
  this->Init();

  // Set up the internal filter pipeline.
  this->MedianFilter = ITKMedianFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter(this->MedianFilter);
  this->SetITKPipelineLastFilter(this->MedianFilter);
}

//----------------------------------------------------------------------------
vtkMedianImageFilter::~vtkMedianImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkMedianImageFilter::UpdateInternalFilters()
{
  ITKImageType::SizeType radius;
  for (unsigned int i = 0; i < 3; i++)
    {
      radius[i] = this->NeighborhoodRadius[i];
    }
  this->MedianFilter->SetRadius(radius);
  this->MedianFilter->Update();

return 1;
}

//----------------------------------------------------------------------------
void vtkMedianImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
