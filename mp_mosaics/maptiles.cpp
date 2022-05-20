/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"
#include "kdtree.h"
//#include "cs225/RGB_HSL.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    if (theTiles.size() == 0) {return NULL;}

    MosaicCanvas * canvas = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
    vector<Point<3>> data;
    map<Point<3>, TileImage *> hmap;
    for (unsigned long i = 0; i < theTiles.size(); i++) {
        Point<3> point = convertToXYZ(theTiles[i].getAverageColor());
        data.push_back(point);
        hmap.insert({point, &(theTiles[i])});
    }

    KDTree<3> treeData(data);

    for (int x = 0; x < theSource.getRows(); x++) {
        for (int y = 0; y < theSource.getColumns(); y++) {
            LUVAPixel color = theSource.getRegionColor(x, y);
            Point<3> pointColor = convertToXYZ(color);
            Point<3> setPoint = treeData.findNearestNeighbor(pointColor);
            TileImage * tImage = hmap[setPoint];
            canvas->setTile(x, y, tImage);
        }
    }

    return canvas;
}

