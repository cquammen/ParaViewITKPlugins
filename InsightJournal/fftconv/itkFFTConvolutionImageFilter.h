/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFFTConvolutionImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFFTConvolutionImageFilter_h
#define __itkFFTConvolutionImageFilter_h

#include "itkFFTConvolutionImageFilterBase.h"
#include "itkConceptChecking.h"

namespace itk {

/** \class FFTConvolutionImageFilter
 *
 * \brief Convolves an image by a kernel via multiplication in the
 * Fourier domain.
 *
 * This filter requires two input images. The first input is the image
 * to convolve and the second input is the convolution kernel.
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa FFTShiftImageFilter
 * \sa NormalizeToConstantImageFilter
 * \sa FFTRealToComplexConjugateImageFilter
 */
template<class TInputImage, class TKernelImage=TInputImage, class TOutputImage=TInputImage, class TInternalPrecision=float>
class ITK_EXPORT FFTConvolutionImageFilter :
    public FFTConvolutionImageFilterBase<TInputImage, TKernelImage, TOutputImage, TInternalPrecision>
{
public:
  /** Standard class typedefs. */
  typedef FFTConvolutionImageFilter Self;

  typedef FFTConvolutionImageFilterBase<TInputImage, TKernelImage, TOutputImage, TInternalPrecision> Superclass;

  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef TKernelImage                             KernelImageType;
  typedef TOutputImage                             OutputImageType;
  typedef TInternalPrecision                            InternalPrecisionType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename KernelImageType::Pointer        KernelImagePointer;
  typedef typename KernelImageType::ConstPointer   KernelImageConstPointer;
  typedef typename KernelImageType::PixelType      KernelImagePixelType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  typedef typename InputImageType::RegionType      RegionType;
  typedef typename InputImageType::IndexType       IndexType;
  typedef typename InputImageType::SizeType        SizeType;

  typedef typename Superclass::ComplexImageType    ComplexImageType;
  typedef typename ComplexImageType::Pointer       ComplexImagePointerType;

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
  itkTypeMacro(FFTConvolutionImageFilter, FFTConvolutionImageFilterBase);


#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasPixelTraitsCheck,
    (Concept::HasPixelTraits<InputImagePixelType>));
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<InputImagePixelType>));
  /** End concept checking */
#endif


protected:
  FFTConvolutionImageFilter() {};
  ~FFTConvolutionImageFilter() {};

  /** Single-threaded version of GenerateData.  This filter delegates
   * to other filters. */
  void GenerateData();

private:
  FFTConvolutionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

}; // end of class

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFFTConvolutionImageFilter.txx"
#endif

#endif
