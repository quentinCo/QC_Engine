#include <qc_graphic\Image.hpp>

// 
#include <sstream>

// std
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
// #define STBIW_ZLIB_COMPRESS // TODO see
#define STBI_FAILURE_USERMSG
#ifdef _WIN32
    #define STBI_MSC_SECURE_CRT
    // #define STBIW_WINDOWS_UTF8 // If necessary
#endif
#include <stb_image_write.h>
#include <stb_image.h>

// qc
#include <qc_graphic\Useful.hpp>

using namespace qc;

/*-------------------- IMAGE CONSTRUCTORS ---------------------------------*/
Image::Image(int width, int height, ColorFormat format)
    : width(width), height(height), format(format)
{
    int size = width * height *  format;
    this->data = std::make_unique<unsigned char[]>(size);
}

// Statics ----------------------------------------------------------------------------------------------------------------------------

bool Image::read(const std::string& path, Image& out, ColorFormat desiredFormat)
{
    int width, height, format;

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &format, desiredFormat);
    if (data == nullptr)
    {
        const char* mess = stbi_failure_reason();
        qc::Useful::PrintError("IMAGE IMPORT", mess);
        return false;
    }

    out.width = width;
    out.height = height;

    switch (format)
    {
        case 1:
            out.format = ColorFormat::GRAY;
            break;
        case 2:
            out.format = ColorFormat::GA;
            break;
        case 3:
            out.format = ColorFormat::RGB;
            break;
        case 4:
            out.format = ColorFormat::RGBA;
            break;
        default:
            out.format = ColorFormat::UNDEFINED;
            qc::Useful::PrintError("IMAGE IMPORT", "Image of unknown format ( not GRAY, GRAY ALPHA, RGB or RGBA)");
            return false;
    }
    
    out.data.reset(data);
    std::ostringstream mess;
    mess << "Image have been imported: " << path << "[" << out.width << " x " << out.height << "] - " << out.format << " channels";
    qc::Useful::PrintSucced(mess.str());

    return true;
}

// static void copy(const Image& in, Image& out);
// static void copy(const Buffer& in, Image& out);

void Image::copy(const unsigned char* data, int width, int height, ColorFormat format, Image& out)
{
    int size = width * height *  format;
    out.data = std::make_unique<unsigned char[]>(size);
    memcpy(out.data.get(), data, size * sizeof(unsigned char));

    out.width = width;
    out.height = height;
    out.format = format;
}

bool Image::write(const Image& image, const std::string& path, FileFormat format, int quality)
{
    int res = 0;
    std::string fileName = path;
    switch (format)
    {
    case FileFormat::PNG:
        fileName += ".png";
        res = stbi_write_png(fileName.c_str(), image.width, image.height, image.format, image.data.get(), image.width * image.format * sizeof(unsigned char));
        break;
    case FileFormat::JPG:
        fileName += ".jpg";
        res = stbi_write_jpg(fileName.c_str(), image.width, image.height, image.format, image.data.get(), quality);
        break;
    case FileFormat::BMP:
        fileName += ".bmp";
        res = stbi_write_bmp(fileName.c_str(), image.width, image.height, image.format, image.data.get());
        break;
    case FileFormat::TGA:
        fileName += ".tga";
        res = stbi_write_tga(fileName.c_str(), image.width, image.height, image.format, image.data.get());
        break;
    }

    if (res == 0)
    {
        qc::Useful::PrintError("IMAGE EXPORT", "Error in the image export process");
        return false;
    }

    std::ostringstream mess;
    mess << "Image have been exported at: " << fileName << "[" << image.width << " x " << image.height << "] - " << image.format << " channels";
    qc::Useful::PrintSucced(mess.str());
    return true;
}