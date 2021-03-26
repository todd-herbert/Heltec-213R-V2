//DEMO: Bitmap Data
//=================================
    //You may sometime have the need to draw raw bitmap data to the screen
    //This is is possible with drawBitmap()
    //One possible application here is that drawBitmap accepts data from SRAM, where as drawXBitmap will only draw from PROGMEM

#include "heltec_213r_v2.h"

//Sample set of raw bitmap data
#include "pencils.h"

Heltec_213R_V2 panel(/* DC PIN */ 8, /* CS PIN */ 10, /* BUSY PIN */ 7);

void setup() {
  panel.begin();

  panel.setRotation(1);   //Don't forget to set the correct orientation, so your image fits how you intended

  //Bitmap image data comes in all sorts of weird formats. To get it to work, you might have to play with the settings.
  panel.setFlip(panel.FLIP_NONE);  //If your image comes out flipped, change this
  panel.setDefaultColor(panel.BLACK); //If you get a negative of your image, try setting the background to black, and drawing the data as white

  while( panel.calculating() ) {
    panel.drawBitmap(0, 0, pencils, pencils_width, pencils_height, panel.WHITE);
  }
}

void loop() {}
