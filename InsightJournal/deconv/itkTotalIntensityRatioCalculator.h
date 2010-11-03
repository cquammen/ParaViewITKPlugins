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
#ifndef __itkTotalIntensityRatioCalculator_h
#define __itkTotalIntensityRatioCalculator_h

#include "itkMacro.h"
#include "itkImage.h"

namespace itk
{

/** \class TotalIntensityRatioCalculator
 * \brief Compute the PSNR of a noisy image
 *
 * \ingroup Operators
 */
template < class TInputImage >
class ITK_EXPORT TotalIntensityRatioCalculator : public Object
{
public:
  /** Standard class typedefs. */
  typedef TotalIntensityRatioCalculator Self;
  typedef Object                             Superclass;
  typedef SmartPointer<Self>                 Pointer;
  typedef SmartPointer<const Self>           ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TotalIntensityRatioCalculator, Object);

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
  virtual void SetImage( const InputImageType * image )
    {
    if ( m_Image != image )
      {
      m_Image = image;
      this->Modified();
      m_Valid = false;
      }
    }

  virtual void SetNewImage( const InputImageType * image )
    {
    if ( m_NewImage != image )
      {
      m_NewImage = image;
      this->Modified();
      m_Valid = false;
      }
    }

  void Compute( void );

  const double & GetOutput() const;

protected:
  TotalIntensityRatioCalculator();
  virtual ~TotalIntensityRatioCalculator() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  TotalIntensityRatioCalculator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool           m_Valid;                      // Have moments been computed yet?
  double         m_Output;

  InputImageConstPointer    m_Image;
  InputImageConstPointer    m_NewImage;

};  // class TotalIntensityRatioCalculator

} // end namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkTotalIntensityRatioCalculator.txx"
#endif

#endif /* __itkTotalIntensityRatioCalculator_h */
