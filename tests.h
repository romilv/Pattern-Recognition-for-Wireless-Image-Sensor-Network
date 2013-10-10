#include "variables.h"
#ifndef TESTS_H
#define TESTS_H

void generateDetails(char *st);

void Compression_Test(int pos_CodeBook[][CodeCOL], int val_CodeBook[][CodeCOL], char *st);

void Correlation_Test(int pos_CodeBook[][CodeCOL], int ImgL[][COL], int ImgRecon[][COL], char *st);

void Thumbnail_Test(int ImgL[][COL], int ImgReconstructed[][COL], int ImgThumbnail[][CodeCOL], int ImgReconThumbnail[][CodeCOL], char *st);

void Mean_Test(int pos_CodeBook[][CodeCOL], int ImgRecon[][COL], int ImgL[][COL], char *st);

void Simple_Test(int pos_CodeBook[][CodeCOL], int ImgReconstructed[][COL], int ImgL[][COL], char *st); 

#endif