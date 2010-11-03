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
#ifndef __itkComplexConjugateImageFilter_h
#define __itkComplexConjugateImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

namespace itk
{

/** \class ComplexConjugateImageFilter
 * \brief Computes pixel-wise the complex conjugate.
 *
 * \ingroup IntensityImageFilters  Multithreaded
 */
namespace Function {

template< class TComplex>
class ComplexConjugate
{
public:
  ComplexConjugate() {}
  ~ComplexConjugate() {}
  bool operator!=( const ComplexConjugate & ) const
    {
    return false;
    }
  bool operator==( const ComplexConjugate & other ) const
    {
    return !(*this != other);
    }
  inline TComplex operator()( const TComplex & A ) const
    {
    return std::conj(A);
    }
};
}

template <class TComplexImage>
class ITK_EXPORT ComplexConjugateImageFilter :
    public
UnaryFunctorImageFilter<TComplexImage,TComplexImage,
                        Function::ComplexConjugate<typename TComplexImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef ComplexConjugateImageFilter Self;
  typedef UnaryFunctorImageFilter<
    TComplexImage,TComplexImage,
    Function::ComplexConjugate< typename TComplexImage::PixelType > >
                                    Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(ComplexConjugateImageFilter,
               UnaryFunctorImageFilter);

  typedef typename TComplexImage::PixelType                     InputPixelType;
  typedef typename TComplexImage::PixelType                     OutputPixelType;
  typedef typename NumericTraits< InputPixelType >::ValueType   InputPixelValueType;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputConvertibleToOutputCheck,
    (Concept::Convertible<InputPixelValueType, OutputPixelType>));
  /** End concept checking */
#endif


protected:
  ComplexConjugateImageFilter() {}
  virtual ~ComplexConjugateImageFilter() {}

private:
  ComplexConjugateImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
