#ifndef DECONVOLUTION_RICHARDSON_LUCY_HH
#define DECONVOLUTION_RICHARDSON_LUCY_HH

#include <vector>
#include <Magick++.h>


namespace deconvolution {

class RichardsonLucy
{
public:
    // \b the total width of the PSF is (2*flank+1)^2 
    RichardsonLucy(unsigned flank, const double* psf, Magick::Image &image);
    void step();
private:
    const int flank_;
    const std::vector<double> psf_;
    Magick::Image &image_;
};

} // namespace deconvolution

#endif // #ifndef DECONVOLUTION_RICHARDSON_LUCY_HH

