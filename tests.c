#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "output_images.h"
#include "FileIO.h"
#include "TestBlocks.h"
#include "variables.h"
#include "tests.h"


void generateDetails(char *st, char *bckgd)
{
	FILE *outstream = NULL;
	char str[550];

	strcpy(str, "C:\\Users\\Romil\\Desktop\\Pattern\\Output_Directory\\CompressedTestResults_");
	strcat(str, st);
	strcat(str, ".txt");
	outstream = fopen(str, "a");

	fprintf(outstream, "\n\nDETAILS OF CONSTANTS");
	fprintf(outstream, "\nBACKGROUND = %s", bckgd);
	fprintf(outstream, "\nBLOCK SIZE = %d \nPIX ERROR = %d", BLOCK_SIZE, PIX_ERROR);
	fprintf(outstream, "\nTHRESHOLD_MATCH_SCORE = %d", THRESHOLD_MATCH_SCORE);
	fprintf(outstream, "\nSIMPLE_TEST_THRESHOLD = %d", SIMPLE_TEST_THRESHOLD);
	fprintf(outstream, "\nCORRELATION_THRESHOLD = %d", CORRELATION_THRESHOLD);
	
	fclose(outstream);
}




void Compression_Test(int pos_CodeBook[][CodeCOL], int val_CodeBook[][CodeCOL], char* st)
{
	FILE *outstream = NULL;
	char str[550];

	int count = 0;
	int total = CodeROW*CodeCOL;
	double compression;
	int i, j;
	RejectVal(pos_CodeBook, val_CodeBook);

	for(i = 0; i < CodeROW; i++)
	{
		for(j = 0; j < CodeCOL; j++)
		{
			if(pos_CodeBook[i][j]!=0)
			{
				count++;
			}
		}
	}

	compression = (count*100.0)/total;

	strcpy(str, "C:\\Users\\Romil\\Desktop\\Pattern\\Output_Directory\\CompressedTestResults_");
	strcat(str, st);
	strcat(str, ".txt");
	outstream = fopen(str, "a");

	fprintf(outstream, "\n\n*********************************************************\n");
	fprintf(outstream, "\nCOMPRESSION TEST\n");
	fprintf(outstream, "\nBlock Size = %d \nPIX_ERROR = %d", BLOCK_SIZE, PIX_ERROR);
	fprintf(outstream, "\nTotal Number of Blocks Used from Sensor B = %d", count);
	fprintf(outstream, "\nTotal Number of Blocks = %d", total);
	fprintf(outstream, "\nCompression Percentage = %lf\n\n", compression);

	fclose(outstream);
}

void Thumbnail_Test(int ImgL[][COL], int ImgRecon[][COL], int ImgThumbnail[][CodeCOL], int ImgReconThumbnail[][CodeCOL], char *st)
{
	double MSE_ImgOrig_Recon, MSE_ThumbImgOrig_Recon;
	double PSNR_Orig, PSNR_Thumb;
	double PSNR_difference;
	char str[550];

	FILE *outstream = NULL;

	strcpy(str, "C:\\Users\\Romil\\Desktop\\Pattern\\Output_Directory\\CompressedTestResults_");
	//itoa(PSNR_THRESHOLD, st, 10);
	strcat(str, st);
	strcat(str, ".txt");
	outstream = fopen(str, "a");

	fprintf(outstream, "\n\n*********************************************************\n");
	fprintf(outstream, "\nTHUMBNAIL TEST\n");
	fprintf(outstream, "BLOCK SIZE = %d, \nPIX_ERROR = %d", BLOCK_SIZE, PIX_ERROR);

	createThumbnail(ImgL, ImgThumbnail);
	createThumbnail(ImgRecon, ImgReconThumbnail);

	MSE_ImgOrig_Recon = ImgCheckMSE (ImgL, ImgRecon);

	MSE_ThumbImgOrig_Recon = ThumbImgCheckMSE(ImgThumbnail, ImgReconThumbnail);

	PSNR_Orig = checkPSNR(MSE_ImgOrig_Recon);

	PSNR_Thumb = checkPSNR(MSE_ThumbImgOrig_Recon);

	PSNR_difference = PSNR_Orig - PSNR_Thumb;

	if(PSNR_difference < 0)
		PSNR_difference = PSNR_difference*(-1);

		fprintf(outstream, "\nImage PSNR = %lf,  \nThumbnail PSNR = %lf,  \nAbsolute Difference = %lf\n", PSNR_Orig, PSNR_Thumb, PSNR_difference);

	fclose(outstream);
}


