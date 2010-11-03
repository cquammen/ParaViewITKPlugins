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

#include "itkImprovementInSignalToNoiseRatioCalculator.h"
#include "itkTestingMacros.h"

int main(int argc, char * argv[])
{

  if( argc < 3 )
    {
    std::cerr << "usage: " << argv[0] << " original blurred restored" << std::endl;
    exit(1);
    }

  const int dim = 2;

  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer original = ReaderType::New();
  original->SetFileName( argv[1] );
  original->Update();

  ReaderType::Pointer blurred = ReaderType::New();
  blurred->SetFileName( argv[2] );
  blurred->Update();

  ReaderType::Pointer restored = ReaderType::New();
  restored->SetFileName( argv[3] );
  restored->Update();

  typedef itk::ImprovementInSignalToNoiseRatioCalculator< IType > CalculatorType;
  CalculatorType::Pointer isnr = CalculatorType::New();
  isnr->SetOriginalImage( original->GetOutput() );
  isnr->SetBlurredImage( blurred->GetOutput() );
  isnr->SetRestoredImage( restored->GetOutput() );
  isnr->Compute();
  std::cout << "ISNR: " << isnr->GetOutput() << " dB" << std::endl;

  return 0;
}

