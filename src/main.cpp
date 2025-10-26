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
        const string &extension = std::filesystem::path(filepath).extension().string();

        if(!_imgname.empty())
        {
            // process only specified image
            if (std::filesystem::path(filepath).filename() != _imgname)
            {
                continue;
            }
        }

        try
        {
            // Load image - returns unsigned char* to pixel data
            int width, height, channels;
            unsigned char *input_pixels = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

            if (input_pixels == nullptr)
            {
                cout << "Failed to load image: " << filepath << endl;
                continue;
            }

            float aspect_ratio = static_cast<float>(width) / height;
            int new_width = (int)(width * (_size / 100.0f));
            int new_height = (int)(height * (_size / 100.0f));

            // Based on the channels choose pixel layout. PNGs can have 4 channels, that is the layering effect of the png
            stbir_pixel_layout pixel_layout;
            if (channels == 4) {
                pixel_layout = STBIR_RGBA;
            } else {
                pixel_layout = STBIR_RGB;
            }

            unsigned char *output_pixels = stbir_resize_uint8_srgb(
                input_pixels, width, height, 0,
                NULL, new_width, new_height, 0, // Pass NULL to allocate a new buffer
                pixel_layout
            );
        
            if (output_pixels)
            {
                const string filename = std::filesystem::path(filepath).stem().string();
                const string extension = std::filesystem::path(filepath).extension().string();
                const string outputPath = _outdir + "/" + filename + "_" + std::to_string(_size) + "_" + std::to_string(_quality) + extension;

                if (extension == ".png")
                {
                    int stride_in_bytes = new_width * channels;
                    stbi_write_png(outputPath.c_str(), new_width, new_height, channels, output_pixels, stride_in_bytes);
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

            STBIR_FREE(output_pixels, NULL); //Free the resize output         
            stbi_image_free(input_pixels); //Free the original image
        }
        catch (const std::exception &e)
        {
            cout << "ERROR **** processing file " << filepath << ": " << e.what() << endl;
        }
    }

    return 0;
}