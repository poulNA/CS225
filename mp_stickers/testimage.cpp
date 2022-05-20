#include "Image.h"
#include "StickerSheet.h"

#include <iostream>

int main() {
  Image alma; alma.readFromFile("tests/alma.png");
  alma.scale(180, 80);
  std::cout << alma.width() << " " << alma.height() << std::endl;
  alma.writeToFile("tests/scale2x.png");
  return 0;
}
