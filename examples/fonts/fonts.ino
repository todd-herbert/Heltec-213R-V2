//DEMO: Using fonts
//================================================

#include "heltec_213r_v2.h"

//Include the font you want from the Fonts folder. I believe that they take up a bit of memory, so maybe don't include them all
#include "Fonts/FreeSerifBold12pt7b.h"

Heltec_213R_V2 panel(/* DC PIN */ 8, /* CS PIN */ 10, /* BUSY PIN */ 7);

void setup() {
  panel.begin();
  panel.setRotation(1); //Landscape, text fits better that way

  panel.setFont( &FreeSerifBold12pt7b );   //Pass (the address of) the font to the library
  panel.setTextColor(panel.RED);  //Red text

  while( panel.calculating() ) {

    panel.setCursor(30, 50);            //Set the (word-processor-like) cursor to the abritrary position of x=30, y=50          
    panel.print("Fancy fonty text.");

  } //Note: setCursor needs to run inside of the calculating() loop, as it moves along with each letter typed.
    //This means we need to shunt it back to the start again when we recalculate the next section of the screen
}

void loop() { }
