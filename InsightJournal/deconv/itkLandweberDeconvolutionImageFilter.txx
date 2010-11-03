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
#ifndef __itkLandweberDeconvolutionImageFilter_txx
#define __itkLandweberDeconvolutionImageFilter_txx

#include "itkLandweberDeconvolutionImageFilter.h"
#include "itkFFTConvolveByOpticalTransferFunctionImageFilter.h"

namespace itk {

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
LandweberDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::Init()
{
  Superclass::Init();

  typedef itk::FFTConvolveByOpticalTransferFunctionImageFilter< InternalPrecisionType, ImageDimension > ConvolutionType;

  ConvolutionType * conv = dynamic_cast<ConvolutionType*>( this->m_Convolution.GetPointer() );

  typename ConvolutionType::Pointer conv2 = ConvolutionType::New();
  conv2->SetInput( this->m_Subtract->GetOutput() );
  conv2->SetOpticalTransferFunction( conv->GetOpticalTransferFunction() );
  conv2->SetNumberOfThreads( this->GetNumberOfThreads() );
  conv2->SetReleaseDataFlag( true );

  this->m_Add->SetInput( 0, conv2->GetOutput() );
  m_Convolution2 = conv2;
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
LandweberDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::End()
{
  Superclass::End();
  m_Convolution2 = NULL;
}


}// end namespace itk
#endif
