#include "inout.h"



void initParser(struct parser* p)
{

	p->globalParametersNumber = 2;
	strcpy(p->globalParameters, "[Global_parameters]\n");
	strcpy(p->minValue, "Min_value: ");
	strcpy(p->maxValue, "Max_value: ");

	p->uClusterParametersNumber = 9;
	strcpy(p->newUCluster, "[uCluster]\n");
	strcpy(p->number, "uCluster_number: ");
	strcpy(p->featureNumber, "feature_number: ");
	strcpy(p->dataNumber, "data_number: ");
	strcpy(p->density, "density: ");
	strcpy(p->size, "size: ");
	strcpy(p->center, "center: ");
	strcpy(p->score, "score: ");
	strcpy(p->creationDate, "creation_date: ");
	strcpy(p->lastUpdate, "last_update: ");
}

int createFile(char fileName[], bool autoOverwrite)
{
	FILE* pFile;
	char input[10];

	if (!autoOverwrite) //We do not check if file already exist if autoOverwritte parameter is true
	{
		pFile = fopen(fileName, "r");
		if (pFile == NULL)
		{
			fclose(pFile);
			printf("Warning in createFile: The file %s already exist\nDo you want to delete it and continue ? (y/n)\n", fileName);
			scanf("%s", input);
			if (strcmp(input, "y") == 0 || strcmp(input, "Y") == 0) //Yes condition: delete overwrite last file
			{
				printf("\nOverwriting of file\n");
			}
			else
			{
				return 1;
			}

		}
	}



	pFile = fopen(fileName, "w");
	if (pFile != NULL)
	{
		fclose(pFile);
		printf("\nThe file %s was created\n", fileName);
	}
	else
	{
		printf("\nWARNING in createFile: The file %s was NOT created. Check if the folder already exits\n");
		return 1;
	}


	return 0;

}



int writeData(char fileName[], float data[], int dataSize)
{
	FILE* pFile;

	pFile = fopen(fileName, "a");
	if (pFile == NULL)
	{
		printf("ERROR addData: error opening file %s\n", fileName);
		return 1;
	}

	for (int i = 0; i < dataSize; i++)
	{
		if (i < dataSize - 1)
		{
			fprintf(pFile, "%f,", data[i]);
		}
		if (i >= dataSize - 1)
		{
			fprintf(pFile, "%f", data[i]);
		}

	}
	fprintf(pFile, "\n");

	fclose(pFile);

	return 0;
}



int readData(float data[][ROWNUMBER], char fileName[], int rows, int skippingLine)
{
	//If error in this fonction, check for the delimiter in dataSplit !

	FILE* pFile;
	char dataTemp[MAXCHARPERLINE];
	char* dataSplit;
	int index = 0, arrayIndex = 0, row = 0, arrayRow = 0, actualLine = 0;



	pFile = fopen(fileName, "r");
	if (pFile == NULL)
	{
		printf("\nERROR getData: error opening file %s\n", fileName);
		return 1;
	}

	index = arrayIndex = actualLine = arrayRow = 0;
	while (fgets(dataTemp, MAXCHARPERLINE, pFile) != NULL) //Get the whole line
	{
		if (actualLine >= skippingLine)
		{
			//For below, see https://www.codingame.com/playgrounds/14213/how-to-play-with-strings-in-c/string-split
			dataSplit = strtok(dataTemp, ","); //Divide the line into values seperated by a comma ","
			row = 0;
			while (row < rows) //We continune to search for value until we hit the max number of rows parameter
			{
				if (index * row >= FILESIZEMAX * ROWNUMBER)
				{
					printf("\nWARNING in readData: data file size (%i) is higher than max size (%i) of data array \n", index * row, FILESIZEMAX * LABELNUMBERDYD2);
					return -1;
				}


				sscanf(dataSplit, "%f", &data[arrayIndex][arrayRow]);
				dataSplit = strtok(NULL, ",");

				row++;
				arrayRow++;
				if (arrayRow >= ROWNUMBER)
				{
					arrayIndex++;
					arrayRow = 0;
				}
			}
			index++;
		}
		actualLine++;
	}

	fclose(pFile);
	return index;
}


int writeFile(char fileName[], char string[])
{
	FILE* pFile;

	pFile = fopen(fileName, "a");
	if (pFile == NULL)
	{
		printf("ERROR addData: error opening file %s\n", fileName);
		return 1;
	}

	fprintf(pFile, "%s", string);


	fclose(pFile);

	return 0;
}


