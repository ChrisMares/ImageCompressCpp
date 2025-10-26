// --- STB Implementation ---
// This is the ONE place this block lives
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h" // or stb_image_resize2.h
// --- End STB Implementation ---

#include <iostream>
#include <vector>
#include <filesystem>

#include "file_helpers.h"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[], char *envp[])
{
    cout << "ImageCompressCpp - starting ...." << endl;

    // CLI Args
    string _imgdir;
    string _outdir;
    int _size = 50;
    int _quality = 100;
    string _imgname;

    // print all envp
    //  for (char** env = envp; *env != nullptr; ++env) {
    //      cout << "Env: " << *env << endl;
    //  }

    // process CLI args
    for (int i = 0; i < argc; ++i)
    {
        const string &arg = string(argv[i]);
        if (arg.rfind("--help", 0) == 0 || arg.rfind("-h", 0) == 0)
        {
            cout << "This program compresses images in a specified directory." << endl;
            cout << "It will only compress jpeg and png images." << endl;
            cout << "--imgdir : Specify the input image directory." << endl;
            cout << "--outdir : Specify the output directory." << endl;
            cout << "--size : Specify the size percentage for resizing (default is 50%)." << endl;
            cout << "--quality : Specify the JPEG quality (default is 100)." << endl;
            cout << "--imgname : (Optional) Specify a single image filename to process." << endl;
            cout << "  ImageCompressCpp --imgdir /path/to/images --outdir /path/to/output --size 50 --quality 90" << endl;
            return 0;
        }
        else if (arg == "--imgdir") _imgdir = argv[++i];
        else if (arg == "--outdir") _outdir = argv[++i];
        else if (arg == "--size") _size = std::stoi(argv[++i]);
        else if (arg == "--quality") _quality = std::stoi(argv[++i]);
        else if (arg == "--imgname") _imgname = argv[++i];
    }

    if (_imgdir.empty() || _outdir.empty() || _size <= 0 || _quality <= 0)
    {
        cout << "Error:  --imgdir, --outdir, --size, and --quality are required parameters." << endl;
        return 1;
    }
    if (_imgdir == _outdir)
    {
        cout << "Error: --imgdir and --outdir cannot be the same directory. It will rewrite the files" << endl;
        return 1;
    }
    if (!std::filesystem::exists(_imgdir))
    {
        cout << "Error: Specified image directory does not exist: " << _imgdir << endl;
        return 1;
    }

    // creates outdir if it doesn't exist
    std::filesystem::create_directories(_outdir);

    std::vector<string> allImgFiles;

    for (const auto &entry : std::filesystem::directory_iterator(_imgdir))
    {
        if (entry.is_regular_file())
        {
            string filepath = entry.path().string();
            if (entry.path().extension() == ".jpg" || entry.path().extension() == ".jpeg")
            {
                allImgFiles.push_back(filepath);
            }
            else if (entry.path().extension() == ".png")
            {
                allImgFiles.push_back(filepath);
            }
        }
    }

    cout << "Found " << allImgFiles.size() << " image files in directory: " << _imgdir << endl;

    unsigned int originalFileCount = allImgFiles.size();
    unsigned int processedFileCount = 0;
    for (const string &filepath : allImgFiles)
    {
        try
        {
            const string &extension = std::filesystem::path(filepath).extension().string();

            int width, height, channels;
            unsigned char *input_pixels = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

            if (input_pixels == nullptr)
            {
                cout << "Failed to load image: " << filepath << endl;
                continue;
            }

            float aspect_ratio = (float)width / (float)height;

            int new_width = (float)width * (float)_size / 100.0f;
            int new_height = (float)height * (float)_size / 100.0f;

            // normal rgb layout
            stbir_pixel_layout pixel_layout = STBIR_RGB;

            unsigned char *output_pixels = stbir_resize_uint8_linear(
                input_pixels,          // Input buffer
                width, height,         // Input dimensions
                0,                     // Input stride
                NULL,                  // Output buffer (will be allocated by function)
                new_width, new_height, // Output dimensions
                0,                     // Output stride
                pixel_layout           // STBIR_RGB (3 channels, no alpha processing)
            );

            if (output_pixels)
            {
                string filename = std::filesystem::path(filepath).stem().string();
                string outputPath = _outdir + "/" + filename + "_" + std::to_string(_size) + "_" + std::to_string(_quality) + extension;
                // Where compression can happen via quality parameter
                if (extension == ".png")
                {
                    // convert png to jpg
                    stbi_write_png(outputPath.c_str(), new_width, new_height, channels, output_pixels, _quality);
                }
                else if (extension == ".jpeg" || extension == ".jpg")
                {
                    stbi_write_jpg(outputPath.c_str(), new_width, new_height, channels, output_pixels, _quality);
                }

                cout << ++processedFileCount << " of " << originalFileCount << ": " << outputPath << endl;
            }
            else
            {
                cout << "Failed to resize image: " << filepath << endl;
            }

            stbi_image_free(output_pixels);
            stbi_image_free(input_pixels);
        }
        catch (const std::exception &e)
        {
            cout << "ERROR **** processing file " << filepath << ": " << e.what() << endl;
        }
    }

    return 0;
}