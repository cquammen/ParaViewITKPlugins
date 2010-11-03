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
#ifndef __itkConchelloIntensityPenalizationImageFilter_h
#define __itkConchelloIntensityPenalizationImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace itk
{

namespace Functor {

template< typename TInput, typename  TOutput>
class ConchelloIntensityPenalization
{
public:
  typedef typename NumericTraits< TInput >::RealType RealType;
  ConchelloIntensityPenalization() {m_Lambda = NumericTraits< TInput >::max();}
  ~ConchelloIntensityPenalization() {}

  void SetLambda( double max ) { m_Lambda = max; }

  bool operator!=( const ConchelloIntensityPenalization & other ) const
    {
    if( m_Lambda != other.m_Lambda )
      {
      return true;
      }
    return false;
    }

  bool operator==( const ConchelloIntensityPenalization & other ) const
    {
    return !(*this != other);
    }

  inline TOutput operator()( const TInput & x ) const
    {
    TOutput  result = static_cast<TOutput>( ( - 1.0 + vcl_sqrt( 1.0 + 2.0 * m_Lambda * x ) ) / m_Lambda );
    return result;
    }
private:
  double  m_Lambda;
};

}  // end namespace functor


/** \class ConchelloIntensityPenalizationImageFilter
 * \brief
 *
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa IntensityWindowingImageFilter ShiftScaleImageFilter
 * \ingroup IntensityImageFilters  Multithreaded
 *
 */
template <typename  TInputImage, typename  TOutputImage=TInputImage>
class ITK_EXPORT ConchelloIntensityPenalizationImageFilter :
    public
    UnaryFunctorImageFilter<TInputImage,TOutputImage,
                            Functor::ConchelloIntensityPenalization<
  typename TInputImage::PixelType,
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef ConchelloIntensityPenalizationImageFilter    Self;
  typedef UnaryFunctorImageFilter<TInputImage,TOutputImage,
                                  Functor::ConchelloIntensityPenalization<
    typename TInputImage::PixelType,
    typename TOutputImage::PixelType> > Superclass;
  typedef SmartPointer<Self>            Pointer;
  typedef SmartPointer<const Self>      ConstPointer;

  typedef typename TOutputImage::PixelType                 OutputPixelType;
  typedef typename TInputImage::PixelType                  InputPixelType;
  typedef typename NumericTraits<InputPixelType>::RealType RealType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ConchelloIntensityPenalizationImageFilter,
               UnaryFunctorImageFilter);

  itkSetMacro( Lambda, double );
  itkGetConstReferenceMacro( Lambda, double);

  /** Print internal ivars */
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Process to execute before entering the multithreaded section */
  void BeforeThreadedGenerateData(void);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck,
                  (Concept::HasNumericTraits<InputPixelType>));
  /** End concept checking */
#endif

protected:
  ConchelloIntensityPenalizationImageFilter();
  virtual ~ConchelloIntensityPenalizationImageFilter() {};

private:
  ConchelloIntensityPenalizationImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  double        m_Lambda;
};



} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkConchelloIntensityPenalizationImageFilter.txx"
#endif

#endif
