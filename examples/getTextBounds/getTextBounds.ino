  //DEMO: Placing text accurately with getTextBounds()
  //================================================
        //NOTE: THIS FUNCTION DOES NOT APPEAR TO WORK CORRECTLY WITH THE BUILT-IN FALLBACK FONT
        //So be sure to use setFont()!
        //This is an error in GFX_Root and not an error with the Heltec_213R_V2 library

#include "heltec_213r_v2.h"

#include "Fonts/FreeSerifBoldItalic9pt7b.h"   //Include the particular font

Heltec_213R_V2 panel(/* DC PIN */ 8, /* CS PIN */ 10, /* BUSY PIN */ 7);

void setup() {
  panel.begin();
  panel.setRotation(1);  //Landscape

  panel.setFont( &FreeSerifBoldItalic9pt7b );   //Pass (the address of) the font to the library
  const char *text = {"Fancy fonty text."};

  //Precisely place our text using getTextBounds()
  //================================================


  int16_t text_top_edge; //These will receive information about how much space our text will take up 
  int16_t text_left_edge;
  uint16_t text_width;
  uint16_t text_height;
  
  //This will tell us where the bounds of the text would be, if we setCursor(0,0) and then print(text)
  //Note that the variables are passed by reference
  panel.getTextBounds(text, 0, 0, &text_left_edge, &text_top_edge, &text_width, &text_height);

  //We can use this information in our loop to help place the string
  //We'll show off and alight our text right
  //setCursor()'s Y value is the imaginary line that the characters sit on. In this case, slightly above the base of the screen
  uint16_t cursor_demo_x = panel.right() - text_width;
  uint16_t cursor_demo_y = panel.bottom() - 30;



  //Graphics GO!
  //==============================================

  while( panel.calculating() ) {

    panel.setCursor(cursor_demo_x, cursor_demo_y);          
    panel.print(text);

    //Lets draw a line across the screen at cursor height, to really make the point clear
    panel.drawLine(0, cursor_demo_y, panel.right(), cursor_demo_y, panel.RED);

  }
}

void loop() { }
