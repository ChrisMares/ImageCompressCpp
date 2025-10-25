#include "filesystem"
#include <iostream>
#include "file_helpers.h"
using std::string;

void CreateOutputDirectories(const string& outdir) {
    try {
        std::filesystem::create_directories(outdir + "/sm");
        std::filesystem::create_directories(outdir + "/md");
        std::filesystem::create_directories(outdir + "/original");
    } catch (const std::exception& e) {
        // Catch and report any errors (e.g., lack of permissions, invalid path characters)
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        throw;
    }
}