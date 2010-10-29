/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkExtractReferenceImageRegionImageFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkExtractReferenceImageRegionImageFilter_txx
#define __itkExtractReferenceImageRegionImageFilter_txx

#include "itkExtractReferenceImageRegionImageFilter.h"

namespace itk
{

template <class TInputImage, class TOutputImage>
void
ExtractReferenceImageRegionImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  if( !this->GetInput() || !this->GetReferenceImage() )
    {
    return;
    }

  // Superclass::Superclass::GenerateOutputInformation();
  this->SetExtractionRegion( this->GetReferenceImage()->GetLargestPossibleRegion() );
  Superclass::GenerateOutputInformation();
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void
ExtractReferenceImageRegionImageFilter<TInputImage,TOutputImage>
::SetReferenceImage ( const ReferenceImageType *image )
{
  itkDebugMacro("setting input ReferenceImage to " << image);
  if( image != static_cast<const ReferenceImageType *>(this->GetInput( 1 )) )
    {
    this->ProcessObject::SetNthInput(1, const_cast< ReferenceImageType *>( image ) );
    this->Modified();
    }
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
const typename ExtractReferenceImageRegionImageFilter<TInputImage,TOutputImage>::ReferenceImageType *
ExtractReferenceImageRegionImageFilter<TInputImage,TOutputImage>
::GetReferenceImage() const
{
  Self * surrogate = const_cast< Self * >( this );

  const DataObject * input = surrogate->ProcessObject::GetInput(1);

  const ReferenceImageType * referenceImage = static_cast<const ReferenceImageType *>( input );

  return referenceImage;
}

} // end namespace itk

#endif
