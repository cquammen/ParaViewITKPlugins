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
#ifndef __itkVanCittertDeconvolutionImageFilter_txx
#define __itkVanCittertDeconvolutionImageFilter_txx

#include "itkVanCittertDeconvolutionImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkAddImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkFFTConvolveByOpticalTransferFunctionImageFilter.h"

namespace itk {

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
VanCittertDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::VanCittertDeconvolutionImageFilter()
{
  m_Alpha = 1;
  m_NonNegativity = true;
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
VanCittertDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
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

  // compute the residual
  typedef itk::SubtractImageFilter< InternalImageType,
                InternalImageType,
                InternalImageType > SubtractType;
  typename SubtractType::Pointer sub = SubtractType::New();
  sub->SetInput( 0, input );
  sub->SetInput( 1, conv->GetOutput() );
  sub->SetNumberOfThreads( this->GetNumberOfThreads() );
  sub->SetReleaseDataFlag( true );
  // don't run in place - we need to keep the input image
  m_Subtract = sub;

  typedef itk::BinaryFunctorImageFilter< InternalImageType,
                InternalImageType,
                InternalImageType,
                Functor::VanCittert<TInternalPrecision> >
                  VanCittertType;
  typename VanCittertType::Pointer add = VanCittertType::New();
  add->SetInput( 0, sub->GetOutput() );
  add->GetFunctor().m_Alpha = m_Alpha;
  add->GetFunctor().m_NonNegativity = m_NonNegativity;
  add->SetNumberOfThreads( this->GetNumberOfThreads() );
  add->SetReleaseDataFlag( true );
  add->SetInPlace( true );
  m_Add = add;

  SetEstimate( firstEstimate );

}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
VanCittertDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::SetEstimate( InternalImageType * estimate )
{
  Superclass::SetEstimate( estimate );
  m_Convolution->SetInput( estimate );
  m_Add->SetInput( 1, estimate );
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
typename VanCittertDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>::InternalImageType::Pointer
VanCittertDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::NewEstimate()
{
  return m_Add->GetOutput();
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
VanCittertDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::End()
{
  Superclass::End();

  m_Convolution = NULL;
  m_Subtract = NULL;
  m_Add = NULL;
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
VanCittertDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Alpha: "  << m_Alpha << std::endl;
  os << indent << "NonNegativity: "  << m_NonNegativity << std::endl;
}

}// end namespace itk
#endif
