#include "bmp_reader.h"
#include "bmp_writer.h"

int main() {
    const char* filepath = "/home/IlyaS/Development/ParallelDIP/images/56.bmp";
   
    BMP_Image* image = loadBMP(filepath);
    
    saveBMP("out.bmp", image);

    freeBMP(image);

    return 0;
}