void Simple_Test(int pos_CodeBook[][CodeCOL], int ImgRecon[][COL], int ImgL[][COL], char *st)
{
	/*
	Take a Block and Alternatively Add and Subtract each pixel in the block. Do the Same for the corresponding Block in Reconstructed Image. 
	If the difference is in a Threshold Accept this. Run this Test for different Thresholds
	*/

	FILE *outstream = NULL;
	double average = 0.0;
	int i, j, ci, cj, pi, pj, pos;
	int orig = 0;
	int accept = 0;
	int reject = 0;
	char str[550];
	int differenceOrig, differenceRecon;
	int difference;

	differenceOrig = differenceRecon = 0.0;

	strcpy(str, "C:\\Users\\Romil\\Desktop\\Pattern\\Output_Directory\\CompressedTestResults_");
	//itoa(SIMPLE_TEST_THRESHOLD, st, 10);
	strcat(str, st);
	strcat(str, ".txt");
	outstream = fopen(str, "a");

	fprintf(outstream, "\n\n*********************************************************\n");
	fprintf(outstream, "\nSIMPLE TEST STATISTICS\n\n");
	//fprintf(outstream, "\nOriginal Image : %s \nReconstructed Image : %s\n", name_orig, name_recon);

	for(i = 0; i < CodeROW; i++)
	{
		for(j = 0; j < CodeCOL; j++)
		{
			if(pos_CodeBook[i][j]!=0)
			{
				pos = pos_CodeBook[i][j] - 1;
				pi = i*BLOCK_SIZE;
				pj = j*BLOCK_SIZE;

				ci = (pos/CodeCOL)*BLOCK_SIZE;
				cj = (pos%CodeCOL)*BLOCK_SIZE;

				differenceOrig = getDifference(ImgL, pi, pj);
				differenceRecon = getDifference(ImgRecon, ci, cj);

				difference = differenceOrig - differenceRecon;
				
				if(difference<0)
					difference *= (-1);

				average = average + difference;

				if(difference <= SIMPLE_TEST_THRESHOLD)
				{
					//fprintf(outstream, "\nACCEPTED    Original Block %d,%d  Reconstructed Block %d,%d  Value = %d", pi, pj, ci, cj, difference);
					accept++;
				}
				else
				{
					//fprintf(outstream, "\nREJECTED    Original Block %d,%d  Reconstructed Block %d,%d  Value = %d", pi, pj, ci, cj, difference);
					reject++;
				}
			}
			else
			{
				//fprintf(outstream, "\nOriginal Block Used");
				orig++;
			}
		}
	}

	average = average/(CodeROW*CodeCOL);

	//fprintf (outstream, "\n\nTEST STATISTICS\n");
	fprintf (outstream, "\nBLOCK SIZE = %d \nPIX_ERROR = %d \nSIMPLE_TEST_THRESHOLD = %d", BLOCK_SIZE, PIX_ERROR, SIMPLE_TEST_THRESHOLD);
	fprintf (outstream, "\nORIGINAL BLOCKS = %d", orig);
	fprintf (outstream, "\nACCEPTED BLOCKS = %d \nREJECTED BLOCKS = %d", accept, reject);
	fprintf (outstream, "\nAVERAGE = %lf", average);
	
	fprintf(outstream, "\n\n*********************************************************\n");
	fclose(outstream);
}


