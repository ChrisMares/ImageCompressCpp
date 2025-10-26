#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize2.h"
#include <string> 
#include "image_processor.h"

using std::string;


// void ResizeImage(int percentage, const string& inputPath, const string& outputPath) {
//     int width, height, channels;
//     unsigned char* input_pixels = stbi_load(inputPath.c_str(), &width, &height, &channels, 0);
//     if (input_pixels == nullptr) {
//         throw std::runtime_error("Failed to load image: " + inputPath);
//     }

//     int new_width = width * percentage / 100;
//     int new_height = height * percentage / 100;
//     unsigned char* output_pixels = new unsigned char[new_width * new_height * channels];

//     stbir_resize_uint8(input_pixels, width, height, 0,
//                        output_pixels, new_width, new_height, 0,
//                        channels);

//     stbi_write_jpg(outputPath.c_str(), new_width, new_height, channels, output_pixels, 90);

//     stbi_image_free(input_pixels);
//     delete[] output_pixels;
// }