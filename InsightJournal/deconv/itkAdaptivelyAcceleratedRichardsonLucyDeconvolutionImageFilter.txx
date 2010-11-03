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
#ifndef __itkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter_txx
#define __itkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter_txx

#include "itkAdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter.h"
#include "itkBinaryFunctorImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkAddImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkMultiplyByComplexConjugateImageFilter.h"
#include "itkRelativeChangeCalculator.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkForwardDifferenceOperator.h"
#include "itkDivideOrZeroOutImageFilter.h"

namespace itk {

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
AdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::Init()
{
  // build the pipeline from the superclass
  Superclass::Init();

  // change the multiplication filter by a new one
  typename MultType::Pointer mult = MultType::New();
  mult->SetInput( 0, this->m_Multiplication->GetInput( 0 ) );
  mult->SetInput( 1, this->m_Multiplication->GetInput( 1 ) );
  mult->SetNumberOfThreads( this->GetNumberOfThreads() );
  // mult data can't be released, it is required by two filters on next iteration
  mult->SetInPlace( true );
  this->m_Multiplication = mult;

  // and initialize the variables needed to compute the acceleration parameter
  m_V1   = 1.0;
  m_V2   = 1.0;
  m_Vk_1 = 1.0;
  m_Vk   = 1.0;

  // that's it!
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
AdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::BeforeIteration()
{
  Superclass::BeforeIteration();

  // compute the L2 norm of the derivative and store it so it can be reused later
  double v = ComputeL2NormOfFirstOrderDerivative( this->GetEstimate() );
  if( this->GetIteration() == 1 )
    {
    m_V1 = v;
    }
  else if( this->GetIteration() == 2 )
    {
    m_V2 = v;
    }
  m_Vk_1 = m_Vk;
  m_Vk = v;

  // and update the acceleration parameter
  MultType * mult = dynamic_cast<MultType *>(this->m_Multiplication.GetPointer());
  if( this->GetIteration() < 3 )
    {
    mult->GetFunctor().m_Q = 2;
    }
  else
    {
    mult->GetFunctor().m_Q = vcl_exp( m_Vk / m_Vk_1 ) - m_V2 / m_V1;
    }
}


template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
double
AdaptivelyAcceleratedRichardsonLucyDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::ComputeL2NormOfFirstOrderDerivative( const InternalImageType * input )
{
  double res = 0;

  typedef ForwardDifferenceOperator< InternalPrecisionType, ImageDimension > OperatorType;
  OperatorType op;

  typename InternalImageType::SpacingType spacing = input->GetSpacing();

  for( int i=0; i<ImageDimension; i++ )
    {
    op.SetDirection( i );
    op.CreateDirectional();

    typedef NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InternalImageType>
      BFC;

    typedef typename BFC::FaceListType FaceListType;

    NeighborhoodInnerProduct<InternalImageType, InternalPrecisionType> smartInnerProduct;
    BFC faceCalculator;
    FaceListType faceList;

    // Break the input into a series of regions.  The first region is free
    // of boundary conditions, the rest with boundary conditions. Note,
    // we pass in the input image and the OUTPUT requested region. We are
    // only concerned with centering the neighborhood operator at the
    // pixels that correspond to output pixels.
    faceList = faceCalculator(input, input->GetLargestPossibleRegion(), op.GetRadius());

    typename FaceListType::iterator fit;
    // Process non-boundary region and each of the boundary faces.
    // These are N-d regions which border the edge of the buffer.
    ConstNeighborhoodIterator<InternalImageType> bit;
    for (fit=faceList.begin(); fit != faceList.end(); ++fit)
      {
      bit = ConstNeighborhoodIterator<InternalImageType>(op.GetRadius(), input, *fit);
      ZeroFluxNeumannBoundaryCondition<InternalImageType> nbc;
      bit.OverrideBoundaryCondition( &nbc );
      bit.GoToBegin();
      while ( ! bit.IsAtEnd() )
        {
        double v = smartInnerProduct(bit, op);
        v /= spacing[i];
        res += v * v;
        ++bit;
        }
      }
    }

  return vcl_sqrt( res );
}

}// end namespace itk
#endif
