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
#ifndef __itkIterativeDeconvolutionImageFilter_h
#define __itkIterativeDeconvolutionImageFilter_h

#include "itkFFTConvolutionImageFilterBase.h"
#include "itkConceptChecking.h"
#include "itkRelativeChangeCalculator.h"

namespace itk {

/** \class IterativeDeconvolutionImageFilter
 * \brief
 *
 *
 * \author Gaetan Lehmann
 *
 * \sa FFTShiftImageFilter NormalizeToConstantImageFilter FFTRealToComplexConjugateImageFilter
 */
template<class TInputImage, class TPointSpreadFunction=TInputImage, class TOutputImage=TInputImage, class TInternalPrecision=float>
class ITK_EXPORT IterativeDeconvolutionImageFilter :
    public FFTConvolutionImageFilterBase<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
{
public:
  /** Standard class typedefs. */
  typedef IterativeDeconvolutionImageFilter Self;

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

  typedef typename Superclass::InternalImageType                                   InternalImageType;
  typedef typename Superclass::InternalImagePointerType                            InternalImagePointerType;
  typedef typename Superclass::InternalImageConstPointerType                       InternalImageConstPointerType;
  typedef typename Superclass::BaseImageType                                       BaseImageType;

  typedef typename itk::ImageToImageFilter< InternalImageType, InternalImageType > InternalFilterType;
  typedef RelativeChangeCalculator< InternalImageType >                            ChangeType;

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
  itkTypeMacro(IterativeDeconvolutionImageFilter, FFTConvolutionImageFilterBase);

  /**
   * Set/Get the number of iterations to run. If RelativeChangeThreshold is set to a value
   * greater than zero, then this parameter is the maximum number of iterations which can
   * be exceeded even if the RelativeChangeThreshold has not been reached.
   * Defaults to 10.
   */
  itkGetConstMacro(NumberOfIterations, int);
  itkSetMacro(NumberOfIterations, int);

  /**
   * Set/Get the relative change threshold between two iterations to stop the iteration.
   * A value lower or equal to 0 mean that this feature is not used. A usual value is
   * between 10^-3 and 10^-5.
   * Defaults to 0 (not used).
   */
  itkGetConstMacro(RelativeChangeThreshold, double);
  itkSetMacro(RelativeChangeThreshold, double);

  /**
   * Set/Get the smoothing which filter is applied at the end of an iteration, each
   * SmoothingPeriod iterations. Default is NULL (no smoothing).
   */
  itkGetConstObjectMacro(SmoothingFilter, InternalFilterType);
  itkGetObjectMacro(SmoothingFilter, InternalFilterType);
  itkSetObjectMacro(SmoothingFilter, InternalFilterType);

  /**
   * Set/Get how often a smoothing is applied with the SmoothingFilter.
   * By default, the smoothing is applied every iterations.
   */
  itkGetConstMacro(SmoothingPeriod, int);
  itkSetMacro(SmoothingPeriod, int);

  itkGetConstMacro(Iteration, int);
  itkGetConstMacro(RelativeChange, double);
  itkGetConstObjectMacro(Estimate, InternalImageType);

  /** Set the first estimate image */
  void SetFirstEstimate(const BaseImageType *input)
    {
    // Process object is not const-correct so the const casting is required.
    this->ProcessObject::SetNthInput( 2, const_cast<BaseImageType *>(input) );
    }

  /** Get the first estimate image */
  const BaseImageType * GetFirstEstimate() const
    {
    return static_cast<BaseImageType*>(
      const_cast<DataObject *>(this->ProcessObject::GetInput(2)));
    }

  /** Set the first estimate image */
  void SetInput2(const BaseImageType *input)
    {
    this->SetFirstEstimate( input );
    }

  /** Set the first estimate image */
  void SetStopIteration(bool v)
    {
    // don't call modified so it can be called during the update
    // without triggering a new update
    m_StopIteration = v;
    }
  itkGetConstMacro(StopIteration, bool);


#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasPixelTraitsCheck,
    (Concept::HasPixelTraits<InputImagePixelType>));
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<InputImagePixelType>));
  /** End concept checking */
#endif


protected:
  IterativeDeconvolutionImageFilter();
  ~IterativeDeconvolutionImageFilter() {};

  void PrintSelf(std::ostream& os, Indent indent) const;
  void GenerateInputRequestedRegion();
  virtual void GenerateData();

  virtual void Iterate();

  virtual void Init();
  virtual void BeforeIteration() {};
  virtual void SetEstimate( InternalImageType * estimate )
    {
    m_Estimate = estimate;
    m_RelativeChangeCalculator->SetImage( estimate );
    }
  virtual typename InternalImageType::Pointer NewEstimate()
    {
    itkExceptionMacro(<< "GetNewEstimate() is not implemeted - please implement it.");
    }
  virtual void AfterIteration() {};
  virtual void End();

  void PrepareInputs( InternalImagePointerType & paddedInput, InternalImagePointerType & firstEstimate,
                      ComplexImagePointerType & paddedKernel );

private:
  IterativeDeconvolutionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  int                                        m_NumberOfIterations;
  double                                     m_RelativeChangeThreshold;
  typename InternalFilterType::Pointer       m_SmoothingFilter;
  int                                        m_Iteration;
  double                                     m_RelativeChange;
  int                                        m_SmoothingPeriod;
  // internal only members
  typename ChangeType::Pointer               m_RelativeChangeCalculator;
  typename InternalImageType::Pointer        m_Estimate;
  bool                                       m_StopIteration;

}; // end of class

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkIterativeDeconvolutionImageFilter.txx"
#endif

#endif
