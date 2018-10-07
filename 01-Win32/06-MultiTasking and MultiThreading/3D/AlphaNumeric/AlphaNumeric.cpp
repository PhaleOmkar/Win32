// Headers
#include <Windows.h>
#include "3D.h"
#include "AlphaNumeric.h"

// DllMain
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID Reserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		break;
	}
	return(TRUE);
}

#pragma region Private Functions
// Converts 3D Points into lines using graph
void ConvertToLines(LPSTATE State, int iNoOfPoints, int *matGraph)
{
	int iPointOffset = State->Model.iNoOfPoints - (iNoOfPoints * 2);
	int iLineNo = State->Model.iNoOfLines;

	// for uppper and lower face
	for (int i = 0; i < iNoOfPoints; i++)
	{
		for (int j = i + 1; j < iNoOfPoints; j++)
		{
			if (matGraph[(i*25) + j] == 1)
			{
				// upper face
				State->Model.arrLine[iLineNo].ptStart = &State->Points[iPointOffset + i];
				State->Model.arrLine[iLineNo++].ptEnd = &State->Points[iPointOffset + j];

				// lower face
				State->Model.arrLine[iLineNo].ptStart = &State->Points[iPointOffset + i + iNoOfPoints];
				State->Model.arrLine[iLineNo++].ptEnd = &State->Points[iPointOffset + j + iNoOfPoints];
			}
		}
	}

	// for upper to lower connections
	for (int i = 0; i < iNoOfPoints; i++)
	{
		State->Model.arrLine[iLineNo].ptStart = &State->Points[iPointOffset + i];
		State->Model.arrLine[iLineNo++].ptEnd = &State->Points[iPointOffset + i + iNoOfPoints];
	}

	// set total count of points and lines 
	State->Model.iNoOfLines = iLineNo;

	return;
}

// Convert 2D points into 3D points
void Convert2DTo3D(LPSTATE State, int iNoOfPoints, int *arr2DPoints)
{
	int iPointNo = State->Model.iNoOfPoints;

	// set Z for each point
	for (int i = 0; i < iNoOfPoints * 2; i += 2)
	{
		State->Points[iPointNo].x = (double)arr2DPoints[i];
		State->Points[iPointNo].y = (double)arr2DPoints[i + 1];
		State->Points[iPointNo].z = 2.0;

		State->Points[iPointNo + iNoOfPoints].x = (double)arr2DPoints[i];
		State->Points[iPointNo + iNoOfPoints].y = (double)arr2DPoints[i + 1];
		State->Points[iPointNo + iNoOfPoints].z = -2.0;

		iPointNo++;
	}

	State->Model.iNoOfPoints += (iNoOfPoints * 2);
	return;
}
#pragma endregion


