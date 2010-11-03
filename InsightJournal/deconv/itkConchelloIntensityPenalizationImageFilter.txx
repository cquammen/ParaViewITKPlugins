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
#ifndef __itkConchelloIntensityPenalizationImageFilter_txx
#define __itkConchelloIntensityPenalizationImageFilter_txx

#include "itkConchelloIntensityPenalizationImageFilter.h"

namespace itk
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
ConchelloIntensityPenalizationImageFilter<TInputImage, TOutputImage>
::ConchelloIntensityPenalizationImageFilter()
{
  m_Lambda = 1.0/255;
}

template <class TInputImage, class TOutputImage>
void
ConchelloIntensityPenalizationImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  this->GetFunctor().SetLambda(m_Lambda);
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void
ConchelloIntensityPenalizationImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Lambda: "
     << static_cast<typename NumericTraits<InputPixelType>::PrintType>(m_Lambda)
     << std::endl;
}


} // end namespace itk

#endif
