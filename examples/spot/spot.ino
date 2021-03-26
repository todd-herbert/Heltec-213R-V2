//DEMO: A bare minimum example - one big ol' spot
//================================================

#include "heltec_213r_v2.h"

Heltec_213R_V2 panel(/* DC PIN */  8, /* CS PIN */  10, /* BUSY PIN */ 7);

void setup() {
	//Get everything ready
	panel.begin();
	
	//All drawing commands go intside this WHILE
	while ( panel.calculating() ) {
		//================================
		//Graphics commands here
		//For example:
		panel.fillCircle(50, 100, 20, panel.RED);
		//================================
	}
}

void loop() {}
