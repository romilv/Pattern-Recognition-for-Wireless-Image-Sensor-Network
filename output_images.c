#include <stdio.h>
#include "output_images.h"
#include "FileIO.h"
#include "TestBlocks.h"
#include "variables.h"
#include "tests.h"

void createThumbnail(int ImgL[][COL], int ImgThumbnail[][CodeCOL])
{
	int i, j, x, y;
	int avg = 0;
	int countx = 0, county = 0;
	int sum;


	for(i = 0; i < ROW; i += BLOCK_SIZE)
	{
		for(j = 0; j < COL; j += BLOCK_SIZE)
		{
			countx = county = sum = 0;
			x = i;
			for(; countx < BLOCK_SIZE; countx++, x++)
			{
				y = j;
				for(; county < BLOCK_SIZE; county++, y++)
				{
					sum += ImgL[x][y];
				}
			}
			avg = sum/(BLOCK_SIZE*BLOCK_SIZE);
			avg = avg*2;
			ImgThumbnail[i/BLOCK_SIZE][j/BLOCK_SIZE] = avg;
		}
	}
}

void createReconstructed(int pos_CodeBook[][CodeCOL], int val_CodeBook[][CodeCOL], int ImgReconstructed[][COL], int ImgL[][COL], int ImgR[][COL])
{
	/*Reconstruct Image Based on Codebook. If a value is rejected put the value from the original image */

	int i, j, ci, cj, pi, pj, pos;

	RejectVal(pos_CodeBook, val_CodeBook);

	//Running the Code Book
	for(i = 0; i < CodeROW; i++)
	{
		for(j = 0; j < CodeCOL; j++)
		{
			pos = pos_CodeBook[i][j]; //pos is the number of the block

			//The Index for the Reconstructed Image
			pi = i*BLOCK_SIZE;
			//pi = (i*COL)/(BLOCK_SIZE*BLOCK_SIZE);  
			pj = j*BLOCK_SIZE;

			if(pos == 0) //Call Block at same position in Original Image
			{
				ci = pi;// = pos/60; //The index for ImgR 
				cj = pj;// = pos%60; 
				Reconstructed(pi, pj, ci, cj, ImgReconstructed, ImgL);
			}

			else //Put in the Original Image
			{
				//pos_CodeBook[ci][cj] = ((i/BLOCK_SIZE)*(COL/BLOCK_SIZE)) + (j/BLOCK_SIZE) + 1;
				pos = pos-1;
				ci = (pos/CodeCOL)*BLOCK_SIZE;
				//ci = pos*COL/BLOCK_SIZE;
				cj = (pos%CodeCOL)*BLOCK_SIZE;
				Reconstructed(pi, pj, ci, cj, ImgReconstructed, ImgR);				
			}

		}
	}
}

void Reconstructed(int pi, int pj, int ci, int cj, int ImgReconstructed[][COL], int arr[][COL])
{
	int count1, count2;
	int yp, yc;

	yp = pj;
	yc = cj;

	for(count1 = 0; count1 < BLOCK_SIZE; pi++, ci++, count1++)
	{
		pj = yp;
		cj = yc;
		for(count2 = 0; count2 < BLOCK_SIZE; count2++)
		{
			ImgReconstructed[pi][pj] = arr[ci][cj];
			pj++;
			cj++;
		}
	}
}