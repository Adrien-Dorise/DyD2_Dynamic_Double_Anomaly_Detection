/*
Contains the objects related to DyD2.
It regroups samples, uClusters and detection maps, as well as all functions governing these objects.

For further details on these objects, see the original paper:
https://hal.laas.fr/hal-03609573v2
*/


#pragma once
#ifndef DETECTION_HCE_H
#define DETECTION_HCE_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
//#include "parameters.h" //I cannot do that because parameters.h uses struct in uCluster.h

#include "math_HCE.h"

#define MAXUCLUSTERS 750
#define MAXFEATURES 15


/*
* struct sample
* Smallest object in the algorithm. A data point takes is composed of the coordinates of a point in the feature space.
*/
struct sample {
	int featureNumber;
	int date;
	float features[MAXFEATURES];
};

/*
* struct uCluster
* A uCluster is a group of similar data points.
*/
struct uCluster {
	int featureNumber;
	int dataNumber;
	float density;
	float size[MAXFEATURES];
	float center[MAXFEATURES];
	float score;
	int creationDate;
	int lastUpdate;
};


/*
* struct map
* A map contains multiple uCluster. It is easier to save informations of all created uCLuster if they are grouped in a list.
*/
struct map {
	struct uCluster uCl[MAXUCLUSTERS];
	int uClNumber;
	float trainMinMax[2];
};

//!!!! NOTE: make sure that the featNumber parameter is equivalent to the length of features[] for both sample and uCluster !!!!
/*
* initSample
* Initialisation of a sample structure. You have to give a feature vector in parameters.
* Input:
*	S: type = pointer to sample, sample to initialize 
*	featNumber: type = int, number of features in sample
*	date: type = int, date/indice of the data point
*	features: type = float, values of the point for each dimension in the feature space
* Output: None
*/
void initSample(struct sample* S, int featNumber, float date, float features[]);

/*
* printSample
* Print a sample in the console
* Input:
*	S: type = sample, sample to print
* Output: None
*/
void printSample(struct sample S);

/*
* initUCluster
* Initialisation of a uCluster structure. You have to give a sample in parameters.
* Input:
*	uCl: type = pointer to uCluster, uCluster to initialize
*	S: type = sample, sample to use for uCluster initalization
*	size: type = float[], size of the uCluster for each dimension of the feature space
* Output: None
*/
void initUCluster(struct uCluster* uCl, struct sample S, float size[]);

/*
* printSample
* Print a uCluster in the console
* Input:
*	uCl: type = uCluster, uCluster to print
* Output: None
*/
void printUCluster(struct uCluster uCl);

/*
* setUClusterDensity
* Modify the density property of a uCluster.
* It also adjust dataNumber & lastUpdate properties.
* Input:
*	uCl: type = pointer to uCluster, uCluster to modify
*	newDataNumber: type = int, new number of observations conitained in uCl
*	lastUpdatedate: type = float, date of the last observation added in uCl
* Output: None
*/
void setUClusterDensity(struct uCluster* uCl, int newDataNumber, float lastUpdateDate);


/*
* updateUCluster
* Update an existing uCluster with a new sample. The uCluster is going to move accordingly to the new point position & the numberOfPoints (or density) of the uCluster
* Input:
*	uCl: type = pointer to uCluster, uCluster to update
*	S: type = sample, sample to add to the uCluster
* Output: None
*/
void updateUCluster(struct uCluster* uCl, struct sample S);

/*
* distance
* Calculate the distance between the center of a uCluster and a sample. The currently used distance is manhattan (to verify accordingly to current version)
* Input:
*	uCl: type = pointer to uCluster, initialized uCluster
*	S: type = sample, intialized sample
* Output: type = float, distance between uCl and S. It is a sum of all distance for each dimension.
*/
float distance(struct uCluster uCl, struct sample S);

/*
* reachable
* Compute if a data point is in reach of a uCluster. If so, it returns the distance
* Input:
*	uCl: type = uCluster, initialized uCluster
*	S: type = sample, intialized sample
* Output: type float, the distance between the data point and the uCluster. If not in reach, return -1
*/
float reachable(struct uCluster uCl, struct sample S);

