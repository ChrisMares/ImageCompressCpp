#include <iostream>
#include <filesystem>

using std::cout;
using std::endl;
using std::string;

namespace fs = std::filesystem;

void print_help_msg()
{
    cout << R"(Usage: ImageCompress --imgdir <path> --outdir <path> [OPTIONS]

Compresses JPEG and PNG images in a specified directory.

Required Arguments:
  --imgdir <path>        Specify the input image directory.
  --outdir <path>        Specify the output directory.

Optional Arguments:
  --quality <1-100>      Specify the JPEG quality percentage. (default: 100)
  
  --size-factor <pct>    Resize percentage, keeping aspect ratio (e.g., 50). (default: 100)
  --width <pixels>       Resize to a specific width, keeping aspect ratio.
  --height <pixels>      Resize to a specific height, keeping aspect ratio.
                         (NOTE: Only one resize option: --size-factor, --width, or --height)

  --imgname <file>       Specify a single image filename to process from the imgdir.
  --threads <num>        Number of threads to use. (default: CPU cores - 2)
  -h, --help             Show this help message and exit.

Example:
  $ ./ImageCompress --imgdir /path/to/images --outdir /path/to/output --size-factor 50 --quality 80
)";
}

bool validate_params(const std::string &_imgdir,
                     const std::string &_outdir,
                     int _size,
                     int _quality,
                     int _width,
                     int _height)
{
    if (_imgdir.empty() || _outdir.empty())
    {
        cout << "Error: ****  --imgdir, --outdir, --size, and --quality are required parameters." << endl;
        print_help_msg();
        return false;
    }
    if (!std::filesystem::exists(_imgdir))
    {
        cout << "Error: Specified image directory does not exist: " << _imgdir << endl;
        return false;
    }
    if (_size <= 0)
    {
        cout << "Error: --size must be a positive integer." << endl;
        return false;
    }
    if (_quality < 1 || _quality > 100)
    {
        cout << "Error: --quality must be between 1 and 100." << endl;
        return false;
    }
    if(_width < 0)
    {
        cout << "Error: --width must be a non-negative integer." << endl;
        return false;
    }
    if(_height < 0)
    {
        cout << "Error: --height must be a non-negative integer." << endl;
        return false;
    }

    // check that only _size, _width, or _height is specified, not multiple
    unsigned int resize_params = 0;
    if (_size != 100)
        resize_params++;
    if (_width != 0)
        resize_params++;
    if (_height != 0)
        resize_params++;

    if (resize_params > 1)
    {
        cout << "Error: Only one of --size-factor, --width, or --height should be specified." << endl;
        return false;
    }

    return true; // All validation passed
}
