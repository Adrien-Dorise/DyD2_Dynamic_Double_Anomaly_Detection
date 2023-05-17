#include "DyD2.h"






void training(float trainingSet[][OUTERFEATURENUMBER], int trainingSize, struct map* outerMap, struct map* innerMap)
{
	float outerMinMax[2] = { 0,0 };
	float innerMinMax[2] = { 100000,0 };
	float tempMinMax[2] = { 0,0 };
	float tempOuter[OUTERFEATURENUMBER] = { 0 };
	float tempInner[INNERFEATURENUMBER] = { 0 };
	struct sample S;
	struct uCluster uCl;
	float sizeOuter[OUTERFEATURENUMBER] = { 0 }, sizeInner[INNERFEATURENUMBER] = {0};
	bool isReach = false;
	int reachSize = 0;
	int reachablesID[MAXUCLUSTERS] = { 0 };
	float tempWindow[WINDOWSIZE] = { 0 };


	printf("\nDyD2 training phase start\n");


	for (int i = 0; i < trainingSize; i++)
	{
		for (int j = 0; j < OUTERFEATURENUMBER; j++)
		{
			outerSetDyD2[i][j] = trainingSet[i][j];
		}
	}

	minMax2D(outerMinMax, outerSetDyD2, trainingSize, OUTERFEATURENUMBER);
	scale2D(outerMinMax, outerSetDyD2, trainingSize, OUTERFEATURENUMBER);
	

	//Getting maps size
	for (int i = 0; i < OUTERFEATURENUMBER; i++)
	{
		sizeOuter[i] = UCLSIZEOUTER;
	}
	for (int i = 0; i < INNERFEATURENUMBER; i++)
	{
		sizeInner[i] = UCLSIZEINNER;
	}



	//training for outerMap
	initMap(outerMap);
	for (int i = 0; i < trainingSize; i++)
	{
		//Initialisaiton of Sp
		for (int feat = 0; feat < OUTERFEATURENUMBER; feat++)
		{
			tempOuter[feat] = outerSetDyD2[i][feat];
		}
		initSample(&S, OUTERFEATURENUMBER, i, tempOuter);

		//We have to see if the sample is inside existing uCluster
		isReach = isReachable(*outerMap, S);
		if (isReach)
		{
			reachSize = reachableMap(reachablesID, *outerMap, S);
			for (int i = 0; i < reachSize; i++)
			{
				//updateUCluster(&outerMap->uCl[reachablesID[i]], S);
				setUClusterDensity(&outerMap->uCl[reachablesID[i]], outerMap->uCl[reachablesID[i]].dataNumber + 1, S.date);
			}
		}
		else
		{
			initUCluster(&uCl, S, sizeOuter);
			addMap(outerMap, uCl);
		}
	}
	outerMap->trainMinMax[0] = outerMinMax[0];
	outerMap->trainMinMax[1] = outerMinMax[1];


	//training for innerMap
	initMap(innerMap);

	//Find inner minMax
	for (int i = 0; i < trainingSize; i++)
	{
		if (i < WINDOWSIZE)
		{
			for (int feat = 0; feat < INNERFEATURENUMBER; feat++)
			{
				windowSetDyD2[i][feat] = trainingSet[i][feat];
			}

		}
		else
		{
			//For now, the feature extraction is only done for one outer features
			for (int p = 0; p < WINDOWSIZE; p++)
			{
				tempWindow[p] = windowSetDyD2[p][0];
			}
			featureExtraction(tempWindow, &S, i);
			for (int feat = 0; feat < INNERFEATURENUMBER; feat++)
			{
				tempInner[feat] = S.features[feat];
			}
			minMax(tempMinMax, tempInner, INNERFEATURENUMBER);
			if (tempMinMax[0] < innerMinMax[0])
			{
				innerMinMax[0] = tempMinMax[0];
			}
			if (tempMinMax[1] > innerMinMax[1])
			{
				innerMinMax[1] = tempMinMax[1];
			}

			for (int feat = 0; feat < OUTERFEATURENUMBER; feat++)
			{
				tempOuter[feat] = trainingSet[i][feat];
			}
			shift2DOuter(windowSetDyD2, WINDOWSIZE, tempOuter);
		}
	}
	innerMap->trainMinMax[0] = innerMinMax[0];
	innerMap->trainMinMax[1] = innerMinMax[1];

	//Perform inner map training
	for (int i = 0; i < trainingSize; i++)
	{
		if (i < WINDOWSIZE)
		{
			for (int feat = 0; feat < INNERFEATURENUMBER; feat++)
			{
				windowSetDyD2[i][feat] = trainingSet[i][feat];
			}
			
		}
		else
		{
			//For now, the feature extraction is only done for one outer features
			for (int p = 0; p < WINDOWSIZE; p++)
			{
				tempWindow[p] = windowSetDyD2[p][0];
			}
			featureExtractionScaled(tempWindow, &S, i,innerMinMax);

			//We have to see if the sample is inside existing uCluster
			isReach = isReachable(*innerMap, S);
			if (isReach)
			{
				reachSize = reachableMap(reachablesID, *innerMap, S);
				for (int i = 0; i < reachSize; i++)
				{
					//updateUCluster(&innerMap->uCl[reachablesID[i]], S);
					setUClusterDensity(&innerMap->uCl[reachablesID[i]], innerMap->uCl[reachablesID[i]].dataNumber + 1, S.date);

				}
			}
			else
			{
				initUCluster(&uCl, S, sizeInner);
				addMap(innerMap, uCl);
			}

			for (int feat = 0; feat < OUTERFEATURENUMBER; feat++)
			{
				tempOuter[feat] = trainingSet[i][feat];
			}
			shift2DOuter(windowSetDyD2, WINDOWSIZE, tempOuter);

			
		}
	}
	
	

	/*
	for (int i = 0; i < outerMap.uClNumber; i++)
	{
		outerMap.uCl[i].lastUpdate = 1;
	}


	for (int i = 0; i < innerMap.uClNumber; i++)
	{
		innerMap.uCl[i].lastUpdate = 1;
	}
	*/

	//iteration = -1; //We put -1 because we update iterations at the beginning and want to start at 0

	printf("\nDyD2 training finished\n");
}


