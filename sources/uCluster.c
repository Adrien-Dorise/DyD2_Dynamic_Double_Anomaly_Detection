#include "uCluster.h"



//sample functions

void printSample(struct sample S)
{
	printf("\nSample printing:\n");
	printf("Feature number: %i\n", S.featureNumber);
	printf("Date: %i\n", S.date);
	printf("feature value: ");
	for (int i = 0; i < S.featureNumber; i++)
	{
		printf("%f  ", S.features[i]);
	}
	printf("\n");

}

void initSample(struct sample* S, int featNumber, float date, float features[])
{
	S->featureNumber = featNumber;
	S->date = date;
	for (int i = 0; i < featNumber; i++)
	{
		S->features[i] = features[i];
	}
}



//uCluster functions

void printUCluster(struct uCluster uCl)
{
	printf("\nuCluster printing:\n");
	printf("Feature number: %i\n", uCl.featureNumber);
	printf("Data number: %i\n", uCl.dataNumber);
	printf("Density: %f\n", uCl.density);
	printf("Size: ");
	for (int i = 0; i < uCl.featureNumber; i++)
	{
		printf("%f  ", uCl.size[i]);
	}
	printf("\nCenter: ");
	for (int j = 0; j < uCl.featureNumber; j++)
	{
		printf("%f  ", uCl.center[j]);
	}
	printf("\nScore: %f\n", uCl.score);
	printf("Creation date: %i\n", uCl.creationDate);
	printf("Last update: %i\n\n", uCl.lastUpdate);
}


void initUCluster(struct uCluster* uCl, struct sample S, float size[])
{
	uCl->featureNumber = S.featureNumber;
	uCl->dataNumber = 1;
	for (int i = 0; i < S.featureNumber; i++)
	{
		uCl->size[i] = size[i];
	}
	uCl->density = density(uCl->dataNumber, S.featureNumber,size);
	for (int feat = 0; feat < S.featureNumber; feat++)
	{
		uCl->center[feat] = S.features[feat];
	}
	uCl->score = 0;
	uCl->creationDate = S.date;
	uCl->lastUpdate = S.date;
}

void setUClusterDensity(struct uCluster* uCl, int newDataNumber, float lastUpdateDate)
{
	//uCl->density = density(uCl->dataNumber, uCl->featureNumber, uCl->size);
	uCl->dataNumber = newDataNumber;
	if (lastUpdateDate > uCl->lastUpdate)
	{
		uCl->lastUpdate = lastUpdateDate;
	}
	//uCl->lastUpdate = lastUpdateDate;

}


void updateUCluster(struct uCluster* uCl, struct sample S)
{

	if (!sameFeatureNumber(*uCl, S))
	{
		printf("\nWARNING IN UPDATEUCLUSTERS: uCl features numbers (%i) different from data point feature numbers (%i)", uCl->featureNumber, S.featureNumber);
		exit(1);
	}
	
	for (int i = 0; i < uCl->featureNumber; i++)
	{
		//uCl->center[i] = uCl->center[i] + ((S.features[i] - uCl->center[i]) / (2.0 * (uCl->dataNumber - 1)));
		uCl->center[i] = (S.features[i] + (uCl->dataNumber * uCl->center[i])) / (1 + uCl->dataNumber);
		//newSample = uCl->center[i] + ((S.features[i] - uCl->center[i]) / (uCl->dataNumber - 1.0));
		//uCl->center[i] = (newSample + uCl->center[i]) / 2.0;
	}
	setUClusterDensity(uCl, uCl->dataNumber + 1, S.date);
}


float distance(struct uCluster uCl, struct sample S)
{
	float distance = 0;

	if (!sameFeatureNumber(uCl, S))
	{
		return -1.0;
	}
	for (int feat = 0; feat < uCl.featureNumber; feat++)
	{
		distance += manhattan(uCl.center[feat], S.features[feat]);
	}
	return distance;
}

float reachable(struct uCluster uCl, struct sample S)
{
	float distance = 0.0;

	if (!sameFeatureNumber(uCl, S))
	{
		return -1.0;
	}
	for (int i = 0; i < uCl.featureNumber; i++)
	{
		distance = manhattan(uCl.center[i], S.features[i]);

		//If only one feature of data point is not inside the hyperbox of uCluster, then S is outside uCl
		if (distance >= uCl.size[i]/2.0)
		{
			return -1.0;
		}
	}
	return distance;
}

