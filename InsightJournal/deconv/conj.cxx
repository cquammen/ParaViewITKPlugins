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

#include "itkFFTRealToComplexConjugateImageFilter.h"
#include "itkFFTComplexConjugateToRealImageFilter.h"
#include "itkComplexConjugateImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkTestingMacros.h"


int main(int argc, char * argv[])
{

  if( argc != 3 )
    {
    std::cerr << "usage: " << argv[0] << " input output" << std::endl;
    std::cerr << " input: the input image" << std::endl;
    std::cerr << " output: the output image" << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  const int dim = 2;

  typedef float PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::FFTRealToComplexConjugateImageFilter< PType, 2 > FFTType;
  FFTType::Pointer fft = FFTType::New();
  fft->SetInput( reader->GetOutput() );

  typedef itk::ComplexConjugateImageFilter< FFTType::OutputImageType > ConjType;
  ConjType::Pointer conj = ConjType::New();
  conj->SetInput( fft->GetOutput() );
  itk::SimpleFilterWatcher watcher_norm(conj, "conj");

  typedef itk::FFTComplexConjugateToRealImageFilter< PType, 2 > IFFTType;
  IFFTType::Pointer ifft = IFFTType::New();
  ifft->SetInput( conj->GetOutput() );

  typedef itk::Image< unsigned char, 2 > OutputImageType;
  typedef itk::CastImageFilter< IType, OutputImageType > CastType;
  CastType::Pointer cast = CastType::New();
  cast->SetInput( ifft->GetOutput() );

  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( cast->GetOutput() );
  writer->SetFileName( argv[2] );

  writer->Update();

  return 0;
}
