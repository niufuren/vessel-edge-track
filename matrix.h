/*
 *  matrix.h
 *  retinal
 *
 *  Created by Juan Lu on 11/02/14.
 *  
 *
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <opencv.hpp>
#include <vector>
#include<math.h>

template<class T>
class Matrix
{
private:
	T** matrix; 
	int nRows;
	int nCols;
	
	T** createMatrix(int nRows, int nCols);

public:
	
	//T** matrix; 
	
	Matrix():nRows(0), nCols(0), matrix(NULL){};
	Matrix(int row, int col)
	{
		nRows=row;
		nCols=col;
		
		matrix=createMatrix(nRows, nCols);
	};
	
	Matrix(const Matrix<T>& m);
	
	void InitMatrix();
		
	Matrix<T>& SubMatrix(Matrix<T>& m1, Matrix<T>& m2);
	Matrix<T>& AndMatrix(Matrix<T> m1, Matrix<T> m2);
	Matrix<T>& CreateCircleMask(int c_x, int c_y, double rad);

	Matrix<T>& Img2Matrix(IplImage* img);
	
	IplImage* Matrix2Img(IplImage* img);
	T getValue(int x, int y)
	{
		return matrix[x][y];
	}

	void setValue(int x, int y, T v)
	{
		matrix[x][y]=v;
	}
    ~Matrix();
	
};




#endif

