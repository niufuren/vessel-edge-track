/*
 *  ImageProcessing.cpp
 *  retinal
 *
 *  Created by Juan Lu on 11/02/14.
 *  
 *
 */

#include "ImageProcessing.h"
#include "matrix.cpp"

void ImageProcessing::setRingMaskAndEdgeInRing(double r1, double r2)
{

	
    Matrix<double> m1(height,width);
	Matrix<double> m2(height,width);
	
	m1.CreateCircleMask(centerX,centerY,r1);
	m2.CreateCircleMask(centerX,centerY,r2);
	
	ringMask->SubMatrix(m1, m2);
	edgeInRing->AndMatrix(*myEdge, *ringMask);
		
}

void ImageProcessing::getEdgesFrom(double r)
{
	
	Matrix<bool> FlagEdge(height, width);
	FlagEdge.InitMatrix();
	
	MyContour* c=seq;
	MyContour* temp_seq;

	int cur_px, cur_py;

	for ( cur_px=centerX-r-1; cur_px<centerX+r+1; cur_px++)
		for ( cur_py=centerY-r-1; cur_py<centerY+r+1; cur_py++)
	{
			
		double d2=0;

		if (edgeInRing->getValue(cur_py, cur_px)==0)
			continue;
		
		d2=sqrt(pow(double(cur_px-centerX),2)+pow(double(cur_py-centerY),2));
		

		if (d2<=r+2 && d2>=r)
		{
		
		if (FlagEdge.getValue(cur_py, cur_px)) 
			continue;
		
		if (edgeInRing->getValue(cur_py, cur_px)==255) {
			
			if(!seq->ele.empty()) //allocate memory to store edge points of the next contour
			{
							
						temp_seq=new MyContour();
						c->c_next=temp_seq;	
						temp_seq->c_prev=c;
						c=c->c_next;
			}
			
			FlagEdge.setValue(cur_py,cur_px,1);
			
			MyPoint p={cur_px,cur_py};
			c->ele.push_back(p);
			
			for(; ;)
			{
				int nei_list[2][8]; //neighbouring pixel coordinate list

				int cur_n=0; // number of connected components
				
				
				cur_n=findNeighbouringEdgePoint(nei_list, c, FlagEdge, cur_px, cur_py);
							 
			  if(cur_n==0)
			  {
				  break; //finish to search a contour
			  }
				else
				{
					trackEdgePixels(cur_n, nei_list, c, FlagEdge);
				}
				
			}
			
		}
		}
	}
	
}

void ImageProcessing::trackEdgePixels(int cur_n, int nei_list[][8], MyContour* c, Matrix<bool>& FlagEdge)
{
	int cur_px,cur_py;

	
	for (int n=0; n<cur_n; n++) {
		cur_px=nei_list[0][n];
		cur_py=nei_list[1][n];
		
		int local_nei_list[2][8];
		
		int local_n=0;
		
		local_n=findNeighbouringEdgePoint(local_nei_list, c, FlagEdge, cur_px, cur_py);
		
		
		if (local_n==0)
			continue;
		else
			trackEdgePixels(local_n, local_nei_list, c, FlagEdge);	
			
			}
}

	int ImageProcessing::findNeighbouringEdgePoint(int  (&nei_list)[2][8], MyContour* c, Matrix<bool>& FlagEdge, int cur_px, int cur_py)
	{
        int cur_n=0;
		int reg_x, reg_y;
	    int flag_edge, reg_v;
		
		for (int i=-1; i<=1; i++)
			for (int j=-1; j<=1; j++)
			{
				reg_x=cur_px+i;
				reg_y=cur_py+j;
				
				flag_edge=FlagEdge.getValue(reg_y, reg_x);
				reg_v=edgeInRing->getValue(reg_y, reg_x);
				
				if (flag_edge!=1&& reg_v==255) // if the current point is not among the detected edge points, and it is an edge point
				{
					
					FlagEdge.setValue(reg_y,reg_x,1);
					MyPoint p={reg_x, reg_y};
					c->ele.push_back(p);
					
					//save location information in nei_list
					cur_n++; 
					nei_list[0][cur_n-1]=reg_x;
					nei_list[1][cur_n-1]=reg_y;
					
				}
				
			}
		return cur_n;
		
	}
	
void ImageProcessing::saveContours(const char* s)
{
	MyContour *c;
	c=seq;
	int m=0;
	

	ofstream outFile;
	outFile.open(s);
	
	while(c)
	{
		
		//printf("Contour #%d\n", m );
		outFile<<"Contour #"<<m<<endl;
		
		for( int j=0; j<c->ele.size(); ++j )
		{ 
			MyPoint p;
			p=c->ele[j];
			outFile<<p.x<<" "<<p.y<<endl;
		} 
		
		c=c->c_next;
		m++;
	}
	
	outFile<<"total Contour"<<m<<endl;
	outFile.close();
	
	
}

void ImageProcessing::showDetectedContours()
{
	IplImage* img_tracked_e= cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 3 ); 
	MyContour *c;
	c=seq;
	
	for (int i=0; i<height; i++)
	{uchar* ptr = (uchar*) (img_tracked_e->imageData + i * img_tracked_e->widthStep);
		for (int j=0; j<width;j++)
		{
			ptr[j*img_tracked_e->nChannels+1]=.1*ringMask->getValue(i,j);
			ptr[j*img_tracked_e->nChannels]=myEdge->getValue(i,j);
			
		}
	}
	
	while(c)
	{
		for( int j=0; j<c->ele.size(); ++j )
		{
			MyPoint p;
			p=c->ele[j];
			((uchar *)(img_tracked_e->imageData + p.y*img_tracked_e->widthStep))[p.x*img_tracked_e->nChannels + 2] = 255; 
		    ((uchar *)(img_tracked_e->imageData + p.y*img_tracked_e->widthStep))[p.x*img_tracked_e->nChannels + 1] = 0; 
			((uchar *)(img_tracked_e->imageData + p.y*img_tracked_e->widthStep))[p.x*img_tracked_e->nChannels ] = 0; 
		}
		c=c->c_next;
	}
	
	cvSaveImage("result.jpg",img_tracked_e);
	
	const char* detectedEdgeName="edges starting from the radius r";
	cvShowImage(detectedEdgeName, img_tracked_e);
	cvResizeWindow(detectedEdgeName, 1000, 1000);
	
	
    cvReleaseImage( &img_tracked_e );
    cvDestroyWindow( detectedEdgeName );
	return;
}

	
