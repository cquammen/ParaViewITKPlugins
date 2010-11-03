/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkNoiseImageFilter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkNoiseImageFilter.h"

#include "vtkObjectFactory.h"
#include "vtkImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkImageShiftScale.h"

#include "vtkPointData.h"

// Warning: this is really a hack to avoid creating a library just for
// the following class.
#include "itkThreadSafeMersenneTwisterRandomVariateGenerator.cxx"


vtkStandardNewMacro(vtkNoiseImageFilter);

//----------------------------------------------------------------------------
vtkNoiseImageFilter::vtkNoiseImageFilter()
{
  // Set up number of input and output ports

  // Initialize the ITK-VTK glue.
  this->Init();

  this->NoiseType = GAUSSIAN_NOISE;

  this->Mean = 1.0;
  this->StandardDeviation = 1.0;

  this->GaussianNoiseFilter = ITKGaussianNoiseFilterType::New();
  this->PoissonNoiseFilter  = ITKPoissonNoiseFilterType::New();

  // Set the first and last filters in the internal ITK pipeline.
  this->SetITKPipelineFirstFilter(this->GaussianNoiseFilter);
  this->SetITKPipelineLastFilter(this->GaussianNoiseFilter);
}

//----------------------------------------------------------------------------
vtkNoiseImageFilter::~vtkNoiseImageFilter()
{
}

//----------------------------------------------------------------------------
int vtkNoiseImageFilter::UpdateInternalFilters()
{
  //this->GaussianNoiseFilter->SetInput(NULL);
  //this->PoissonNoiseFilter->SetInput(NULL);

  // Now connect the ITK pipeline output to the VTK output
  if (NoiseType == GAUSSIAN_NOISE)
    {
    this->GaussianNoiseFilter->SetInput(this->ITKImporter->GetOutput());
    this->GaussianNoiseFilter->SetMean(this->Mean);
    this->GaussianNoiseFilter->SetStandardDeviation(this->StandardDeviation);

    this->GaussianNoiseFilter->Update();

    //this->ITKExporter->SetInput(this->GaussianNoiseFilter->GetOutput());
    this->SetITKPipelineFirstFilter(this->GaussianNoiseFilter);
    this->SetITKPipelineLastFilter(this->GaussianNoiseFilter);
    }
  else if (NoiseType == POISSON_NOISE)
    {
    this->PoissonNoiseFilter->SetInput(this->ITKImporter->GetOutput());
    this->PoissonNoiseFilter->Update();

    //this->ITKExporter->SetInput(this->PoissonNoiseFilter->GetOutput());
    this->SetITKPipelineFirstFilter(this->PoissonNoiseFilter);
    this->SetITKPipelineLastFilter(this->PoissonNoiseFilter);
    }

  return 1;
}

//----------------------------------------------------------------------------
void vtkNoiseImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
