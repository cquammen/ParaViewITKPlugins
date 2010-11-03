/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkImageFileWriter.h"

#include "itkSmoothingRecursiveGaussianImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkTestingMacros.h"
#include "itkMath.h"


int main(int argc, char * argv[])
{

  if( argc < 3 )
    {
    std::cerr << "usage: " << argv[0] << " output sigma" << std::endl;
    std::cerr << " output: the output image" << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  const int dim = 2;

  typedef float RPType;
  typedef itk::Image< RPType, dim > RIType;

  RIType::Pointer img = RIType::New();
  RIType::SizeType s;
  s.Fill( (int)itk::Math::Ceil(atof(argv[2])) * 3 * 2 + 1 );
  img->SetRegions( s );
  img->Allocate();
  img->FillBuffer( 0 );
  RIType::IndexType idx;
  idx.Fill( s[0]/2 );
  img->SetPixel( idx, 1.0 );

  typedef itk::SmoothingRecursiveGaussianImageFilter< RIType, RIType > GaussType;
  GaussType::Pointer gauss = GaussType::New();
  gauss->SetInput( img );
  gauss->SetSigma( atof(argv[2]) );

  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::RescaleIntensityImageFilter< RIType, IType > RescaleType;
  RescaleType::Pointer rescale = RescaleType::New();
  rescale->SetInput( gauss->GetOutput() );

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( rescale->GetOutput() );
  writer->SetFileName( argv[1] );

  writer->Update();

  return 0;
}