void Correlation_Test(int pos_CodeBook[][CodeCOL], int ImgL[][COL], int ImgRecon[][COL], char *st)
{
	int i, j, pi, pj, ci, cj, c_adj, pos, pos2;
	int ii, jj, x, y;
	int px, py, cx, cy;
	int Nx = BLOCK_SIZE*BLOCK_SIZE;
	double NumR, DenR;
	double meanOrig1, meanOrig2;
	double meanRecon1, meanRecon2;

	int orig = 0;
	int accept = 0;
	int reject = 0;

	char str[550];
	//char st[50];
	int count = 0;
	int count1, count2;
	double r, r1,r2;
	double average = 0.0;
	double Sx2, Sy2;

	FILE* outstream = NULL;


	strcpy(str, "C:\\Users\\Romil\\Desktop\\Pattern\\Output_Directory\\CompressedTestResults_");
	//itoa(CORRELATION_THRESHOLD, st, 10);
	strcat(str, st);
	strcat(str, ".txt");
	outstream = fopen(str, "a");

	fprintf(outstream, "\n\n*****************************************************************");
	fprintf(outstream, "\n\nCORRELATION TEST STATISTICS\n");
	//fprintf(outstream, "\nOriginal Image : %s \nReconstructed Image : %s\n", name_orig, name_recon);

	for(i = 0; i < CodeROW; i++)
	{
		for(j = 0; j < CodeCOL; j++)
		{

			if(pos_CodeBook[i][j]!=0)
			{
				/**************************************************************
				RUNNING CORRELATION FOR ORIGINAL IMAGE
				**************************************************************/
				count1 = 1;
				count2 = 1;
				while(pos_CodeBook[i][j+count1] == 0 && (j+count1) < CodeCOL-1)
				{
					count1++;
				}
				while(pos_CodeBook[i][j-count2] == 0 && (j-count2)> 0)
				{
					count2++;
				}

				if(count1>=count2)
				{
					c_adj = count1+j;
				}
				else
				{
					c_adj = j-count2;
				}

				
				pi = i*BLOCK_SIZE;
				pj = j*BLOCK_SIZE;
				ci = i*BLOCK_SIZE;
				cj = c_adj*BLOCK_SIZE;

				NumR = getPixelMean(ImgL, pi, pj, ImgL, ci, cj);

				meanOrig1 = getMean(ImgL, Nx, pi, pj);
				meanOrig2 = getMean(ImgL, Nx, ci, cj);

				Sx2 = getStatistic(ImgL, meanOrig1, Nx, pi, pj);
				Sy2 = getStatistic(ImgL, meanOrig2, Nx, ci, cj);

				DenR = Sx2*Sy2;
				if(DenR<0)
					DenR *= (-1);

				DenR = sqrt(DenR);

				r1 = NumR/DenR;


				/**************************************************************
				RUNNING CORRELATION FOR RECONSTRUCTED IMAGE
				**************************************************************/

				pos = pos_CodeBook[i][j] - 1;
				px = (pos/CodeCOL)*BLOCK_SIZE;
				py = (pos%CodeCOL)*BLOCK_SIZE;

				cx = px;
				//cy = (py + c_adj*BLOCK_SIZE);
				if(count1>=count2)
				{
					cy = py + ((count1%CodeCOL)*BLOCK_SIZE);
					//c_adj = count1+j;
				}
				else
				{
					cy = py - ((count2%CodeCOL)*BLOCK_SIZE);
					//c_adj = j-count2;
				}


				//pos2 = pos_CodeBook[i][c_adj] - 1;
				//cx = (pos2/CodeCOL)*BLOCK_SIZE;
				//cy = (pos2%CodeCOL)*BLOCK_SIZE;

				NumR = getPixelMean(ImgRecon, px, py, ImgRecon, cx, cy);

				meanRecon1 = getMean(ImgRecon, Nx, px, py);
				meanRecon2 = getMean(ImgRecon, Nx, cx, cy);

				Sx2 = getStatistic(ImgRecon, meanRecon1, Nx, px, py);
				Sy2 = getStatistic(ImgRecon, meanRecon2, Nx, cx, cy);

				DenR = Sx2*Sy2;
				if(DenR<0)
					DenR *= (-1);

				DenR = sqrt(DenR);
				r2 = NumR/DenR;

				/*********************************************************
				COMPARING THRESHOLD VALUES
				*********************************************************/

				r = r1 - r2;
				if(r<0)
					r*=(-1);

				average += r;

				if(r<CORRELATION_THRESHOLD)
				{
					//fprintf(outstream, "\n\nACCEPTED    Original Blocks (%d,%d),(%d,%d)      Correlation Value = %lf", pi,pj,ci,cj, r1);
					//fprintf(outstream, "\n              Reconstructed Blocks (%d,%d),(%d,%d) Correlation Value = %lf", px,py,cx,cy, r2);
					//fprintf(outstream, "\n              Correlated Value = %lf", r);
					accept++;
				}
				else
				{
					//fprintf(outstream, "\n\REJECTED     Original Blocks (%d,%d),(%d,%d)      Correlation Value = %lf", pi,pj,ci,cj, r1);
					//fprintf(outstream, "\n              Reconstructed Blocks (%d,%d),(%d,%d) Correlation Value = %lf", px,py,cx,cy, r2);
					//fprintf(outstream, "\n              Correlated Value = %lf", r);
					reject++;
				}
			}//end if

			else
			{
				//fprintf(outstream, "\n\nORIGINAL BLOCK USED FOR  Block (%d,%d)", pi,pj);
				orig++;
			}

		}// end nested for
	}//end for

	average = average/(CodeROW*CodeCOL);


	fprintf (outstream, "\n\n\nTEST STATISTICS\n");
	fprintf (outstream, "\nCORRELATION_THRESHOLD = %d", CORRELATION_THRESHOLD);
	fprintf (outstream, "\nBLOCK SIZE = %d \nPIX_ERROR = %d \n", BLOCK_SIZE, PIX_ERROR);
	fprintf (outstream, "\nOriginal Blocks used = %d", orig);
	fprintf (outstream, "\nACCEPTED Blocks = %d \nREJECTED Block = %d", accept, reject);
	fprintf (outstream, "\nAVERAGE = %lf", average);

	fprintf(outstream, "\n\n*****************************************************************");

	fclose(outstream);
}





