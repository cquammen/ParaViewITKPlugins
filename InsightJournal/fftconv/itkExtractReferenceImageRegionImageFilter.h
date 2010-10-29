/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkExtractReferenceImageRegionImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkExtractReferenceImageRegionImageFilter_h
#define __itkExtractReferenceImageRegionImageFilter_h

#include "itkExtractImageFilter.h"

namespace itk
{

/** \class ExtractReferenceImageRegionImageFilter
 * \brief Decrease the image size by cropping the image by an itk::Size at
 * both the upper and lower bounds of the largest possible region.
 *
 * ExtractReferenceImageRegionImageFilter changes the image boundary of an image by removing
 * pixels outside the target region.  The target region is not specified in
 * advance, but calculated in BeforeThreadedGenerateData().
 *
 * This filter uses ExtractImageFilter to perform the cropping.
 *
 * \ingroup GeometricTransforms
 */
template <class TInputImage, class TOutputImage=TInputImage>
class ITK_EXPORT ExtractReferenceImageRegionImageFilter:
    public ExtractImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ExtractReferenceImageRegionImageFilter        Self;
  typedef ExtractImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ExtractReferenceImageRegionImageFilter, ExtractImageFilter);

  /** Typedef to describe the output and input image region types. */
  typedef typename Superclass::OutputImageRegionType OutputImageRegionType;
  typedef typename Superclass::InputImageRegionType  InputImageRegionType;

  /** Typedef to describe the type of pixel. */
  typedef typename Superclass::OutputImagePixelType OutputImagePixelType;
  typedef typename Superclass::InputImagePixelType  InputImagePixelType;

  /** Typedef to describe the output and input image index and size types. */
  typedef typename Superclass::OutputImageIndexType OutputImageIndexType;
  typedef typename Superclass::InputImageIndexType  InputImageIndexType;
  typedef typename Superclass::OutputImageSizeType  OutputImageSizeType;
  typedef typename Superclass::InputImageSizeType   InputImageSizeType;
  typedef InputImageSizeType                        SizeType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      Superclass::InputImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      Superclass::OutputImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int,
                      Superclass::OutputImageDimension);

  typedef ImageBase< itkGetStaticConstMacro( ImageDimension ) > ReferenceImageType;

  /** Copy the output information from another Image. */
  void SetReferenceImage ( const ReferenceImageType *image );

  const ReferenceImageType * GetReferenceImage() const;

#if 0
  /** Set the input image */
  void SetInput1(const TInputImage *input)
    {
    this->SetInput( input );
    }

  /** Set the reference image */
  void SetInput2(const ReferenceImageType *input)
    {
    this->SetReferenceImage( input );
    }
#endif


#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputConvertibleToOutputCheck,
    (Concept::Convertible<InputImagePixelType, OutputImagePixelType>));
  itkConceptMacro(SameDimensionCheck,
    (Concept::SameDimension<InputImageDimension, OutputImageDimension>));
  /** End concept checking */
#endif

protected:
  ExtractReferenceImageRegionImageFilter()
    {
    this->SetNumberOfRequiredInputs(2);
    }
  ~ExtractReferenceImageRegionImageFilter() {}

  void GenerateOutputInformation();

private:
  ExtractReferenceImageRegionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};


} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkExtractReferenceImageRegionImageFilter.txx"
#endif

#endif
