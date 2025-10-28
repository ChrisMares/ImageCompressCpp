// --- STB Implementation ---
// This is the ONE place this block lives
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h" // or stb_image_resize2.h
// --- End STB Implementation ---

#include "image_processor.h"
#include <iostream>
#include <filesystem>

using std::cout;
using std::endl;
using std::string;

void ResizeImage(const std::string &filepath,
                 const std::string &_outdir,
                 int _size,
                 int _quality,
                 int _width,
                 int _height)
{
    try
    {
        int orig_width, orig_height, channels;
        unsigned char *input_pixels = stbi_load(filepath.c_str(), &orig_width, &orig_height, &channels, 0);

        if (input_pixels == nullptr)
        {
            cout << "Failed to load image: " << filepath << endl;
            return;
        }

        float aspect_ratio = (float)orig_width / (float)orig_height;

        int new_width, new_height;

        //only one of these size params will be set or there is an error thrown validating the params
        
        if(_width != 0)
        {
            new_width = _width;
            new_height = static_cast<int>(_width / aspect_ratio);
        }
        else if(_height != 0)
        {
            new_height = _height;
            new_width = static_cast<int>(_height * aspect_ratio);
        }
        else //size
        {
            new_width = (int)(orig_width * (_size / 100.0f));
            new_height = (int)(orig_height * (_size / 100.0f));
        }

        // Based on the channels choose pixel layout. PNGs can have 4 channels, that is the layering effect of the png
        stbir_pixel_layout pixel_layout;
        if (channels == 4)
            pixel_layout = STBIR_RGBA;
        else
            pixel_layout = STBIR_RGB;

        unsigned char *output_pixels = stbir_resize_uint8_srgb(
            input_pixels,
            orig_width,
            orig_height,
            0,
            NULL,
            new_width,
            new_height,
            0,
            pixel_layout);

        if (output_pixels)
        {
            const string filename = std::filesystem::path(filepath).stem().string();
            const string extension = std::filesystem::path(filepath).extension().string();
            const string outputFile = _outdir + "/" + filename + "_" + std::to_string(_size) + "_" + std::to_string(_quality) + extension;

            if (extension == ".png")
            {
                int stride_in_bytes = new_width * channels;
                stbi_write_png(outputFile.c_str(), new_width, new_height, channels, output_pixels, stride_in_bytes);
            }
            else if (extension == ".jpeg" || extension == ".jpg")
            {
                stbi_write_jpg(outputFile.c_str(), new_width, new_height, channels, output_pixels, _quality);
            }
        }
        else
        {
            cout << "ERROR **** Failed to resize image: " << filepath << endl;
        }

        STBIR_FREE(output_pixels, NULL); // Free the resize output
        stbi_image_free(input_pixels);   // Free the original image
    }
    catch (const std::exception &e)
    {
        cout << "Exception occurred while processing image: " << filepath << ". Error: " << e.what() << endl;
        return;
    }

    return;
}