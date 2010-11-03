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
#ifndef __itkDampedRichardsonLucyDeconvolutionImageFilter_h
#define __itkDampedRichardsonLucyDeconvolutionImageFilter_h

#include "itkRichardsonLucyDeconvolutionImageFilter.h"
#include "itkConceptChecking.h"

namespace itk {

namespace Functor {

template< class TReal>
class DampedRichardsonLucy
{
public:
  DampedRichardsonLucy() {};
  ~DampedRichardsonLucy() {};
  bool operator!=( const DampedRichardsonLucy & other ) const
    {
    return !(*this == other);
    }
  bool operator==( const DampedRichardsonLucy & other ) const
    {
    return true;
    }
  // arguments are in an unusual order to allow to run in place
  // of the denominator image
  inline TReal operator()( const TReal & o, const TReal & i )
    {
    assert( !std::isnan( o ) );
    assert( !std::isnan( i ) );
    assert( !std::isnan( m_T ) );
    assert( !std::isnan( m_N ) );
    if( o < 1e-5 || i < 1e-5 || m_T < 1e-5 )
      {
      return NumericTraits<TReal>::One;
      }
    TReal r = std::min( -2 / ( m_T * m_T ) * ( i * vcl_log( o / i ) - o + i ),
                         NumericTraits<TReal>::One );
    TReal u = vcl_pow( r, m_N-1 ) * ( m_N - ( m_N - 1 ) * r );
    assert( !std::isnan( 1 + u * ( i - o ) / o ) );
    return 1 + u * ( i - o ) / o;
    }
  TReal m_T;
  TReal m_N;
};

}

/** \class DampedRichardsonLucyDeconvolutionImageFilter
 * \brief
 *
 *
 * \author Gaetan Lehmann
 *
 * \sa FFTShiftImageFilter NormalizeToConstantImageFilter FFTRealToComplexConjugateImageFilter
 */
template<class TInputImage, class TPointSpreadFunction=TInputImage, class TOutputImage=TInputImage, class TInternalPrecision=float>
class ITK_EXPORT DampedRichardsonLucyDeconvolutionImageFilter :
    public RichardsonLucyDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
{
public:
  /** Standard class typedefs. */
  typedef DampedRichardsonLucyDeconvolutionImageFilter Self;

  typedef RichardsonLucyDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>  Superclass;

  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef TPointSpreadFunction                             PointSpreadFunctionType;
  typedef TOutputImage                             OutputImageType;
  typedef TInternalPrecision                            InternalPrecisionType;
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

  typedef typename Superclass::FFTFilterType       FFTFilterType;
  typedef typename Superclass::IFFTFilterType      IFFTFilterType;
  typedef typename Superclass::ComplexImageType    ComplexImageType;
  typedef typename ComplexImageType::Pointer       ComplexImagePointerType;
  typedef typename ComplexImageType::PixelType     ComplexType;

  typedef typename Superclass::InternalImageType   InternalImageType;
  typedef typename InternalImageType::Pointer      InternalImagePointerType;
  typedef typename Superclass::InternalFilterType  InternalFilterType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(DampedRichardsonLucyDeconvolutionImageFilter, RichardsonLucyDeconvolutionImageFilter);

  /**
   * Set/Get the threshold where the damping begins.
   * Defaults to 2.
   */
  itkGetConstMacro(Threshold, InternalPrecisionType);
  itkSetMacro(Threshold, InternalPrecisionType);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasPixelTraitsCheck,
    (Concept::HasPixelTraits<InputImagePixelType>));
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<InputImagePixelType>));
  /** End concept checking */
#endif


protected:
  DampedRichardsonLucyDeconvolutionImageFilter();
  ~DampedRichardsonLucyDeconvolutionImageFilter() {};

  /** Single-threaded version of GenerateData.  This filter delegates
   * to other filters. */
  void Init();

  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  DampedRichardsonLucyDeconvolutionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InternalPrecisionType m_Threshold;

}; // end of class

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkDampedRichardsonLucyDeconvolutionImageFilter.txx"
#endif

#endif
