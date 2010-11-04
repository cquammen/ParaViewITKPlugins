/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkIterativeDeconvolutionImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkIterativeDeconvolutionImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkStreamingDemandDrivenPipeline.h"


vtkStandardNewMacro(vtkIterativeDeconvolutionImageFilter);

//----------------------------------------------------------------------------
vtkIterativeDeconvolutionImageFilter::vtkIterativeDeconvolutionImageFilter()
{
}

//----------------------------------------------------------------------------
vtkIterativeDeconvolutionImageFilter::~vtkIterativeDeconvolutionImageFilter()
{
}

//----------------------------------------------------------------------------
void vtkIterativeDeconvolutionImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
