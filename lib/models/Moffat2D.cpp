#include "models/Moffat2D.hh"

namespace models {

double Moffat2D::integrate(double minX, double maxX, double minY, double maxY) const
{
    return 0.0;
}

double Moffat2D::pdAmplitude(const double x, const double y) const
{
    const double sinTheta = sin(theta_);
    const double cosTheta = cos(theta_);
    const double xp = (x - x0_)*cosTheta + (y - y0_)*sinTheta;
    const double yp = (x0_ - x)*sinTheta + (y - y0_)*cosTheta;
    const double rx = xp/sx_;
    const double ry = yp/sy_;
    return 1.0D / pow(1 + rx*rx + ry*ry, beta_);
}

} //namespace models

