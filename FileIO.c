#include <stdio.h>
#include "variables.h"
#include "TestBlocks.h"
#include "FileIO.h"
#include "output_images.h"
#include "tests.h"
#include <string.h>
#include <stdlib.h>


void GetHeader(int file_Header[15], char *file_name)
{
	int i = 0;
	FILE *fp = NULL;

	fp = fopen(file_name, "rb");

	while(i<15)
	{
		file_Header[i] = fgetc(fp);
		printf("%d", file_Header[i]);
		i++;
	}
	fclose(fp);

}

void copyFile() //Copies 1 file into another
{
     int ch;
     FILE *fp1 = NULL;
     FILE *fp2 = NULL;


     //fp1=fopen("\\..\\..\\Images_1_20_360\\imgL160.pgm", "rb");
     //fp2=fopen("\\..\\..\\Images_1_20_360\\test.pgm", "wb");

     fp1=fopen("C:\\Users\\Romil\\Desktop\\Pattern\\Images_1_20_360\\imgL20.pgm", "rb");
     fp2=fopen("C:\\Users\\Romil\\Desktop\\Pattern\\Images_1_20_360\\test.pgm", "wb");

	 //fseek to 14th byte

	 fseek(fp1, 14, SEEK_SET);

     while((ch=fgetc(fp1))!=EOF)
		 fputc(ch,fp2);

	 fclose(fp1);
	 fclose(fp2);
}


void ReadandLoadFiles(char *source_directory, char files_left[][80], char files_right[][80])  //Read and load Multiple L and corresponding R Files from a directory
{
	FILE *fp = NULL;
	int i,j;
	char line[81];
	int count;

	fp = fopen(source_directory, "r");

	if(fp == NULL)
	{
		printf("\nCannot Open");
		exit(1);
	}

	i = j = count = 0;

	while (fgets (line, 80, fp) != NULL ) // read a line
	{
		line[strlen(line)-1] = 0;
		if(count%2 == 0)
		{
			
			strcpy(files_left[i], line); // write the line
			//puts(files_left[i]);
			count++;
			i++;
			//printf("IF");
		}
		else
		{
			strcpy(files_right[j], line); // write the line
			//puts(files_right[j]);
			count++;
			j++;
			//printf("\nELSE");
		}
	}

	fclose(fp);
}


void LoadFileIntoArray(int arr[][COL], char *file_name) //For the Unsplit Image having 320 Columns
{

	FILE *fp1 = NULL;
	int ch = 0;
	int i = 0;
	int j = 0;
	int count = 0;

	fp1 = fopen(file_name, "rb"); //"C:\\Users\\Romil\\Desktop\\Pattern\\Images_1_20_360\\imgL20.pgm"

	if(fp1 == NULL)
	{
		printf("\nFile Not Found");
		exit(1);
	}
	//do fseek to 15th byte, header for thumbnail

	fseek(fp1, 15, SEEK_SET);

	while((ch=fgetc(fp1))!=EOF)
	{
		arr[i][j] = ch;
		i = (j==239)?i+1:i;
		j = (j==239)?0:j+1;
	}
	fclose(fp1);
}

void LoadArrayIntoFile(int arr[][COL], char *file_name)
{
	int ch;
	int i, j;
	char str[250];	

	FILE *fp1 = NULL;
	i = 0;
	j = 0;

	//strcpy(str, "C:\\Users\\Romil\\Desktop\\Pattern\\Images_1_20_360\\");
	//strcat(str, file_name);
	

	fp1=fopen(file_name, "wb");

	/*for(i = 0; i < 15; i++)
	{
		ch = file_Header[i];
		fputc(ch, fp1);
	}*/

	for(i = 0; i < ROW; i ++)
	{
		for(j = 0; j < COL; j++)
		{
			ch = arr[i][j];
			fputc(ch,fp1);
		}
	}

	fclose(fp1);
}

void LoadThumbnailArrayIntoFile(int arr[][CodeCOL], char *file_name)
{
	int ch;
	int i, j;
	char str[250];	

	FILE *fp1 = NULL;
	i = 0;
	j = 0;

	//strcpy(str, "C:\\Users\\Romil\\Desktop\\Pattern\\Images_1_20_360\\");
	//strcat(str, file_name);
	

	fp1=fopen(file_name, "wb");

	/*for(i = 0; i < 15; i++)
	{
		fputc(0,fp1);
	}*/


	for(i = 0; i < CodeROW; i ++)
	{
		for(j = 0; j < CodeCOL; j++)
		{
			ch = arr[i][j];
			fputc(ch,fp1);
		}
	}

	fclose(fp1);
}

void SplitImageLR(int ImgL[ROW][COL], int ImgR[ROW][COL], int Img[ROW][COL_LARGE], char file_ext[]) //eg Img180.pgm So file.ext will have 180.pgm
{
	int flag = 1;
	int ch, i, j, countj;

	char strL[250];
	char strR[250];

	strcpy(strL, (strcat("L", file_ext)));
	strcpy(strR, (strcat("R", file_ext)));

	for(i = 0; i < ROW; i ++)
	{
		for(j = 0; j < COL; j ++)
		{
			ImgL[i][j] = Img[i][j];
		}
	}
	//LoadArrayIntoFile(ImgL, strL);


	for(i = 0; i < ROW; i ++) 
	{
		for(j = 0, countj = COL_LARGE - COL; countj < COL_LARGE; j++, countj ++)
		{
			ImgR[i][j] = Img[i][countj];
		}
	}
	//LoadArrayIntoFile(ImgR, strR);
}