int saveUCluster(struct uCluster uCl, char fileName[], int uClusterNumber)
{
	struct parser p;
	FILE* pFile;


	initParser(&p);
	pFile = fopen(fileName, "a");
	if (pFile == NULL)
	{
		printf("ERROR addData: error opening file %s\n", fileName);
		return 1;
	}

	fprintf(pFile, "\n");
	fprintf(pFile, "%s", p.newUCluster);

	fprintf(pFile, "%s", p.number);
	fprintf(pFile, "%i\n", uClusterNumber);


	fprintf(pFile, "%s", p.featureNumber);
	fprintf(pFile, "%i\n", (int)uCl.featureNumber);


	fprintf(pFile, "%s", p.dataNumber);
	fprintf(pFile, "%i\n", (int)uCl.dataNumber);


	fprintf(pFile, "%s", p.density);
	fprintf(pFile, "%f\n", (float)uCl.density);


	fprintf(pFile, "%s", p.size);
	for (int i = 0; i < uCl.featureNumber; i++)
	{
		if (i < uCl.featureNumber - 1)
		{
			fprintf(pFile, "%f,", uCl.size[i]);
		}
		if (i >= uCl.featureNumber - 1)
		{
			fprintf(pFile, "%f\n", uCl.size[i]);
		}
	}


	fprintf(pFile, "%s", p.center);
	for (int i = 0; i < uCl.featureNumber; i++)
	{
		if (i < uCl.featureNumber - 1)
		{
			fprintf(pFile, "%f,", uCl.center[i]);
		}
		if (i >= uCl.featureNumber - 1)
		{
			fprintf(pFile, "%f\n", uCl.center[i]);
		}
	}

	fprintf(pFile, "%s", p.score);
	fprintf(pFile, "%f\n", (float)uCl.score);

	fprintf(pFile, "%s", p.creationDate);
	fprintf(pFile, "%i\n", (int)uCl.creationDate);


	fprintf(pFile, "%s", p.lastUpdate);
	fprintf(pFile, "%i\n", (int)uCl.lastUpdate);

	fclose(pFile);
	return 0;
}



int loadUCluster(struct uCluster* uCl, char fileName[], int desiredUClusterNumber)
{
	struct parser p;
	FILE* pFile;
	char dataTemp[200];
	int index = 0, uClusterNumber = 0;
	bool isDesiredUClusterFound = false;
	char* dataSplit1;


	initParser(&p);
	pFile = fopen(fileName, "r");
	if (pFile == NULL)
	{
		printf("ERROR getData: error opening file %s\n", fileName);
		return 1;
	}

	while (fgets(dataTemp, MAXCHARPERLINE, pFile) != NULL && !isDesiredUClusterFound)
	{
		//See this for strncmphttp: www.cplusplus.com/reference/cstring/strncmp/
		if (strncmp(dataTemp, p.newUCluster, strlen(p.newUCluster)) == 0) //Means we found the token [uCluster]
		{

			//We want to see if this is the desired uCluster number
			fgets(dataTemp, MAXCHARPERLINE, pFile);
			dataSplit1 = strtok(dataTemp, " : ");
			dataSplit1 = strtok(NULL, " : ");
			sscanf(dataSplit1, "%i", &uClusterNumber);

			if (desiredUClusterNumber == uClusterNumber)//If this is true, we found the desired uCluster
			{
				isDesiredUClusterFound = true;
				for (int i = 1; i < p.uClusterParametersNumber; i++)
				{
					if (fgets(dataTemp, MAXCHARPERLINE, pFile) == NULL) //We pass the [uCluster token with this condition
					{
						printf("\nWARNING in loadUCluster: Cannot load all uCluster informations. Data file must be corrupted %s\n", fileName);
						fclose(pFile);
						return 1;
					}

					//We get access to wanted parameter by playing with strtok
					dataSplit1 = strtok(dataTemp, " : ");
					dataSplit1 = strtok(NULL, " : ");


					switch (i)
					{
					case 0:
						//uCluster_number is only used for map, not for uCluster
						//sscanf(dataSplit1, "%i", &uClusterNumber);
						break;
					case 1:
						sscanf(dataSplit1, "%i", &uCl->featureNumber);
						break;
					case 2:
						sscanf(dataSplit1, "%i", &uCl->dataNumber);
						break;
					case 3:
						sscanf(dataSplit1, "%f", &uCl->density);
						break;
					case 4:
						index = 0;
						dataSplit1 = strtok(dataSplit1, ",");
						while (dataSplit1 != NULL)
						{
							if (index >= MAXFEATURES)
							{
								printf("\nWARNING in loadUCluster: index %i is higher than max number of features %i\n", index, MAXFEATURES);
								return 1;
							}
							sscanf(dataSplit1, "%f", &uCl->size[index]);
							dataSplit1 = strtok(NULL, ",");
							index++;

						}
						break;
					case 5:
						index = 0;
						dataSplit1 = strtok(dataSplit1, ",");
						while (dataSplit1 != NULL)
						{
							if (index >= MAXFEATURES)
							{
								printf("\nWARNING in loadUCluster: index %i is higher than max number of features %i\n", index, MAXFEATURES);
								return 1;
							}
							sscanf(dataSplit1, "%f", &uCl->center[index]);
							dataSplit1 = strtok(NULL, ",");
							index++;
						}
						break;
					case 6:
						sscanf(dataSplit1, "%f", &uCl->score);
						break;
					case 7:
						sscanf(dataSplit1, "%i", &uCl->creationDate);
						break;
					case 8:
						sscanf(dataSplit1, "%i", &uCl->lastUpdate);
						break;
					}
				}
			}
		}
	}

	fclose(pFile);
	if (!isDesiredUClusterFound) //If we didn't find the disered uCluster in the data file
	{
		printf("\nWARNING in loadUCluster: Desired uCluster not found in data file %s", fileName);
		return 1;
	}

	return 0;
}


