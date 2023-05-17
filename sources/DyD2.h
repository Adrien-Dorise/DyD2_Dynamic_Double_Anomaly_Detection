/*
This file contains all the algorithmic parts of DyD2.
It corresponds to the different algorithms described in the paper
https://hal.laas.fr/hal-03609573v2
*/



#pragma once
#ifndef DYD2_H
#define DYD2_H


#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "parameters.h"
#include "uCluster.h"
#include "inout.h"



float setFromFile[FILESIZEMAX][OUTERFEATURENUMBER + 1];
float dataSetFromFile[FILESIZEMAX][OUTERFEATURENUMBER];
float testDate[FILESIZEMAX];
float outerSetDyD2[FILESIZEMAX][OUTERFEATURENUMBER];
float innerSetDyD2[FILESIZEMAX][INNERFEATURENUMBER];
float windowSetDyD2[WINDOWSIZE][OUTERFEATURENUMBER];
float dataSave[FILESIZEMAX][OUTERFEATURENUMBER * 2]; // [raw data, scale raw data]
float logSave[FILESIZEMAX][LOGROWDYD2]; // [iterationSave, timeSave, outerDetectionStatus, innerdetectionStatus, resetStatus]
float faultLogSave[FILESIZEMAX][FAULTLOGROWDYD2]; // [faultIteration, faultTime, faultCodeValue]
float testLabelsDyD2[FILESIZEMAX][LABELNUMBERDYD2];
float execTimeDyD2;
clock_t t1DyD2;
clock_t t2DyD2;
struct map outerMapDyD2, innerMapDyD2;
struct uCluster ruptMapDyD2;
struct sample Sp, Sw;



/*
* training 
* Performs the offline training part of DyD². The outer and inner maps are created in the process.
* This allow for a mapping of the training set using as less ressources as possible
* As a one-class classification method, DyD² only needs normal observations for this phase. Thus, the nominal behaviour oof the system is learned from historical normal data streams.
* Observations are taking out a sliding window and transformed into the feature space (now only statistical calculations are done) that are then stored in a sample.
* After that, for each new sample, we see if a uCluster is reachable in the detection maps.
* If it is, then we update the closest uCluster.
* If not, we create a new uCluster from this sample.
* Input:
*	trainingSet: type = float[INDEX][FEATURES], 2D array containing the training set. The feature space size must be specified
*	trainingSize: type = int, size of the trainingSet	
*	outerMap: type = pointer to map, outer map of DyD² to initialised
*	innerMap: type = pointer to map, inner map of DyD² to initialised
* Output: None
*/
void training(float trainingSet[][OUTERFEATURENUMBER], int trainingSize, struct map* outerMap, struct map* innerMap);

/*
* saveMaps
* Saves the outer and inner maps in a txt file.
* By doing so, it is possible to visualise the maps or load them for future use.
* Input:
*	outerMap: type = pointer to map, outer map of DyD² to initialised
*	innerMap: type = pointer to map, inner map of DyD² to initialised
*	savePath: type = string, path to the folder where the maps will be saved
* Output: None
*/
void saveMaps(struct map outerMap, struct map innerMap, char savePath[]);

/*
* dataAcquisition
* Acquires the next observation to be processed by DyD².
* This function may be modified depending on your experiment.
* In this version, this function perform offline, and read the next value current value of a data set.
* Input:
*	iteration: type = int, index of the observation to be extracted
*	data: type = float[INDEX][OUTERFEATURE+1], 2D array containing the data set to be extracted. We put OuterFeature+1 to take the time column in consideration
*	dataSave: type = float[INDEX][OUTERFEATURE*2], 2D array used to create the logs after a DyD² run. We put OuterFeature*2 because we put scaled values as well
*	lastPoint: type = float[OUTERFEATURE], vector used to store the monitored observation.
* Output: None
*/
void dataAcquisition(int iteration, float data[][OUTERFEATURENUMBER + 1], float dataSave[][OUTERFEATURENUMBER * 2], float lastPoint[]);

