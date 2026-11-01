#include "bmp_writer.h"

#include <stdlib.h>
#include <stdio.h>

int saveBMP(const char* filepath, BMP_Image* image) {
    FILE* file = fopen(filepath, "wb");
    if(file == NULL) {
        fprintf(stderr, "Error: Can not create BMP file\n");
        return -1;
    }

    if(fwrite(&image->fileHeader, sizeof(BMP_FileHeader), 1, file) != 1) {
        fprintf(stderr, "Error: Failed to write BMP file header\n");
        goto cleanup;
    }

    if(fwrite(&image->infoHeader, sizeof(BMP_InfoHeader), 1, file) != 1) {
        fprintf(stderr, "Error: Failed to write BMP info header\n");
        goto cleanup;
    }

    uint32_t paletteSize = image->infoHeader.biClrUsed;

    if (paletteSize == 0) {
        paletteSize = 1u << image->infoHeader.biBitCount;
    }

    if(fwrite(image->palette, 4, paletteSize, file) != paletteSize) {
        fprintf(stderr, "Error: Failed to write BMP color palette\n");
        goto cleanup;
    }

    uint32_t width = (uint32_t)image->infoHeader.biWidth;
    uint32_t height = (uint32_t)image->infoHeader.biHeight;
    size_t imageSize = width * height;

    if(fwrite(image->pixels, sizeof(*image->pixels), imageSize, file) != imageSize) {
        fprintf(stderr, "Error: Failed to write BMP color palette\n");
        goto cleanup;
    }
    
    fclose(file);

    return 0;

    cleanup:
        fclose(file);
        return -1;
}