int saveMap(struct map M, char fileName[], float minMaxValues[2])
{
	int error = 0;
	float min[1] = { minMaxValues[0] };
	float max[1] = { minMaxValues[1] };

	createFile(fileName, true);
	writeFile(fileName, "[Global_parameters]\n");
	writeFile(fileName, "Min_value: ");
	writeData(fileName, min, 1);
	writeFile(fileName, "Max_value: ");
	writeData(fileName, max, 1);

	for (int i = 0; i < M.uClNumber; i++)
	{
		error = saveUCluster(M.uCl[i], fileName, i);
	}
	return error;
}

int loadMap(struct map* M, char fileName[], float minMaxValues[2])
{
	struct uCluster uCl;
	initMap(M);

	//We first have to know how many uCluster are stored in the file
	struct parser p;
	FILE* pFile;
	char dataTemp[200];
	char* dataSplit1;
	int uClusterNumber = 0;

	initParser(&p);
	pFile = fopen(fileName, "r");
	if (pFile == NULL)
	{
		printf("ERROR getData: error opening file %s\n", fileName);
		return 1;
	}

	while (fgets(dataTemp, MAXCHARPERLINE, pFile) != NULL)
	{
		//Get global parameters
		if (strncmp(dataTemp, p.globalParameters, strlen(p.globalParameters)) == 0) //Means we found the token [Global_parameters]
		{
			for (int i = 0; i < p.globalParametersNumber; i++)
			{
				if (fgets(dataTemp, MAXCHARPERLINE, pFile) == NULL)
				{
					printf("\nWARNING in loadMap: Cannot load all global Parameters. Data file must be corrupted %s\n", fileName);
					fclose(pFile);
					return 1;
				}

				//We get access to wanted parameter by playing with strtok
				dataSplit1 = strtok(dataTemp, " : ");
				dataSplit1 = strtok(NULL, " : ");

				switch (i)
				{
				case 0:
					sscanf(dataSplit1, "%f", &minMaxValues[0]);
					break;
				case 1:
					sscanf(dataSplit1, "%f", &minMaxValues[1]);
					break;
				}
			}
		}

		//Get uCluster number
		//See this for strncmphttp: www.cplusplus.com/reference/cstring/strncmp/
		if (strncmp(dataTemp, p.newUCluster, strlen(p.newUCluster)) == 0) //Means we found the token [uCluster]
		{
			uClusterNumber++;
		}


	}


	for (int i = 0; i < uClusterNumber; i++)
	{
		loadUCluster(&uCl, fileName, i);
		addMap(M, uCl);
	}

	fclose(pFile);
	return 0;
}


int getLabels(char labelFile[], float labels[][LABELNUMBERDYD2], int labelsNumber, int skippingLines)
{

	FILE* pFile;
	char dataTemp[MAXCHARPERLINE];
	char* dataSplit;
	int index = 0, arrayIndex = 0, row = 0, arrayRow = 0, actualLine = 0;



	pFile = fopen(labelFile, "r");
	if (pFile == NULL)
	{
		printf("\nERROR getData: error opening file %s\n", labelFile);
		return 1;
	}

	index = arrayIndex = actualLine = arrayRow = 0;
	while (fgets(dataTemp, MAXCHARPERLINE, pFile) != NULL) //Get the whole line
	{
		if (actualLine >= skippingLines)
		{
			//For below, see https://www.codingame.com/playgrounds/14213/how-to-play-with-strings-in-c/string-split
			dataSplit = strtok(dataTemp, ","); //Divide the line into values seperated by a comma ","
			row = 0;
			while (row < labelsNumber) //We continune to search for value until we hit the max number of rows parameter
			{
				if (index * row >= FILESIZEMAX * LABELNUMBERDYD2)
				{
					printf("\nWARNING in readData: data file size (%i) is higher than max size (%i) of data array \n", index * row, FILESIZEMAX * LABELNUMBERDYD2);
					return -1;
				}


				sscanf(dataSplit, "%f", &labels[arrayIndex][arrayRow]);
				dataSplit = strtok(NULL, ",");

				row++;
				arrayRow++;
				if (arrayRow >= LABELNUMBERDYD2)
				{
					arrayIndex++;
					arrayRow = 0;
				}
			}
			index++;
		}
		actualLine++;
	}

	fclose(pFile);
	return index;
}