//DEMO: Shapes  - Kandinsky would be proud
//================================================

#include "heltec_213r_v2.h"


Heltec_213R_V2 panel(/* DC PIN */ 8, /* CS PIN */ 10, /* BUSY PIN */ 7);

void setup() {
  panel.begin();
  panel.setRotation(3); //Landscape, rotated 270 degrees from upright

  panel.setDefaultColor(panel.RED); //Our background color is now red

  while( panel.calculating() ) {

    panel.drawRoundRect(120, 20, 80, 60, 10, panel.WHITE); //Hollow
    panel.fillRoundRect(130, 30, 85, 65, 10, panel.WHITE); //Filled

    panel.fillRect(160, 50, 30, 30, panel.RED); //Drawing with our background color can help "erase" things

    panel.fillCircle(40, -10, 25, panel.BLACK);   //Drawing out of bounds is allowed
    panel.drawLine(40, -10, 300, 500, panel.WHITE); // <-- !!! But don't go crazy as every pixel gets processed, even if most of them are offscreen

    panel.fillTriangle(0,0, 30,panel.bottom(), 60,panel.bottom(), panel.BLACK);  //Handy functions like bottom() make drawing easier
    panel.fillCircle(panel.centerX(), panel.centerY(), 20, panel.BLACK);  //Right in the center
  }
}

void loop() {}
