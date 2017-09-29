#include "deconvolution/RichardsonLucy.hh"

namespace deconvolution {

RichardsonLucy::RichardsonLucy(unsigned flank, const double* psf, Magick::Image &image)
    : flank_(flank)
    , psf_(psf, psf + (2*flank+1)*(2*flank+1))
    , image_(image)
{
}

void RichardsonLucy::step()
{
    image_.modifyImage();
    const int startX = 0;
    const int startY = 0;
    const int widthX = image_.columns();
    const int widthY = image_.rows();
    Magick::Pixels view(image);
    Magick::PixelPacket *pixels = view.get(startX, startY, widthX, widthY);
    const std::vector<Magick::PixelPacket> imageCopy(pixels, pixels + widthX * widthY);
    for (int i = flank_; i + flank_ < widthX)
    {
        for (int j = flank_; j + flank_ < widthY)
        {
        }
    }
    view.sync();
}

} // namespace deconvolution

