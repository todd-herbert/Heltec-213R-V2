//DEMO: Multicolor XBitmap Images
//=================================
    //XBitmap is an old image file format from the early days of the internet
    //It was very inefficient as the imagedata was stored more or less as human readable C code
    //This, however, serves our purposes very well. As such, Adafruit have chosen to add support for XBM images
    //These can be easily created with the free GIMP software.

    //The process of creating these images (especially multicolored) is a little bit involved.
    //Please see the tutorial at:

    // https://github.com/todd-herbert/Heltec-213R-V2/blob/main/docs/XBitmapTutorial/xbitmap-tutorial.md

#include "heltec_213r_v2.h"

//Here we are including the two (slightly modified) xbm files that we created earlier, one for each of our color
#include "apples_black.h"
#include "apples_red.h"



Heltec_213R_V2 panel(/* DC PIN */ 8, /* CS PIN */ 10, /* BUSY PIN */ 7);

void setup() {
  panel.begin();

  panel.setRotation(1);   //Don't forget to set the orientation, so your image fits how you intended

  while( panel.calculating() ) {
    //Draw each image to its destination color
    panel.drawXBitmap(0, 0, apples_black_bits, apples_black_width, apples_black_height, panel.BLACK);
    panel.drawXBitmap(0, 0, apples_red_bits, apples_red_width, apples_red_height, panel.RED);
  }
}

void loop() {}