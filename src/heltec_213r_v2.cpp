#include "heltec_213r_v2.h"

///Begin the E-Ink library
void Heltec_213R_V2::begin(const PageProfile profile,  void (*wake_callback)(void), void (*sleep_callback)(void)) {
	//Store the page profile
	this->page_profile = profile;

	//Store the callbacks
	this->wake_callback = wake_callback;
	this->sleep_callback = sleep_callback;

	//Set the digital pins that supplement the SPI interface
	pinMode(pin_cs, OUTPUT);		//Incase we weren't give the standard pin 10 as SS
	pinMode(pin_dc, OUTPUT);
	pinMode(pin_busy, INPUT); 	//NOTE: do not use internal pullups, as we're reading a 3.3v output with our ~5v arduino
	
	//Prepare SPI
	SPI.begin();

	//Grab some memory for the pages
	page_bytecount = panel_width * page_profile.height / 8;		//nb: this is a class member and gets reused
	page_black = new uint8_t[page_bytecount];
	page_red = new uint8_t[page_bytecount];
	
	//Set height in the library
	_width = WIDTH = panel_width;
	_height = HEIGHT = panel_height;

	//Set an initial configuration for drawing
	setDefaultColor(WHITE);
	setTextColor(BLACK);
}



///Draw a single pixel. 
///This method is overriden from GFX_Root, and all other drawing methods pass through here
void Heltec_213R_V2::drawPixel(int16_t x, int16_t y, uint16_t color) {
	//Page cursor reads 1 during first iteration
	//So we often need to subtract 1

	//Flip the image if needed
	if (img_flip == FLIP_WIDTH)
		x = drawing_width - x;
	else if (img_flip == FLIP_HEIGHT)
		y = drawing_width - y;
	else if (img_flip == FLIP_WIDTH_AND_HEIGHT) {
		x = drawing_width - x;
		y = drawing_height - y;
	}

	//Translate the pixel here to allow screen rotation
	int16_t x1, y1;
	switch(rotation) {
		case 0:			//No rotation
			x1=x;
			y1=y;
			break;
		case 1:			//90deg clockwise
			x1 = (panel_width - y) - 7;	//First 8 pixels of data are discarded by screen
			y1 = x;
			break;
		case 2:			//180deg
			x1 = (panel_width - x) - 7;
			y1 = (panel_height - 1) - y;
			break;
		case 3:			//270deg clockwise
			x1 = y;
			y1 = (panel_height - 1) - x;
			break;
	}
	x = x1;
	y = y1;

	//Check if pixel falls in our page
	if(y >= page_top && y <= page_bottom && x >= 0 && x <= drawing_width - 1) {

		//Calculate a memory location for our pixel
		//A whole lot of emperically derived "inverting" went on here
		//The y values of the pixels in each page are inverted, but not the pages themselves
		//The bit order of the x pixels is inverted, but not the order of the pixels themselves
		//To top it off, one final inversion is needed in writePage(), but all the nonsense seems to balance out eventually
		//(This is probably all my fault)
		uint16_t memory_location;
		memory_location = (y - ((page_cursor - 1) * page_profile.height));	//Allow for y offset, to place it correctly in the current page
		memory_location = (page_profile.height - 1) - memory_location;	//Inverting our y location in the page. No idea why, but it makes the screen happy.
		memory_location *=  (panel_width / 8);	//Y value is the "outer loop"
		memory_location += (x / 8);	//Find which byte our desired bit is in
		uint8_t bit_location = x % 8;	//Find the location of the bit in which the value will be stored
		bit_location = 7 - bit_location;	//For some reason, the screen wants the bit order flipped. MSB vs LSB?
		
		//Insert the correct color values into the appropriate location
		uint8_t bitmask = ~(1 << bit_location);
		page_black[memory_location] &= bitmask;
		page_black[memory_location] |= (color & WHITE) << bit_location;
		page_red[memory_location] &= bitmask;
		page_red[memory_location] |= (color >> 1) << bit_location;

	}
}



///Set the color of the blank canvas, before any drawing is done
///Note: Function is efficient, but only takes effect at the start of a calculation. At any other time, use fillScreen()
void Heltec_213R_V2::setDefaultColor(uint16_t bgcolor) {
	default_color = bgcolor;
}

void Heltec_213R_V2::setFlip(Flip flip) {
	this->img_flip = flip;
}



///Clear the data arrays in between pages
void Heltec_213R_V2::clearPage(uint16_t bgcolor) {
	for (uint16_t i = 0; i < page_bytecount; i++) {
		uint8_t black_byte = (bgcolor & WHITE) * 255;	//We're filling in bulk here; bits are either all on or all off
		uint8_t red_byte = ((bgcolor & RED) >> 1) * 255;
		page_black[i] = black_byte;
		page_red[i] = red_byte;
	}
}



///Used with a WHILE loop, to break the graphics into small parts, and draw them one at a time
bool Heltec_213R_V2::calculating(bool blocking = true) {
	//NOTE: this loop runs BEFORE every new page.
	//This means that it is actually also processing all the data generated in the last loop

	//Some of this looks weird, but it's that everything is being evaluated at the start of the next loop
	//No real reason for this over a do while, just personal preference

	//Beginning of first loop
	//===============

	if (page_cursor == 0) {
		clearPage(default_color);
		wake();					//Get the panel ready to receive the spi data
		page_top = 0;
		page_bottom = page_profile.height - 1;
	}

	//End of each loop
	//===============
	else { 
		//Send off the old page
		writePage();

		//Calculate memory locations for the new page
		page_top = page_cursor * page_profile.height;
		page_bottom = page_top + page_profile.height - 1;
		clearPage(default_color);
	}

	//Check whether loop should continue
	//=============
	if ((page_cursor) == page_profile.count) {
		page_cursor = 0; //Reset for next time
		update(blocking);		//Refresh changes onto the screen
		return false;	//We're done
	}
	else {
		page_cursor++;
		return true;	//Keep looping
	}
}

///Set the text cursor according to the desired upper left corner
void Heltec_213R_V2::setCursorCorner( char* text, uint16_t x, uint16_t y) {
	int16_t offset_x, offset_y;
	getTextBounds(text, 0, 0, &offset_x, &offset_y, NULL, NULL);
	setCursor(x + offset_x, y + offset_y);
}