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
#ifndef __itkVanCittertDeconvolutionImageFilter_h
#define __itkVanCittertDeconvolutionImageFilter_h

#include "itkIterativeDeconvolutionImageFilter.h"
#include "itkConceptChecking.h"

namespace itk {

namespace Functor {

template< class TReal>
class VanCittert
{
public:
  VanCittert() {};
  ~VanCittert() {};
  bool operator!=( const VanCittert & other ) const
    {
    return !(*this == other);
    }
  bool operator==( const VanCittert & other ) const
    {
    return true;
    }
  inline TReal operator()( const TReal & residual, const TReal & img )
    {
    TReal res = residual * m_Alpha + img;
    if( m_NonNegativity && res < 0 )
      {
      res = 0;
      }
    return res;
    }
  TReal m_Alpha;
  bool  m_NonNegativity;
};

}

/** \class VanCittertDeconvolutionImageFilter
 * \brief
 *
 *
 * \author Gaetan Lehmann
 *
 * \sa FFTShiftImageFilter NormalizeToConstantImageFilter FFTRealToComplexConjugateImageFilter
 */
template<class TInputImage, class TPointSpreadFunction=TInputImage, class TOutputImage=TInputImage, class TInternalPrecision=float >
class ITK_EXPORT VanCittertDeconvolutionImageFilter :
    public IterativeDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
{
public:
  /** Standard class typedefs. */
  typedef VanCittertDeconvolutionImageFilter Self;

  typedef IterativeDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>  Superclass;

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
  itkTypeMacro(VanCittertDeconvolutionImageFilter, IterativeDeconvolutionImageFilter);

  /**
   * Set/Get the convergence parameter. Defaults to 1.
   */
  itkGetConstMacro(Alpha, InternalPrecisionType);
  itkSetMacro(Alpha, InternalPrecisionType);

  /**
   * Set/Get wether the filter should enforce the non negativity of the image.
   * Defaults to true.
   */
  itkGetConstMacro(NonNegativity, bool);
  itkSetMacro(NonNegativity, bool);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasPixelTraitsCheck,
    (Concept::HasPixelTraits<InputImagePixelType>));
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<InputImagePixelType>));
  /** End concept checking */
#endif


protected:
  VanCittertDeconvolutionImageFilter();
  ~VanCittertDeconvolutionImageFilter() {};

  virtual void Init();
  virtual void SetEstimate( InternalImageType * estimate );
  virtual typename InternalImageType::Pointer NewEstimate();
  virtual void End();

  typename InternalFilterType::Pointer m_Convolution;
  typename InternalFilterType::Pointer m_Subtract;
  typename InternalFilterType::Pointer m_Add;

  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  VanCittertDeconvolutionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InternalPrecisionType                      m_Alpha;
  bool                                       m_NonNegativity;

}; // end of class

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVanCittertDeconvolutionImageFilter.txx"
#endif

#endif
