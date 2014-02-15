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
	Matrix<double> *myEdge;
	Matrix<double> *ringMask;
	Matrix<double> *edgeInRing;
	int centerX;
	int centerY;
	int height;
    int width;
    MyContour* seq;
	
	void getEdgesFrom(double r);
	void trackEdgePixels(int cur_n, int nei_list[][8], MyContour* c, Matrix<bool>& FlagEdge );
	int findNeighbouringEdgePoint(int (&nei_list)[2][8], MyContour* c, Matrix<bool>& FlagEdge, int cur_px, int cur_py);
	
public:
	ImageProcessing (const int centerX, const int centerY, const int ImgH, const int ImgW)
	{
		this->centerX=centerX;
		this->centerY=centerY;
		height=ImgH;
		width=ImgW;
		
		myEdge=new Matrix<double>(height, width);
		ringMask=new Matrix<double>(height, width);
		edgeInRing=new Matrix<double>(height, width);
		
		seq=new MyContour();
	}
	
	void setEdges(IplImage* img)
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