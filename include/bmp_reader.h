#ifndef BMP_READER_H
#define BMP_READER_H

#include "bmp.h"

BMP_Image *loadBMP(const char *filepath);
static int validateFileHeader(const BMP_FileHeader* fileHeader);
static int validateInfoHeader(const BMP_InfoHeader* infoHeader);
void freeBMP(BMP_Image* image);

#endif
