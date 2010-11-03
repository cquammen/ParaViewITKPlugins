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
#ifndef __itkImprovementInSignalToNoiseRatioCalculator_txx
#define __itkImprovementInSignalToNoiseRatioCalculator_txx
#include "itkImprovementInSignalToNoiseRatioCalculator.h"

#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionConstIterator.h"

namespace itk
{


template < class TInputImage>
ImprovementInSignalToNoiseRatioCalculator<TInputImage>
::ImprovementInSignalToNoiseRatioCalculator(void)
{
  m_Valid = false;
  m_OriginalImage = NULL;
  m_BlurredImage = NULL;
  m_RestoredImage = NULL;
  m_Output = NumericTraits< InputPixelType >::Zero;
}


template < class TInputImage >
void
ImprovementInSignalToNoiseRatioCalculator<TInputImage>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "OriginalImage: " << m_OriginalImage.GetPointer() << std::endl;
  os << indent << "BlurredImage: " << m_BlurredImage.GetPointer() << std::endl;
  os << indent << "RestoredImage: " << m_RestoredImage.GetPointer() << std::endl;
  os << indent << "Valid: " << m_Valid << std::endl;
  os << indent << "Output: " << m_Output << std::endl;
}


template < class TInputImage >
void
ImprovementInSignalToNoiseRatioCalculator<TInputImage>
::Compute()
{

  typedef typename InputImageType::IndexType IndexType;

  if( !m_OriginalImage || !m_BlurredImage || !m_RestoredImage )
    {
    return;
    }

  ImageRegionConstIteratorWithIndex< InputImageType > oIt( m_OriginalImage,
                                                     m_OriginalImage->GetRequestedRegion() );
  oIt.GoToBegin();
  ImageRegionConstIteratorWithIndex< InputImageType > bIt( m_BlurredImage,
                                                     m_OriginalImage->GetRequestedRegion() );
  bIt.GoToBegin();
  ImageRegionConstIteratorWithIndex< InputImageType > rIt( m_RestoredImage,
                                                     m_OriginalImage->GetRequestedRegion() );
  rIt.GoToBegin();

  // init the values
  double bmse = 0;
  double rmse = 0;

  while( !oIt.IsAtEnd() )
    {
    bmse += pow( (double)bIt.Get() - (double)oIt.Get(), 2 );
    rmse += pow( (double)rIt.Get() - (double)oIt.Get(), 2 );
    ++oIt;
    ++bIt;
    ++rIt;
    }

  m_Output = 10 * vcl_log10( bmse / rmse );
  m_Valid = true;

}


template < class TInputImage >
const double &
ImprovementInSignalToNoiseRatioCalculator<TInputImage>
::GetOutput() const
{
  if (!m_Valid)
    {
    itkExceptionMacro( << "GetOutput() invoked, but the output have not been computed. Call Compute() first.");
    }
  return m_Output;
}

} // end namespace itk

#endif
