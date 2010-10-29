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
   
  std::cout << fftwf_import_system_wisdom() << std::endl;
   
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
  
  std::vector< int > radiusList;
  for( int s=3; s<=10; s++)
    { radiusList.push_back( s ); }
  for( int s=15; s<=30; s+=5)
    { radiusList.push_back( s ); }
  radiusList.push_back( 40 );
  radiusList.push_back( 50 );
  radiusList.push_back( 100 );
  
  std::cout << "#size" << "\t" 
            << "conv" << "\t" 
            << "fft" << "\t" 
            << std::endl;

  for( std::vector< int >::iterator it=radiusList.begin(); it !=radiusList.end() ; it++)
    {
    itk::TimeProbe convtime;
    itk::TimeProbe fftconvtime;
  
    IType::Pointer kernel = IType::New();
    IType::SizeType size;
    size.Fill(*it);
    kernel->SetRegions( size );
    kernel->Allocate();
    kernel->FillBuffer(255);
    
    conv->SetInput( 1, kernel );
    fftconv->SetInput( 1, kernel );

    int nbOfRepeats;
    if( *it <= 10 )
      { nbOfRepeats = 20; }
    else if( *it <= 20 )
      { nbOfRepeats = 10; }
    else if( *it <= 50 )
      { nbOfRepeats = 5; }
    else
      { nbOfRepeats = 2; }
    nbOfRepeats = 1;

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
      
    std::cout << std::setprecision(3) << *it << "\t" 
              << convtime.GetMeanTime() << "\t" 
              << fftconvtime.GetMeanTime() << "\t" 
              <<std::endl;
    }
  
  
  return 0;
}

