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
#ifndef __itkMaximumAPosterioriLeastSquaresDeconvolutionImageFilter_h
#define __itkMaximumAPosterioriLeastSquaresDeconvolutionImageFilter_h

#include "itkFFTConvolutionImageFilterBase.h"
#include "itkConceptChecking.h"

namespace itk {

namespace Functor {

template< class TRegion, class TComplex>
class MaximumAPosterioriLeastSquaresDeconvolution
{
public:
  typedef typename TComplex::value_type  RealType;
  typedef TRegion                        RegionType;
  typedef typename RegionType::IndexType IndexType;
  typedef typename RegionType::SizeType  SizeType;

  MaximumAPosterioriLeastSquaresDeconvolution()
   {
   m_Alpha = NumericTraits< RealType >::min();
   };
  ~MaximumAPosterioriLeastSquaresDeconvolution() {};
  bool operator!=( const MaximumAPosterioriLeastSquaresDeconvolution & other ) const
    {
    return !(*this == other);
    }
  bool operator==( const MaximumAPosterioriLeastSquaresDeconvolution & other ) const
    {
    return true;
    }
  inline TComplex operator()(const IndexType & idx, const TComplex & input, const TComplex & psf )
    {
    const IndexType & idx0 = m_Region.GetIndex();
    const SizeType & size = m_Region.GetSize();
    RealType nw = 0;
    for( unsigned int i=0; i<IndexType::GetIndexDimension(); i++ )
      {
      RealType w = itk::Math::pi * ( idx[i] - idx0[i] ) / (RealType)size[i];
      if( w > itk::Math::pi ) w = 2*itk::Math::pi - w;
      nw += w * w;
      }
    return input * std::conj(psf) / ( std::norm(psf) + 2 * m_Alpha * nw );
    }
  RealType m_Alpha;
  RegionType m_Region;
};
}

/** \class MaximumAPosterioriLeastSquaresDeconvolutionImageFilter
 * \brief
 *
 *
 * \author Gaetan Lehmann
 *
 * \sa FFTShiftImageFilter NormalizeToConstantImageFilter FFTRealToComplexConjugateImageFilter
 */
template<class TInputImage, class TPointSpreadFunction=TInputImage, class TOutputImage=TInputImage, class TInternalPrecision=float>
class ITK_EXPORT MaximumAPosterioriLeastSquaresDeconvolutionImageFilter :
    public FFTConvolutionImageFilterBase<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
{
public:
  /** Standard class typedefs. */
  typedef MaximumAPosterioriLeastSquaresDeconvolutionImageFilter Self;

  typedef FFTConvolutionImageFilterBase<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision> Superclass;

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

  typedef typename Superclass::FFTFilterType       FFTFilterType;
  typedef typename Superclass::IFFTFilterType      IFFTFilterType;
  typedef typename Superclass::ComplexImageType    ComplexImageType;
  typedef typename ComplexImageType::Pointer       ComplexImagePointerType;
  typedef typename ComplexImageType::PixelType     ComplexType;

  typedef typename Superclass::InternalImageType   InternalImageType;
  typedef typename InternalImageType::Pointer      InternalImagePointerType;

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
  itkTypeMacro(MaximumAPosterioriLeastSquaresDeconvolutionImageFilter, ImageToImageFilter);

  /**
   * Set/Get the smoothing parameter. Usual values are in the range 0.001-0.1.
   * Higher value produce smoother results.
   * Defaults to 0.001.
   */
  itkGetConstMacro(Alpha, InternalPrecisionType);
  itkSetMacro(Alpha, InternalPrecisionType);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasPixelTraitsCheck,
    (Concept::HasPixelTraits<InputImagePixelType>));
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<InputImagePixelType>));
  /** End concept checking */
#endif


protected:
  MaximumAPosterioriLeastSquaresDeconvolutionImageFilter();
  ~MaximumAPosterioriLeastSquaresDeconvolutionImageFilter() {};

  /** Single-threaded version of GenerateData.  This filter delegates
   * to other filters. */
  void GenerateData();

  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  MaximumAPosterioriLeastSquaresDeconvolutionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InternalPrecisionType m_Alpha;

}; // end of class

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMaximumAPosterioriLeastSquaresDeconvolutionImageFilter.txx"
#endif

#endif
