#include <iostream>
#include <vector>
#include <filesystem>

#include "file_helpers.h"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[], char *envp[])
{
    // CLI Args 
    string imgdir = "";
    string outdir = "";

    // print all envp
    //  for (char** env = envp; *env != nullptr; ++env) {
    //      cout << "Env: " << *env << endl;
    //  }

    for (int i = 0; i < argc; ++i)
    {
        cout << "Arg " << i << ": " << argv[i] << endl;
        string arg = argv[i];
        cout << "Processing argument: " << arg << endl;
        if (arg.rfind("--help", 0) == 0 || arg.rfind("-h", 0) == 0)
        {
            cout << "This program compresses images in a specified directory." << endl;
            cout << "It will only compress jpeg and png images." << endl;
            cout << "In the output directory specified it will create 3 folders sm, md, original." << endl;
            cout << "  ImageCompressCpp --imgdir /path/to/images --outdir /path/to/output" << endl;
        }
        if (arg.rfind("-", 0) == 0)
        {
            cout << "  This is an option argument." << endl;
        }
        if (arg.rfind("--", 0) == 0)
        {
            if (arg.find("imgdir") != std::string::npos)
            {
                imgdir = argv[i + 1];
                cout << "Image directory set to: " << imgdir << endl;
            }
            if (arg.find("outdir") != std::string::npos)
            {
                outdir = argv[i + 1];
                cout << "Output directory set to: " << outdir << endl;
            }
        }
    }

    if(imgdir.empty() || outdir.empty())
    {
        cout << "Error: Both --imgdir and --outdir must be specified." << endl;
        return 1;
    }
    if(!std::filesystem::exists(imgdir))
    {
        cout << "Error: Specified image directory does not exist: " << imgdir << endl;
        return 1;
    }

    CreateOutputDirectories(outdir);

    std::vector<string> allJpegFiles;
    std::vector<string> allPngFiles;

    for (const auto& entry : std::filesystem::directory_iterator(imgdir))
    {
        if (entry.is_regular_file())
        {
            string filepath = entry.path().string();
            if (entry.path().extension() == ".jpg" || entry.path().extension() == ".jpeg")
            {
                allJpegFiles.push_back(filepath);
                cout << "Found JPEG file: " << filepath << endl;
            }
            else if (entry.path().extension() == ".png")
            {
                allPngFiles.push_back(filepath);
                cout << "Found PNG file: " << filepath << endl;
            }
        }
    }

    //copy original files to outdir/original

    cout<<"Copying original files to " << outdir + "/original/" << endl;
    for (const auto& filepath : allJpegFiles)
    {
        std::filesystem::copy(filepath, outdir + "/original/" + std::filesystem::path(filepath).filename().string(), std::filesystem::copy_options::overwrite_existing);
    }

    for (const auto& filepath : allPngFiles)
    {
        std::filesystem::copy(filepath, outdir + "/original/" + std::filesystem::path(filepath).filename().string(), std::filesystem::copy_options::overwrite_existing);
    }

    return 0;
}   