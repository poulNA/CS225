#include "StickerSheet.h"
#include <iostream>
#include <cmath>
#include <cstdlib>



using namespace std;


StickerSheet::StickerSheet(const Image &picture, unsigned max) {
    img.resize(max, NULL);
    topLeft.resize(max);

    base = picture;
    
}


void StickerSheet::changeMaxStickers (unsigned max) {
    
    // if (max > img.size()) {
    //     while (max != img.size()) {
    //          img.push_back(NULL);
    //      }
    // }
    // if (max < img.size()) {
    //      while (max != img.size()) {
    //          img.pop_back();
    //     }
    // }
    img.resize(max, NULL);
    topLeft.resize(max);
    
}

int StickerSheet::addSticker (Image &sticker, unsigned x, unsigned y) {
    
    for (unsigned int i = 0; i < img.size(); i++) {
        if (img[i] == NULL) {
            img[i] = &sticker;
            topLeft[i].x = x;
            topLeft[i].y = y;
            // for (unsigned int j = 0; j < img.size(); j++) {
            //     std::cout << img[j]->x << " " << img[j]->y << " " << img[j]->width() << " " << img[j]->height() << std::endl;
            // }
            // std::cout << "done" << std::endl;
            return i;
        }
    }
    return -1;
}

bool StickerSheet::translate (unsigned index, unsigned x, unsigned y) {
    
    if (index > img.size()) {return false;}
    if (img[index] == NULL) {return false;}

    topLeft[index].x = x;
    topLeft[index].y = y;
    
    return true;
}

void StickerSheet::removeSticker (unsigned index){
    
    if (index < 0 || img.size() < index) {return;}
    // img.erase(img.begin() + index);
    img[index] = NULL;
    topLeft[index].x = 0;
    topLeft[index].y = 0;

    
}

Image* StickerSheet::getSticker (unsigned index) {
    if (0 > index || index > img.size()) {return NULL;}
    return img[index];
}

Image StickerSheet::render () const {
    Image result(base);
    
    unsigned int newW = base.width();
    unsigned int newH = base.height();
    // resize
    
    for (unsigned int i = 0; i < img.size(); i++) {
        if (img[i] == NULL) {continue;}
        unsigned int maxW = topLeft[i].x + img[i]->width();
        unsigned int maxH = topLeft[i].y + img[i]->height();
        
        if (maxW > newW) {
            newW = maxW;  
        }
        
        if (maxH > newH) {
            newH = maxH;
        }
    }
    
    result.resize(newW, newH);

    for (unsigned int i = 0; i < img.size(); i++) {
        
            
        if (img[i] == NULL) {
            continue;
        }
        for (unsigned int j = 0; j < img[i]->width(); j++) {
            for (unsigned int k = 0; k < img[i]->height(); k++) {
                
                if ((img[i]->getPixel(j, k)).a == 0) {continue;}
                
                unsigned int newX = topLeft[i].x + j;
                unsigned int newY = topLeft[i].y + k;
                
                result.getPixel(newX, newY) = img[i]->getPixel(j, k);
                
            }
        }
    }
    
    return result;
}