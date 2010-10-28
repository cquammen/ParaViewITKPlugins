#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkExtractReferenceImageRegionImageFilter.h"
#include "itkConstantPadImageFilter.h"
#include "itkCropImageFilter.h"


int main(int argc, char * argv[])
{

  if( argc != 3 )
    {
    std::cerr << "usage: " << argv[0] << " intput output" << std::endl;
    std::cerr << " input: the input image" << std::endl;
    std::cerr << " output: the output image" << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  const int dim = 2;

  typedef unsigned char PType;
  typedef itk::Image< PType, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::ConstantPadImageFilter< IType, IType > PadType;
  PadType::Pointer pad = PadType::New();
  pad->SetInput( reader->GetOutput() );
  // huge pad shouldn't have any memory requirement, since the padded image is lazily evaluated
  // lets test that as well
  PadType::InputImageSizeType s;
  s.Fill(100000000);
  pad->SetPadBound( s );

  // crop the image to test that our filter trigger the update of the output information of the
  // reference image as it should
  typedef itk::CropImageFilter< IType, IType > CropType;
  CropType::Pointer crop = CropType::New();
  crop->SetInput( reader->GetOutput() );
  s.Fill(20);
  crop->SetBoundaryCropSize( s );

  typedef itk::ExtractReferenceImageRegionImageFilter< IType, IType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( pad->GetOutput() );
  filter->SetReferenceImage( crop->GetOutput() );

  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}

