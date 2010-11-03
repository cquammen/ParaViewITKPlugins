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
#ifndef __itkMultiplyByComplexConjugateImageFilter_h
#define __itkMultiplyByComplexConjugateImageFilter_h

#include "itkBinaryFunctorImageFilter.h"

namespace itk
{

/** \class MultiplyByComplexConjugateImageFilter
 * \brief Implements an operator for pixel-wise multiplication of the first image by the complex
 * conjugate of the second.
 *
 * Pixel types must be complex.
 *
 * \ingroup IntensityImageFilters  Multithreaded
 */
namespace Functor {

template< class TComplex>
class MultiplyByComplexConjugate
{
public:
  MultiplyByComplexConjugate() {};
  ~MultiplyByComplexConjugate() {};
  bool operator!=( const MultiplyByComplexConjugate & other ) const
    {
    return !(*this == other);
    }
  bool operator==( const MultiplyByComplexConjugate & other ) const
    {
    return true;
    }
  inline TComplex operator()( const TComplex & c1, const TComplex & c2 )
    {
    return c1 * std::conj(c2);
    }
};
}

template <class TComplexImage>
class ITK_EXPORT MultiplyByComplexConjugateImageFilter :
    public
BinaryFunctorImageFilter<TComplexImage,TComplexImage,TComplexImage,
                         Functor::MultiplyByComplexConjugate< typename TComplexImage::PixelType > >
{
public:
  /** Standard class typedefs. */
  typedef MultiplyByComplexConjugateImageFilter       Self;
  typedef BinaryFunctorImageFilter<TComplexImage,TComplexImage,TComplexImage,
                         Functor::MultiplyByComplexConjugate< typename TComplexImage::PixelType> >
                                    Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(MultiplyByComplexConjugateImageFilter,
               BinaryFunctorImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
#endif

protected:
  MultiplyByComplexConjugateImageFilter() {}
  virtual ~MultiplyByComplexConjugateImageFilter() {}

private:
  MultiplyByComplexConjugateImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
