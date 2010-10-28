/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkNormalizeToConstantImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNormalizeToConstantImageFilter_h
#define __itkNormalizeToConstantImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkConceptChecking.h"

namespace itk {

/** \class NormalizeToConstantImageFilter
 *
 * \brief Scales the pixel values to make their sum equal a user defined constant.
 *
 * The default value of the constant is 1. It can be changed with SetConstant().
 *
 * This transform is especially useful to normalize to 1 a kernel
 * image in order to perform a convolution that preserves the
 * integrated sum of intensities from the original image.
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa NormalizeImageFilter
 * \sa StatisticsImageFilter
 * \sa DivideByConstantImageFilter
 * \sa ShiftScaleImageFilter
 *
 */
template<class TInputImage, class TOutputImage>
class ITK_EXPORT NormalizeToConstantImageFilter :
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef NormalizeToConstantImageFilter Self;

  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;

  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef TOutputImage                             OutputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  typedef typename InputImageType::RegionType      RegionType;
  typedef typename InputImageType::IndexType       IndexType;
  typedef typename InputImageType::SizeType        SizeType;

  typedef typename NumericTraits<InputImagePixelType>::RealType RealType;

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
  itkTypeMacro(NormalizeToConstantImageFilter, ImageToImageFilter);

  /** Set/get the normalization constant. */
  itkSetMacro(Constant, RealType);
  itkGetConstMacro(Constant, RealType);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasPixelTraitsCheck,
    (Concept::HasPixelTraits<InputImagePixelType>));
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<InputImagePixelType>));
  /** End concept checking */
#endif


protected:
  NormalizeToConstantImageFilter();
  ~NormalizeToConstantImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  void GenerateInputRequestedRegion();

  /** Single-threaded version of GenerateData.  This filter delegates
   * to other filters. */
  void GenerateData();


private:
  NormalizeToConstantImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  RealType m_Constant;

}; // end of class

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkNormalizeToConstantImageFilter.txx"
#endif

#endif
