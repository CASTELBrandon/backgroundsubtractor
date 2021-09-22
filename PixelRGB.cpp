#include "PixelRGB.h"

PixelRGB::PixelRGB(int p_r, int p_g, int p_b) : r(p_r), g(p_g), b(p_b){}

bool PixelRGB::operator==(int const& num) const{
    return (r == num && g == num && b == num);
}