/*
* changePointDetection
* Performs the change point detection part of the algorithm.
* A change point is defined as a "transition between different states in the process that generates the time series data"
* In DyD², the focus is on detecting anomalies represented by change point. Hence, a this funtions is used to quickly exlude non-anomalous observations.
* Change point deteciton is performed using a single uCluster called rupture uCluster and a sample point. 
* A change point is detected when the rupture uCluster is not reachable by the current sample point. 
* In that case, the rupture uCluster is destroyed and created using the next normal sample point.
* Otherwise, the rupture uCluster is updated using the sample point.
* Input:
*   Sp: type = sample, initialised sample point
*	uCl: type = pointer to uCluster, initialized rupture uCluster
* Output: type = bool, True if change point detected, false otherwise
*/
bool changePointDetection(struct sample Sp, struct uCluster* uClRupt);

/*
* anomalyDetection
* Performs the map detection part of the algorithm. 
* DyD² includes two detection phases in order to distinguish critical from non-critical anomalies.
* For the first phase, point sample and outer map are used, while window sample and inner map are used.
* Anomaly detection is peformed by checking the reachability of the sample and the detection map.
* Input:
*	S: type = sample, intialized sample (you can use featureExtraction for that). It is either a point sample Sp or a window sample Sw
*	map: type = pointer to map, initialized & pre-trained map (you can use training function to do that). It can be either the outer or inner map
* Output: type = bool, True if sample is faulty, false otherwise
*/
bool anomalyDetection(struct sample S, struct map map);

/*
* featureExtraction
* Performs a feature extraction from the time window W to create a window sample Sw
* Note that this function might heavily vary depending on the studied system
* For now, onlys statistical features are calculated using math_HCE package
* Input:
*	window: type = float[], sliding time window storing latest observations
*	Sw: type = pointer to sample, window sample updated during the feature extraction
*	date: type = int, time date/indice of the current observation
* Output: None
*/
void featureExtraction(float window[], struct sample* Sw, float date);

/*
* featureExtractionScaled
* Performs a feature extraction from the time window W to create a window sample Sw. 
* In addition, the features are scaled accordingly to min max of the detection map
* Note that this function might heavily vary depending on the studied system
* For now, onlys statistical features are calculated using math_HCE package
* Input:
*	window: type = float[], sliding time window storing latest observations
*	Sw: type = pointer to sample, window sample updated during the feature extraction
*	date: type = int, time date/indice of the current observation
*	minMax: type = float[2], array containing the min and the max value of the inner map during training
* Output: None
*/
void featureExtractionScaled(float window[], struct sample* Sw, float date, float minMax[2]);

/*
* mapUpdate
* Performs the update of a single detection map.
* It reflects the dynamic aspect of DyD², by integreting new knowledge about the normal behaviour of the system.
* This phase is divide into to steps:
* First, the center of mass of the reachable uCluster is updated
* Then, all uCluster are updated using the mapUpdate() function
* Input:
*	S: type = sample, sample to be predicted as normal or anomaly
*	map: type = pointer to map, detection map used to check the normality of a sample
*	date: type = float, current date of observation
*	ageThreshold: type = float, age limit at which a uCluster start to degrade
*   penalty: type = float, penalty to be applied when degrading a uCluster. It is currently implemented as a linear degradation function <newDensity = penalty * oldDensity>
*	dataNumberMin: type = float, minimum density allowed before a uCluster is replaced in a map
*	scoreThreshold: type = float, used for Christoffel update (NOT YET IMPLEMENTED)
*	christoffelObjectPath: type = float, used for Christoffel update (NOT YET IMPLEMENTED)
* Output: None
*/
void mapUpdate(struct sample S, struct map* map, float date, float ageThreshold, float agePenalty, float dataNumberMin, float scoreThreshold, char christoffelObjectPath[]);

