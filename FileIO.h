#include"variables.h"
#ifndef FILEIO_H
#define FILEIO_H

void copyFile();

void ReadandLoadFiles(char *source_directory, char files_left[][80], char files_right[][80]);

void GetHeader(int file_Header[], char *file_name);

void LoadFileIntoArray(int arr[][COL], char *file_name); //For the Unsplit Image having 320 Columns

void LoadArrayIntoFile(int arr[][COL], char *file_name);

void LoadThumbnailArrayIntoFile(int arr[][CodeCOL], char *file_name);

void SplitImageLR(int ImgL[][COL], int ImgR[][COL], int Img[][COL_LARGE], char file_ext[]);

#endif