/*
* sameFeatureNumber
* Verify that a uCluster and a sample have the same feature space. 
* Input:
*	uCl: type = uCluster, initialized uCluster
*	S: type = sample, intialized sample
* Output: type bool, True if feature space is equal, false otherwise
*/
bool sameFeatureNumber(struct uCluster uCl, struct sample S);


/*
* initMap
* Initialisation of a map structure. This is used to stock multiple uClusters. No uCluster is needed in parameter as Null value is given for each paramaters
* Input:
*	M: type = pointer to map, map to initialize
* Output: None
*/
void initMap(struct map* M);

/*
* updateUClListMinMax
* Update the trainMinMax array of the uCluster list. It then can be easily access for further treatment
* Input:
*	M: type = pointer to map, map to initialize
*	minMaxValues: type = array of float, minimum & maximum values to put in the array
* Output: None
*/
void updateUClListMinMax(struct map* M, float minMaxValues[2]);

/*
* addMap
* Add a uCluster in the map
* Input:
*	M: type = pointer to map, initialized map
*	uCl: type = uCluster, initialized uCluster
* Output: None
*/
void addMap(struct map* M, struct uCluster uCl);


/*
* printMap
* print a map
* Input:
*	M: type = map, initialized map
* Output: None
*/
void printMap(struct map M);


/*
* isReachable
* Compute if a data point is in reach of any uCluster inside a map. 
* Note that we do not check for closest uCluster, the function stops when finding a reachable uCluster in order to compute faster. Use closestReachableUCl() to find the closest uCluster in the list.
* Input:
*	M: type = uCluster, initialized map
*	S: type = sample, intialized sample
* Output: type = bool, If a uCluster is in reach, return true, else return false
*/
bool isReachable(struct map M, struct sample S);


/*
* closestReachableUCl
* Find the closest reachable Ucluster in the list to the sample. It returns the uCluster ID or -1 if no uCluster in reach.
* Input:
*	M: type = uCluster, initialized map
*	S: type = sample, intialized sample
* Output: type = int, return uCluster ID in the uCluster list, or -1 if no uCluster in reach
*/
int closestReachableUCl(struct map M, struct sample S);

/*
* closestUCl
* Find the closest Ucluster in the list to the sample even if it is not reachable.
* Input:
*	M: type = uCluster, initialized map
*	S: type = sample, intialized sample
* Output: type = int, return closest uCluster ID in the uCluster list
*/
int closestUCl(struct map M, struct sample S);

/*
* reachableMap
* Compute if a data point is in reach of any uCluster inside a map.
* We save all the uClusters ID that are in reach of the data point.
* Input:
*	reachableID: type = float, array containing all the uClusters ID reached by S. Size must be >= to the uCluster number of the list
*	M: type = map, initialized map
*	S: type = sample, intialized sample
* Output: type = int, Number of reachable uClusters (correspond to reachableID size)
*/
int reachableMap(int reachableID[], struct map M, struct sample S);

/*
* replaceUCluster
* Replace a uCluster in a detection map.
* It is mainly used when the density of a uCluster reach a lower threshold and is not used anymore
* Input:
*	M: type = pointer to map, initialized map that will be modified
*	uCl: type = uCluster, initialized uCluster to be placed in the map M
*   uClusterNumber: type = int, the ID of the uCluster to be replaced in the map M
* Output: None
*/
void replaceUCluster(struct map* M, struct uCluster uCl, int uClusterNumber);

/*
* setLastUpdate
* Set the time properties of uClusters in a given map M to a given date.
* It is mostly used when initialising a map after training.
* Input:
*	M: type = pointer to map, initialized map that will be modified
*   firstIteration: type = float, date at which all uClusters of M are initialised
* Output: None
*/
void setLastUpdate(struct map* M, float firstIteration);

/*
* mapAgeing
* Set the time properties of uClusters in a given map M to a given date.
* It is mostly used when initialising a map after training.
* Input:
*	M: type = pointer to map, initialized map that will be modified
*   currentDate: type = float, date of the current monitored observation
*   ageThreshold: type = float, age limit at which a uCluster start to degrade
*   penalty: type = float, penalty to be applied when degrading a uCluster. It is currently implemented as a linear degradation function <newDensity = penalty * oldDensity>
* Output: None
*/
void mapAgeing(struct map* M, float currentDate, float ageThreshold, float penalty);

#endif