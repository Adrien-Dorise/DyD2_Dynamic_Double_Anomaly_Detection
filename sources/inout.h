/*
This file regroups the functions related to communicating with the folder system.
It means creating, reading and writing in a file.
It also regroups the function related to the save of DyD2 processes, such as saving detection maps

Note to self: Creating a save system in C is HELL !!!
*/


#pragma once
#ifndef INOUT_H
#define INOUT_H

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parameters.h"
//#include "uCluster.h"


#define PARSERSIZE 30
#define MAXCHARPERLINE 500 //Define the maximum number of character allowed per line for the fgets function


/*
* parser class is used to distinguish between different values of uCluster attribute.
* Each string variable correspond to a token in the saving format created in this code.
* The format is as follow (DONT FORGET TO MODIFY INITPARSER & SAVE/LOAD FUNCTIONS IF SOMETHING IS CHANGED):
* {Global_parameters]
* Min_value : X
* Max_value : X
* 
* [uCluster]
* uCluster_number : X
* feature_number : X
* data_number : X
* density : X
* size : X X ...
* center : X X ..
* creation_date : X
* last_update : X
*
* {uCluster]
* ...
*/
struct parser
{
	int globalParametersNumber;
	int uClusterParametersNumber;
	char globalParameters[PARSERSIZE];
	char minValue[PARSERSIZE];
	char maxValue[PARSERSIZE];
	char newUCluster[PARSERSIZE];
	char number[PARSERSIZE];
	char featureNumber[PARSERSIZE];
	char dataNumber[PARSERSIZE];
	char density[PARSERSIZE];
	char size[PARSERSIZE];
	char center[PARSERSIZE];
	char score[PARSERSIZE];
	char creationDate[PARSERSIZE];
	char lastUpdate[PARSERSIZE];
};

/*
* initParser
* Initialise a parser with predefined tokens
* Input:
*	p: type = pointer to parser, the aprser to intialise
* Output: None
*/
void initParser(struct parser* p);

/*
* CreateFile
* Creates a new file after checking its existence. If the file already exists, a message prompt is displayed to aske about its overwriting.
* Input: 
*	filename: type = string, the path of the file you want to create (C:example\\path\\file.txt)
*	autoOverwrite: type = bool, if set to false, we check and send a warning if the file already exist. If true, we automatically overwrite the file
* Output: error value
*/
int createFile(char fileName[], bool autoOverwrite);

/*
* writeData
* Add data to a file. The data are floats that will be put on a single line of the file. The file is not overwritten if already existing
* Input:
*	filename: type = string, the path of the file you want to modify (C:example\\path\\file.txt)
*	data: type = array of float, array containing the data to put into the file
*	dataSize : type = int, number of the data in the array to put in the text file
* Output: type = int, error value
*/
int writeData(char fileName[], float data[], int dataSize);



/*
* readData
* Put data from a file to a variable. The file is not overwritten if already existing
* https://stackoverflow.com/questions/11656532/returning-an-array-using-c
* Input:
*   data: type = float[][], variable where the file data are going to be stored. Be careful as the row size of this parameter MUST BE EQUAL to rows parameter
*	filename: type = string, the path of the file where the data are(C:example\\path\\file.txt)
*	rows: type = int, number of rows to take out from the data file. It must not be higher than LABELNUMBERDYD2 in parameters
*	skipingLine: type = int, Number of lines to skip (in case of header/title in the file)
* Output: type = int, error value or the number of data lines found if no error
*/
int readData(float data[][LABELNUMBERDYD2], char fileName[], int rows, int skippingLine);


/*
* writeFile
* Add a string into a file. The file is not overwritten if already existing
* Input:
*   data: type = array of float, variable where the file data are going to be stored
*	filename: type = string, the path of the file where the data are (C:example\\path\\file.txt)
*	string: type = char[], String to put into the file ("Example of a string\n")
* Output: type = int, error value
*/
int writeFile(char fileName[], char string[]);


/*
* saveUCluster
* Save uCluster in a txt file. The file is not overwritten if already existing
* The format is as follow:
* [uCluster]
* uCluster_number : X
* feature_number : X
* data_number : X
* density : X
* size : X X ...
* center : X X ..
* creation_date : X
* last_update : X
* Input:
*   uCl: type = uCluster, uCluster variable that will be saved in a file
*	filename: type = string, the path of the file (C:example\\path\\file.txt)
*	uClusterNumber: type =int, Number associated to the saved uCluster. It will be stored in the "uCluster_number" variable in the file
* Output: type = int, error value
*/
int saveUCluster(struct uCluster uCl, char fileName[], int uClusterNumber);

/*
* loadUCluster
* Load uCluster from a file. We only load the uCluster corresponding to the number given
* Input:
*   uCl: type = pointer to uCluster, uCluster variable that will be replaced by the uCLuster in the file. The values inside the uCluster will be replaced. You have to make sure that the uCluster size correspond to the one loaded.
*	filename: type = string, the path of the file (C:example\\path\\file.txt)
*	desiredUClusterNumber: type = int, Number associated to the loaded uCluster. Only the corresponding uCluster in the file will be loaded.
* Output: type = int, error value
*/
int loadUCluster(struct uCluster* uCl, char fileName[], int desiredUClusterNumber);

/*
* saveMap
* Save all the uCLuster contained in a map into a file.
* Input:
*   M: type = map, map containing all uClusters to save
*	filename: type = string, the path of the file (C:example\\path\\file.txt)
*	minMaxValues: type = char[2], array containing the minimum and maximum value found during training (used for scaling)
* Output: type = int, error value
*/
int saveMap(struct map M, char fileName[], float minMaxValues[2]);

/*
* loadMap
* Load the uClusters contained in a file into a map variable. The file is not overwritten if already existing.
* Input:
*   M: type = pointer to map, map that will contain the uClusters. You have to be carefull about the size of the map (mostly size and center variables). Also it is best to declare the map as a static variable as it can quickly overflow the stack.
*	filename: type = string, the path of the file (C:example\\path\\file.txt)
* 	minMaxValues: type = char[2], array containing the minimum and maximum value found during training (used for scaling)
* Output: type = int, error value
*/
int loadMap(struct map* M, char fileName[], float minMaxValues[2]);


/*
* getLabels
* Get the labels inside a .txt file in a single array.
* Be careful to correctly set skippingLines and LABELNUMBERDYD2 parameters correctly
* Input:
*	label: type = char[], name of the file containing the labels
*	labels: type = float[][], array that will contain the label values
*	labelsNumber: type = int, Number of labels to extract (or number of rows in the label file)
*	skippingLines: type = int, number of lines to skip (in case of header in the label file)
* Output: type = int, error value or number of lines in the label file.
*/
int getLabels(char labelFile[], int labels[][LABELNUMBERDYD2], int labelsNumber, int skippingLines);

#endif
