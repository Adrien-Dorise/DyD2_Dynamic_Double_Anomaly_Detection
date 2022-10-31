/*
Regroup all the parameters of DyD2 as well as global variables
This file can be modifyed by the user to change the behaviour of DyD2.
It is here that you can input your own data set to set DyD2
*/

#pragma once
#ifndef PARAMETERS_DYD2
#define PARAMETERS_DYD2

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


//DyD2 parameters
#define WINDOWSIZE 20 //Sliding time window legnth. Used to extract inner features from an obsevation
#define UCLSIZERUPT 0.20 //Size of the rupture uCluster
#define UCLSIZEOUTER 0.01 //Size of all uClusters in the outer map
#define UCLSIZEINNER 0.05 //Size of all uClusters in the inner map
#define OUTERFEATURENUMBER 1 //Number of outer features
#define INNERFEATURENUMBER 4 //Number of inner features

#define OUTERDENSITYTHRESHOLD 2 //Min number of sample before outerUcluster being replaced
#define INNERDENSITYTHRESHOLD 2 //Min number of sample before innerUcluster being replaced
#define AGELIMIT 10000 //Time before ageing begin (be carefull, as the time is taken into consideration, not the index position)
#define AGEPENALTY 0.975 //Penalty given to an old uCluster. It is computed as <density = AGEPENALTY * oldDensity>. Must be float


//UserParameters
#define DOCROSSVALIDATION true //TRUE if you want to do multiple data sets at once
#define WHICHDATASET 0 // 0 for experimental heavy ion data sets with destructive anomalies
					   // TO COME:
					   // 1 for simulated stationnary destructive anomalies
					   // 2 for simulated stationnary non-destrucrive anomalies
					   // 3 for simulated dynamic destructive anomalies
					   // 4 for simulated dynamic non-destructive anomalies


//File path parameters
#define TRAINFILE "datas/trainSet/heavyIon/datas1/normalCurrent.txt" //Path to the train set
#define TESTFILE "datas/testSet/heavyIon/datas1/dataSet.txt" //Path to the test set
#define LABELTESTFILE "datas/testSet/heavyIon/datas1/statusData.txt" //Path to the labels (put "NONE" if none available)

//Results saves (NOTE: make sure that the folder path already exist)
#define MAPPATH "DyD2_logs/" // Path for the map after training
#define POSTMAPPATH "DyD2_logs/" //Path for the map after testing
#define LOGPATH "DyD2_logs/" //Path for the log data



//Parameters used for reading/writing files
#define SKIPPINGLINE 2 //Number of line to skip when reading the data file (in case of headers in the file)
#define FILESIZEMAX 100000 //Maximum amount of observations in a data set
#define MAXPATHLENGTH 250 //Maximum legnth of a path name
#define ROWNUMBER 2 //Number of column in the input data file
#define INDEX 0 //index column selection
#define FEATUREROW 1 //Dataset column selection

#define LABELNUMBERDYD2 11 //Number of columns in the label file
#define LABELSKIPPINGLINE 3  //Number of line to skip when reading the label file (in case of headers in the file)
#define LABELROW 0 //Row of the true class information in the label file
#define FAULTVALUE 5 //Label considered as fault in the label file


//DyD2 global variables
#define LOGROWDYD2 5
#define FAULTLOGROWDYD2 3



//Christophel part (NOT YET IMPLEMENTED)
#define SCORETHRESHOLD 0 // Used for Christoffel
#define BATPATH "H:\\DIAG_RAD\\Programs\\christoffel-envelope\\tests\\run_Dyd2_christoffel.bat" //Path of the bat file executing christoffel python script
#define CHRISTOFFELMAPPATH "H:\\DIAG_RAD\\Results\\DyD2\\dummy\\christoffelMap.txt" //Temporary map created to store data while running christoffel
#define INNERCHRISTOFFELPATH "H:\\DIAG_RAD\\Results\\DyD2\\dummy\\christoffelOuterMap.pkl" //Path to christoffel.pkl object for the outer map
#define OUTERCHRISTOFFELPATH "H:\\DIAG_RAD\\Results\\DyD2\\dummy\\christoffelInenrMap.pkl" //Path to christoffel.pkl object for the inner map


/*
#include <time.h>
#include <stdbool.h>
#include "uCluster.h"
#include "inout.h"
#include "math_HCE.h"
#include "christoffel.h"
#include "utility.h"
#include "DyD2.h"
*/

#endif