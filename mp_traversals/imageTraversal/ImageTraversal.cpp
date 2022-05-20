#include <cmath>
#include <iterator>
#include <iostream>

#include "../cs225/HSLAPixel.h"
#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"

/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 *
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double h = fabs(p1.h - p2.h);
  double s = p1.s - p2.s;
  double l = p1.l - p2.l;

  // Handle the case where we found the bigger angle between two hues:
  if (h > 180) { h = 360 - h; }
  h /= 360;

  return sqrt( (h*h) + (s*s) + (l*l) );
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator() {
  /** @todo [Part 1] */
  imgT = NULL;
  finish = false;
}

ImageTraversal::Iterator::Iterator(const Point & start_, ImageTraversal * traversal) {
  imgT = traversal;
  png = traversal->getPNG();
  start = start_;
  curr = traversal->peek();
  tolerance = traversal->getTol();
  finish = false;
  visited = vector<bool>(png->height() * png->width(), false);
}

// ImageTraversal::Iterator::~Iterator() {
//   delete imgT;
// }
/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  /** @todo [Part 1] */

  // Mark visited & Push Neighbors
  helper();

  // Until S is empty or point is not visited and within tolerance, point = S.pop().

  // bool visit = visited[curr.y * png->width() + curr.x];
  // bool delta = calculateDelta(png->getPixel(curr.x, curr.y), png->getPixel(start.x, start.y)) >= tolerance;

  while (!imgT->empty() && (visited[curr.y * png->width() + curr.x] || calculateDelta(png->getPixel(curr.x, curr.y), png->getPixel(start.x, start.y)) >= tolerance)) {
    curr = imgT->pop();
  }
    //If S is not empty a)Set current = point b)Go to step 3.
  if (imgT->empty()) {
    finish = true;
  }
  return *this;
}

void ImageTraversal::Iterator::helper() {
  // Mark visited
  visited[curr.y * png->width() + curr.x] = true;

  // Push neighbors
  // imgT->add(curr);
  // Right
  if (curr.x < png->width()-1) {
    Point temp (curr.x+1, curr.y);
    imgT->add(temp);
  }
  // Down
  if (curr.y < png->height()-1) {
    Point temp (curr.x, curr.y+1);
    imgT->add(temp);
  }
  // Left
  if (0 < curr.x) {
    Point temp (curr.x-1, curr.y);
    imgT->add(temp);
  }
  // Up
  if (0 < curr.y) {
    Point temp (curr.x, curr.y-1);
    imgT->add(temp);
  }
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  /** @todo [Part 1] */
  return curr;
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  /** @todo [Part 1] */
  // Handle for the end case. Flag for the end.
  if (finish == other.finish) {return true;}
  return false;
}

