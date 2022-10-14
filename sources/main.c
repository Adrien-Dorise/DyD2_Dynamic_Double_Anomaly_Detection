
#include "parameters.h"


//gcc -pg
//DEBUGGER: valgrind ./myprgm




void validationMain()
{
	char trainFolder[] = "H:\\DIAG_RAD\\DataSets\\endThesisValidationData\\simulations\\trainSet\\destructiveLatch\\datas";
	int trainIteration = 1;
	char testFolder[] = "H:\\DIAG_RAD\\DataSets\\endThesisValidationData\\simulations\\testSet\\smallLinearDeviationMicroLatch\\datas";
	int testIteration = 1;
	bool doAnomalyRemoval = FALSE;

	validation(trainFolder, trainIteration, testFolder, testIteration, doAnomalyRemoval);
}


void runDyD2()
{
	int confMatrix[4] = { 0,0,0,0 };
	int testSize = 0;
	float execTime[1] = { 0 };

	printf("\nDyD2 Start training on file: %s", TRAINFILE);
	offlinePhase(TRAINFILE, MAPPATH);

	printf("\nDyD2 Start testing on file: %s", TESTFILE);
	testSize = onlinePhase(TESTFILE, POSTMAPPATH, LOGPATH);

	if (strcmp(LABELTESTFILE, "NONE") != -1)
	{
		
		getLabels(LABELTESTFILE, testLabelsDyD2, LABELNUMBERDYD2, LABELSKIPPINGLINE);
		confusionMatrix(confMatrix, logSave, testLabelsDyD2, LABELRAW, FAULTVALUE, testSize, FALSE);
		execTime[0] = execTimeDyD2;
		printResults(confMatrix, execTime, TRAINFILE, TESTFILE, LABELTESTFILE, 1);
	}
	


}


void main()
{
	if (DoCrossValidation)
	{
		switch (WhichDataSet)
		{
			case 0:
				validation("datas/trainSet/heavyIon/datas", 2, "datas/testSet/heavyIon/datas", 3, FALSE);
				break;
		
			case 1:
				validation("datas/trainSet/simulation/datas", 2, "datas/testSet/destructiveAnomalies/datas", 4, FALSE);
				break;
		
			case 2:
				validation("datas/trainSet/simulation/datas", 2, "datas/testSet/smallAnomalies/datas", 4, FALSE);
				break;
		
			case 3:
				validation("datas/trainSet/simulation/datas", 2, "datas/testSet/dynamicDestructiveAnomalies/datas", 2, FALSE);
				break;
		
			case 4:
				validation("datas/trainSet/simulation/datas", 2, "datas/testSet/dynamicSmallAnomalies/datas", 2, FALSE);
				break;

			default:
				printf("\nWARNING: wrong value entered for [WhichDataSet] parameter\n");
		}
	}
	else
	{
		runDyD2();
	}
}

