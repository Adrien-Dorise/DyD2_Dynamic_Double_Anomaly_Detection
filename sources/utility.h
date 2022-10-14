#pragma once
#ifndef UTILITY_H
#define UTILITY_H

#include "parameters.h"

/*
* isEven
* Check if a number is odd or even
* Input:
*	number: type = int, number to check
* Output: type = bool, TRUE if even, FALSE if odd
*/
bool isEven(int number);


/*
* minMax
* Find the min and max from a data set with a single feature
* Input:
*	result: type = float[2], min and max of data set (result[0] = min, result[1] = max)
*	data: type = float[INDEX], data set to analyse
*	size: type = int, length of data
* Output: None
*/
void minMax(float result[2], float data[], int size);

/*
* minMax2D
* Find the min and max from a data sets with multiple features
* Input:
*	returnedMinMax: type = float[2], min and max of data set (returnedMinMax[0] = min, returnedMinMax[1] = max)
*	data: type = float[INDEX][FEATURE], data set to analyse
*	indexSize: type = int, length of data
*	featureNumber: type = int, number of features in the data parameter
* Output: None
*/
void minMax2D(float returnedMinMax[], float data[][OUTERFEATURENUMBER], int indexSize, int featureNumber);


/*
* shiftArray
* Shift all values of an array from one indice lower (i-1 = i) and replaces the last value by the given value in newValue
* It is done for float arrays
* Input:
*	array: type = float[INDEX], array to shift
*	size: type = int, size of the array (or last value)
*	newValue: type = float, new value to put at the end of the array
* Output: None
*/
void shiftArray(float array[], int size, float newValue);

/*
* shiftIntArray
* Shift all values of an array from one indice lower (i-1 = i) and replaces the last value by the given value in newValue
* It is done for int arrays
* Input:
*	array: type = int[INDEX], array to shift
*	size: type = int, size of the array (or last value)
*	newValue: type = int, new value to put at the end of the array
* Output: None
*/
void shiftIntArray(int array[], int size, int newValue);

/*
* shift2DOuter
* Shift all values of an array from one indice lower (i-1 = i) and replaces the last value by the given value in newValue
* It is done for the outer features processed by DyD², so that the dimension are similar
* Input:
*	array: type = int[INDEX][FEATURE], array to shift
*	size: type = int, size of the array (or last value)
*	newValue: type = float[], new values to put at the end of the array
* Output: None
*/
void shift2DOuter(float array[][OUTERFEATURENUMBER], int size, float newValues[]);

/*
* shift2DInner
* Shift all values of an array from one indice lower (i-1 = i) and replaces the last value by the given value in newValue
* It is done for the inner features processed by DyD², so that the dimension are similar
* Input:
*	array: type = int[INDEX][FEATURE], array to shift
*	size: type = int, size of the array (or last value)
*	newValue: type = float[], new values to put at the end of the array
* Output: None
*/
void shift2DInner(float array[][INNERFEATURENUMBER], int size, float newValues[]);


/*
* scale
* Do a min-max normalization on data set. Calculation is done as follow: data[i] = (data[i] - min) / (max - min)
* //https://en.wikipedia.org/wiki/Feature_scaling#Rescaling_(min-max_normalization)
* Input:
*	data: type = float[], data set to normalize
*	min: type = float, min value for the normalization (usually -1 or 0)
*	max: type = float, max value for the normalization (usually 1)
*	size: type = int, size of the array
* Output: None
*/
void scale(float data[], float min, float max, int size);

/*
* scale
* Do a min-max normalization on data set. Calculation is done as follow: data[i] = (data[i] - min) / (max - min)
* //https://en.wikipedia.org/wiki/Feature_scaling#Rescaling_(min-max_normalization)
* It is done for a 2D array
* Input:
*	returnedMinMax: type = float[2], min and max of data set (returnedMinMax[0] = min, returnedMinMax[1] = max). It must be already initialised with the correct values
*	data: type = float[INDEX][FEATURES], data set to be scaled
*	indexSize: type = int, INDEX value
*	featureNumber: type = int, size of the FEATURES value
* Output: None
*/
void scale2D(float returnedMinMax[], float data[][OUTERFEATURENUMBER], int indexSize, int featureNumber);

/*
* sortArray
* Sort an array from smallest to highes value
* Input:
*	data: type = float[], data set to sort
*	size: type = int, length of data
* Output: None
*/
void sortArray(float data[], int size);


#endif