#include"ImageProcessing.h"
#include"matrix.h"
#include"matrix.cpp"


int main (int argc, char * const argv[]) {
   
	const char* name = "Image Window";
	
	IplImage* img = cvLoadImage( "../../images/image1.png", 0 );
	
	// Edge detection with Canny operation
	IplImage* img_e = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, img->nChannels );//canny edge detection result
	double lowThresh = 80;
	double highThresh = 200;
	cvCanny( img, img_e, lowThresh*7*7, highThresh*7*7, 7 );
	
	//the center point of the ring mask	
	int centerX=1923;
	int centerY=1035;
	
	//the radii of the ring mask 
	double r1=360; //the inner circle radius
	double r2=540; //the outer circle radius
	
	//the size of the image
	int width=img->width;
	int height=img->height;
	
	ImageProcessing ImgProcess(centerX,centerY,height, width);
	ImgProcess.setEdgeImage(img_e);
	ImgProcess.setRingMaskAndEdgeInRing(r2,r1);
	
	
	ImgProcess.getEdgesStartedFrom(r1);
	
	//save the tracked points
	const char* savedPath="edgePointCoordinate.txt";
	ImgProcess.saveContours(savedPath);
	
	//show detected edges
	ImgProcess.showDetectedContours();
	
	//show original image
	cvNamedWindow( name, 0);
	cvShowImage(name, img);
	cvResizeWindow(name, 1000, 1000);
	cvWaitKey(0);
	
	// Release
	cvReleaseImage( &img );
	cvDestroyWindow( name );
	
	return 0;	
}