void saveMaps(struct map outerMap, struct map innerMap, char savePath[])
{
	char outerPath[MAXPATHLENGTH] = "";
	char innerPath[MAXPATHLENGTH] = "";

	strcat(outerPath, savePath);
	strcat(outerPath, "outerMap.txt");

	strcat(innerPath, savePath);
	strcat(innerPath, "innerMap.txt");


	printf("\nMap saving\n");
	saveMap(outerMap, outerPath, outerMap.trainMinMax);
	printf("Outer map has %i uClusters\n", outerMap.uClNumber);

	saveMap(innerMap, innerPath, innerMap.trainMinMax);
	printf("Inner map has %i uClusters\n", innerMap.uClNumber);

	printf("\nMap saving done\n");

}

void dataAcquisition(int iteration, float data[][OUTERFEATURENUMBER+1], float dataSave[][OUTERFEATURENUMBER*2], float lastPoint[])
{
	for (int feat = 0; feat < OUTERFEATURENUMBER; feat++)
	{
		lastPoint[feat] = data[iteration][feat+1];
		dataSave[iteration][feat] = data[iteration][feat+1];

	}
}

bool changePointDetection(struct sample Sp, struct uCluster* uClRupt)
{
	bool rupt = false;
	float distance;
	distance = reachable(*uClRupt, Sp);
	if (distance < 0) //If data point not in uCluster
	{
		rupt = true;
		initUCluster(uClRupt, Sp, uClRupt->size);
	}
	else
	{
		rupt = false;
		updateUCluster(uClRupt, Sp);

	}
	return rupt;

}

bool anomalyDetection(struct sample S, struct map map)
{
	//Return true if sample not reachable => An anomaly is detected
	return !isReachable(map, S);	
}


void featureExtraction(float window[], struct sample* Sw, float date)
{
	float features[MAXFEATURES];
	float minMaxRes[2];

	minMax(minMaxRes, window, WINDOWSIZE);

	features[0] = mean(window, WINDOWSIZE);
	features[1] = variance(window, WINDOWSIZE);
	features[2] = strdDev(window, WINDOWSIZE);
	features[3] = MAD(window, WINDOWSIZE);
	features[4] = minMaxRes[0];
	features[5] = minMaxRes[1];
	//features[6] = skewness(window, WINDOWSIZE);
	//features[7] = kurtosis(window, WINDOWSIZE);





	initSample(Sw, INNERFEATURENUMBER, date, features);
	
}

void featureExtractionScaled(float window[], struct sample* Sw,  float date, float dataMinMax[2])
{
	float features[MAXFEATURES];
	float minMaxRes[2];

	minMax(minMaxRes, window, WINDOWSIZE);

	features[0] = mean(window, WINDOWSIZE);
	features[1] = variance(window, WINDOWSIZE);
	features[2] = strdDev(window, WINDOWSIZE);
	features[3] = MAD(window, WINDOWSIZE);
	features[4] = minMaxRes[0];
	features[5] = minMaxRes[1];
	//features[6] = skewness(window, WINDOWSIZE);
	//features[7] = kurtosis(window, WINDOWSIZE);

	scale(features, dataMinMax[0], dataMinMax[1], INNERFEATURENUMBER);
	initSample(Sw, INNERFEATURENUMBER, date, features);

}


