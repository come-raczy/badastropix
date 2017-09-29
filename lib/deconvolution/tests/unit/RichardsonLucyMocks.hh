#ifndef COMMON_TIME_STAMP_HH
#define COMMON_TIME_STAMP_HH

#include <iostream>

#define THREAD_CERR std::ostream(0)

#endif // #ifndef COMMON_TIME_STAMP_HH

#include <vector>

// mocks for GraphicsMagick
namespace deconvolution {
namespace Magick {
namespace MocksForRichardsonLucy { // force local resolution

struct PixelPacket
{
};

struct Image
{
    virtual int columns() = 0;
    virtual int rows() = 0;
    // this implementation of RL applies to the whole image
    PixelPacket *get(int , int , int , int )
    {
        return pixels_.data();
    }
private:
    std::vector<PixelPacket> pixels_;
};

struct Point: public Image
{
};

struct Pixels
{
    Pixels(Image &image) : image_(&image) {}
    PixelPacket *get(int startX, int startY, int widthX, int widthY) {return image_->get(startX, startY, widthX, widthY);}
    Image *image_;
};

} // namespace MocksForRichardsonLucy

using MocksForRichardsonLucy::Image;
using MocksForRichardsonLucy::Point;

} // namespace Magick

} // namespace deconvolution

