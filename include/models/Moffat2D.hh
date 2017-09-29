#ifndef MODELS_MOFFAT_2_D_HH
#define MODELS_MOFFAT_2_D_HH

#include <cmath>

namespace models {

/**
 ** \b Moddat distribution in a two-dimensional space - suitable for digital images
 **/
class Moffat2D
{
public:
    /**
     ** \b Constructor
     **/
    Moffat2D(double background = 0.0, double amplitude = 1.0, double beta = 4.0, double x0 = 0.0, double y0 = 0.0, double sx = 1.0, double sy = 1.0, double theta = 0.0)
    : background_(background), amplitude_(amplitude), beta_(beta), x0_(x0), y0_(y0), sx_(sx), sy_(sy), theta_(theta)
    {}
    double getBackground() const {return background_;}
    double getAmplitude() const {return amplitude_;}
    double getBeta() const {return beta_;}
    double getX0() const {return x0_;}
    double getY0() const {return y0_;}
    double getSx() const {return sx_;}
    double getSy() const {return sy_;}
    double getTheta() const {return theta_;}
    double getFwhmX() const {return 2.0D * sx_ * sqrt(pow(2.0D, 1.0D / beta_) - 1.0D);}
    double getFwhmY() const {return 2.0D * sy_ * sqrt(pow(2.0D, 1.0D / beta_) - 1.0D);}
    void setBackground(double background) {background_ = background;}
    void setAmplitude(double amplitude) {amplitude_ = amplitude;}
    void setBeta(double beta) {beta_ = beta;}
    void setX0(double x0) {x0_ = x0;}
    void setY0(double y0) {y0_ = y0;}
    void setSx(double sx) {sx_ = sx;}
    void setSy(double sy) {sy_ = sy;}
    void setTheta(double theta) {theta_ = theta;}
    double operator()(const double x, const double y) const
    {
        const double sinTheta = sin(theta_);
        const double cosTheta = cos(theta_);
        const double xp = (x - x0_)*cosTheta + (y - y0_)*sinTheta;
        const double yp = (x0_ - x)*sinTheta + (y - y0_)*cosTheta;
        const double rx = xp/sx_;
        const double ry = yp/sy_;
        return background_ + amplitude_ / pow(1 + rx*rx + ry*ry, beta_);
    }
    double integrate(double minX, double maxX, double minY, double maxY) const;
    // partial derivative wrt background
    double pdBackground(const double, const double) const {return 1.0D;}
    // partial derivative wrt amplitude
    double pdAmplitude(const double x, const double y) const;
private:
    double background_;
    double amplitude_;
    double beta_;
    double x0_;
    double y0_;
    double sx_;
    double sy_;
    double theta_;
};

} // namespace models

#endif // #ifndef MODELS_MOFFAT_2_D_HH