// Exported Functions
// Calculate the coordinates of the points to be drawn using predefined points of letters
// LPSTATE MUST CONTAIN NO POINTS! 
extern "C" void SetLetters(LPSTATE State, char *szLetters)
{
	State->Model.iNoOfPoints = 0;
	State->Model.iNoOfLines = 0;

	int matGraphOfLetter[25][25] = { 0 };
	int *arr2DPoints = NULL;
	int *matGraph = NULL;
	int iOffsetOfPoint = 0;
	int iNoOfPointsToShift = 0;
	int iLen = strlen(szLetters);
	int i = -iLen / 2;
	int iOffset = 0;

	while (*szLetters)
	{
		iOffset = (12) * (i++);
		memset(matGraphOfLetter, 0, sizeof(int) * 25 * 25);
		switch (*szLetters)
		{
		case 'a':
		case 'A':			
			iNoOfPointsToShift = sizeof(arr2DPointsOfA) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfA) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfA, sizeof(arr2DPointsOfA));

			matGraphOfLetter[0][1] = 1;
			matGraphOfLetter[1][3] = 1;
			matGraphOfLetter[3][4] = 1;
			matGraphOfLetter[4][5] = 1;
			matGraphOfLetter[2][5] = 1;
			matGraphOfLetter[0][2] = 1;
			matGraphOfLetter[0][1] = 1;
			matGraphOfLetter[1][3] = 1;
			matGraphOfLetter[3][4] = 1;
			matGraphOfLetter[4][5] = 1;
			matGraphOfLetter[2][5] = 1;
			matGraph = (int *)matGraphOfLetter;

			break;

		case 'b':
		case 'B':
			iNoOfPointsToShift = sizeof(arr2DPointsOfB) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfB) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfB, sizeof(arr2DPointsOfB));
			break;

		case 'c':
		case 'C':
			iNoOfPointsToShift = sizeof(arr2DPointsOfC) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfC) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfC, sizeof(arr2DPointsOfC));
			break;

		case 'd':
		case 'D':
			iNoOfPointsToShift = sizeof(arr2DPointsOfD) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfD) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfD, sizeof(arr2DPointsOfD));
			break;

		case 'e':
		case 'E':
			iNoOfPointsToShift = sizeof(arr2DPointsOfE) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfE) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfE, sizeof(arr2DPointsOfE));
			break;

		case 'f':
		case 'F':
			iNoOfPointsToShift = sizeof(arr2DPointsOfF) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfF) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfF, sizeof(arr2DPointsOfF));
			break;

		case 'g':
		case 'G':
			iNoOfPointsToShift = sizeof(arr2DPointsOfG) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfG) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfG, sizeof(arr2DPointsOfG));
			break;

		case 'h':
		case 'H':
			iNoOfPointsToShift = sizeof(arr2DPointsOfH) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfH) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfH, sizeof(arr2DPointsOfH));
			break;

		case 'i':
		case 'I':
			iNoOfPointsToShift = sizeof(arr2DPointsOfI) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfI) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfI, sizeof(arr2DPointsOfI));
			break;

		case 'j':
		case 'J':
			iNoOfPointsToShift = sizeof(arr2DPointsOfJ) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfJ) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfJ, sizeof(arr2DPointsOfJ));
			break;

		case 'k':
		case 'K':
			iNoOfPointsToShift = sizeof(arr2DPointsOfK) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfK) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfK, sizeof(arr2DPointsOfK));
			break;

		case 'l':
		case 'L':
			iNoOfPointsToShift = sizeof(arr2DPointsOfL) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfL) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfL, sizeof(arr2DPointsOfL));
			break;

		case 'm':
		case 'M':
			iNoOfPointsToShift = sizeof(arr2DPointsOfM) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfM) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfM, sizeof(arr2DPointsOfM));
			break;

		case 'n':
		case 'N':
			iNoOfPointsToShift = sizeof(arr2DPointsOfN) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfN) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfN, sizeof(arr2DPointsOfN));
			break;
		
		case 'o':
		case 'O':
			iNoOfPointsToShift = sizeof(arr2DPointsOfO) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfO) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfO, sizeof(arr2DPointsOfO));
			break;
		
		case 'p':
		case 'P':
			iNoOfPointsToShift = sizeof(arr2DPointsOfP) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfP) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfP, sizeof(arr2DPointsOfP));
			break;

		case 'q':
		case 'Q':
			iNoOfPointsToShift = sizeof(arr2DPointsOfQ) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfQ) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfQ, sizeof(arr2DPointsOfQ));
			break;

		case 'r':
		case 'R':
			iNoOfPointsToShift = sizeof(arr2DPointsOfR) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfR) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfR, sizeof(arr2DPointsOfR));
			break;

		case 's':
		case 'S':
			iNoOfPointsToShift = sizeof(arr2DPointsOfS) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfS) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfS, sizeof(arr2DPointsOfS));
			break;

		case 't':
		case 'T':
			iNoOfPointsToShift = sizeof(arr2DPointsOfT) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfT) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfT, sizeof(arr2DPointsOfT));
			break;

		case 'u':
		case 'U':
			iNoOfPointsToShift = sizeof(arr2DPointsOfU) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfU) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfU, sizeof(arr2DPointsOfU));
			break;

		case 'v':
		case 'V':
			iNoOfPointsToShift = sizeof(arr2DPointsOfV) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfV) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfV, sizeof(arr2DPointsOfV));

			matGraphOfLetter[0][1] = 1;
			matGraphOfLetter[1][3] = 1;
			matGraphOfLetter[3][4] = 1;
			matGraphOfLetter[4][5] = 1;
			matGraphOfLetter[2][5] = 1;
			matGraphOfLetter[0][2] = 1;
			matGraph = (int *)matGraphOfLetter;

			break;

		case 'w':
		case 'W':
			iNoOfPointsToShift = sizeof(arr2DPointsOfW) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfW) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfW, sizeof(arr2DPointsOfW));

			matGraphOfLetter[0][1] = 1;
			matGraphOfLetter[1][3] = 1;
			matGraphOfLetter[3][5] = 1;
			matGraphOfLetter[5][11] = 1;
			matGraphOfLetter[7][11] = 1;
			matGraphOfLetter[7][8] = 1;
			matGraphOfLetter[8][9] = 1;
			matGraphOfLetter[9][10] = 1;
			matGraphOfLetter[6][10] = 1;
			matGraphOfLetter[4][6] = 1;
			matGraphOfLetter[2][4] = 1;
			matGraphOfLetter[0][2] = 1;
			matGraph = (int *)matGraphOfLetter;

			break;

		case 'x':
		case 'X':
			iNoOfPointsToShift = sizeof(arr2DPointsOfX) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfX) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfX, sizeof(arr2DPointsOfX));
			
			matGraphOfLetter[0][1] = 1;
			matGraphOfLetter[1][8] = 1;
			matGraphOfLetter[2][8] = 1;
			matGraphOfLetter[2][3] = 1;
			matGraphOfLetter[3][10] = 1;
			matGraphOfLetter[7][10] = 1;
			matGraphOfLetter[6][7] = 1;
			matGraphOfLetter[6][9] = 1;
			matGraphOfLetter[5][9] = 1;
			matGraphOfLetter[4][5] = 1;
			matGraphOfLetter[4][11] = 1;
			matGraphOfLetter[0][11] = 1;
			matGraph = (int *)matGraphOfLetter;

			break;

		case 'y':
		case 'Y':
			iNoOfPointsToShift = sizeof(arr2DPointsOfY) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfY) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfY, sizeof(arr2DPointsOfY));

			matGraphOfLetter[0][1] = 1;
			matGraphOfLetter[1][4] = 1;
			matGraphOfLetter[2][4] = 1;
			matGraphOfLetter[2][3] = 1;
			matGraphOfLetter[3][8] = 1;
			matGraphOfLetter[6][8] = 1;
			matGraphOfLetter[5][6] = 1;
			matGraphOfLetter[5][7] = 1;
			matGraphOfLetter[0][7] = 1;
			matGraph = (int *)matGraphOfLetter;

			break;

		case 'z':
		case 'Z':
			iNoOfPointsToShift = sizeof(arr2DPointsOfZ) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfZ) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfZ, sizeof(arr2DPointsOfZ));

			matGraphOfLetter[0][1] = 1;
			matGraphOfLetter[2][3] = 1;
			matGraphOfLetter[0][2] = 1;
			matGraphOfLetter[3][6] = 1;
			matGraphOfLetter[1][4] = 1;
			matGraphOfLetter[4][5] = 1;
			matGraphOfLetter[6][7] = 1;
			matGraphOfLetter[5][7] = 1;
			matGraph = (int *)matGraphOfLetter;

			break;

			case ' ':
				break;
		}
		
		for (int j = 0; j < iNoOfPointsToShift; j += 2)
		{
			arr2DPoints[j] += iOffset;
		}

		Convert2DTo3D(State, iNoOfPointsToShift/2, arr2DPoints);
		ConvertToLines(State, iNoOfPointsToShift/2, matGraph);

		free(arr2DPoints);

		//iOffset += 12;
		szLetters++;
	}

	return;
}