void mapUpdate(struct sample S, struct map* map, float date, float ageThreshold, float agePenalty, float dataNumberMin, float scoreThreshold, char christoffelObjectPath[])
{
	struct uCluster uCl;
	int reachSize;
	int closestuClID;
	int reachablesID[MAXUCLUSTERS];


	//OPTION1: Update only closest uCluster from sample (Note: we still have to update lastUpdate from reachable uCluster to avoid execessive ageing)
	closestuClID = closestUCl(*map, S);
	updateUCluster(&map->uCl[closestuClID], S);
	reachSize = reachableMap(reachablesID, *map, S);
	if (reachSize != 0)
	{
		for (int i = 0; i < reachSize; i++)
		{
			if (reachablesID[i] != closestuClID)
			{
				int b = reachablesID[i];
				setUClusterDensity(&map->uCl[reachablesID[i]], map->uCl[reachablesID[i]].dataNumber + 1, S.date);
			}
		}

	}
	

	
	//OPTION2: Update all reachable uClusters positions
	/*
	closestuClID = closestUCl(*map, S);
	reachSize = reachableMap(reachablesID, *map, S);
	if (reachSize == 0)
	{
		closestuClID = closestUCl(*map, S);
		updateUCluster(&map->uCl[closestuClID], S);
	}
	else
	{
		
		for (int i = 0; i < reachSize; i++)
		{
			updateUCluster(&map->uCl[reachablesID[i]], S);
		}
		
	}
	*/

	//Ageing process
	mapAgeing(map, date, ageThreshold, agePenalty);

	if ((int)date % 100 == 0)
	{
		//printf("\ndate %f\n", date);
		//fitChristoffel(map, BATPATH, CHRISTOFFELMAPPATH, "predict", christoffelObjectPath);
		//mapScoring(map, SCORETHRESHOLD);
	}
	

	//Replacing obsolete uClusters
	for (int i = 0; i < map->uClNumber; i++)
	{
		//Density check
		if (map->uCl[i].dataNumber <= dataNumberMin)
		{
			//printf("\nuCluster %i replaced because not dense enough\n" , i);
			initUCluster(&(map->uCl[i]), S, map->uCl[0].size);
			setUClusterDensity(&(map->uCl[i]), 98+1, S.date);
			//replaceUCluster(map, uCl, i);

		}

		//Christoffel check
		if(map->uCl[i].score > scoreThreshold)
		{
			//printf("\nuCluster %i replaced because of christoffel score\n", i);
			initUCluster(&(map->uCl[i]), S, map->uCl[0].size);
			setUClusterDensity(&uCl, dataNumberMin+1, S.date);
			//replaceUCluster(map, uCl, i);
		}
	}
	int a = 1;
}

int resultSave(char postMapPath[], char logFolderPath[], int faultIter, float dataSave[][OUTERFEATURENUMBER * 2], int dataSize, float logSave[][5], float faultLogSave[][3])
{
	char logPath[MAXPATHLENGTH] = "";
	char faultLogPath[MAXPATHLENGTH] = "";
	char postOuterPath[MAXPATHLENGTH] = "";
	char postInnerPath[MAXPATHLENGTH] = "";
	int faultID = 0;
	FILE* logFile, * faultLogFile;

	strcat(logPath, logFolderPath);
	strcat(logPath, "logDyD2.txt");

	strcat(faultLogPath, logFolderPath);
	strcat(faultLogPath, "faultLogDyD2.txt");

	strcat(postOuterPath, postMapPath);
	strcat(postOuterPath, "outerMapPost.txt");

	strcat(postInnerPath, postMapPath);
	strcat(postInnerPath, "innerMapPost.txt");

	//Log init
	createFile(logPath, true);
	logFile = fopen(logPath, "a");
	if (logFile == NULL)
	{
		printf("ERROR addData: error opening file %s\n", logFile);
		return 1;
	}
	fprintf(logFile, "Log of Dyd2\n");
	fprintf(logFile, "Training on train set: %s\n", TRAINFILE);
	fprintf(logFile, "Testing on test set: %s\n", TESTFILE);
	fprintf(logFile, "Window size: %i\n", WINDOWSIZE);
	fprintf(logFile, "UCL size rupture: %f\n", UCLSIZERUPT);
	fprintf(logFile, "UCL size outer: %f\n", UCLSIZEOUTER);
	fprintf(logFile, "UCL size inner: %f\n", UCLSIZEINNER);
	fprintf(logFile, "feature number: %i\n", INNERFEATURENUMBER);
	fprintf(logFile, "age threshold raw uCl: %i\n", OUTERDENSITYTHRESHOLD);
	fprintf(logFile, "age threshold precise uCl: %i\n", INNERDENSITYTHRESHOLD);
	fprintf(logFile, "score threshold: %f\n", SCORETHRESHOLD);
	fprintf(logFile, "age penalty: %f\n", AGEPENALTY);
	fprintf(logFile, "Iteration,Time(s),Value,Scaled_value,Code_Outer,Code_Inner,Reset_State\n");
	for (int i = 0; i < dataSize; i++)
	{
		//printf("Log: %i / %i\n", i, testSize);
		//fprintf(logFile, "%i\t%f\t%f\t%f\t%i\t%i\t%i\n", RADiterationSave[i], RADtimeTest[i], RADdata[i], RADdataScale[i], RADcodeOuterSave[i], RADcodeInnerSave[i], RADresetSave[i]);
		fprintf(logFile, "%f,%f,%f,%f,%f,%f,%f\n", logSave[i][0], logSave[i][1], dataSave[i][0], dataSave[i][1], logSave[i][2], logSave[i][3], logSave[i][4]);

	}
	fclose(logFile);

	createFile(faultLogPath, true);
	writeFile(faultLogPath, "Fault log of Dyd2\n");
	writeFile(faultLogPath, "Iteration,Time(s),Value,Code\n");

	//Saving
	printf("\n%i faults were recorded for %i samples", faultIter, dataSize);
	printf("\n%f seconds past", execTimeDyD2);
	printf("\nSave in progress...\n");
	
	

	faultLogFile = fopen(faultLogPath, "a");
	for (int i = 0; i < faultIter; i++)
	{
		faultID = faultLogSave[i][0];
		//printf("Fault Log: %i / %i\n", i, faultIter);
		//fprintf(faultLogFile, "%i\t%f\t%f\t%i\n", RADfaultIteration[i], RADfaultTime[i], RADfaultData[i], RADfaultCode[i]);
		fprintf(faultLogFile, "%f,%f,%f,%f\n", faultLogSave[i][0], faultLogSave[i][1], dataSave[faultID][1], faultLogSave[i][2]);
	}
	fclose(faultLogFile);
	
	saveMap(outerMapDyD2, postOuterPath, outerMapDyD2.trainMinMax);
	saveMap(innerMapDyD2, postInnerPath, innerMapDyD2.trainMinMax);
	
	printf("Save complete!\n");
	return 0;

}

