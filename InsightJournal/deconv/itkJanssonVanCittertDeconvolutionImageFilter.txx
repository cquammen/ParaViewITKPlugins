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
#ifndef __itkJanssonVanCittertDeconvolutionImageFilter_txx
#define __itkJanssonVanCittertDeconvolutionImageFilter_txx

#include "itkJanssonVanCittertDeconvolutionImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"

namespace itk {

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
JanssonVanCittertDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::Init()
{
  Superclass::Init();

  typedef itk::MinimumMaximumImageCalculator< TInputImage > CalculatorType;
  typename CalculatorType::Pointer calc = CalculatorType::New();
  calc->SetImage( this->GetInput() );
  calc->Compute();

  typedef itk::BinaryFunctorImageFilter< InternalImageType,
                InternalImageType,
                InternalImageType,
                Functor::JanssonVanCittert<TInternalPrecision> >
                  VanCittertType;
  typename VanCittertType::Pointer add = VanCittertType::New();
  add->SetInput( 1, this->m_Add->GetInput( 1 ) );
  add->SetInput( 0, this->m_Add->GetInput( 0 ) );
  add->GetFunctor().m_Alpha = this->GetAlpha();
  add->GetFunctor().m_NonNegativity = this->GetNonNegativity();
  add->GetFunctor().m_B2 = (TInternalPrecision) calc->GetMaximum() / 2.0;
  add->SetNumberOfThreads( this->GetNumberOfThreads() );
  add->SetReleaseDataFlag( true );
  add->SetInPlace( true );

  this->m_Add = add;
}


}// end namespace itk
#endif
