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
#ifndef __itkWienerDeconvolutionImageFilter_txx
#define __itkWienerDeconvolutionImageFilter_txx

#include "itkWienerDeconvolutionImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"

namespace itk {

template <class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
WienerDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::WienerDeconvolutionImageFilter()
{
  m_Gamma = 0.001;
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
WienerDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::GenerateData()
{
  ComplexImagePointerType input;
  ComplexImagePointerType psf;

  this->PrepareInputs( input, psf, 0.6f );

  typedef itk::BinaryFunctorImageFilter< ComplexImageType,
                                    ComplexImageType,
                                    ComplexImageType,
                typename Functor::WienerDeconvolution< ComplexType > >
                  MultType;
  typename MultType::Pointer mult = MultType::New();
  mult->SetInput( 0, input );
  mult->SetInput( 1, psf );
  mult->GetFunctor().m_Gamma = m_Gamma;
  mult->SetNumberOfThreads( this->GetNumberOfThreads() );
  mult->SetReleaseDataFlag( true );
  mult->SetInPlace( true );
  this->RegisterInternalFilter( mult, 0.1f );

  this->ProduceOutput( mult->GetOutput(), 0.3f );
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
WienerDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Gamma: "  << m_Gamma << std::endl;
}

}// end namespace itk
#endif