void Mean_Test(int pos_CodeBook[][CodeCOL], int ImgRecon[][COL], int ImgL[][COL], char *st)
{
	/*
	Test two blocks in the Reconstructed and Original Image. The blocks to be tested should be from the reconstructed side.

	H0: Block i,j of original image is same as Block i,j of Reconstructed Image
	H1: The Two Blocks are not the same
	*/

	FILE *outstream = NULL;

	int Nx = BLOCK_SIZE*BLOCK_SIZE;
	int pos = 0;
	int i, j, ci, cj, pi, pj;
	double NumR, DenR;

	int reject_c95 = 0;
	int reject_c99 = 0;
	int accept_c95 = 0;
	int accept_c99 = 0;
	int reject_c = 0;
	int accept_c = 0;

	int orig_c = 0;
	double Sx2;
	double result;
	double meanOrig, meanRecon;
	char str[550];

	meanOrig = meanRecon = 0.0;

	strcpy(str, "C:\\Users\\Romil\\Desktop\\Pattern\\Output_Directory\\CompressedTestResults_");
	strcat(str, st);
	strcat(str, ".txt");
	outstream = fopen(str, "a");

	fprintf(outstream, "\n\n*********************************************************\n");
	fprintf(outstream, "\nMEAN TEST STATISTICS\n\n");
	//fprintf(outstream, "\nOriginal Image : %s \nReconstructed Image : %s\n", name_orig, name_recon);
	

	for(i = 0; i < CodeROW; i++)
	{
		for(j = 0; j < CodeCOL; j++)
		{
			if(pos_CodeBook[i][j]!=0)
			{
				pos = pos_CodeBook[i][j] - 1;
				pi = i*BLOCK_SIZE;
				pj = j*BLOCK_SIZE;

				ci = (pos/CodeCOL)*BLOCK_SIZE;
				cj = (pos%CodeCOL)*BLOCK_SIZE;

				meanOrig = getMean(ImgL, Nx, pi, pj);
				meanRecon = getMean(ImgRecon, Nx, ci, cj);

				NumR = meanOrig - meanRecon;

				if(NumR < 0)
					NumR = (-1)*NumR;

				Sx2 = getStatistic(ImgL, meanOrig, Nx, pi, pj);

				DenR = sqrt(Sx2);

				result = NumR/DenR;

				if(result > Z0_025 || result > Z0_005)
					reject_c++;
				else
					accept_c++;

				if(result > Z0_005) //95%
				{
					reject_c95++;
					//fprintf (outstream, "\nREJECT 95%%  Original Block %d,%d   Reconstructed Block %d,%d = %lf", pi, pj, ci, cj, result);
				}

				if(result > Z0_025) //99%
				{
					reject_c99++;
					//fprintf (outstream, "\nREJECT 99%%  Original Block %d,%d   Reconstructed Block %d,%d = %lf", pi, pj, ci, cj, result);
				}

				if(result < Z0_005) //95%
				{
					accept_c95 ++;
					//fprintf (outstream, "\nACCEPT 95%%  Original Block %d,%d   Reconstructed Block %d,%d = %lf", pi, pj, ci, cj, result);
				}

				if(result < Z0_025) //95%
				{
					accept_c99++;
					//fprintf (outstream, "\nACCEPT 99%%  Original Block %d,%d   Reconstructed Block %d,%d = %lf", pi, pj, ci, cj, result);
				}
			}

			else
			{
				orig_c++;
				//fprintf(outstream, "\nOriginal Block %d,%d Used", pi, pj);
			}
		}
	}

	fprintf (outstream, "\n\nTEST STATISTICS\n");
	fprintf (outstream, "\nBLOCK SIZE = %d \nPIX_ERROR = %d", BLOCK_SIZE, PIX_ERROR);
	fprintf (outstream, "\nZ0_005 = %lf \nZ0_025 = %lf", Z0_005, Z0_025);
	fprintf (outstream, "\nORIGINAL BLOCKS = %d", orig_c);
	fprintf (outstream, "\nACCEPTED BLOCKS = %d, \nREJECTED BLOCKS = %d\n", accept_c, reject_c);
	fprintf (outstream, "\nReconstructed Blocks with 95%% confidence ACCEPT = %d   REJECT = %d", accept_c95, reject_c95);
	fprintf (outstream, "\nReconstructed Blocks with 99%% confidence ACCEPT = %d   REJECT = %d", accept_c99, reject_c99);
	
	fprintf(outstream, "\n\n*********************************************************\n");
	fclose(outstream);
}