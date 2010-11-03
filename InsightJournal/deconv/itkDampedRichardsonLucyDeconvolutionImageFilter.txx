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
#ifndef __itkDampedRichardsonLucyDeconvolutionImageFilter_txx
#define __itkDampedRichardsonLucyDeconvolutionImageFilter_txx

#include "itkDampedRichardsonLucyDeconvolutionImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkAddImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkMultiplyByComplexConjugateImageFilter.h"
#include "itkRelativeChangeCalculator.h"

namespace itk {

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
DampedRichardsonLucyDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::DampedRichardsonLucyDeconvolutionImageFilter()
{
  m_Threshold = 2;
}


template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
DampedRichardsonLucyDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::Init()
{
  Superclass::Init();

  // replace the simple division by our more complex implementation
  typedef itk::BinaryFunctorImageFilter< InternalImageType,
                InternalImageType,
                InternalImageType,
                typename Functor::DampedRichardsonLucy< TInternalPrecision > >
                  DampedRichardsonLucyType;
  typename DampedRichardsonLucyType::Pointer div = DampedRichardsonLucyType::New();
  div->SetInput( 0, this->m_Division->GetInput( 0 ) );
  div->SetInput( 1, this->m_Division->GetInput( 1 ) );
  div->GetFunctor().m_T = m_Threshold;
  div->GetFunctor().m_N = 10;
  div->SetNumberOfThreads( this->GetNumberOfThreads() );
  div->SetReleaseDataFlag( true );
  div->SetInPlace( true );

  this->m_Convolution2->SetInput( div->GetOutput() );
  this->m_Division = div;
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
DampedRichardsonLucyDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Threshold: "  << m_Threshold << std::endl;
}

}// end namespace itk
#endif
