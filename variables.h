#ifndef VARIABLES_H
#define VARIABLES_H

#define BLOCK_SIZE 6//4 = 4x4, 16 = 16x16

#define PIX_ERROR 5
#define THRESHOLD_MATCH_SCORE (PIX_ERROR*PIX_ERROR)
#define SIMPLE_TEST_THRESHOLD (BLOCK_SIZE*BLOCK_SIZE)
#define CORRELATION_THRESHOLD (BLOCK_SIZE*3)

#define Z0_005 2.575 //95%
#define Z0_025 1.960 //99%


#define L 255
#define L_SQUARE 65025 //255*255

#define ROW 240
#define COL 240

#define CodeROW (ROW/BLOCK_SIZE)
#define CodeCOL (COL/BLOCK_SIZE)

#define COL_LARGE 320


#endif
