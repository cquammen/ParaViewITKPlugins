#include "itkFFTWRealToComplexConjugateImageFilter.h"
#include "itkFFTWComplexConjugateToRealImageFilter.h"

#include "itkImageFileReader.h"
#include "itkTimeProbe.h"
#include "itkConvolutionImageFilter.h"
#include "itkFFTConvolutionImageFilter.h"
#include <iomanip>

int main(int argc, char * argv[])
{
  if( argc < 2 )                                                                                                                                                           
    {                                                                                                                                                                      
    std::cerr << "usage: " << argv[0] << " intput [gpf]" << std::endl;                                                                                                     
    }                                                                                                                                                                      
  
  fftwf_import_system_wisdom();
  
  const int dim = 3;
  typedef unsigned char PType;
  typedef itk::Image< PType, dim >    IType;
  
  // read the input image
  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::ConvolutionImageFilter< IType, IType > ConvType;

  ConvType::Pointer conv = ConvType::New();
  conv->SetInput( reader->GetOutput() );
  conv->SetNormalize( true );

  typedef itk::FFTConvolutionImageFilter< IType > FFTConvType;
  FFTConvType::Pointer fftconv = FFTConvType::New();
  fftconv->SetInput( reader->GetOutput() );
  if( argc >= 3 )
    {
    fftconv->SetGreatestPrimeFactor( atoi(argv[2]) );
    }

  reader->Update();
  
    IType::Pointer kernel = IType::New();
    IType::SizeType size;
    size.Fill(2);
    kernel->SetRegions( size );
    kernel->Allocate();
    kernel->FillBuffer(255);
    
    conv->SetInput( 1, kernel );
    fftconv->SetInput( 1, kernel );

  std::cout << "#th" << "\t" 
            << "conv" << "\t" 
            << "fft" << "\t" 
            << std::endl;

  for( int n=1; n<16; n++ )
    {
    itk::TimeProbe convtime;
    itk::TimeProbe fftconvtime;
    
    conv->SetNumberOfThreads( n );
    fftconv->SetNumberOfThreads( n );

    int nbOfRepeats = 10;

    for( int i=0; i<nbOfRepeats; i++ )
      {
      convtime.Start();
      conv->Update();
      convtime.Stop();
      conv->Modified();

      fftconvtime.Start();
      fftconv->Update();
      fftconvtime.Stop();
      fftconv->Modified();


      }
      
    std::cout << std::setprecision(3) << n << "\t" 
              << convtime.GetMeanTime() << "\t" 
              << fftconvtime.GetMeanTime() << "\t" 
              <<std::endl;
    }
  
  
  return 0;
}