//Here we always reset DyD2. This option is mostly used when no anomalies are present in the test set.
bool resetDyD2Version0(bool* interrupt, int* codeOuter, int* codeInner, int codeOuterMemory[], int codeInnerMemory[])
{
	struct sample S;
	bool isReset = false;
	if (*interrupt)
	{

		isReset = true;
		*interrupt = false;
		*codeOuter = 0;
		*codeInner = 0;
		for (int i = 0; i < WINDOWSIZE; i++)
		{
			codeOuterMemory[i] = 0;
			codeInnerMemory[i] = 0;
		}
		
	}
	return isReset;
}


//Here we use DyD2 to determine when it has to reset. When the next sample is reachable by outerMap, DyD2 is reset.
bool resetDyD2Version1(bool* interrupt, struct map outerMap, float lastPoint[], int* codeOuter, int* codeInner, int codeOuterMemory[], int codeInnerMemory[])
{
	struct sample S;
	bool isReset = false;
	if (*interrupt)
	{
		initSample(&S, OUTERFEATURENUMBER, 0, lastPoint);
		if (isReachable(outerMap, S))
		{
			isReset = true;
			*interrupt = false;
			*codeOuter = 0;
			*codeInner = 0;
			for (int i = 0; i < WINDOWSIZE; i++)
			{
				codeOuterMemory[i] = 0;
				codeInnerMemory[i] = 0;
			}
		}
	}
	return isReset;
}

//Combination of method 1 and 3
bool resetDyD2Version2(bool* interrupt, float label, float faultValue, int* codeOuter, int* codeInner, int codeOuterMemory[], int codeInnerMemory[])
{
	bool isReset = false;
	if (*interrupt)
	{
		if (label != faultValue)
		{
			isReset = true;
			*interrupt = false;
			*codeOuter = 0;
			*codeInner = 0;
			for (int i = 0; i < WINDOWSIZE; i++)
			{
				codeOuterMemory[i] = 0;
				codeInnerMemory[i] = 0;
			}
		}
	}
	return isReset;
}

//Here we use the set labels. While the fault is still active, we interrupot DyD2.
bool resetDyD2Version3(bool* interrupt, struct map outerMap, float lastPoint[], float label, float faultValue, int* codeOuter, int* codeInner, int codeOuterMemory[], int codeInnerMemory[])
{
	struct sample S;
	bool isReset = false;
	if (*interrupt)
	{
		initSample(&S, OUTERFEATURENUMBER, 0, lastPoint);
		if (label != faultValue && isReachable(outerMap, S))
		{
			isReset = true;
			*interrupt = false;
			*codeOuter = 0;
			*codeInner = 0;
			for (int i = 0; i < WINDOWSIZE; i++)
			{
				codeOuterMemory[i] = 0;
				codeInnerMemory[i] = 0;
			}
		}
	}
	return isReset;
}

