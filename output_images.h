#include"variables.h"
#ifndef OUTPUT_IMAGES_H
#define OUTPUT_IMAGES_H

void createThumbnail(int ImgL[][COL], int ImgThumbnail[][CodeCOL]);

void createReconstructed(int pos_CodeBook[][CodeCOL], int val_CodeBook[][CodeCOL], int ImageReconstructed[][COL], int ImgL[][COL], int ImgR[][COL]);

void Reconstructed(int pi, int pj, int ci, int cj, int ImgReconstructed[][COL], int ImgR[][COL]);

#endif