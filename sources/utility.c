#include "utility.h"


void minMax(float result[2], float data[], int size)
{
	float min, max;
	min = max = data[0];
	for (int i = 1; i < size; i++)
	{
		if (min > data[i])
		{
			min = data[i];
		}
		if (max < data[i])
		{
			max = data[i];
		}
	}
	result[0] = min;
	result[1] = max;
}

void minMax2D(float returnedMinMax[], float data[][OUTERFEATURENUMBER], int indexSize, int featureNumber)
{
	returnedMinMax[0] = returnedMinMax[1] = data[0][0];
	// This method does not work because MAXROW has to be same size as the data passed to the function -> But it is not !
	// The problem is that data size is based on MAXROW, so when doing a double loop method, it misses overflowed row that are greater than "featureNumber"
	//OPTION1: two loops
	for (int i = 0; i < indexSize; i++)
	{
		for (int j = 0; j < featureNumber; j++)
		{
			if (returnedMinMax[0] > data[i][j])
			{
				returnedMinMax[0] = data[i][j];
			}
			if (returnedMinMax[1] < data[i][j])
			{
				returnedMinMax[1] = data[i][j];
			}
		}
	}


	// OPTION2: Pointer arithmetic
	//Not working yet -> It does not read all values in data
	/*
	for (int index = 0; index < indexSize; index++, data++)
	{
		for (int feat = 0; feat < featureNumber; feat++, *data++)
		{
			printf("%i %f %f %f\n", index, **data, returnedMinMax[0], returnedMinMax[1]);
			if (returnedMinMax[0] > **data)
			{
				returnedMinMax[0] = **data;
			}
			if (returnedMinMax[1] < **data)
			{
				returnedMinMax[1] = **data;
			}
		}
	}
	*/
}


void scale(float data[], float min, float max, int size)
{
	for (int i = 0; i < size; i++)
	{
		data[i] = (data[i] - min) / (max - min);
	}
}


void scale2D(float minMax[], float data[][OUTERFEATURENUMBER], int indexSize, int featureNumber)
{
	for (int i = 0; i < indexSize; i++)
	{
		for (int j = 0; j < featureNumber; j++)
		{
			data[i][j] = (data[i][j] - minMax[0]) / (minMax[1] - minMax[0]);
		}
	}
	
	/*
	for (int index = 0; index < indexSize; index++, data++)
	{
		for (int feat = 0; feat < featureNumber; feat++, *data++)
		{
			**data = (**data - returnedMinMax[0]) / (returnedMinMax[1] - returnedMinMax[0]);
		}
	}
	*/
}



void shiftArray(float array[], int size, float newValue)
{
	for (int i = 0; i < size - 1; i++)
	{
		array[i] = array[i + 1];
	}
	array[size - 1] = newValue;
}

void shiftIntArray(int array[], int size, int newValue)
{
	for (int i = 0; i < size - 1; i++)
	{
		array[i] = array[i + 1];
	}
	array[size - 1] = newValue;
}

void shift2DOuter(float array[][OUTERFEATURENUMBER], int size, float newValues[])
{
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < OUTERFEATURENUMBER; j++)
		{
			array[i][j] = array[i + 1][j];
		}

	}
	for (int j = 0; j < OUTERFEATURENUMBER; j++)
	{
		array[size - 1][j] = newValues[j];
	}

}

void shift2DInner(float array[][INNERFEATURENUMBER], int size, float newValues[])
{
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < INNERFEATURENUMBER; j++)
		{
			array[i][j] = array[i + 1][j];
		}

	}
	for (int j = 0; j < INNERFEATURENUMBER; j++)
	{
		array[size - 1][j] = newValues[j];
	}

}