#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

// #include "itkFFTWRealToComplexConjugateImageFilter.h"
// #include "itkFFTWComplexConjugateToRealImageFilter.h"
#include "itkFFTConvolutionImageFilter.h"
#include "itkTestingMacros.h"


int main(int argc, char * argv[])
{

  if( argc < 4 )
    {
    std::cerr << "usage: " << argv[0] << " input kernel output [method [gpf]]" << std::endl;
    std::cerr << " input: the input image" << std::endl;
    std::cerr << " output: the output image" << std::endl;
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
  // itk::SimpleFilterWatcher watcher_reader(reader, "reader");

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[2] );
  // itk::SimpleFilterWatcher watcher_reader2(reader2, "reader2");

  typedef itk::FFTConvolutionImageFilter< IType > FFTConvolutionType;
  FFTConvolutionType::Pointer conv = FFTConvolutionType::New();
  conv->SetInput( reader->GetOutput() );
  TEST_SET_GET_VALUE( reader->GetOutput(), conv->GetInput() );
  conv->SetKernelImage( reader2->GetOutput() );
  TEST_SET_GET_VALUE( reader2->GetOutput(), conv->GetInput( 1 ) );
  TEST_SET_GET_VALUE( reader2->GetOutput(), conv->GetKernelImage() );
  TEST_SET_GET_VALUE( reader2->GetOutput(), conv->GetPointSpreadFunction() );

  // to exercise the methods
  conv->SetInput( NULL );
  conv->SetInput( reader->GetOutput() );
  TEST_SET_GET_VALUE( reader->GetOutput(), conv->GetInput() );
  conv->SetKernelImage( NULL );
  conv->SetKernelImage( reader2->GetOutput() );
  TEST_SET_GET_VALUE( reader2->GetOutput(), conv->GetInput( 1 ) );
  TEST_SET_GET_VALUE( reader2->GetOutput(), conv->GetKernelImage() );
  TEST_SET_GET_VALUE( reader2->GetOutput(), conv->GetPointSpreadFunction() );

  // test default value
  TEST_SET_GET_VALUE( 13, conv->GetGreatestPrimeFactor() );
  TEST_SET_GET_VALUE( FFTConvolutionType::PadType::ZERO_FLUX_NEUMANN, conv->GetPadMethod() );
  if( argc >= 5 )
    {
    conv->SetPadMethod( atoi(argv[4]) );
    TEST_SET_GET_VALUE( atoi(argv[4]), conv->GetPadMethod() );
    }
  if( argc >= 6 )
    {
    conv->SetGreatestPrimeFactor( atoi(argv[5]) );
    TEST_SET_GET_VALUE( atoi(argv[5]), conv->GetGreatestPrimeFactor() );
    }
  itk::SimpleFilterWatcher watcher_norm(conv, "conv");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( conv->GetOutput() );
  writer->SetFileName( argv[3] );
  // itk::SimpleFilterWatcher watcher_writer(writer, "writer");

  writer->Update();

  return 0;
}
