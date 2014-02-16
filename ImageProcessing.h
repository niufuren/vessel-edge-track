/*
 *  ImageProcessing.h
 *  retinal
 *
 *  Created by Juan Lu on 11/02/14.
 *  
 *
 */

#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H

#include "matrix.h"
#include <fstream.h>

typedef struct MyPoint{
	int x;
	int y;
};

typedef struct MyContour{
	MyContour* c_prev; //previous contour
	MyContour* c_next; // next contour
	vector <MyPoint> ele; // save current contour
	MyContour()
	{   c_prev=NULL;
		c_next=NULL;
	}
};

class ImageProcessing
{
private:
	Matrix<double> *myEdge; //save the canny edge detection result
	Matrix<double> *ringMask; 
	Matrix<double> *edgeInRing; // edges in the ring mask
	int centerX;
	int centerY;
	int height;
    int width;
    MyContour* seq; //tracked edge points are saved in *seq
	
	void getEdgesFrom(double r);
	void trackEdgePixels(int cur_n, int nei_list[][8], MyContour* c, Matrix<bool>& flagEdge );
	int findNeighbouringEdgePoint(int (&nei_list)[2][8], MyContour* c, Matrix<bool>& flagEdge, int cur_px, int cur_py);
	
public:
	ImageProcessing (const int pointX, const int pointY, const int ImgH, const int ImgW)
	{
		centerX=pointX;
		centerY=pointY;
		height=ImgH;
		width=ImgW;
		
		myEdge=new Matrix<double>(height, width);
		ringMask=new Matrix<double>(height, width);
		edgeInRing=new Matrix<double>(height, width);
		
		seq=new MyContour();
	}
	
	void setEdgeImage(IplImage* img)
	{
		myEdge->Img2Matrix(img);
	}
	
	void setRingMaskAndEdgeInRing(double r1, double r2);
	
	void getEdgesStartedFrom(double r)
	{
		getEdgesFrom(r);
	};
	
	IplImage* getEdgeInRingMaskImage(IplImage* img)
	{
		 img=edgeInRing->Matrix2Img(img);
		
		return img;
	}
	
	void saveContours(const char* s);
	
	void showDetectedContours();
	
	~ImageProcessing()
	{
		delete myEdge;
		delete ringMask;
		delete edgeInRing;
		
		while(seq)		
		{  
			MyContour* c=seq;
			seq=seq->c_next;
			delete c;
		}
	}
};


#endif