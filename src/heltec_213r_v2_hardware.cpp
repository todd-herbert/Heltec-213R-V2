//HARDWARE SPECIFIC METHODS
//==========================

#include "heltec_213r_v2.h"

///Wait until the PanelHardware is idle. Important as any commands made while PanelHardware is busy will be discarded.
void Heltec_213R_V2::wait() {
	while(digitalRead(pin_busy) == HIGH)	{	//Low = idle	
		delay(1);
	}
}

void Heltec_213R_V2::sendCommand(uint8_t command) {
	SPI.beginTransaction(spi_settings);
	digitalWrite(pin_dc, LOW);	//DC pin low, tell PanelHardware this spi transfer is a command
	digitalWrite(pin_cs, LOW);

	SPI.transfer(command);

	digitalWrite(pin_cs, HIGH);
	SPI.endTransaction();
}

void Heltec_213R_V2::sendData(uint8_t data) {
	SPI.beginTransaction(spi_settings);
	digitalWrite(pin_dc, HIGH);	//DC pin low, tell PanelHardware this spi transfer is a command
	digitalWrite(pin_cs, LOW);

	SPI.transfer(data);

	digitalWrite(pin_cs, HIGH);
	SPI.endTransaction();
}

///Wake the PanelHardware from sleep mode, so it can be changed
void Heltec_213R_V2::wake() {
	//To be honest, not having read the datasheet, this is all well over my head
	//It came copied and pasted from the half-baked heltec driver

	//But first, callback anyone?
	if(wake_callback != NULL) 
		(*wake_callback)();

	wait();

	sendCommand(0x12); // soft reset
	wait();

	sendCommand(0x74); //set analog block control     
	sendData(0x54);
	sendCommand(0x7E); //set digital block control          
	sendData(0x3B);

	sendCommand(0x01); //Driver output control      
	sendData(0xF9);
	sendData(0x00);
	sendData(0x00);

	sendCommand(0x11); //data entry mode       
	sendData(0x01);

	sendCommand(0x44); //set Ram-X address start/end position   
	sendData(0x01);
	sendData(0x10);    //0x0F-->(15+1)*8=128

	sendCommand(0x45); //set Ram-Y address start/end position          
	sendData(0xF9);   //0xF9-->(249+1)=250
	sendData(0x00);
	sendData(0x00);
	sendData(0x00); 

	sendCommand(0x3C); //BorderWavefrom
	sendData(0x01);	

  	sendCommand(0x18); 
	sendData(0x80);	

	sendCommand(0x4E);   // set RAM x address count to 0;
	sendData(0x01);
	sendCommand(0x4F);   // set RAM y address count to 0xF9-->(249+1)=250;    
	sendData(0xF9);
	sendData(0x00);

	wait();
}



///Draw the image in PanelHardware's memory to the screen
void Heltec_213R_V2::update(bool blocking) {
	sendCommand(0x22);
	sendData(0xF7);
	sendCommand(0x20);

	if (blocking) {
		wait();	//Block while the command runs
		if(sleep_callback != NULL) 
			(*sleep_callback)();
	}
}


///Clear the screen in one step
void Heltec_213R_V2::clear() {
	wake();
	wait();

	uint16_t x, y;
	uint8_t black_byte = (default_color & WHITE) * 255;	//We're filling in bulk here; bits are either all on or all off
	uint8_t red_byte = ((default_color & RED) >> 1) * 255;

	sendCommand(0x24);   //write RAM for black(0)/white (1)
	for(y = 0; y < panel_width; y++) {
		for(x = 0; x < panel_height; x++) {
			sendData(black_byte);
		}
  	}
	
    sendCommand(0x26);   //write RAM for red(1)/white (0)
	for(y = 0; y < panel_width; y++) {
		for(x=0; x < panel_height; x++) {
			sendData(red_byte);
		}
  	}
	update();
}



///Write one small slice of the screen
void Heltec_213R_V2::writePage() {

	//Calculate memory values for the slice
	//Note, x values are divided by 8 as horizontal lines are rows of 8bit bytes
	uint8_t start_x = 1;	//Note the offset
	uint8_t end_x = 16;		//This is taken from the official heltec driver

	//y locations are two bytes
	uint8_t end_y = page_top;		//Note the inversion of these two. No idea why, just note it.
	uint8_t start_y = page_bottom;	
	

	//Inform the panel hardware of our chosen memory location
	sendCommand(0x44);	//Memory X start - end
	sendData(start_x);
	sendData(end_x);
	sendCommand(0x45);	//Memory Y start - end
	sendData(start_y);
	sendData(0);										//Bit 8 - not required, max y is 250
	sendData(end_y);
	sendData(0);										//Bit 8 - not required, max y is 250
	sendCommand(0x4E);	//Memory cursor X
	sendData(start_x);
	sendCommand(0x4F);	//Memory cursor y
	sendData(start_y);
	sendData(0);										//Bit 8 - not required, max y is 250

	//Now we can send over our image data

	sendCommand(0x24);   //write memory for black(0)/white (1)
	for (uint16_t i = 0; i < page_bytecount; i++) {
		sendData(page_black[i]);
		//sendData(0);
	}

    sendCommand(0x26);   //write memory for red(1)/white (0)
	for (uint16_t i = 0; i < page_bytecount; i++) {
		sendData(page_red[i]);
	}

	wait();	
	//Nothing happens now until hardware.update() is called
}
