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
#ifndef __itkImprovementInSignalToNoiseRatioCalculator_h
#define __itkImprovementInSignalToNoiseRatioCalculator_h

#include "itkMacro.h"
#include "itkImage.h"

namespace itk
{

/** \class ImprovementInSignalToNoiseRatioCalculator
 * \brief Compute the PSNR of a noisy image
 *
 * \ingroup Operators
 */
template < class TInputImage >
class ITK_EXPORT ImprovementInSignalToNoiseRatioCalculator : public Object
{
public:
  /** Standard class typedefs. */
  typedef ImprovementInSignalToNoiseRatioCalculator Self;
  typedef Object                             Superclass;
  typedef SmartPointer<Self>                 Pointer;
  typedef SmartPointer<const Self>           ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImprovementInSignalToNoiseRatioCalculator, Object);

  /** Extract the dimension of the image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Standard image type within this class. */
  typedef TInputImage    InputImageType;

  /** Standard image type pointer within this class. */
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::PixelType       InputPixelType;

  /** Set the input image. */
  virtual void SetOriginalImage( const InputImageType * image )
    {
    if ( m_OriginalImage != image )
      {
      m_OriginalImage = image;
      this->Modified();
      m_Valid = false;
      }
    }

  virtual void SetBlurredImage( const InputImageType * image )
    {
    if ( m_BlurredImage != image )
      {
      m_BlurredImage = image;
      this->Modified();
      m_Valid = false;
      }
    }

  virtual void SetRestoredImage( const InputImageType * image )
    {
    if ( m_RestoredImage != image )
      {
      m_RestoredImage = image;
      this->Modified();
      m_Valid = false;
      }
    }

  void Compute( void );

  const double & GetOutput() const;

protected:
  ImprovementInSignalToNoiseRatioCalculator();
  virtual ~ImprovementInSignalToNoiseRatioCalculator() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  ImprovementInSignalToNoiseRatioCalculator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool           m_Valid;                      // Have moments been computed yet?
  double         m_Output;

  InputImageConstPointer    m_OriginalImage;
  InputImageConstPointer    m_BlurredImage;
  InputImageConstPointer    m_RestoredImage;

};  // class ImprovementInSignalToNoiseRatioCalculator

} // end namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImprovementInSignalToNoiseRatioCalculator.txx"
#endif

#endif /* __itkImprovementInSignalToNoiseRatioCalculator_h */
