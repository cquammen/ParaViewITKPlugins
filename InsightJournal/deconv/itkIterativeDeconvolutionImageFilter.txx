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
#ifndef __itkIterativeDeconvolutionImageFilter_txx
#define __itkIterativeDeconvolutionImageFilter_txx

#include "itkIterativeDeconvolutionImageFilter.h"

namespace itk {

template <class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
IterativeDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::IterativeDeconvolutionImageFilter()
{
  m_NumberOfIterations = 10;
  m_RelativeChangeThreshold = 0;
  m_SmoothingFilter = NULL;
  m_Iteration = 0;
  m_RelativeChange = 0;
  m_SmoothingPeriod = 1;
  m_RelativeChangeCalculator = NULL;
  m_Estimate = NULL;
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
IterativeDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  BaseImageType * input = dynamic_cast<BaseImageType *>(this->ProcessObject::GetInput(2));
  if ( !input )
    {
    return;
    }
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
IterativeDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::GenerateData()
{
  Init();
  Iterate();
  End();
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
IterativeDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::Init()
{
  m_StopIteration = false;
  m_RelativeChangeCalculator = ChangeType::New();
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
IterativeDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::Iterate()
{
  for( m_Iteration=1; m_Iteration<=this->GetNumberOfIterations(); m_Iteration++ )
    {
    BeforeIteration();
    // should we use smoothing filter? -- tested in the iteration on purpose, to be able to
    // m_RelativeChangeCalculator the filter by looking at the iteration event
    typename InternalImageType::Pointer newEstimate = NewEstimate();
    if( m_SmoothingFilter.IsNotNull() && m_Iteration % m_SmoothingPeriod == 0 )
      {
      m_SmoothingFilter->SetInput( newEstimate );
      newEstimate = m_SmoothingFilter->GetOutput();
      }
    newEstimate->Update();
    newEstimate->DisconnectPipeline();

    // do we have to stop the iterations based on the relative m_RelativeChangeCalculator?
    m_RelativeChangeCalculator->SetNewImage( newEstimate );
    m_RelativeChangeCalculator->Compute();
    // std::cout << m_RelativeChangeCalculator->GetOutput() << std::endl;
    m_RelativeChange = m_RelativeChangeCalculator->GetOutput();
    if( ( m_RelativeChangeThreshold > 0 && m_RelativeChange < m_RelativeChangeThreshold )
        || m_StopIteration )
      {
      break;
      }
    else
      {
      // ok, lets go for another round
      SetEstimate( newEstimate );
      AfterIteration();
      this->UpdateProgress( m_Iteration/(float)this->GetNumberOfIterations() );
      this->InvokeEvent( IterationEvent() );
      }
    }
  // to keep the actual number of iteration in m_Iteration
  m_Iteration--;
  // nothing more to do - just make sure that the data will be released
  m_Estimate->SetReleaseDataFlag( true );
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
IterativeDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::End()
{
  this->ProduceOutput( m_Estimate, 0 );
  this->UpdateProgress( 1.0 );
  // destroy now useless objects
  m_RelativeChangeCalculator = NULL;
  m_Estimate = NULL;
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
IterativeDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::PrepareInputs( InternalImagePointerType & paddedInput, InternalImagePointerType & paddedFirstEstimate,
                 ComplexImagePointerType & paddedKernel )
{
  this->Superclass::PrepareInputs( paddedInput, paddedKernel, 0 );

  // now try to get a first estimate
  InternalImageConstPointerType firstEstimate = this->InternalInput( 2 );
  if( firstEstimate.IsNull() )
    {
    // no usable first estimate provided by the user - simply use the input image
    paddedFirstEstimate = paddedInput;
    return;
    }
  this->PrepareImage( paddedFirstEstimate, firstEstimate, false, false, 0 );
}

template<class TInputImage, class TPointSpreadFunction, class TOutputImage, class TInternalPrecision>
void
IterativeDeconvolutionImageFilter<TInputImage, TPointSpreadFunction, TOutputImage, TInternalPrecision>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "StopIteration: "  << m_StopIteration << std::endl;
  os << indent << "Iteration: "  << m_Iteration << std::endl;
  os << indent << "NumberOfIterations: "  << m_NumberOfIterations << std::endl;
  os << indent << "RelativeChange: "  << m_RelativeChange << std::endl;
  os << indent << "RelativeChangeThreshold: "  << m_RelativeChangeThreshold << std::endl;
  os << indent << "SmoothingPeriod: "  << m_SmoothingPeriod << std::endl;
  os << indent << "SmoothingFilter: ";
  if( m_SmoothingFilter.IsNull() )
    {
    std::cout << "NULL" << std::endl;
    }
  else
    {
    m_SmoothingFilter->Print( os, indent.GetNextIndent() );
    }
}

}// end namespace itk
#endif