//Data set must have time on first row !
void offlinePhase(char trainingPath[], char mapSavePath[])
{
	int trainingSize;

	trainingSize = readData(setFromFile, trainingPath, ROWNUMBER, SKIPPINGLINE);
	for (int i = 0; i < trainingSize; i++)
	{
		for (int feat = 0; feat < OUTERFEATURENUMBER; feat++)
		{
			dataSetFromFile[i][feat] = setFromFile[i][feat + FEATUREROW];
		}

	}
	training(dataSetFromFile, trainingSize, &outerMapDyD2, &innerMapDyD2);
	//We set outer and inner maps dataNumber to the threhosl so they do not get erased when the online phase starts
	for (int i = 0; i < outerMapDyD2.uClNumber; i++)
	{
		if (outerMapDyD2.uCl[i].dataNumber < OUTERDENSITYTHRESHOLD)
		{
			outerMapDyD2.uCl[i].dataNumber = OUTERDENSITYTHRESHOLD;
		}
	}
	for (int i = 0; i < innerMapDyD2.uClNumber; i++)
	{
		if (innerMapDyD2.uCl[i].dataNumber < INNERDENSITYTHRESHOLD)
		{
			innerMapDyD2.uCl[i].dataNumber = INNERDENSITYTHRESHOLD;
		}
	}
	
	//Christoffel part
	//fitChristoffel(&outerMapDyD2, BATPATH, CHRISTOFFELMAPPATH, "fit", OUTERCHRISTOFFELPATH);
	//fitChristoffel(&innerMapDyD2, BATPATH, CHRISTOFFELMAPPATH, "fit", INNERCHRISTOFFELPATH);

	saveMaps(outerMapDyD2, innerMapDyD2, mapSavePath);


}

