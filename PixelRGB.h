#ifndef PIXELRGB_H
#define PIXELRGB_H

struct PixelRGB{
    int r;
    int g;
    int b;

    PixelRGB(int p_r=0, int p_g=0, int p_b=0);

    bool operator==(int const& num) const;
};

#endif // PIXELRGB_H
