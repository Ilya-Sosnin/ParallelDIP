#include "bmp_reader.h"

#include <stdio.h>
#include <stdlib.h>

BMP_Image* loadBMP(const char* filepath) {
    FILE* file = fopen(filepath, "rb");
    if(file == NULL) {
        fprintf(stderr, "Error: Can not open BMP file\n");
        return NULL;
    }

    BMP_Image* image = calloc(1, sizeof(*image));
    if (image == NULL) {
        fclose(file);
        return NULL;
    }

    if(fread(&image->fileHeader, sizeof(BMP_FileHeader), 1, file) != 1) {
        fprintf(stderr, "Error: Failed to read BMP file header\n");
        goto cleanup;
    }

    if (validateFileHeader(&image->fileHeader) == -1) {
        goto cleanup;
    }

    if(fread(&image->infoHeader, sizeof(BMP_InfoHeader), 1, file) != 1) {
        fprintf(stderr, "Error: Failed to read BMP info header\n");
        goto cleanup;
    }

    if (validateInfoHeader(&image->infoHeader) == -1) {
        goto cleanup;
    }

    uint32_t paletteSize = image->infoHeader.biClrUsed;

    if(paletteSize == 0) {
        paletteSize = 1u << image->infoHeader.biBitCount;
    }

    image->palette = malloc(paletteSize * 4);
    if (image->palette == NULL) {
        goto cleanup;
    }

    if (fread(image->palette, 4, paletteSize, file) != paletteSize) {
        fprintf(stderr, "Error: Failed to read BMP color palette\n");
        goto cleanup;
    }

    uint32_t width = (uint32_t)image->infoHeader.biWidth;
    uint32_t height = (uint32_t)image->infoHeader.biHeight;
    size_t imageSize = width * height;

    image->pixels = malloc(imageSize);
    if (image->pixels == NULL) {
        goto cleanup;
    }

    if (fseek(file, image->fileHeader.bfOffBits, SEEK_SET) != 0) {
        fprintf(stderr, "Error: Failed to seek to BMP pixel data\n");
        goto cleanup;
    }

    if (fread(image->pixels, 1, imageSize, file) != imageSize) {
        fprintf(stderr, "Error: Failed to read BMP pixel data\n");
        goto cleanup;
    }

    fclose(file);

    return image;

    cleanup:
        freeBMP(image);
        fclose(file);
        return NULL;
}

int validateFileHeader(const BMP_FileHeader* fileHeader) {
    if (fileHeader == NULL) {
        fprintf(stderr, "Error: File header is NULL\n");
        return -1;
    }

    if (fileHeader->bfType != 0x4D42) {
        fprintf(stderr, "Error: Invalid BMP signature\n");
        return -1;
    }
  
    if (fileHeader->bfReserved1 != 0 || fileHeader->bfReserved2 != 0) {
        fprintf(stderr, "Error: Reserved fields must be zero\n");
        return -1;
    }

    return 0;
}

int validateInfoHeader(const BMP_InfoHeader* infoHeader) {
    if (infoHeader == NULL) {
        fprintf(stderr, "Error: Info header is NULL\n");
        return -1;
    }

    if (infoHeader->biSize != 40) {
        fprintf(stderr, "Error: Unsupported BMP header size\n");
        return -1;
    }

    if (infoHeader->biBitCount != 8) {
        fprintf(stderr, "Error: Unsupported bit depth\n");
        return -1;
    }

    if (infoHeader->biWidth <= 0) {
        fprintf(stderr, "Error: Invalid image width\n");
        return -1;
    }

    if (infoHeader->biHeight == 0) {
        fprintf(stderr, "Error: Invalid image height\n");
        return -1;
    }

    if (infoHeader->biCompression != 0) {
        fprintf(stderr, "Error: Compressed BMP images are not supported\n");
        return -1;
    }

    return 0;
}

void freeBMP(BMP_Image* image) {
    if (image != NULL) {
        free(image->palette);
        free(image->pixels);
        free(image);
    }
}