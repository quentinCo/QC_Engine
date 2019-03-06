#pragma once

#include <string>
#include <glm\vec2.hpp>

namespace qc
{

enum class FileFormat
{
    PNG,
    JPG,
    BMP,
    TGA
};

enum ColorFormat
{
    UNDEFINED = 0,
    GRAY,
    GA, // Gray alpha
    RGB,
    RGBA
};

class Image
{
public:

    // Constructor - destructor
    Image() : width(0), height(0), format(ColorFormat::UNDEFINED), data(nullptr) {};
    Image(int width, int height, ColorFormat format);

    Image(const Image& o) = delete;
    Image(Image&& r) = default;

    ~Image() = default;

    // Operators
    Image& operator=(const Image& o) = delete;
    Image& operator=(Image&& r) = default;

    // Getters / Setters
    int getWidth() const
    {
        return this->width;
    }

    int getHeight() const
    {
        return this->height;
    }

    glm::vec2 getSize() const
    {
        return glm::vec2(this->width, this->height);
    }

    ColorFormat getColorFormat() const
    {
        return this->format;
    }

    int getBiteSize() const
    {
        return this->width * this->height * this->format * sizeof(unsigned char);
    }

    const unsigned char* getData() const
    {
        return this->data.get();
    }

    unsigned char* getData()
    {
        return this->data.get();
    }

    // Methods
    void flipOnYAxes(); // TODO
    
    //Image subChannel(int channel); // TODO: return an image make from the channel (in).


    // Statics
    static bool read(const std::string& path, Image& out, ColorFormat desiredFormat = UNDEFINED);
    // static void copy(const Image& in, Image& out);
    // static void copy(const Buffer& in, Image& out);
    static void copy(const unsigned char* data, int width, int height, ColorFormat format, Image& out);
    static bool write(const Image& image, const std::string& path, FileFormat format, int quality = 100);

private:
    int width;
    int height;
    ColorFormat format;
    std::unique_ptr<unsigned char[]> data;
};


}   //! namespace qc
