#include <stdio.h>
#include <math.h>
#include "FileIO.h"
#include "TestBlocks.h"
#include "variables.h"
#include "output_images.h"
#include "tests.h"

int getDifference(int arr[][COL], int r, int c)
{
	int i, j;
	int term;
	int sum = 0;
	int set = -1;
	for(i = r; i < (r+BLOCK_SIZE); i++)
	{
		for(j = c; j < (c+BLOCK_SIZE); j++)
		{
			set = (-1)*set;
			term = set*arr[i][j];
			sum = sum + term;
		}
	}
	return sum;
}

double getStatistic(int arr[][COL], double meanOrig, int Nx, int r, int c)
{
	int i, j;
	double DenR = 0.0;
	double term = 0.0;
	double NumR = 0.0;
	double result = 0.0;

	DenR = 1.0/(Nx - 1);

	for(i = r; i < (r+BLOCK_SIZE); i++)
	{
		for(j = c; j < (c+BLOCK_SIZE); j++)
		{
			term = (arr[i][j] - meanOrig)*(arr[i][j] - meanOrig);
			NumR += term;
		}
	}

	result = NumR*DenR;
	return result;
}

double getPixelMean(int Arr1[][COL], int pi, int pj, int Arr2[][COL], int ci, int cj)
{
	int B[BLOCK_SIZE][BLOCK_SIZE] = {0};
	int C[BLOCK_SIZE][BLOCK_SIZE] = {0};

	double sum = 0.0;
	double term = 0.0;
	double mean1 = 0.0;
	double mean2 = 0.0;
	int i, j;
	int x,y;

	for(i = pi, x = 0; x < BLOCK_SIZE; i++, x++)
	{
		for(j = pj, y = 0; y < BLOCK_SIZE; j++, y++)
		{
			B[x][y] = Arr1[i][j];
			mean1 += Arr1[i][j];
		}
	}

	for(i = ci, x = 0; x < BLOCK_SIZE; i++, x++)
	{
		for(j = cj, y = 0; y < BLOCK_SIZE; j++, y++)
		{
			C[x][y] = Arr2[i][j];
			mean2 += Arr2[i][j];
		}
	}

	mean1 = mean1/(BLOCK_SIZE*BLOCK_SIZE);
	mean2 = mean2/(BLOCK_SIZE*BLOCK_SIZE);

	for(i = 0; i < BLOCK_SIZE; i++)
	{
		for(j = 0; j < BLOCK_SIZE; j++)
		{
			term = (B[i][j] - mean1)*(C[i][j] - mean2);
			sum += term;
		}
	}

	return sum;
}


double getMean(int arr[][COL], int Nx, int r, int c)
{
	double sum = 0.0;
	double mean = 0.0;
	int i, j;
	for(i = r; i < (r+BLOCK_SIZE); i++)
	{
		for(j = c; j < (c+BLOCK_SIZE); j++)
		{
			sum += arr[i][j];
		}
	}

	mean = sum/Nx;
	return mean;
}

double ThumbImgCheckMSE (int B[][CodeCOL], int C[][CodeCOL])
{
	int i = 0, j = 0;
	double MSE = 0.0;
	double sum = 0.0;
	double term = 0.0;
	int N = CodeROW*CodeCOL;

	for(i = 0; i < CodeROW; i++)
	{
		for(j = 0; j < CodeCOL; j++)
		{
			term =(B[i][j]-C[i][j])*(B[i][j]-C[i][j]);
			sum += term;
		}
	}

	MSE = (1.0*sum)/N;

	return MSE;
}

double ImgCheckMSE (int B[][COL], int C[][COL])
{
	int i = 0, j = 0;
	double MSE = 0.0;
	double sum = 0.0;
	double term = 0.0;
	int N = ROW*COL;

	for(i = 0; i < ROW; i++)
	{
		for(j = 0; j < COL; j++)
		{
			term =(B[i][j]-C[i][j])*(B[i][j]-C[i][j]);
			sum += term;
		}
	}

	MSE = (1.0*sum)/N;

	return MSE;
}


