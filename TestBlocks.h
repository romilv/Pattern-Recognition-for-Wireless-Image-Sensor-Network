#include"variables.h"
#ifndef TEST_BLOCKS_H
#define TEST_BLOCKS_H

void checkArrC(int ci, int cj, int B[][BLOCK_SIZE], int ImgR[][COL], int pos_CodeBook[][CodeCOL], int val_CodeBook[][CodeCOL]); 

double checkMSE (int B[][BLOCK_SIZE], int C[][BLOCK_SIZE]); 

void createCodeBook(int ImgL[][COL], int ImgR[][COL], int pos_CodeBook[][CodeCOL], int  val_CodeBook[][CodeCOL]);

double ImgCheckMSE (int B[][COL], int C[][COL]);

double ThumbImgCheckMSE(int B[][CodeCOL], int C[][CodeCOL]);

double checkPSNR(double MSE); 

void RejectVal(int pos_CodeBook[][CodeCOL], int val_CodeBook[][CodeCOL]);

double getMean(int arr[][COL], int Nx, int r, int c);

double getPixelMean(int ImgL[][COL], int pi, int pj, int ImgRecon[][COL], int ci, int cj);

double getStatistic(int arr[][COL], double meanOrig, int r, int c);

int getDifference(int arr[][COL], int r, int c);

#endif