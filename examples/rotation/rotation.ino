//DEMO: setRotation() printing text different orientations
//================================================

#include "heltec_213r_v2.h"

Heltec_213R_V2 panel(/* DC PIN */ 8, /* CS PIN */ 10, /* BUSY PIN */ 7);

void setup() {
  panel.begin();
  panel.setTextSize(2);

  while( panel.calculating() ) {
    
    panel.setRotation(0);
    panel.setCursor(10, 10);
    panel.print("Zero");

    panel.setRotation(1);
    panel.setCursor(10, 10);
    panel.print("One");

    panel.setRotation(2);
    panel.setCursor(10, 10);
    panel.print("Two");

    panel.setRotation(3);
    panel.setCursor(10, 10);
    panel.print("Three");
    
  }
}

void loop() { }
