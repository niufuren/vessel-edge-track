/*
 *  matrix.cpp
 *  retinal
 *
 *  Created by Juan Lu on 11/02/14.
 *  Copyright 2014 Melb uni. All rights reserved.
 *
 */


#include "matrix.h"

template<class T> 
T** Matrix<T>:: createMatrix(int nRows, int nCols)
{
	 T** matrix_temp = new T*[nRows];
	for( int i = 0 ; i < nRows ; i++ )
		matrix_temp[i] = new T [nCols];
	
	return matrix_temp;
}

template<class T>
void Matrix<T>:: InitMatrix()
{
	for (int i=0; i<nRows; i++)
		for (int j=0; j<nCols;j++)
			matrix[i][j]=0;
	
}

template<class T>
Matrix<T>:: Matrix(const Matrix<T>& m)
{

	nRows=m.nRows;
	nCols=m.nCols;
	
	matrix=createMatrix(m.nRows, m.nCols);
	
	for (int i=0; i<nCols; i++)
		for (int j=0; j<nRows; j++)
			matrix[j][i]=m.matrix[j][i];
	
}



template<class T>
Matrix<T>& Matrix<T>::AndMatrix(Matrix<T> m1, Matrix<T> m2)
{
	for (int i=0; i<nCols; i++)
		for (int j=0; j<nRows; j++)
		{
			if (m2.matrix[j][i])
				matrix[j][i]=m1.matrix[j][i];
			else
				matrix[j][i]=0;
			
		}
	
	return *this;	
}

template<class T>
Matrix<T>& Matrix<T>:: SubMatrix(Matrix<T>& m1, Matrix<T>& m2)
{
	for (int i=0; i<nCols; i++)
		for (int j=0; j<nRows; j++)
		{
		//	if (m1.matrix[j][i]-m2.matrix[j][i])
//				int c=1;
			
			matrix[j][i]=m1.matrix[j][i]-m2.matrix[j][i];
			
			//if(matrix[j][i])
//				int c=1;
				
		}
	return *this;
}

template<class T>
Matrix<T>& Matrix<T>::CreateCircleMask ( int c_x, int c_y, double rad)
{
	double d; //distance^2
	for (int i=0; i<nCols; i++)
		for (int j=0;j<nRows; j++)
		{
			d=pow(double(i-c_x),2)+pow(double(j-c_y),2);
			int t_rad=pow(rad,2);
			if(d<t_rad)
				matrix[j][i]=255;
			else
				matrix[j][i]=0;		
		}
	return *this;
}

template<class T>
Matrix<T>& Matrix<T>::Img2Matrix(IplImage* img)
{
	int Rows=img->height;
	int Cols=img->width;
	
	//createMatrix(nRows, nCols);
	
	for (int i=0; i<Rows; i++)
		for (int j=0; j<Cols; j++)
		{
			T cur_v=((uchar*)(img->imageData+img->widthStep*i))[j];
			this->matrix[i][j]=cur_v;
		}
	
	return *this;
	
}

template<class T>
IplImage* Matrix<T>::Matrix2Img(IplImage* img )
{
	//IplImage* img = cvCreateImage(cvSize(nCols,nRows), IPL_DEPTH_8U, 1);
	
	for (int i=0; i<nRows; i++)
	{uchar* ptr = (uchar*) (img->imageData + i * img->widthStep);
		for (int j=0; j<nCols;j++)
		{
			ptr[j]=matrix[i][j];
		}
	}
    return img;	
}


template<class T>
Matrix<T>::~Matrix()
{
 if(matrix!=NULL)
 {
	for(int i=0; i<nRows; i++)
		delete [] matrix[i];
	
	delete [] matrix;
 }
}








