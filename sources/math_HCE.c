#include "math_HCE.h"


#include <stdio.h>



float absolute(float value)
{
	float result = value;
	if (value < 0)
	{
		result = -value;
	}
	return result;
}

float power(float data, int exponent)
{
	float result = 1.0;
	if (exponent < 0)
	{
		printf("\n\nWARNING in pow function: negative exponent given\n\n");
		return 0.0;
	}
	else if (exponent == 0)
	{
		printf("\n\nreturn 1 %i\n\n", exponent);
		return 1.0;
	}
	else
	{
		for (int i = 0; i < exponent; i++)
		{
			result *= data;
		}
	}
	return result;

}

float sqroot(float data)
{
	float result, temp;
	result = data / 2.0;
	temp = 0;
	if (data < 0)
	{
		printf("\n\nWARNING in sqrt function: negative data given\n\n");
	}
	else
	{
		while (result != temp)
		{
			temp = result;
			result = (data / temp + temp) / 2.0;
		}
	}
	return result;
}



float manhattan(float x1, float x2)
{
	float distance;

	distance = x2 - x1;
	return absolute(distance);
}

float density(int dataNumber, int featureNumber, float size[])
{
	float volume = 1;
	float density;
	for (int i = 0; i < featureNumber; i++)
	{
		volume *= size[i];
	}

	density = dataNumber / volume;
	if (density < 0.0000001)
	{
		density = 0.0000001;
	}
	return density;
}




float mean(float data[], int size)
{
	float result = 0;
	for (int i = 0; i < size; i++)
	{
		result += data[i];
	}

	result = result / size;
	return result;
}

float variance(float data[], int size)
{
	float result = 0;
	float dataMean;

	dataMean = mean(data, size);
	for (int i = 0; i < size; i++)
	{
		result += power((data[i] - dataMean), 2);
	}
	result = result / (size - 1.0);
	return result;
}

float strdDev(float data[], int size)
{
	return sqroot(variance(data, size));
}

float skewness(float data[], int size)
{
	float result, dataMean, dataVariance, firstHalf, sumResult, secondHalf;

	sumResult = 0;
	dataMean = mean(data, size);
	dataVariance = variance(data, size);
	firstHalf = (power(size, 2)) / ((size - 1.0) * (size - 2.0));
	for (int i = 0; i < size; i++)
	{
		sumResult += power(data[i] - dataMean, 3);
	}
	secondHalf = sumResult / (size * power(dataVariance, 1.5));
	result = firstHalf * secondHalf;

	return result;
}

/*
* https://www.educba.com/kurtosis-formula/
* NOT WORKING
float kurtosis(float data[],int size)
{
	float result, dataMean, numerator=0, denominator=0;
	dataMean = mean(data, size);

	for (int i = 0; i < size; i++)
	{
		numerator += power(data[i] - dataMean, 4);
		denominator += power(data[i] - dataMean, 2);
	}
	denominator = power(denominator, 2);
	result = (size * numerator) / denominator;
	return result;
}
*/

float kurtosis(float data[], int size)
{
	float result, dataMean, deviation, firstHalf, sum = 0, secondHalf;

	dataMean = mean(data, size);
	deviation = strdDev(data, size);

	for (int i = 0; i < size; i++)
	{
		sum += power((data[i] - dataMean) / deviation, 4);
	}

	firstHalf = (size * (size + 1.0)) / ((size - 1.0) * (size - 2.0) * (size - 3.0));
	firstHalf *= sum;
	secondHalf = (3 * power(size - 1.0, 2)) / ((size - 2.0) * (size - 3.0));
	result = firstHalf - secondHalf;

	return result;
}


float median(float data[], const int size)
{
	float temp[MAXSIZEMEDIAN] = { 0 };
	float result = 0;
	for (int i = 0; i < size; i++)
	{
		temp[i] = data[i];
	}
	sortArray(temp, size);
	if (isEven(size))
	{
		result = (temp[size / 2 - 1] + temp[size / 2]) / 2.0;
	}
	else
	{
		result = temp[size / 2];
	}
	return result;
}


float MAD(float data[], int size)
{
	float med;
	float tempData[MAXSIZEMEDIAN] = { 0 };
	float result = 0;

	med = median(data, size);
	for (int i = 0; i < size; i++)
	{

		tempData[i] = absolute(data[i] - med);
	}
	result = median(tempData, size);
	return result;
}

int describeStats(float statsResult[7], float data[], int size)
{
	int featureNumber;
	float minMaxRes[2];
	minMax(minMaxRes, data, size);

	statsResult[0] = minMaxRes[0];
	statsResult[1] = minMaxRes[1];
	statsResult[2] = mean(data, size);
	statsResult[3] = variance(data, size);
	statsResult[4] = strdDev(data, size);
	statsResult[5] = skewness(data, size);
	statsResult[6] = kurtosis(data, size);

	return 0;
}
