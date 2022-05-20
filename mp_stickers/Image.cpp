#include "Image.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"

using cs225::HSLAPixel;
using cs225::PNG;

void Image::lighten() {
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            HSLAPixel & pixel = this->getPixel(x, y);
            pixel.l += 0.1;
            if (pixel.l > 1) {pixel.l = 1;}
        }
    }
}

void Image::lighten(double amount) {
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            HSLAPixel & pixel = this->getPixel(x, y);
            pixel.l += amount;
            if (pixel.l > 1) {pixel.l = 1;}
        }
    }
}

void Image::darken() {
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            HSLAPixel & pixel = this->getPixel(x, y);
            pixel.l -= 0.1;
            if (pixel.l < 0) {pixel.l = 0;}
        }
    }
}

void Image::darken(double amount) {
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            HSLAPixel & pixel = this->getPixel(x, y);
            pixel.l -= amount;
            if (pixel.l < 0) {pixel.l = 0;}
        }
    }
}

void Image::saturate() {
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            HSLAPixel & pixel = this->getPixel(x, y);
            pixel.s += 0.1;
            if (pixel.l > 1) {pixel.l = 1;}
        }
    }
}

void Image::saturate(double amount) {
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            HSLAPixel & pixel = this->getPixel(x, y);
            pixel.s += amount;
            if (pixel.l > 1) {pixel.l = 1;}
        }
    }
}

void Image::desaturate() {
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            HSLAPixel & pixel = this->getPixel(x, y);
            pixel.s -= 0.1;
            if (pixel.l < 0) {pixel.l = 0;}
        }
    }
}

void Image::desaturate(double amount) {
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            HSLAPixel & pixel = this->getPixel(x, y);
            pixel.s -= amount;
            if (pixel.l < 0) {pixel.l = 0;}
        }
    }
}

void Image::grayscale() {
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            HSLAPixel & pixel = this->getPixel(x, y);
            pixel.s = 0;
        }
    }
}

void Image::rotateColor (double degrees) {
    for (unsigned x = 0; x < this->width(); x++) {
        for (unsigned y = 0; y < this->height(); y++) {
            HSLAPixel & pixel = this->getPixel(x, y);
            if (pixel.h + degrees > 0) {
                pixel.h = fmod(pixel.h + degrees, 360);
            }
            
            if (pixel.h + degrees < 0) {
                pixel.h = pixel.h + degrees + 360;
            }
        }
    }
}

void Image::scale (double factor) {
    unsigned int newWidth = factor * this->width();
    unsigned int newHeight = factor * this->height();
    // Utilizes operator= to make a new copy
    Image copy = *this;
    this->resize(newWidth, newHeight);

    for (unsigned x = 0; x < newWidth; x++) {
        for (unsigned y = 0; y < newHeight; y++) {
                unsigned int newX = x / factor;
                unsigned int newY = y / factor;
                HSLAPixel & pixel = copy.getPixel(newX, newY);
                HSLAPixel & newPixel = this->getPixel(x, y);
                newPixel.h = pixel.h;
                newPixel.s = pixel.s;
                newPixel.l = pixel.l;
                newPixel.a = pixel.a;
        }
    }

}

void Image::scale (unsigned w, unsigned h) {
    double factor;
    double x = (double)w/(this->width());
    double y = (double)h/(this->height());
    if (x <= y) {
        factor = x;
    } else {
        factor = y;
    }
    this->scale(factor);
}