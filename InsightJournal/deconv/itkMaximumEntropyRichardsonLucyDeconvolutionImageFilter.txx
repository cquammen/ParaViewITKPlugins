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
#ifndef __itkMaximumEntropyRichardsonLucyDeconvolutionImageFilter_txx
#define __itkMaximumEntropyRichardsonLucyDeconvolutionImageFilter_txx

#include "itkMaximumEntropyRichardsonLucyDeconvolutionImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkAddImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkMultiplyByComplexConjugateImageFilter.h"
#include "itkRelativeChangeCalculator.h"
#include "itkDivideOrZeroOutImageFilter.h"

namespace itk {

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
MaximumEntropyRichardsonLucyDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::MaximumEntropyRichardsonLucyDeconvolutionImageFilter()
{
  m_Lambda = 0.005;
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
MaximumEntropyRichardsonLucyDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::Init()
{
  Superclass::Init();

  typedef itk::BinaryFunctorImageFilter< InternalImageType,
                InternalImageType,
                InternalImageType,
                typename Functor::MultiplyByXMinusLambdaXLnX< TInternalPrecision > > MultType;
  typename MultType::Pointer mult = MultType::New();
  mult->SetInput( 0, this->m_Multiplication->GetInput( 0 ) );
  mult->SetInput( 1, this->m_Multiplication->GetInput( 1 ) );
  mult->GetFunctor().m_Lambda = m_Lambda;
  mult->SetNumberOfThreads( this->GetNumberOfThreads() );
  // mult data can't be released, it is required by two filters on next iteration
  mult->SetInPlace( true );

  this->m_Multiplication = mult;
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
MaximumEntropyRichardsonLucyDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Lambda: "  << m_Lambda << std::endl;
}

}// end namespace itk
#endif
