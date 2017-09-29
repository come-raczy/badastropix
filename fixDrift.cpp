#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <numeric>
#include <cmath>
#include <Magick++.h>

#include "common/TimeStamp.hh"
#include "models/Moffat2D.hh"

const std::string imagePath("test-60x40.tif");
const std::string outputPath("new-60x40.tif");
const std::string imageUrl(std::string("file://") + imagePath);

class Feature
{
public:
    // enable sub-pixel resolution of the center
    float x;
    float y;
    float fwhm;
private:
};

std::vector<bool> findMaxima(const std::vector<double> &intensities, unsigned widthX, unsigned widthY, unsigned neighborhood = 1, double background = 0.0);
std::vector<Feature> extractFeatures(const std::vector<double> &intensities, const std::vector<bool> &isMaxima, unsigned widthX, unsigned widthY, unsigned neighborhood, double background);

int main()
{
    std::cerr << "Using " << MagickPackageName << " " << MagickLibVersionText << " (" << MagickReleaseDate << ") with QuantumDepth=" << QuantumDepth << std::endl;
    Magick::InitializeMagick("");
    //Magick::Image image(imageUrl);
    Magick::Image image(imagePath);
    // Ensure that there is only one reference to underlying image
    // If this is not done, then image pixels will not be modified.
    image.modifyImage();
    //image.read(imageUrl);
    //image.read(imagePath);
    std::cerr << "geometry: " << image.columns() << "x" << image.rows() << " (" << (image.columns() * image.rows()) << " pixels)" << std::endl;
    std::cerr << "Quantum size: " << sizeof(Magick::Quantum) << std::endl;
    const Magick::Color &color = image.pixelColor(100, 100);
    const Magick::Quantum &red = color.redQuantum();
    const auto &alpha = color.alphaQuantum();
    std::cerr << "red: " << red << ", alpha: " << alpha << std::endl;
    Magick::Pixels view(image);
    // work on the underlying Quantum array for a local area of the pixelCache
    const unsigned startX = 0;
    const unsigned startY = 0;
    const unsigned widthX = image.columns();
    const unsigned widthY = image.rows();
    // ImageMagick returns a pointer to Quantum
    // Magick::Quantum *pixels = view.get(startX, startY, widthX, widthY);
    // GraphicsMagic uses an intermediate PixelPacket data type
    Magick::PixelPacket *pixels = view.get(startX, startY, widthX, widthY);
    const Magick::PixelPacket &p=pixels[0];
    THREAD_CERR << "pixels[0]: {red=" << p.red << ", green=" << p.green << ", blue=" << p.blue << ", opacity=" << p.opacity << "}" << std::endl;
    std::vector<double> intensities;
    intensities.reserve(widthX * widthY);
    std::transform(pixels, pixels + widthX * widthY, std::back_inserter(intensities), [](const Magick::PixelPacket &p) {return (unsigned)(sqrt((double)p.red*p.red + (double)p.green*p.green + (double)p.blue*p.blue));} );
    THREAD_CERR << "intensities.size(): " << intensities.size() << std::endl;
    const double background = [](const std::vector<double> &intensities){auto tmp = intensities; std::sort(tmp.begin(), tmp.end()); return tmp[tmp.size()/1.2];}(intensities);
    THREAD_CERR << "10% background threshold: " << background << std::endl;
    for (unsigned i = 0; i < 10; ++i)
    {
        const Magick::PixelPacket &p=pixels[i];
        THREAD_CERR << "Offset " << i << ": {red=" << p.red << ", green=" << p.green << ", blue=" << p.blue << ", opacity=" << p.opacity << "}: intensity=" << intensities[i] << std::endl; 
    }
    const std::vector<bool> isMaxima = findMaxima(intensities, widthX, widthY, 1, background);
    const size_t total = std::accumulate(isMaxima.begin(), isMaxima.end(), 0);
    THREAD_CERR << "count of maxima: " << total << std::endl;
    const Magick::Quantum maxChanel = Magick::Quantum(0) - 1;
    for (size_t offset = 0; isMaxima.size() > offset; ++offset)
    {
        if (isMaxima[offset])
        {
            pixels[offset - widthX].red = maxChanel;
            pixels[offset + widthX].red = maxChanel;
            pixels[offset - 1].red = maxChanel;
            pixels[offset + 1].red = maxChanel;
            pixels[offset].red = maxChanel;
            const Magick::PixelPacket marker{maxChanel/2, maxChanel, maxChanel/2, 0};
            pixels[offset] = marker;
        }
    }
    THREAD_CERR << "creating a Moffat structure" << std::endl;
    const double X0 = 20;
    const double Y0 = 20;
    const double BACKGROUND = 0;
    const double AMPLITUDE = 50000;
    const double BETA = 4.7;
    const double SX = 2;
    const double SY = 3;
    const double THETA = M_PI / 6.0;
    const models::Moffat2D moffat(BACKGROUND, AMPLITUDE, BETA, X0, Y0, SX, SY, THETA);
    const int N = 10;
    const int x0 = X0;
    const int y0 = Y0;
    for (int i = -N; i <= N; ++i)
    {
        for (int j = -N; j <= N; ++j)
        {
            const double m = moffat(x0+i, y0+j);
            const auto r = std::min(60000, (int)(pixels[(x0 + i) * widthX + y0 + j].red + m));
            const auto g = std::min(60000, (int)(pixels[(x0 + i) * widthX + y0 + j].green + m));
            const auto b = std::min(60000, (int)(pixels[(x0 + i) * widthX + y0 + j].blue + m));
            pixels[(x0 + i) * widthX + y0 + j].red = r;
            pixels[(x0 + i) * widthX + y0 + j].green = g;
            pixels[(x0 + i) * widthX + y0 + j].blue = b;
        }
    }
    view.sync();
    image.write(outputPath);
}


std::vector<bool> findMaxima(const std::vector<double> &intensities, const unsigned widthX, const unsigned widthY, const unsigned neighborhood, const double background)
{
    std::vector<bool> isMaxima(intensities.size(), false);
    for (size_t row = neighborhood; row + neighborhood < widthY; ++row)
    {
        const size_t rowBegin = row * widthX;
        const size_t rowEnd = rowBegin + widthX;
        for (size_t offset = row * widthX + neighborhood; offset + neighborhood < rowEnd; ++offset)
        {
            // check if local intensity is a maximum
            const auto current = intensities[offset];
            isMaxima[offset] = (current > background);
            for (int i = -(int)neighborhood; i <= (int)neighborhood; ++i)
            {
                for (int j = -(int)neighborhood; j <= (int)neighborhood; ++j)
                {
                    isMaxima[offset] = isMaxima[offset] & (current >= intensities[(int)offset + (int)i*(int)widthX + j]);
                }
            }
        }
    }
    return isMaxima;
}

std::vector<Feature> extractFeatures(const std::vector<double> &intensities, const std::vector<bool> &isMaxima, const unsigned widthX, const unsigned widthY, const unsigned neighborhood, const double background)
{
    std::vector<Feature> features;
    return features;
}