/*
* resultSave
* WARNING: it is only working for a single outer feature
* Save the results of a detection phase in two .txt files. 
* Also saves the state of the outer and inner maps after detection
* 
* The first log is stating the result for each observations.
* It gives the parameters used for the detection. It is described as follow:
* Index / Time / Value / Value scaled / Outer feature detection / Inner feature detection / Reset state
* 
* The second log file only states the anomaly's iteration. It is described as follow:
* Index / Time / Value / Anomaly detected (-1 for outer / -2 for inner)
* 
* Input:
*	postMapPath: type = string, path to the folder to save the post map files
*	logFolderPath: type = string, path to the folder to save the log files
*	faultIter: type = int, number of faults recorded during the detection phase of DyD²
*	dataSave: type = float[INDEX][OUTERFEAT+1], observations of the whole detection phase
*	dataSize: type = int, index size of the dataSave parameter
*	logSave: type = float[INDEX][5], data used to create the log file
*	faultLogSave: type = float[numberOfFaults][3], data used to create the fault log file
* Output: None
*/
int resultSave(char postMapPath[], char logFolderPath[], int faultIter, float dataSave[][OUTERFEATURENUMBER*2], int dataSize, float logSave[][5], float faultLogSave[][3]);

/*
* resetDyD2Version0
* Reset strategy of DyD² when an anomaly is detected
* In this version, the reset is systematically performed when an anomaly is detected.
* It can be used to test the false positive performance of DyD² with a data set that contains no faulty observations
* Input:
*	interrupt: type = pointer to bool, parameter used to know if DyD² is in standby
*	code1: type = pointer to int, detection state of outer features
* 	code2: type = pointer to int, detection state of inner features
*	code1Memory: type = array of int, detection state of previous outer features
*	code2Memory: type = array of int, detection state of previous inner features
* Output: Return True if DyD² has to be reset, false otherwise
*/
bool resetDyD2Version0(bool* interrupt, int* code1, int* code2, int code1Memory[], int code2Memory[]);


/*
* resetDyD2Version1
* Reset strategy of DyD² when an anomaly is detected
* DyD² is reset when the next observation is reachable by the outer map
* It is used to reset automatically DyD² with the next normal point.
* Input:
*	interrupt: type = pointer to bool, parameter used to know if DyD² is in standby
*	outerMap: type = map, initalised and trained outer map of DyD²
*	lastPoint, type = array[OUTERFEATURE], most recent monitored observation.
*	code1: type = pointer to int, detection state of outer features
* 	code2: type = pointer to int, detection state of inner features
*	code1Memory: type = array of int, detection state of previous outer features
*	code2Memory: type = array of int, detection state of previous inner features
* Output: Return True if DyD² has to be reset, false otherwise
*/
bool resetDyD2Version1(bool* interrupt, struct map outerMap, float lastPoint[], int* code1, int* code2, int code1Memory[], int code2Memory[]);

/*
* resetDyD2Version2
* Reset strategy of DyD² when an anomaly is detected
* Combination of version 1 and 3
* Input:
*	interrupt: type = pointer to bool, parameter used to know if DyD² is in standby
*	label: type = float, label of the last monitored observation
*	faultValue: type = float, label used for the anomaly class
*	outerMap: type = map, initalised and trained outer map of DyD²
*	lastPoint, type = array[OUTERFEATURE], most recent monitored observation.
*	code1: type = pointer to int, detection state of outer features
* 	code2: type = pointer to int, detection state of inner features
*	code1Memory: type = array of int, detection state of previous outer features
*	code2Memory: type = array of int, detection state of previous inner features
* Output: Return True if DyD² has to be reset, false otherwise
*/
bool resetDyD2Version2(bool* interrupt, float label, float faultValue, int* code1, int* code2, int code1Memory[], int code2Memory[]);

/*
* resetDyD2Version3
* Reset strategy of DyD² when an anomaly is detected
* DyD² is reset based on the true label of the observation. It is reset when the observation is normal
* It is the most accurate way ot reset, but the labels are needed to use it.
* Input:
*	interrupt: type = pointer to bool, parameter used to know if DyD² is in standby
*	label: type = float, label of the last monitored observation
*	faultValue: type = float, label used for the anomaly class
*	outerMap: type = map, initalised and trained outer map of DyD²
*	lastPoint, type = array[OUTERFEATURE], most recent monitored observation.
*	code1: type = pointer to int, detection state of outer features
* 	code2: type = pointer to int, detection state of inner features
*	code1Memory: type = array of int, detection state of previous outer features
*	code2Memory: type = array of int, detection state of previous inner features
* Output: Return True if DyD² has to be reset, false otherwise
*/
bool resetDyD2Version3(bool* interrupt, struct map outerMap, float lastPoint[], float label, float faultValue, int* code1, int* code2, int code1Memory[], int code2Memory[]);

