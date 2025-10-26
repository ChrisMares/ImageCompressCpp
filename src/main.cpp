#include <iostream>
#include <vector>
#include <filesystem>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "image_processor.h"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[], char *envp[])
{
    //add a stopwatch
    auto start = std::chrono::high_resolution_clock::now();
    cout << "ImageCompressCpp - starting ...." << endl;

    // CLI Args
    string _imgdir;
    string _outdir;
    int _size = 50;
    int _quality = 100;
    string _imgname;
    unsigned int _threads = std::thread::hardware_concurrency();

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
            cout << "--threads : Number of threads (default: CPU cores)." << endl;
            cout << "  ImageCompressCpp --imgdir /path/to/images --outdir /path/to/output --size 50 --quality 90 --threads 8" << endl;
            return 0;
        }
        else if (arg == "--imgdir") _imgdir = argv[++i];
        else if (arg == "--outdir") _outdir = argv[++i];
        else if (arg == "--size") _size = std::stoi(argv[++i]);
        else if (arg == "--quality") _quality = std::stoi(argv[++i]);
        else if (arg == "--imgname") _imgname = argv[++i];
        else if (arg == "--threads") _threads = std::stoi(argv[++i]);
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
            if(!_imgname.empty())
            {
                // if imgname is specified, only add that file if it matches
                if (entry.path().filename() == _imgname)
                {
                    allImgFiles.push_back(filepath);
                }
                continue;
            }
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
    cout << "Using " << _threads << " threads for processing." << endl;

    unsigned int originalFileCount = allImgFiles.size();
    std::atomic<unsigned int> processedFileCount{0};
    std::atomic<unsigned int> fileIndex{0};

    // Lamda function. Pass referecne to local varriables as needed 
    auto resize_img_processor = [&fileIndex, &allImgFiles, &_outdir, _size, _quality, &processedFileCount]() 
    {
        while (true) {
            int index = fileIndex.fetch_add(1);
            if (index >= allImgFiles.size()) {
                break;
            }
            
            ResizeImage(allImgFiles[index], _outdir, _size, _quality);
            processedFileCount++;
        }
    };

    // Create and launch threads
    std::vector<std::thread> threads;
    threads.reserve(_threads);
    for (unsigned int i = 0; i < _threads; ++i)
    {
        threads.emplace_back(resize_img_processor); //adds the worker function directly to the vector without extra copy
    }

    for (std::thread &thread : threads)
    {
        //Main function waits for all threads to finish here
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << " seconds." << endl;
    cout << "ImageCompressCpp - completed processing " << processedFileCount << " files." << endl;

    return 0;
}