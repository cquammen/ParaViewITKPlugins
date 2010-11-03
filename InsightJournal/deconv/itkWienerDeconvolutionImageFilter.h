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
#ifndef __itkWienerDeconvolutionImageFilter_h
#define __itkWienerDeconvolutionImageFilter_h

#include "itkFFTConvolutionImageFilterBase.h"
#include "itkConceptChecking.h"

namespace itk {

namespace Functor {

template< class TComplex>
class WienerDeconvolution
{
public:
  typedef typename TComplex::value_type RealType;
  WienerDeconvolution()
   {
   m_Gamma = NumericTraits< RealType >::min();
   };
  ~WienerDeconvolution() {};
  bool operator!=( const WienerDeconvolution & other ) const
    {
    return !(*this == other);
    }
  bool operator==( const WienerDeconvolution & other ) const
    {
    return true;
    }
  inline TComplex operator()( const TComplex & input, const TComplex & psf )
    {
    return input * std::conj(psf) / ( std::norm(psf) + m_Gamma );
    }
  RealType m_Gamma;
};
}

/** \class WienerDeconvolutionImageFilter
 * \brief
 *
 *
 * \author Gaetan Lehmann
 *
 * \sa FFTShiftImageFilter NormalizeToConstantImageFilter FFTRealToComplexConjugateImageFilter
 */
template<class TInputImage, class TPointSpreadFunction=TInputImage, class TOutputImage=TInputImage, class TInternalPrecision=float>
class ITK_EXPORT WienerDeconvolutionImageFilter :
    public FFTConvolutionImageFilterBase<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
{
public:
  /** Standard class typedefs. */
  typedef WienerDeconvolutionImageFilter Self;

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

  typedef typename Superclass::ComplexImageType    ComplexImageType;
  typedef typename ComplexImageType::Pointer       ComplexImagePointerType;
  typedef typename ComplexImageType::PixelType     ComplexType;

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
  itkTypeMacro(WienerDeconvolutionImageFilter, ImageToImageFilter);

  /**
   * Set/Get the smoothing parameter. Usual values are in the range 0.001-0.1.
   * Higher value produce smoother results.
   * Defaults to 0.001.
   */
  itkGetConstMacro(Gamma, InternalPrecisionType);
  itkSetMacro(Gamma, InternalPrecisionType);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasPixelTraitsCheck,
    (Concept::HasPixelTraits<InputImagePixelType>));
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<InputImagePixelType>));
  /** End concept checking */
#endif


protected:
  WienerDeconvolutionImageFilter();
  ~WienerDeconvolutionImageFilter() {};

  /** Single-threaded version of GenerateData.  This filter delegates
   * to other filters. */
  void GenerateData();

  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  WienerDeconvolutionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InternalPrecisionType m_Gamma;

}; // end of class

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkWienerDeconvolutionImageFilter.txx"
#endif

#endif