//We use innerMap and outerMap from parameters.h
int onlinePhase(char testPath[], char postMapSavePath[], char logPath[])
{
	int testSize = 0;
	float ruptSize[OUTERFEATURENUMBER] = { 0 };
	float lastPoint[OUTERFEATURENUMBER] = { 0 };
	float lastPointScaled[OUTERFEATURENUMBER] = { 0 };
	int codeOuter = 0, codeInner = 0;
	bool codeRupt = false;
	bool interrupt = false;
	int faultIter = 0;
	int codeOuterMemory[WINDOWSIZE] = { 0 }, codeInnerMemory[WINDOWSIZE] = { 0 };
	float tempWindow[WINDOWSIZE] = { 0 };

	t1DyD2 = clock();

	//OPTION1: Test data pre-recorded in a file
	testSize = readData(setFromFile, testPath, ROWNUMBER, SKIPPINGLINE);
	for (int i = 0; i < testSize; i++)
	{
		testDate[i] = setFromFile[i][INDEX];
		for (int feat = 0; feat < OUTERFEATURENUMBER; feat++)
		{
			outerSetDyD2[i][feat] = setFromFile[i][feat + FEATUREROW];
		}
	}
	

	//Initialisation of window
	for (int iteration = 0; iteration < WINDOWSIZE; iteration++)
	{
		//Getting new data point
		dataAcquisition(iteration, setFromFile, dataSave, lastPoint);

		
		for (int feat = 0; feat < OUTERFEATURENUMBER; feat++)
		{
			windowSetDyD2[iteration][feat] = lastPoint[feat];
			lastPointScaled[feat] = lastPoint[feat];
			scale(lastPointScaled, outerMapDyD2.trainMinMax[0], outerMapDyD2.trainMinMax[1], OUTERFEATURENUMBER);
			dataSave[iteration][feat + OUTERFEATURENUMBER] = lastPointScaled[feat];
		}
		logSave[iteration][0] = iteration;
		logSave[iteration][1] = testDate[iteration];
		
	}
	
	//Rupture map init
	for (int i = 0; i < OUTERFEATURENUMBER; i++)
	{
		ruptSize[i] = UCLSIZERUPT;
		lastPointScaled[i] = lastPoint[i];
	}
	scale(lastPointScaled, outerMapDyD2.trainMinMax[0], outerMapDyD2.trainMinMax[1], OUTERFEATURENUMBER);
	initSample(&Sp, OUTERFEATURENUMBER, testDate[WINDOWSIZE-1], lastPointScaled);
	initUCluster(&ruptMapDyD2, Sp, ruptSize);

	//Init maps parameter lastUpdate to current date
	setLastUpdate(&outerMapDyD2, testDate[WINDOWSIZE-1]);
	setLastUpdate(&innerMapDyD2, testDate[0]);




	//Beginning loop
	for (int iteration = WINDOWSIZE; iteration < testSize; iteration++)
	{
		//Getting new data point
		dataAcquisition(iteration, setFromFile, dataSave, lastPoint);

		//Scaling of current iteration
		for (int i = 0; i < OUTERFEATURENUMBER; i++)
		{
			lastPointScaled[i] = lastPoint[i];
		}

		scale(lastPointScaled, outerMapDyD2.trainMinMax[0], outerMapDyD2.trainMinMax[1], OUTERFEATURENUMBER);
		for (int feat = 0; feat < OUTERFEATURENUMBER; feat++)
		{
			dataSave[iteration][feat + OUTERFEATURENUMBER] = lastPointScaled[feat];
		}

		//Stop DYD2 in case of anomaly
		if (codeOuter == -1 || codeInner == -2)
		{
			interrupt = true;
		}

		if (!interrupt)
		{

			//Change point detection
			initSample(&Sp, OUTERFEATURENUMBER, testDate[iteration], lastPointScaled);
			codeOuter = 0;
			codeInner = 0;

			//Change point detection
			codeRupt = changePointDetection(Sp, &ruptMapDyD2);
			if (codeRupt) //Rupture is detected
			{
				//Outer anomaly detection
				if (anomalyDetection(Sp, outerMapDyD2))
				{
					//Outer anomaly detected
					codeOuter = -1;
					faultLogSave[faultIter][0] = iteration;
					faultLogSave[faultIter][1] = testDate[iteration];
					faultLogSave[faultIter][2] = -1;
					faultIter++;
				}
				else
				{
					//No outer anomaly detected
					codeOuter = 1;
				}
			}
			shiftIntArray(codeOuterMemory, WINDOWSIZE, codeOuter);


			//Inner anomaly detection
			if (codeOuter != -1)
			{
				shift2DOuter(windowSetDyD2, WINDOWSIZE, lastPoint);
				if (codeOuterMemory[0] == 1)
				{
					//Rupture without outer anomaly detected previously

					//Feature extraction phase
					//For now, the feature extraction is only done for one outer features
					for (int p = 0; p < WINDOWSIZE; p++)
					{
						tempWindow[p] = windowSetDyD2[p][0];
					}
					featureExtractionScaled(tempWindow, &Sw, testDate[iteration - WINDOWSIZE], innerMapDyD2.trainMinMax);

					//Inner anomaly detection phase
					if (anomalyDetection(Sw, innerMapDyD2))
					{
						//Inner anomaly detected
						codeInner = -2;
						faultLogSave[faultIter][0] = iteration;
						faultLogSave[faultIter][1] = testDate[iteration];
						faultLogSave[faultIter][2] = -2;
						faultIter++;
					}
					else
					{
						//No inner anomaly detected
						codeInner = 2;
					}
				}
				//End of detection
			}
			shiftIntArray(codeInnerMemory, WINDOWSIZE, codeInner);



			//Maps update

			//Outer map update with current data point and inner map with first point of window
			if (codeOuter == 0 && codeInner == 0)
			{
				mapUpdate(Sp, &outerMapDyD2, testDate[iteration], AGELIMIT, AGEPENALTY, OUTERDENSITYTHRESHOLD, SCORETHRESHOLD, OUTERCHRISTOFFELPATH);
				//featureExtraction(windowSetDyD2, &Sw, testDate[iteration - WINDOWSIZE]);
				//mapUpdate(Sw, &innerMapDyD2, testDate[iteration - WINDOWSIZE], INNERDENSITYTHRESHOLD, AGEPENALTY, AGELIMIT, SCORETHRESHOLD, SCORETHRESHOLD);
			}


			//Update of inner map at each non fault (ALWAYS UPDATE MODE)
			//!!! Have to add the case when a fault occured during the time window !!!
			if (logSave[iteration][2] == 0 && codeInner == 0)
			{
				for (int feat = 0; feat < OUTERFEATURENUMBER; feat++)
				{
					lastPoint[feat] = windowSetDyD2[0][feat];
				}
				featureExtractionScaled(windowSetDyD2, &Sw, testDate[iteration - WINDOWSIZE], innerMapDyD2.trainMinMax);
				mapUpdate(Sw, &innerMapDyD2, testDate[iteration - WINDOWSIZE], AGELIMIT, AGEPENALTY, INNERDENSITYTHRESHOLD, SCORETHRESHOLD, INNERCHRISTOFFELPATH);

			}


			//Update of both maps from earlier normal data point
			if (codeInner == 2) //We update both map, as it means that codeOuter == 1 when the point arrived at the current iteration and raw map wasn't updated
			{
				//printMap(outerMap);
				//printMap(innerMap);
				for (int feat = 0; feat < OUTERFEATURENUMBER; feat++)
				{
					lastPoint[feat] = windowSetDyD2[0][feat];
					lastPointScaled[feat] = lastPoint[feat];
				}
				scale(lastPointScaled, outerMapDyD2.trainMinMax[0], outerMapDyD2.trainMinMax[1], OUTERFEATURENUMBER);
				initSample(&Sp, OUTERFEATURENUMBER, testDate[iteration - WINDOWSIZE], lastPointScaled);
				mapUpdate(Sp, &outerMapDyD2, testDate[iteration - WINDOWSIZE], AGELIMIT, AGEPENALTY, OUTERDENSITYTHRESHOLD, SCORETHRESHOLD, OUTERCHRISTOFFELPATH);
				featureExtractionScaled(windowSetDyD2, &Sw, testDate[iteration - WINDOWSIZE], innerMapDyD2.trainMinMax);
				mapUpdate(Sw, &innerMapDyD2, testDate[iteration - WINDOWSIZE], AGELIMIT, AGEPENALTY, INNERDENSITYTHRESHOLD, SCORETHRESHOLD, INNERCHRISTOFFELPATH);
			}
		}
		


		//Reset the aglorithm in case of anomaly
		//if (resetDyD2Version0(&interrupt, &codeOuter, &codeInner, codeOuterMemory, codeInnerMemory)) //Always reset
		if (resetDyD2Version1(&interrupt, outerMapDyD2, lastPointScaled, &codeOuter, &codeInner, codeOuterMemory, codeInnerMemory)) //Reset with outermap
		//if (resetDyD2Version2(&interrupt, testLabelsDyD2[iteration][LABELROW], FAULTVALUE, &codeOuter, &codeInner, codeOuterMemory, codeInnerMemory)) //Reset1 + Reset2
		//if (resetDyD2Version3(&interrupt, outerMapDyD2, lastPoint, testLabelsDyD2[iteration][LABELROW], FAULTVALUE, &codeOuter, &codeInner, codeOuterMemory, codeInnerMemory)) //Reset with labels
		{
			logSave[iteration][4] = 1;
		}


		//Log file update
		logSave[iteration][0] = iteration;
		logSave[iteration][1] = testDate[iteration];
		logSave[iteration][2] = codeOuter;
		logSave[iteration][3] = codeInner;

		/*
		//Save map mid testing
		if (iteration == testSize / 2)
		{
			char outerPathMid[MAXPATHLENGTH] = "";
			char innerPathMid[MAXPATHLENGTH] = "";
			strcat(outerPathMid, "H:\\DIAG_RAD\\Results\\DyD2\\dummy\\");
			strcat(outerPathMid, "outerMapMid.txt");

			strcat(innerPathMid, "H:\\DIAG_RAD\\Results\\DyD2\\dummy\\");
			strcat(innerPathMid, "innerMapMid.txt");

			saveMap(outerMapDyD2, outerPathMid, outerMapDyD2.trainMinMax);
			saveMap(innerMapDyD2, innerPathMid, innerMapDyD2.trainMinMax);
		}
		*/
	}

	//Result save
	t2DyD2 = clock();
	execTimeDyD2 = (float)(t2DyD2 - t1DyD2) / CLOCKS_PER_SEC;
	resultSave(postMapSavePath, logPath, faultIter, dataSave, testSize, logSave, faultLogSave);

	return testSize;

}




