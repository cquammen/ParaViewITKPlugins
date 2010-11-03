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
#ifndef __itkPoissonMaximumAPosterioriDeconvolutionImageFilter_txx
#define __itkPoissonMaximumAPosterioriDeconvolutionImageFilter_txx

#include "itkPoissonMaximumAPosterioriDeconvolutionImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkFFTConvolveByOpticalTransferFunctionImageFilter.h"

namespace itk {

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
PoissonMaximumAPosterioriDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::Init()
{
  Superclass::Init();

  InternalImagePointerType input;
  InternalImagePointerType firstEstimate;
  ComplexImagePointerType psf;

  this->PrepareInputs( input, firstEstimate, psf );

  // first convolve the input image by the psf
  typedef itk::FFTConvolveByOpticalTransferFunctionImageFilter< InternalPrecisionType, ImageDimension > ConvolutionType;
  typename ConvolutionType::Pointer conv = ConvolutionType::New();
  conv->SetOpticalTransferFunction( psf );
  conv->SetNumberOfThreads( this->GetNumberOfThreads() );
  conv->SetReleaseDataFlag( true );
  m_Convolution = conv;

  // divide the input by (the convolved image + epsilon)
  typedef itk::BinaryFunctorImageFilter< InternalImageType,
                InternalImageType,
                InternalImageType,
                typename Functor::PoissonMaximumAPosteriori< TInternalPrecision > >
                  PoissonMaximumAPosterioriType;
  typename PoissonMaximumAPosterioriType::Pointer div = PoissonMaximumAPosterioriType::New();
  div->SetInput( 1, input );
  div->SetInput( 0, conv->GetOutput() );
  div->SetNumberOfThreads( this->GetNumberOfThreads() );
  div->SetReleaseDataFlag( true );
  div->SetInPlace( true );
  m_Division = div;

  // convolve the divided image by the psf
  typename ConvolutionType::Pointer conv2 = ConvolutionType::New();
  conv2->SetInput( div->GetOutput() );
  conv2->SetOpticalTransferFunction( psf );
  conv2->SetTranspose( true );
  conv2->SetNumberOfThreads( this->GetNumberOfThreads() );
  conv2->SetReleaseDataFlag( true );
  m_Convolution2 = conv2;

  // multiply the result with the input

  typedef itk::BinaryFunctorImageFilter< InternalImageType,
                InternalImageType,
                InternalImageType,
                typename Functor::PoissonMaximumAPosteriori2< TInternalPrecision > >
                  RealMultType;
  typename RealMultType::Pointer mult = RealMultType::New();
  mult->SetInput( 0, conv2->GetOutput() );
  // mult->SetInput( 1, input );
  mult->SetNumberOfThreads( this->GetNumberOfThreads() );
  // mult data can't be released, it is required by two filters on next iteration
  mult->SetInPlace( true );
  m_Multiplication = mult;

  SetEstimate( firstEstimate );
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
PoissonMaximumAPosterioriDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::SetEstimate( InternalImageType * estimate )
{
  Superclass::SetEstimate( estimate );
  m_Convolution->SetInput( estimate );
  m_Multiplication->SetInput( 1, estimate );
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
typename PoissonMaximumAPosterioriDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>::InternalImageType::Pointer
PoissonMaximumAPosterioriDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::NewEstimate()
{
  return m_Multiplication->GetOutput();
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
PoissonMaximumAPosterioriDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::End()
{
  Superclass::End();

  m_Convolution = NULL;
  m_Division = NULL;
  m_Convolution2 = NULL;
  m_Multiplication = NULL;
}

}// end namespace itk
#endif
