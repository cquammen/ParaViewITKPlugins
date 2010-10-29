#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkFFTConvolutionPadImageFilter.h"
#include "itkTestingMacros.h"


int main(int argc, char * argv[])
{

  if( argc != 5 )
    {
    std::cerr << "usage: " << argv[0] << " intput output method gpf" << std::endl;
    std::cerr << " input: the input image" << std::endl;
    std::cerr << " output: the padded output image" << std::endl;
    std::cerr << " method: padding method - 1: zero flux, 2: zero, 3: mirror, 4: wrap" << std::endl;
    std::cerr << " gpf: greatest prime factor of the size of the padded image" << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  const int dim = 2;

  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::FFTConvolutionPadImageFilter< IType, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );

  TEST_SET_GET_VALUE(FilterType::ZERO_FLUX_NEUMANN , filter->GetPadMethod() );
  filter->SetPadMethod( atoi(argv[3]) );
  TEST_SET_GET_VALUE( atoi(argv[3]), filter->GetPadMethod() );

  // test default value
  TEST_SET_GET_VALUE( 13, filter->GetGreatestPrimeFactor() );

  // test PadToPowerOfTwo behavior
  TEST_SET_GET_VALUE( 0, filter->GetPadToPowerOfTwo() );
  filter->SetPadToPowerOfTwo( true );
  TEST_SET_GET_VALUE( 1, filter->GetPadToPowerOfTwo() );
  TEST_SET_GET_VALUE( 2, filter->GetGreatestPrimeFactor() );

  // ok, now use the user provided value
  filter->SetGreatestPrimeFactor( atoi(argv[4]) );
  TEST_SET_GET_VALUE( atoi(argv[4]), filter->GetGreatestPrimeFactor() );

  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}

