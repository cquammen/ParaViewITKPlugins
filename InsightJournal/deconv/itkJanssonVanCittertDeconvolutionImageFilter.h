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
#ifndef __itkJanssonVanCittertDeconvolutionImageFilter_h
#define __itkJanssonVanCittertDeconvolutionImageFilter_h

#include "itkVanCittertDeconvolutionImageFilter.h"
#include "itkConceptChecking.h"

namespace itk {

namespace Functor {

template< class TReal>
class JanssonVanCittert
{
public:
  JanssonVanCittert() {};
  ~JanssonVanCittert() {};
  bool operator!=( const JanssonVanCittert & other ) const
    {
    return !(*this == other);
    }
  bool operator==( const JanssonVanCittert & other ) const
    {
    return true;
    }
  inline TReal operator()( const TReal & residual, const TReal & img )
    {
    TReal gamma = m_Alpha * ( 1 - std::abs( img - m_B2 ) / m_B2 );
    TReal result = img + gamma * residual;
    if( m_NonNegativity && result < 0 )
      {
      result = 0;
      }
    return result;
    }
  TReal m_B2;
  TReal m_Alpha;
  bool  m_NonNegativity;
};

}

/** \class JanssonVanCittertDeconvolutionImageFilter
 * \brief
 *
 *
 * \author Gaetan Lehmann
 *
 * \sa FFTShiftImageFilter NormalizeToConstantImageFilter FFTRealToComplexConjugateImageFilter
 */
template<class TInputImage, class TPointSpreadFunction=TInputImage, class TOutputImage=TInputImage, class TInternalPrecision=float>
class ITK_EXPORT JanssonVanCittertDeconvolutionImageFilter :
    public VanCittertDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision >
{
public:
  /** Standard class typedefs. */
  typedef JanssonVanCittertDeconvolutionImageFilter Self;

  typedef VanCittertDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision > Superclass;

  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef TPointSpreadFunction                     PointSpreadFunctionType;
  typedef TOutputImage                             OutputImageType;
  typedef TInternalPrecision                       InternalPrecisionType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename PointSpreadFunctionType::Pointer        PointSpreadFunctionPointer;
  typedef typename PointSpreadFunctionType::ConstPointer   PointSpreadFunctionConstPointer;
  typedef typename PointSpreadFunctionType::PixelType      PointSpreadFunctionPixelType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  typedef typename InputImageType::RegionType      RegionType;
  typedef typename InputImageType::IndexType       IndexType;
  typedef typename InputImageType::SizeType        SizeType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  typedef typename itk::Image< InternalPrecisionType, ImageDimension > InternalImageType;
  typedef typename itk::ImageToImageFilter< InternalImageType, InternalImageType > InternalFilterType;

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(JanssonVanCittertDeconvolutionImageFilter, ImageToImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasPixelTraitsCheck,
    (Concept::HasPixelTraits<InputImagePixelType>));
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<InputImagePixelType>));
  /** End concept checking */
#endif


protected:
  JanssonVanCittertDeconvolutionImageFilter() {};
  ~JanssonVanCittertDeconvolutionImageFilter() {};

  virtual void Init();

private:
  JanssonVanCittertDeconvolutionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
}; // end of class

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkJanssonVanCittertDeconvolutionImageFilter.txx"
#endif

#endif
