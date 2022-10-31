#pragma once
#ifndef MATH_HCE_H
#define MATH_HCE_H

#define MAXROW 6
#define MAXSIZEMEDIAN 1000

#include "utility.h"


/*
* absolute
* Return the absolute value of the number given
* Input:
*	data: type = float, value to consider for absolute operation
* Output: type = float, absolute value of "data" parameter
*/
float absolute(float data);

/*
* power
* Return the the power value of given number (data^exponent)
* Input:
*	data: type = float, value to consider for power operation
*	exponent: type = int, exponent used for power operation
* Output: type = float, power value of "data" parameter
*/
float power(float data, int exponent);

/*
* sqroot
* Return the the square root value of given number
* //https://ourcodeworld.com/articles/read/884/how-to-get-the-square-root-of-a-number-without-using-the-sqrt-function-in-c
* Input:
*	data: type = float, value to consider for square root operation
* Output: type = float, square root value of "data" parameter
*/
float sqroot(float data);



/*
* manhattan
* Compute the manhattan distance between 2 value: dist = absolute(x2 - x1)
* Input:
*	x1: type = float, first value
*	x2: type = float, second value
* Output: type = float, distance between x1 & x2
*/
float manhattan(float x1, float x2);

/*
* density
* Compute the density of a uCluster according to number of points and volume
* Input:
*	dataNumber: type = int, number of data in the uCluster
*	featureNumber: type = int, number of features in the uCluster (corresponds to size[] length)
*	size: type = float[], length of each dimension of the uCluster
* Output: type = float, density of uCluster
*/
float density(int dataNumber, int featureNumber, float size[]);


/*
* mean
* Compute the mean of a data set
* Input:
*	data: type = float[], data set to analyse
*	size: type = int, length of data
* Output: type = float, mean of data set
*/
float mean(float data[], int size);

/*
* variance
* Compute the variance of a data set
* Input:
*	data: type = float[], data set to analyse
*	size: type = int, length of data
* Output: type = float, variance of data set
*/
float variance(float data[], int size);

/*
* strdDev
* Compute the standard deviation of a data set
* Input:
*	data: type = float[], data set to analyse
*	size: type = int, length of data
* Output: type = float, standard deviation of data set
*/
float strdDev(float data[], int size);

/*
* skewness
* Compute the skewness of a data set
* More info: https://fr.wikipedia.org/wiki/Asym%C3%A9trie_(statistiques)
* Input:
*	data: type = float[], data set to analyse
*	size: type = int, length of data
* Output: type = float, skewness of data set
*/
float skewness(float data[], int size);

/*
* kurtosis
* Compute the kurtosis of a data set
* More info: https://support.microsoft.com/fr-fr/office/fonction-kurtosis-bc3a265c-5da4-4dcb-b7fd-c237789095ab
* Input:
*	data: type = float[], data set to analyse
*	size: type = int, length of data
* Output: type = float, kurtosis of data set
*/
float kurtosis(float data[], int size);

/*
* median
* Compute the median of a data set
* More info: https://en.wikipedia.org/wiki/Median
* Input:
*	data: type = float[], data set to analyse
*	size: type = int, length of data
* Output: type = float, median of data set
*/
float median(float data[], int size);

/*
* MAD
* Compute the median absolute deviation of a data set
* More info: https://en.wikipedia.org/wiki/Median_absolute_deviation
* Input:
*	data: type = float[], data set to analyse
*	size: type = int, length of data
* Output: type = float, MAD of data set
*/
float MAD(float data[], int size);


/*
* describeStats
* Compute all statical analysis from a data set
* For now, 7 statistical features are available: {min, max, mean, variance, standard deviation, skewness, kurtosis}
* !!!(NOTE: Don't forget to change output when adding/removing features)!!!
* Input:
*	statResult: type = float[7], vector containing the result of statistical calculations
*	data: type = float[], data set to analyse
*	size: type = int, length of data
* Output: type = int, Number of statistical calculations performed (7 right now)
*/
int describeStats(float statsResult[7], float data[], int size);


#endif