void confusionMatrix(int results[4], float logData[][LOGROWDYD2], float testLabels[][LABELNUMBERDYD2], int trueLabelRow, int faultValue, int size, bool doAnomalyRemoval)
{
	// TP/FP/FN/TN
	bool isFaultPrediction = false;
	bool isFaultLabel = false;

	for (int i = 0; i < size; i++)
	{
		if (logData[i][2] == -1 || logData[i][3] == -2)
		{
			isFaultPrediction = true;
		}
		else
		{
			isFaultPrediction = false;
		}
		
		if (testLabels[i][trueLabelRow] == faultValue)
		{
			isFaultLabel = true;
		}
		else
		{
			isFaultLabel = false;
		}

		//TP
		if (isFaultPrediction == true && isFaultLabel == true) 
		{
			results[0]++;

			//Anomaly removal part
			if (doAnomalyRemoval)
			{
				
				while (isFaultLabel && i < size) //While the real class is an anomaly
				{
					i++;
					if (testLabels[i][trueLabelRow] == faultValue)
					{
						isFaultLabel = true;
					}
					else
					{
						isFaultLabel = false;
					}
					results[0]++;
				}
			}

		}
		
		
		//FP
		if (isFaultPrediction == true && isFaultLabel == false)
		{
			results[1]++;
		}

		//FN
		if (isFaultPrediction == false && isFaultLabel == true)
		{
			results[2]++;
		}

		//TN
		if (isFaultPrediction == false && isFaultLabel == false)
		{
			results[3]++;
		}
		
	}
}

