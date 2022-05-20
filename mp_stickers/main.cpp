#include "Image.h"
#include "StickerSheet.h"


int main() {

  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //
  Image logo; logo.readFromFile("tests/spider.png");
  Image ag;    ag.readFromFile("tests/ag.png");
  Image th;    th.readFromFile("tests/th.png");
  Image tm;    tm.readFromFile("tests/tm.png");

  StickerSheet sheet(logo, 3);

  sheet.addSticker(ag, 100, 200);
  sheet.addSticker(th, 1000, 200);
  sheet.addSticker(tm, 2000, 200);
 
  sheet.render().writeToFile("myImage.png");

  return 0;
}
