#ifndef __SummationImageFilter_hxx
#define __SummationImageFilter_hxx

#include "SummationImageFilter.h"

#include "itkNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkConstantBoundaryCondition.h"
#include "itkRescaleIntensityImageFilter.h"
#include <itkImageLinearConstIteratorWithIndex.h>
#include <itkImageLinearIteratorWithIndex.h>

#include <iostream>
#include <time.h>

namespace itk{

template <class TInputImage, class TOutputImage>
SummationImageFilter<TInputImage, TOutputImage>::SummationImageFilter()
{
    itkDebugMacro(<< "SummationImageFilter called");
}

template< class TInputImage, class TOutputImage>
void
SummationImageFilter< TInputImage, TOutputImage>
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template< class TInputImage, class TOutputImage>
void SummationImageFilter< TInputImage, TOutputImage >::GenerateData()
{
  clock_t t1,t2;
  float diff;

  itkDebugMacro(<< "SummationImageFilter::GenerateData() called");

  InputImageConstPointer inputPtr  = this->GetInput();
  OutputImagePointer     outputPtr = this->GetOutput();

  outputPtr->SetRegions(inputPtr->GetRequestedRegion());
  outputPtr->Allocate();

  ImageRegionConstIterator<TInputImage> inputIterator(inputPtr, inputPtr->GetRequestedRegion());
  ImageRegionIterator<TOutputImage> outputIterator(outputPtr, outputPtr->GetRequestedRegion());
  
  inputIterator.GoToBegin();
  outputIterator.GoToBegin();
 
  for(;!inputIterator.IsAtEnd(); ++inputIterator, ++outputIterator)
  {
    outputIterator.Set(inputIterator.Get());
  }
  //perform 3D summation with neighborhood iterators

  //pad boundary to zero
  RadiusType radius;
  radius.Fill(1);
  NeighborhoodIteratorType it( radius, outputPtr, outputPtr->GetLargestPossibleRegion());

  OffsetType offset0 = {{0,0,0}};
  OffsetType offset1 = {{-1,0,0}};
  OffsetType offset2 = {{0,-1,0}};
  OffsetType offset3 = {{-1,-1,0}};
  OffsetType offset4 = {{0,0,-1}};
  OffsetType offset5 = {{-1,0,-1}};
  OffsetType offset6 = {{0,-1,-1}};
  OffsetType offset7 = {{-1,-1,-1}};

  float sum;
  float temp[8];
  
  t1=clock();

  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
  {   
    sum = 0;

    temp[0] = it.GetCenterPixel(); 
    temp[1] = it.GetPixel(offset1); 
    temp[2] = it.GetPixel(offset2); 
    temp[3] = -1.0 * it.GetPixel(offset3);
    temp[4] = it.GetPixel(offset4);
    temp[5] = -1.0 * it.GetPixel(offset5);
    temp[6] = -1.0 * it.GetPixel(offset6);
    temp[7] = it.GetPixel(offset7); 

    for( int i = 0; i < 8; i++)
    {     
      sum += temp[i];  
    }

    it.SetCenterPixel(sum);
  } 

  t2=clock();
  diff = ((float)t2-(float)t1);
  std::cout<<"sum image: "<<diff/CLOCKS_PER_SEC<<"s"<<std::endl;

}

}

#endif