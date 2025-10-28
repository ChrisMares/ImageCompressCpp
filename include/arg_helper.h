#include <iostream>
#include <filesystem>

using std::cout;
using std::endl;
using std::string;

namespace fs = std::filesystem;

void print_help_msg();
bool validate_params(const std::string &_imgdir,
                     const std::string &_outdir,
                     int _size,
                     int _quality,
                     int _width,
                     int _height);