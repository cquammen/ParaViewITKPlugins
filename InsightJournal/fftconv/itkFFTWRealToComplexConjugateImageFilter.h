/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFFTWRealToComplexConjugateImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFFTWRealToComplexConjugateImageFilter_h
#define __itkFFTWRealToComplexConjugateImageFilter_h

#include "itkFFTRealToComplexConjugateImageFilter.h"
#include "itkFFTWCommon.h"


namespace itk
{
/** \class FFTWRealToComplexConjugateImageFilter
 * \brief
 *
 * \ingroup
 */

template <class TPixel, unsigned int VDimension = 3>
class ITK_EXPORT FFTWRealToComplexConjugateImageFilter :
    public FFTRealToComplexConjugateImageFilter<TPixel,VDimension>
{
public:
  typedef FFTWRealToComplexConjugateImageFilter                   Self;
  typedef FFTRealToComplexConjugateImageFilter<TPixel,VDimension> Superclass;
  typedef SmartPointer<Self>                                      Pointer;
  typedef SmartPointer<const Self>                                ConstPointer;

  /** Standard class typedefs. */
  typedef typename Superclass::TInputImageType  TInputImageType;
  typedef typename Superclass::TOutputImageType TOutputImageType;

  /**
   * the proxy type is a wrapper for the FFTW API
   * since the proxy is only defined over double and float,
   * trying to use any other pixel type is inoperative, as
   * is trying to use double if only the float FFTW version is
   * configured in, or float if only double is configured.
   */
  typedef typename fftw::Proxy<TPixel> FFTWProxyType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FFTWRealToComplexConjugateImageFilter,
               FFTRealToComplexConjugateImageFilter);

  //
  // these should be defined in every FFT filter class
  virtual void GenerateData();  // generates output from input

protected:
  FFTWRealToComplexConjugateImageFilter()
    {
    }
  ~FFTWRealToComplexConjugateImageFilter()
    {
    }

  virtual bool FullMatrix();

  virtual void UpdateOutputData(DataObject *output);

private:
  FFTWRealToComplexConjugateImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_CanUseDestructiveAlgorithm;

};
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFFTWRealToComplexConjugateImageFilter.txx"
#endif

#endif //__itkFFTWRealToComplexConjugateImageFilter_h
