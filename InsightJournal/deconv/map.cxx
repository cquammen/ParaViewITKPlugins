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
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkPoissonMaximumAPosterioriDeconvolutionImageFilter.h"
#include "itkTestingMacros.h"


int main(int argc, char * argv[])
{

  if( argc < 4 )
    {
    std::cerr << "usage: " << argv[0] << " input kernel output [nbIt [th [method [gpf]]]]" << std::endl;
    std::cerr << " input: the input image" << std::endl;
    std::cerr << " output: the output image" << std::endl;
    std::cerr << " method: padding method - 1: zero flux, 2: zero, 3: mirror, 4: wrap" << std::endl;
    std::cerr << " gpf: greatest prime factor of the size of the padded image" << std::endl;
    exit(1);
    }

  const int dim = 2;

  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[2] );

  typedef itk::FFTConvolutionPadImageFilter< IType > FFTConvolutionPadType;
  typedef itk::PoissonMaximumAPosterioriDeconvolutionImageFilter< IType > FFTConvolutionType;
  FFTConvolutionType::Pointer conv = FFTConvolutionType::New();
  conv->SetInput( reader->GetOutput() );
  conv->SetPointSpreadFunction( reader2->GetOutput() );

  // test default value
  TEST_SET_GET_VALUE( 13, conv->GetGreatestPrimeFactor() );
  TEST_SET_GET_VALUE( FFTConvolutionPadType::ZERO_FLUX_NEUMANN, conv->GetPadMethod() );
  TEST_SET_GET_VALUE( 10, conv->GetNumberOfIterations() );
  TEST_SET_GET_VALUE( 0.0, conv->GetRelativeChangeThreshold() );

  if( argc >= 5 )
    {
    conv->SetNumberOfIterations( atoi(argv[4]) );
    TEST_SET_GET_VALUE( atoi(argv[4]), conv->GetNumberOfIterations() );
    }

  if( argc >= 6 )
    {
    conv->SetRelativeChangeThreshold( atof(argv[5]) );
    TEST_SET_GET_VALUE( atof(argv[5]), conv->GetRelativeChangeThreshold() );
    }

  if( argc >= 7 )
    {
    conv->SetPadMethod( atoi(argv[6]) );
    TEST_SET_GET_VALUE( atoi(argv[6]), conv->GetPadMethod() );
    }

  if( argc >= 8 )
    {
    conv->SetGreatestPrimeFactor( atoi(argv[7]) );
    TEST_SET_GET_VALUE( atoi(argv[7]), conv->GetGreatestPrimeFactor() );
    }
  itk::SimpleFilterWatcher watcher_norm(conv, "conv");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( conv->GetOutput() );
  writer->SetFileName( argv[3] );

  writer->Update();

  return 0;
}