double checkMSE (int B[][BLOCK_SIZE], int C[][BLOCK_SIZE])
{
	/*
	Mean Square Error of Ea\ch Block:

	MSE = (1/N) [Sum of (xi - yi)^2]
	N = number of pixels in the block
	xi = ith pixel in original signal
	yi = ith pixel in distorted signal
	L = 255

	*/
	int i = 0, j = 0;
	double MSE = 0.0;
	double sum = 0.0;
	double term = 0.0;
	int N = BLOCK_SIZE*BLOCK_SIZE;

	for(i = 0; i < BLOCK_SIZE; i++)
	{
		for(j = 0; j < BLOCK_SIZE; j++)
		{
			term =(B[i][j]-C[i][j])*(B[i][j]-C[i][j]);
			sum += term;
		}
	}

	MSE = sum/N;

	return MSE;
}

double checkPSNR(double MSE)
{
	/*
	Peak Signal to Noise Ration of Each Block

	PSNR = 10 log(base 10) [(L^2)/MSE]
	L = 255
	*/

	double PSNR = 0.0;

	PSNR = 10 * log10(L_SQUARE/MSE);

	return PSNR;
}

void RejectVal(int pos_CodeBook[][CodeCOL], int val_CodeBook[][CodeCOL])
{
	//int THRESHOLD_MATCH_SCORE
	double val;
	int i, j;

	for(i = 0; i < CodeROW; i++)
	{
		for(j = 0; j < CodeCOL; j++)
		{
			val = val_CodeBook[i][j];
			if (val > THRESHOLD_MATCH_SCORE)
			{
				//REJECT THE PATTERN MATCH and PUT VALUE FOR ORIGINAL IMAGE
				pos_CodeBook[i][j] = 0;
			}
		}
	}
}

void checkArrC(int ci, int cj, int B[][BLOCK_SIZE], int ImgR[][COL], int pos_CodeBook[][CodeCOL], int val_CodeBook[][CodeCOL])
{
	double MSE = 0.0;

	int C[BLOCK_SIZE][BLOCK_SIZE] = {0};

	int i,j;
	int x,y;
	int count1, count2;

	int flag = 0;
	i = j = x = y = count1 = count2 = 0;



	for(i = 0; i < ROW; i = i + BLOCK_SIZE)
	{
		for(j = 0; j < COL; j = j + BLOCK_SIZE)
		{
			x = i;
			//printf("\t%d,%d", i, j);
			for(count1 = 0; count1 < BLOCK_SIZE; x++, count1++)
			{
				y = j;
				for(count2 = 0; count2 < BLOCK_SIZE; y++, count2++)
				{
					C[count1][count2] = ImgR[x][y];
				}//for4
			}//for3

			MSE = checkMSE(B, C);

			if(flag == 0)
			{
				flag = 1;
				pos_CodeBook[ci][cj] = ((i/BLOCK_SIZE)*(COL/BLOCK_SIZE)) + (j/BLOCK_SIZE) + 1;
				val_CodeBook[ci][cj] = MSE;
			}

			if(MSE < val_CodeBook[ci][cj])
			{
				pos_CodeBook[ci][cj] = ((i/BLOCK_SIZE)*(COL/BLOCK_SIZE)) + (j/BLOCK_SIZE) + 1; //+1 stands for it does not start from 0
				val_CodeBook[ci][cj] = MSE;
			}
			//printf("MSE %lf  ", MSE);
		}//for2
		//printf("\n");
	}//for1
}

void createCodeBook(int ImgL[][COL], int ImgR[][COL], int pos_CodeBook[][CodeCOL], int  val_CodeBook[][CodeCOL])
{
	int B[BLOCK_SIZE][BLOCK_SIZE] = {0};
	int i,j,x,y;
	int count1, count2;

	int ci = 0;
	int cj = 0;

	i = j = x = y =0;

	/*Test Each Block of B with every Block in C and store the block number in which
	the highest match is found
	*/

	for(i = 0; i < ROW; i = i + BLOCK_SIZE)
	{
		for(j = 0; j < COL; j = j + BLOCK_SIZE)
		{
			x = i;		
			for(count1 = 0; count1 < BLOCK_SIZE; x++, count1++)
			{
				y = j;
				for(count2 = 0; count2 < BLOCK_SIZE; y++, count2++)
				{
					B[count1][count2] = ImgL[x][y];
				}
			}
			//printf("\nRunning For Row %d, Col %d", i, j);

			ci = i/BLOCK_SIZE;
			cj = j/BLOCK_SIZE;
			checkArrC (ci, cj, B, ImgR, pos_CodeBook, val_CodeBook); //ci, cj passes the indices of the current position in code book
			//printf("\n%d", i);
			//printf("%d  ", pos_CodeBook[ci][cj]);
		}
		//printf("\n");
	}
} 