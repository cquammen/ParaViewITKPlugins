/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFFTConvolutionPadImageFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFFTConvolutionPadImageFilter_txx
#define __itkFFTConvolutionPadImageFilter_txx

#include "itkFFTConvolutionPadImageFilter.h"
#include "itkProgressAccumulator.h"
#include "itkNumericTraits.h"
#include "itkConstantPadImageFilter.h"
#include "itkZeroFluxNeumannPadImageFilter.h"
#include "itkMirrorPadImageFilter.h"
#include "itkWrapPadImageFilter.h"
#include "itkChangeInformationImageFilter.h"

namespace itk {

template <class TInputImage, class TInputKernel, class TOutputImage, class TKernelOutput>
FFTConvolutionPadImageFilter<TInputImage, TInputKernel, TOutputImage, TKernelOutput>
::FFTConvolutionPadImageFilter()
{
  m_GreatestPrimeFactor = 13;
  m_PadMethod = ZERO_FLUX_NEUMANN;

  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput( 1, TKernelOutput::New() );
}

template <class TInputImage, class TInputKernel, class TOutputImage, class TKernelOutput>
void
FFTConvolutionPadImageFilter<TInputImage, TInputKernel, TOutputImage, TKernelOutput>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  InputImageType * input = const_cast<InputImageType *>(this->GetInput(0));
  InputImageType * kernel = const_cast<InputImageType *>(this->GetInput(1));
  if ( !input )
    {
    return;
    }

  OutputImageType * output = this->GetOutput();

  RegionType region = output->GetRequestedRegion();
  region.Crop( input->GetLargestPossibleRegion() );
  input->SetRequestedRegion( region );

  // kernel is not required
  if ( !kernel )
    {
    return;
    }

  region = output->GetRequestedRegion();
  region.Crop( kernel->GetLargestPossibleRegion() );
  kernel->SetRequestedRegion( region );
}


template <class TInputImage, class TInputKernel, class TOutputImage, class TKernelOutput>
void
FFTConvolutionPadImageFilter<TInputImage, TInputKernel, TOutputImage, TKernelOutput>
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  const InputImageType * input = this->GetInput();
  const InputKernelType * kernel = this->GetInputKernel();
  if ( !input )
    {
    return;
    }

  OutputImageType * output = this->GetOutput();
  OutputKernelType * kernelOutput = this->GetOutputKernel();

  RegionType inputRegion = input->GetLargestPossibleRegion();
  RegionType kernelRegion; // set later

  // dummy region to avoid code duplication when there is no kernel
  IndexType nullidx;
  nullidx.Fill(0);
  SizeType nullsize;
  nullsize.Fill(0);
  RegionType nullregion( nullidx, nullsize );

  if( kernel )
    {
    kernelRegion = kernel->GetLargestPossibleRegion();
    }
  else
    {
    kernelRegion = nullregion;
    }

  RegionType region;
  if( m_PadMethod == NO_PADDING )
    {
    region = inputRegion;
    }
  else
    {
    // increase the size of the output by the size of the kernel
    SizeType size;
    IndexType idx;
    for( int i=0; i<ImageDimension; i++ )
      {
      long s1 = std::max( (long)kernelRegion.GetSize()[i] - 1, (long)0 );
      if( m_GreatestPrimeFactor > 1 )
        {
        while( GreatestPrimeFactor( inputRegion.GetSize()[i] + s1 ) > m_GreatestPrimeFactor )
          {
          s1++;
          }
        }
      else if( m_GreatestPrimeFactor == 1 )
        {
        // make sure the total size is even
        s1 += ( inputRegion.GetSize()[i] + s1 ) % 2;
        }
      idx[i] = inputRegion.GetIndex()[i] - s1/2;
      size[i] = inputRegion.GetSize()[i] + s1;
      }
    region = RegionType( idx, size );
    }
  output->SetLargestPossibleRegion( region );
  // make sure that kernelOutput is actually there - it can be set to NULL by subclasses
  if( kernelOutput )
    {
    if( kernel )
      {
      kernelOutput->SetLargestPossibleRegion( region );
      }
    else
      {
      kernelOutput->SetLargestPossibleRegion( nullregion );
      }
    }
}


