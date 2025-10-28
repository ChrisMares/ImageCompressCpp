#include <iostream>
#include <vector>
#include <filesystem>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "image_processor.h"
#include "arg_helper.h"

using std::cout;
using std::endl;
using std::string;

void print_progress(int processed, int total) {

    const short bar_width = 60;
    float progress = (float)processed / total;

    if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;

    int pos = (int)(bar_width * progress);
    int percent = (int)(progress * 100.0);

    // Create the bar string
    std::string bar_filled(pos, '#');
    std::string bar_empty(bar_width - pos, ' ');

    // Print the bar with \r to return to the start of the line
    std::cout << "\r[" << bar_filled << bar_empty << "] " << percent << " % (" << processed << "/" << total << ")";
    std::cout.flush(); // Ensure it prints immediately
}

int main(int argc, char *argv[], char *envp[])
{
    // add a stopwatch
    auto start = std::chrono::high_resolution_clock::now();
    cout << "ImageCompressCpp - starting ...." << endl;

    // CLI Args
    string _imgdir;
    string _outdir;
    int _size = 100;
    int _quality = 100;
    int _width = 0;
    int _height = 0;
    string _imgname;

    unsigned int _threads = std::thread::hardware_concurrency();
    unsigned int _hardware_cores = _threads;

    // process CLI args
    for (int i = 0; i < argc; ++i)
    {
        const string &arg = string(argv[i]);
        if (arg.rfind("--help", 0) == 0 || arg.rfind("-h", 0) == 0)
        {
            print_help_msg();
            return 0;
        }
        else if (arg == "--imgdir")
            _imgdir = argv[++i];
        else if (arg == "--outdir")
            _outdir = argv[++i];
        else if (arg == "--size-factor")
            _size = std::stoi(argv[++i]);
        else if (arg == "--width")
            _width = std::stoi(argv[++i]);
        else if (arg == "--height")
            _height = std::stoi(argv[++i]);
        else if (arg == "--quality")
            _quality = std::stoi(argv[++i]);
        else if (arg == "--imgname")
            _imgname = argv[++i];
        else if (arg == "--threads")
        {
            int thread_arg = std::stoi(argv[++i]);
            if (thread_arg > 0)
            {
                if (thread_arg > _hardware_cores)
                    _threads = _hardware_cores;
                else
                    _threads = (unsigned int)thread_arg;
            }
        }

    } // End of CLI parsing loop

    if (!validate_params(_imgdir, _outdir, _size, _quality, _width, _height))
    {
        return 1; // exit on invalid args
    }

    // creates outdir if it doesn't exist
    std::filesystem::create_directories(_outdir);

    std::vector<string> allImgFiles;

    for (const auto &entry : std::filesystem::directory_iterator(_imgdir))
    {
        if (entry.is_regular_file())
        {
            string filepath = entry.path().string();
            if (!_imgname.empty())
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

    unsigned int originalFileCount = allImgFiles.size();
    std::atomic<unsigned int> processedFileCount{0};
    std::atomic<unsigned int> fileIndex{0};

    // adjust threads to match file count if less files than threads
    if (allImgFiles.size() < _threads)
    {
        _threads = allImgFiles.size();
    }

    // on a higher core cpus, leave two cores free for OS and Monitor thread
    if (_threads == _hardware_cores && _hardware_cores > 7)
    {
        _threads -= 2;
    }

    cout << "Found " << allImgFiles.size() << " image files in directory: " << _imgdir << endl;
    cout << "Moving resized images to output directory: " << _outdir << endl;
    cout << "Using " << _threads << " threads for processing." << endl;

    // Lamda function. Pass referecne to local varriables as needed
    auto resize_img_processor = [&fileIndex, &allImgFiles, &_outdir, &_size, &_quality, &processedFileCount, &_width, &_height]()
    {
        // Thread will run forever until all files are processed, either by this thread or others
        while (true)
        {
            // Each thread will fetch a thread-safe unique index to process
            int index = fileIndex.fetch_add(1);
            if (index >= allImgFiles.size())
            {
                break;
            }

            ResizeImage(allImgFiles[index], _outdir, _size, _quality, _width, _height);
            processedFileCount++;
        }
    };

    // Lamda function for monitoring progress
    auto monitor_worker = [&processedFileCount, &originalFileCount]()
    {
        unsigned short bar_width = 70;

        while (processedFileCount < originalFileCount)
        {
            print_progress(processedFileCount, originalFileCount);

            std::this_thread::sleep_for(std::chrono::milliseconds(40));
        }

        // Print final progress as 100%
        print_progress(processedFileCount, originalFileCount);
        cout << endl << endl;
    };

    std::thread monitor_thread = std::thread(monitor_worker);

    // Create and launch threads
    std::vector<std::thread> threads;
    threads.reserve(_threads);
    for (unsigned int i = 0; i < _threads; ++i)
    {
        threads.emplace_back(resize_img_processor); // adds the worker function directly to the vector without extra copy
    }

    // Main function waits for all threads to finish here
    for (std::thread &thread : threads)
    {
        thread.join();
    }

    monitor_thread.join(); // Wait for monitor thread to finish

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << " seconds." << endl;
    cout << "ImageCompressCpp - completed processing " << processedFileCount << " files." << endl;

    return 0;
}