/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkClampImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkClampImageFilter_h
#define __itkClampImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkProgressReporter.h"

namespace itk
{

/** \class ClampImageFilter
 *
 * \brief Clamps input pixels to output pixel type.
 *
 * This filter is templated over the input image type
 * and the output image type.
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \ingroup IntensityImageFilters  Multithreaded
 * \sa UnaryFunctorImageFilter
 * \sa CastImageFilter
 */
namespace Functor {

template< class TInput, class TOutput>
class Clamp
{
public:
  Clamp() {};
  virtual ~Clamp() {};
  bool operator!=( const Clamp & ) const
    {
    return false;
    }
  bool operator==( const Clamp & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInput & A ) const
    {
    if( (double)A > (double)NumericTraits<TOutput>::max() )
      {
      return NumericTraits<TOutput>::max();
      }
    if( (double)A < (double)NumericTraits<TOutput>::NonpositiveMin() )
      {
      return NumericTraits<TOutput>::NonpositiveMin();
      }
    return static_cast<TOutput>( A );
    }
};
}

template <class TInputImage, class TOutputImage>
class ITK_EXPORT ClampImageFilter :
    public
UnaryFunctorImageFilter<TInputImage,TOutputImage,
                        Functor::Clamp<
  typename TInputImage::PixelType,
  typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef ClampImageFilter               Self;
  typedef UnaryFunctorImageFilter<TInputImage,TOutputImage,
                        Functor::Clamp<
                          typename TInputImage::PixelType,
                          typename TOutputImage::PixelType>
                                     >  Superclass;
  typedef SmartPointer<Self>            Pointer;
  typedef SmartPointer<const Self>      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ClampImageFilter, UnaryFunctorImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputConvertibleToOutputCheck,
    (Concept::Convertible<typename TInputImage::PixelType,
                          typename TOutputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  ClampImageFilter() {}
  virtual ~ClampImageFilter() {}

  void GenerateData()
    {
    if( this->GetInPlace() && this->CanRunInPlace() )
      {
      // nothing to do, so avoid iterating over all the pixels
      // for nothing! Allocate the output, generate a fake progress and exit
      this->AllocateOutputs();
      ProgressReporter progress(this, 0, 1);
      return;
      }
    Superclass::GenerateData();
    }


private:
  ClampImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};


} // end namespace itk

#endif