template<class TInputImage, class TInputKernel, class TOutputImage, class TKernelOutput>
void
FFTConvolutionPadImageFilter<TInputImage, TInputKernel, TOutputImage, TKernelOutput>
::GenerateData()
{
  this->AllocateOutputs();
  const InputImageType  * input        = this->GetInput();
  const InputKernelType * kernel       = this->GetInputKernel();
  OutputImageType       * output       = this->GetOutput();
  OutputKernelType      * kernelOutput = this->GetOutputKernel();

  RegionType inputRegion        = input->GetLargestPossibleRegion();
  RegionType kernelRegion;      // set later
  RegionType outputRegion       = output->GetLargestPossibleRegion();
  RegionType kernelOutputRegion = kernelOutput->GetLargestPossibleRegion();

  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  typedef typename itk::PadImageFilter< InputImageType, OutputImageType > PadType;
  typedef typename itk::ConstantPadImageFilter< InputImageType, OutputImageType > ConstantPadType;
  typedef typename itk::ZeroFluxNeumannPadImageFilter< InputImageType, OutputImageType > ZeroFluxPadType;
  typedef typename itk::MirrorPadImageFilter< InputImageType, OutputImageType > MirrorPadType;
  typedef typename itk::WrapPadImageFilter< InputImageType, OutputImageType > WrapPadType;
  SizeType s;

  typename PadType::Pointer padInput;
  switch( m_PadMethod )
    {
    case ZERO_FLUX_NEUMANN:
      {
      padInput = ZeroFluxPadType::New();
      break;
      }
    case NO_PADDING:
    case ZERO:
      {
      padInput = ConstantPadType::New();
      break;
      }
    case MIRROR:
      {
      padInput = MirrorPadType::New();
      break;
      }
    case WRAP:
      {
      padInput = WrapPadType::New();
      break;
    default:
      itkExceptionMacro(<< "Unknown pad method: " << m_PadMethod);
      break;
      }
    }
  padInput->SetInput( input );
  padInput->SetNumberOfThreads( this->GetNumberOfThreads() );
  if( m_PadMethod != NO_PADDING )
    {
    for( int i=0; i<ImageDimension; i++ )
      {
      s[i] = inputRegion.GetIndex()[i] - outputRegion.GetIndex()[i];
      }
    padInput->SetPadLowerBound( s );
    for( int i=0; i<ImageDimension; i++ )
      {
      s[i] = outputRegion.GetSize()[i] - ( inputRegion.GetIndex()[i] - outputRegion.GetIndex()[i] + inputRegion.GetSize()[i]);
      }
    padInput->SetPadUpperBound( s );
    }
  progress->RegisterInternalFilter( padInput, 0.5f );
  padInput->GraftOutput( output );
  padInput->Update();
  this->GraftOutput( padInput->GetOutput() );


  if( kernel )
    {
    kernelRegion = kernel->GetLargestPossibleRegion();

    typedef typename itk::ConstantPadImageFilter< InputKernelType, OutputKernelType > KernelPadType;
    typename KernelPadType::Pointer padKernel = KernelPadType::New();
    padKernel->SetInput( kernel );
    padKernel->SetNumberOfThreads( this->GetNumberOfThreads() );
    for( int i=0; i<ImageDimension; i++ )
      {
      s[i] = ( kernelOutputRegion.GetSize()[i] - kernelRegion.GetSize()[i] ) / 2;
      }
    padKernel->SetPadUpperBound( s );
    for( int i=0; i<ImageDimension; i++ )
      {
      s[i] = itk::Math::Ceil
        (( kernelOutputRegion.GetSize()[i] - kernelRegion.GetSize()[i] ) / 2.0 );
      // this line should do the same, but without requirement on ITK cvs
      //s[i] = ( kernelOutputRegion.GetSize()[i] - kernelRegion.GetSize()[i] ) / 2 +  ( kernelOutputRegion.GetSize()[i] - kernelRegion.GetSize()[i] ) % 2;
      }
    padKernel->SetPadLowerBound( s );
    progress->RegisterInternalFilter( padKernel, 0.5f );

    typedef typename itk::ChangeInformationImageFilter< OutputKernelType > ChangeType;
    typename ChangeType::Pointer change = ChangeType::New();
    change->SetInput( padKernel->GetOutput() );
    change->SetUseReferenceImage( true );
    change->SetReferenceImage( kernelOutput );
    change->SetChangeRegion( true );
    // no progress for change - it does almost nothing

    change->GraftOutput( kernelOutput );
    change->Update();
    this->GraftNthOutput( 1, change->GetOutput() );
    }
}


template<class TInputImage, class TInputKernel, class TOutputImage, class TKernelOutput>
void
FFTConvolutionPadImageFilter<TInputImage, TInputKernel, TOutputImage, TKernelOutput>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "GreatestPrimeFactor: "  << m_GreatestPrimeFactor << std::endl;
  os << indent << "PadMethod: "  << m_PadMethod << std::endl;
}

}// end namespace itk
#endif