void printResults(int results[4], float time[], char trainSet[], char testSet[], char labelSet[], int testIter)
{
	float resultsPercent[4] = { 0,0,0,0 };
	float sum = 0;
	float meanTime = 0;
	float totalTime = 0;


	printf("\n\nTraining on train set: %s\n", trainSet);
	printf("Testing on test set: %s\n", testSet);
	printf("Label file: %s\n", labelSet);
	printf("Window size: %i\n", WINDOWSIZE);
	printf("UCL size rupture: %f\n", UCLSIZERUPT);
	printf("UCL size outer: %f\n", UCLSIZEOUTER);
	printf("UCL size inner: %f\n", UCLSIZEINNER);
	printf("Outer feature number: %i\n", OUTERFEATURENUMBER);
	printf("Inner feature number: %i\n", INNERFEATURENUMBER);
	//printf("age threshold raw uCl: %i\n", OUTERDENSITYTHRESHOLD);
	//printf("age threshold precise uCl: %i\n", INNERDENSITYTHRESHOLD);
	//printf("score threshold: %f\n", SCORETHRESHOLD);
	printf("Ageing threshold: %f\n", AGELIMIT);
	printf("Ageing penalty: %f\n", AGEPENALTY);
	printf("\nResults are (TP / FP / FN / TN): %i / %i / %i / %i\n", results[0], results[1], results[2], results[3]);
	sum = results[0] + results[1] + results[2] + results[3];
	resultsPercent[0] = (results[0] / sum) * 100.0;
	resultsPercent[1] = (results[1] / sum) * 100.0;
	resultsPercent[2] = (results[2] / sum) * 100.0;
	resultsPercent[3] = (results[3] / sum) * 100.0;
	printf("Results in %% are: %f%% / %f%% / %f%% / %f%%\n", resultsPercent[0], resultsPercent[1], resultsPercent[2], resultsPercent[3]);
	
	if (time[0] != -1) //Put time[0] parameter at -1 to not print
	{
		for (int i = 0; i < testIter; i++)
		{
			meanTime += time[i];
			totalTime += time[i];
		}
		meanTime = meanTime / testIter;
		printf("\nMean time (s): %f\n", meanTime);
		printf("Total time (s): %f\n\n", totalTime);
	}
	
	

}


void validation(char trainFolder[], int trainIteration, char testFolder[], int testIteration, bool doAnomalyRemoval)
{
	//File parameters
	
	char trainFileName[] = "/normalCurrent.txt";
	char testNormalFileName[] = "/normalCurrent.txt";
	char testFileName[] = "/dataSet.txt";
	char labelFileName[] = "/statusData.txt";
	char mkdirStart[] = "mkdir ";
	char trainFile[500];
	char testFile[500];
	char normalTestFileDyD2[500];
	char labelFile[500];
	char trainNumber[10];
	char testNumber[10];
	char outerPath[500] = "";
	char innerPath[500] = "";

	//Results parameters
	int confMatrix[4] = { 0,0,0,0 };
	int testSize = 0;
	float execTime[100] = { -1 };

	//Map path initisalisation
	strcat(outerPath, MAPPATH);
	strcat(outerPath, "outerMap.txt");
	strcat(innerPath, MAPPATH);
	strcat(innerPath, "innerMap.txt");


	for (int trainingIt = 1; trainingIt <= trainIteration; trainingIt++)
	{
		//training folder name intialisation
		strcpy(trainFile, trainFolder); //C:training/datas
		sprintf(trainNumber, "%i", trainingIt);
		strcat(trainFile, trainNumber); //C:training/datas5

		strcat(trainFile, trainFileName); //C:training/datas5/normalData.txt
		printf("\nTraining %i/%i on file %s\n", trainingIt, trainIteration, trainFile);

		//Training
		offlinePhase(trainFile, MAPPATH);
		
		for (int testIt = 1; testIt <= testIteration; testIt++)
		{
			//Loading trained map
			loadMap(&outerMapDyD2, outerPath, outerMapDyD2.trainMinMax);
			loadMap(&innerMapDyD2, innerPath, innerMapDyD2.trainMinMax);

			//Testing folder name initialisation
			strcpy(testFile, testFolder); //C:test/datas
			sprintf(testNumber, "%i", testIt);
			strcat(testFile, testNumber); //C:test/datas5
			strcpy(labelFile, testFile);
			strcpy(normalTestFileDyD2, testFile);
			strcat(testFile, testFileName); //C:test/datas5/dataSet.txt
			strcat(labelFile, labelFileName); //C:test/datas5/status.txt
			strcat(normalTestFileDyD2, testNormalFileName);  //C:test/datas5/normalCurrent.txt

			printf("Testing %i/%i on file %s\n", testIt, testIteration, testFile);


			//Testing
			getLabels(labelFile, testLabelsDyD2, LABELNUMBERDYD2, LABELSKIPPINGLINE);
			testSize = onlinePhase(testFile, POSTMAPPATH, LOGPATH);
			confusionMatrix(confMatrix, logSave, testLabelsDyD2, LABELROW, FAULTVALUE, testSize, doAnomalyRemoval);
			execTime[testIt-1] = execTimeDyD2;
		}


	}
	printResults(confMatrix, execTime, trainFolder, testFolder, labelFileName, testIteration);
}