bool sameFeatureNumber(struct uCluster uCl, struct sample S)
{
	if (uCl.featureNumber == S.featureNumber)
	{
		return true;
	}
	else
	{
		printf("\n!!! WARNING: uCluster and sample don't have same number of features !!!");
		printf("\nuCluster has %i features; sample has %i features\n", uCl.featureNumber, S.featureNumber);
		return false;
	}
}

//map functions
void initMap(struct map* M)
{
	//M->uCl[0] = NULL;
	M->uClNumber = 0;
	M->trainMinMax[0] = 0;
	M->trainMinMax[1] = 1;
}

void updateUClListMinMax(struct map* M, float minMaxValues[2])
{
	M->trainMinMax[0] = minMaxValues[0];
	M->trainMinMax[1] = minMaxValues[1];
}


void addMap(struct map* M, struct uCluster uCl)
{
	
	M->uClNumber += 1;
	if (M->uClNumber > MAXUCLUSTERS)
	{
		printf("\nWARNING in addMap: Max number of uCluster allowed in a list (%i) is higher than required number of uCluster in the list %i\n", MAXUCLUSTERS, M->uClNumber);
	}
	else
	{
		M->uCl[M->uClNumber - 1] = uCl;
	}
	
}

void printMap(struct map M)
{
	printf("\nPrinting of uCluster list:\n");
	for (int i = 0; i < M.uClNumber; i++)
	{
		printf("uCluster number %i on %i in list:", i + 1, M.uClNumber);
		printUCluster(M.uCl[i]);
	}
}


bool isReachable(struct map M, struct sample S)
{

	for (int i = 0; i < M.uClNumber; i++)
	{
		if (reachable(M.uCl[i], S) >= 0) //reachable() return -1 when S not in reach
		{
			return true;
		}
	}
	return false;
}


int closestReachableUCl(struct map M, struct sample S)
{
	float minDistance = -1.0;
	float distance;
	int closestUCluster = -1;

	minDistance = -1.0;
	for (int i = 0; i < M.uClNumber; i++)
	{
		distance = reachable(M.uCl[i], S);

		if (distance >= 0) //If uCluster is reachable for S
		{
			if (minDistance < 0 || minDistance > distance) //minDistance<0 means that we are in the first reachable uCLuster, so we initialize minDist
			{
				closestUCluster = i;
				minDistance = distance;
			}
		}
	}

	return closestUCluster;
}


int closestUCl(struct map M, struct sample S)
{
	float minDistance = -1.0;
	float distance;
	int closestUCluster = -1;

	for (int i = 0; i < M.uClNumber; i++)
	{
		distance = 0.0;
		for (int j = 0; j < M.uCl[i].featureNumber; j++)
		{
			distance = manhattan(M.uCl[i].center[j], S.features[j]);
		}

		if (minDistance < 0 || minDistance > distance) //minDistance<0 means that we are in the first reachable uCLuster, so we initialize minDist
		{
			closestUCluster = i;
			minDistance = distance;
		}
		
	}

	return closestUCluster;
}

int reachableMap(int reachableID[], struct map M, struct sample S)
{
	int iteration = 0;
	for (int i = 0; i < M.uClNumber; i++)
	{
		if (reachable(M.uCl[i], S) >= 0) //reachable() return -1 when S not in reach
		{			
			reachableID[iteration] = i;
			iteration++;
		}
	}
	return iteration;
}

void replaceUCluster(struct map* M, struct uCluster uCl, int uClusterNumber)
{
	M->uCl[uClusterNumber] = uCl;
}


void setLastUpdate(struct map* M, float firstIteration)
{
	for (int i = 0; i < M->uClNumber; i++)
	{
		M->uCl[i].lastUpdate = firstIteration;
		M->uCl[i].creationDate = firstIteration;
		//M->uCl[i].dataNumber = 1;
	}
}

void mapAgeing(struct map* M, float currentDate, float ageThreshold, float penalty)
{
	for (int i = 0; i < M->uClNumber; i++)
	{
		if ((M->uCl[i].lastUpdate < currentDate - ageThreshold) && (M->uCl[i].dataNumber > 1))
		{
			M->uCl[i].dataNumber = (int)(M->uCl[i].dataNumber * penalty);
			if (M->uCl[i].dataNumber <= 1)
			{
				M->uCl[i].dataNumber = 1;
			}
			M->uCl[i].density = density(M->uCl[i].dataNumber, M->uCl[i].featureNumber, M->uCl[i].size);
		}
	}
}