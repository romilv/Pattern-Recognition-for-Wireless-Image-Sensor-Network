#include <stdio.h>
#include "variables.h"
#include "TestBlocks.h"
#include "FileIO.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tests.h"
#include "output_images.h"

int main()
{
	int i = 0, j = 0;
	char ch;
	
	int file_Header[15] = {0};

	int ImgL[ROW][COL] = {0};
	int ImgR[ROW][COL] = {0};
	int ImgOrig[ROW][COL] = {0};

	int ImgThumbnail[CodeROW][CodeCOL] = {0};
	int ImgReconstructed[ROW][COL] = {0};
	int ImgReconThumbnail[CodeROW][CodeCOL] = {0};

	int pos_CodeBook[CodeROW][CodeCOL] = {0};
	double val_CodeBook[CodeROW][CodeCOL] = {0};

	char source_directory[550];
	char file_name[550];
	char line[80];
	char files_left[20][80];
	char files_right[20][80];
	char str[550];
	char st[500];
	char ch1[10], ch2[10], ch3[10], ch4[10];
	double MSE_Orig, MSE_Thumb;
	double PSNR_Orig, PSNR_Thumb = 0.0;

	FILE *fp = NULL;



	//BACKGROUND GENERATION 1
	//Files Loaded into files_left and files_right


	strcpy(source_directory, "C:\\Users\\Romil\\Desktop\\Pattern\\Background3.txt");
	ReadandLoadFiles(source_directory, files_left, files_right);

	//LOADING CODEBOOK

	for(i = 0; i < 4; i++)
	{
		LoadFileIntoArray(ImgL, files_left[i]);
		LoadFileIntoArray(ImgR, files_right[i]);
		printf("\nLoading Files into CodeBook");
		createCodeBook(ImgL, ImgR, pos_CodeBook, val_CodeBook);
	}

	LoadFileIntoArray(ImgL, "C:\\Users\\Romil\\Desktop\\Pattern\\Images_380_20_800\\imgL580.pgm");
	LoadFileIntoArray(ImgR, "C:\\Users\\Romil\\Desktop\\Pattern\\Images_380_20_800\\imgR580.pgm");


	strcpy(st, "block6_pix5_background3_imgL580_imgR580_NEW");


	//RECONSTRUCTING IMAGE USING ImgL, ImgR
	createReconstructed(pos_CodeBook, val_CodeBook, ImgReconstructed, ImgL, ImgR);

	//GENERATING DETAILS OF CONSTANTS IN FILE
	generateDetails(st, "From 1 - 160, 4 Frames");
	//GENERATING BOTH THUMBNAILS
	createThumbnail(ImgReconstructed, ImgReconThumbnail);
	createThumbnail(ImgL, ImgThumbnail);

	//RUNNING THE THUMBNAIL TEST
	Thumbnail_Test(ImgL, ImgReconstructed, ImgThumbnail, ImgReconThumbnail, st);
	//RUNNING THE COMPRESSION TEST
	Compression_Test(pos_CodeBook, val_CodeBook, st);
	//RUNNING THE SIMPLE TEST
	Simple_Test(pos_CodeBook, ImgReconstructed, ImgL, st);
	//RUNNING THE MEAN TEST
	Mean_Test(pos_CodeBook, ImgReconstructed, ImgL, st);
	//RUNNING THE CORRELATION TEST
	Correlation_Test(pos_CodeBook, ImgL, ImgReconstructed, st);

	/*strcpy(file_name, "C:\\Users\\Romil\\Desktop\\Pattern\\Output_Directory\\ImgRecon_");
	strcpy(file_name, st);
	strcpy(file_name, ".pgm");
	LoadArrayIntoFile(ImgReconstructed, file_name);

	strcpy(file_name, "C:\\Users\\Romil\\Desktop\\Pattern\\Output_Directory\\ThumbImgRecon_");
	strcpy(file_name, st);
	strcpy(file_name, ".pgm");
	LoadThumbnailArrayIntoFile(ImgReconThumbnail, file_name);*/

	printf("\nEnd of Program\n");


	return 0;
}