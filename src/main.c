#include "bmp_reader.h"

int main() {
    const char* filepath = "/home/IlyaS/Development/ParallelDIP/images/56.bmp";
   
    BMP_Image* image = loadBMP(filepath);
    freeBMP(image);

    return 0;
}