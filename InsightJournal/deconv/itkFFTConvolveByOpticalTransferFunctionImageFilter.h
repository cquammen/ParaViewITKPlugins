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
#ifndef __itkFFTConvolveByOpticalTransferFunctionImageFilter_h
#define __itkFFTConvolveByOpticalTransferFunctionImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkConceptChecking.h"
#include "itkFFTRealToComplexConjugateImageFilter.h"
#include "itkFFTComplexConjugateToRealImageFilter.h"
#include "itkProgressAccumulator.h"

namespace itk {

/** \class FFTConvolveByOpticalTransferFunctionImageFilter
 * \brief
 *
 * \author Gaetan Lehmann
 *
 * \sa FFTShiftImageFilter NormalizeToConstantImageFilter FFTRealToComplexConjugateImageFilter
 */
template<class TPixel=float, unsigned int NDimension = 3 >
class ITK_EXPORT FFTConvolveByOpticalTransferFunctionImageFilter :
    public ImageToImageFilter< Image< TPixel, NDimension >,
                               Image< TPixel, NDimension > >
{
public:
  /** Standard class typedefs. */
  typedef FFTConvolveByOpticalTransferFunctionImageFilter Self;

  typedef Image< TPixel, NDimension >                   InputImageType;
  typedef InputImageType                                OutputImageType;
  typedef Image< std::complex< TPixel > , NDimension >  OpticalTransferFunctionType;

  typedef ImageToImageFilter<InputImageType, OutputImageType> Superclass;

  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TPixel                       PixelType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename OpticalTransferFunctionType::Pointer        OpticalTransferFunctionPointer;
  typedef typename OpticalTransferFunctionType::ConstPointer   OpticalTransferFunctionConstPointer;
  typedef typename OpticalTransferFunctionType::PixelType      OpticalTransferFunctionPixelType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  typedef typename InputImageType::RegionType      RegionType;
  typedef typename InputImageType::IndexType       IndexType;
  typedef typename InputImageType::SizeType        SizeType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      NDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      NDimension);
  itkStaticConstMacro(ImageDimension, unsigned int,
                      NDimension);

  typedef itk::FFTRealToComplexConjugateImageFilter< PixelType, ImageDimension > FFTFilterType;
  typedef itk::FFTComplexConjugateToRealImageFilter< PixelType, ImageDimension > IFFTFilterType;
  typedef typename OpticalTransferFunctionType::PixelType                                    ComplexType;

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(FFTConvolveByOpticalTransferFunctionImageFilter, ImageToImageFilter);

   /** Set the kernel image */
  void SetOpticalTransferFunction(const OpticalTransferFunctionType *input)
    {
    // Process object is not const-correct so the const casting is required.
    this->SetNthInput( 1, const_cast<OpticalTransferFunctionType *>(input) );
    }

  /** Get the kernel image */
  const OpticalTransferFunctionType * GetOpticalTransferFunction() const
    {
    return static_cast<OpticalTransferFunctionType*>(
      const_cast<DataObject *>(this->ProcessObject::GetInput(1)));
    }

  /** Set the input image */
  void SetInput1(const InputImageType *input)
    {
    this->SetInput( input );
    }

  /** Set the kernel image */
  void SetInput2(const OpticalTransferFunctionType *input)
    {
    this->SetOpticalTransferFunction( input );
    }

  itkGetConstMacro(Transpose, bool);
  itkSetMacro(Transpose, bool);
  itkBooleanMacro(Transpose);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasPixelTraitsCheck,
    (Concept::HasPixelTraits<InputImagePixelType>));
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<InputImagePixelType>));
  /** End concept checking */
#endif


protected:
  FFTConvolveByOpticalTransferFunctionImageFilter();
  ~FFTConvolveByOpticalTransferFunctionImageFilter() {};

  void GenerateInputRequestedRegion();
  void GenerateData();
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  FFTConvolveByOpticalTransferFunctionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool                         m_Transpose;

}; // end of class

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFFTConvolveByOpticalTransferFunctionImageFilter.txx"
#endif

#endif
