//DEMO: Small XBitmaps 
//=================================
  //XBitmaps are well suited for storing graphic elements that can be prerendered and then placed where neeeded

#include "heltec_213r_v2.h"

//Here we are including a small image of a ball
#include "ball.xbm"

Heltec_213R_V2 panel(/* DC PIN */ 8, /* CS PIN */ 10, /* BUSY PIN */ 7);

void setup() {
  panel.begin();

  panel.setRotation(1);   //Don't forget to set the orientation, so your image fits how you intended

  while( panel.calculating() ) {
    //First draw a ball in the background, in red, slightly off screen
    panel.drawXBitmap(panel.centerX() - ball_width / 2, -10, ball_bits, ball_width, ball_height, panel.RED);

    //Now draw over it with:
    //A ball in the bottom left, in black
    panel.drawXBitmap(0, panel.bottom() - ball_height, ball_bits, ball_width, ball_height, panel.BLACK);
    
    //And another one in the top bottom, in black
    panel.drawXBitmap(panel.right() - ball_width, panel.bottom() - ball_height, ball_bits, ball_width, ball_height, panel.BLACK);
  }
}

void loop() {}