/*
* offlinePhase
* Perform the offline phase of DyD².
* The whole training set is read from a .txt file
* The training is performed to create both outer and inner maps by using the training() function.
* The maps are saved in a .txt file
* Note that global variables are used
* Input:
*	trainingPath: type = string, path of the file containing the training set
*	mapSavePath: type = string, path to the folder to save the two maps
* Output: None
*/
void offlinePhase(char trainingPath[], char mapSavePath[]);

/*
* onlinePhase
* Perform the online phase of DyD².
* First the data are acquired from the input file
* Second the change point detection is performed
* Third the outer anomaly detection is performed
* Fourth the data are saved in a window. Note that during this process, the data are still acquired in the first phase. The algorithm does not stop
* Fifth, feature extraciton is performed on the time window
* Sixth, the inner anomaly detection is performed
* Lastly, the detection maps are updated
* 
* Note that all this process is done online, so each new observation has to be treated as soon as possible -> There is no real waiting period, but memory variables are used to store information* 
* Input:
*	testPath: type = string, path of the file containing the test set
*	postMapSavePath: type = string, path to the folder to save the two maps after the detection phase
*	logPath: type = string, path to the folder to save the log file after the detection phase
* Output:
	testSize: type = int, return the size of the test set
*/
int onlinePhase(char testPath[], char postMapSavePath[], char logPath[]);

/*
* confusionMatrix
* Perform the confusion matrix on a data set treated by DyD²
* The log data are compared to the real labels.
*
* Input:
*	result: type = int[4], the confusion matrix result. By putting it as a global variable, it is possible to add up the results of different data sets
*	logData: type = float[INDEX][LOGROW], results of the prediction phased stored as the log file
*	testLabels: type = float[INDEX][LABELNUMBERDYD2], label file of the test set
*	trueLabelRow: type = int, row containing the true label in the testLabel variable
*	faultValue: type = int, class value of the anomaly class
*	size: type = int, Number of observations in the test set
*	doAnomalyRemoval: type = bool, True to activate: when a TP is detected, count all following consecutive positive values as TP
* Output: None
*/
void confusionMatrix(int results[4], float logData[][LOGROWDYD2], float testLabels[][LABELNUMBERDYD2], int trueLabelRow, int faultValue, int size, bool doAnomalyRemoval);

/*
* printResults
* Print the confusion matrix in the terminal
*
* Input:
*	result: type = int[4], the confusion matrix result. 
*	time: type = float[INDEX], contains the processing time for each observations of the test set 
*	trainSet: type = string, path of the file containing the train set
*	testSet: type = string, path of the file containing the test set
*	testIter: type = int, INDEX value representing the number of observations in the test set
* Output: None
*/
void printResults(int results[4], float time[], char trainSet[], char testSet[], char labelSet[], int testIter);

/*
* validation
* Perform DyD² for a set of multiple train and test set
* For N train sets:
*	DyD² is trained on N
*	for M test sets:
*		Perform the detection and confusion matrix using DyD² trained on N
* The results are saved
* 
* Input:
*	result: type = int[4], the confusion matrix result.
*	trainFolder: type = string, path of the folder containing the N train sets
*	trainIteration: type = int, Number N of train files in the folder
*	testFolder: type = string, path of the folder containing the M test set
*	testIteration: type = int, Number M of test files in the folder
*	doAnomalyRemoval: type = bool, True to activate: when a TP is detected, count all following consecutive positive values as TP (for the confusionMatric() function)
* Output: None
*/
void validation(char trainFolder[], int trainIteration, char testFolder[], int testIteration, bool doAnomalyRemoval);


#endif
