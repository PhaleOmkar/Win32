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

		switch (*szLetters)
		{
		case 'x':
		case 'X':
			iNoOfPointsToShift = sizeof(arr2DPointsOfZ) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfZ) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfZ, sizeof(arr2DPointsOfZ));

			matGraphOfLetter[25][25] = { 0 };

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

		case 'y':
		case 'Y':
			iNoOfPointsToShift = sizeof(arr2DPointsOfY) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfY) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfY, sizeof(arr2DPointsOfY));

			matGraphOfLetter[25][25] = { 0 };

			matGraphOfLetter[0][1] = 1;
			matGraphOfLetter[1][4] = 1;
			matGraphOfLetter[4][2] = 1;
			matGraphOfLetter[2][3] = 1;
			matGraphOfLetter[3][6] = 1;
			matGraphOfLetter[6][8] = 1;
			matGraphOfLetter[8][7] = 1;
			matGraphOfLetter[7][5] = 1;
			matGraphOfLetter[5][0] = 1;
			matGraph = (int *)matGraphOfLetter;

			break;

		case 'z':
		case 'Z':
			iNoOfPointsToShift = sizeof(arr2DPointsOfZ) / sizeof(int);
			arr2DPoints = (int *)malloc(sizeof(arr2DPointsOfZ) * sizeof(int));
			memcpy(arr2DPoints, arr2DPointsOfZ, sizeof(arr2DPointsOfZ));

			matGraphOfLetter[25][25] = { 0 };

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


