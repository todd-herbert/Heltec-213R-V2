
# Heltec-213R-V2
Arduino Library for the **Heltec 2.13 Inch Red V2** E-Ink Display
Supports run-time graphics and text generation using Adafruit-GFX (via [ZinggJM/GFX_Root](https://github.com/ZinggJM/GFX_Root))

This is made possible with *"paging".*

[Huh? Paging?](#what-is-paging)

[Using the library](#using-the-library)

[Wiring](#wiring)

[Configuration](#configuration)

[Is my display supported?](#is-my-display-supported)

[Acknowledgements](#acknowledgements)


## What is paging?
I don't know. But we're going to use the word anyway.
In this context it means that the display is calculated and transmitted in several small pieces, rather than in one big memory-clogging lump. 

This means more calculations are performed, but less RAM is needed.
The resulting increase in calculation time ends up being insignificant, especially when compared to the slow refresh time of E-INK displays.

#### Alright, but how do I use it?




## Using the library
```c++
#include "heltec_213r_v2.h"

Heltec_213R_V2  panel(/* DC PIN */  8, /* CS PIN */  10, /* BUSY PIN */ 7);

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
```
To summarise: 
* Set your hardware pins in the constructor

* Call ```begin()```

* All drawing commands go inside the ```while ( calculating () )``` loop
This loop repeats the commands for each little slice (page) of the screen, as many times as needed

That's it! Everything else (should) be taken care of automatically.
### Drawing
In the interest of laziness, I'm going to direct you to [the official adafruit-gfx tutorial](https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives) for information on the drawing commands. This library *should* work pretty much the same, with a few small exceptions:
* The Heltec 2.13" Red V2 display only supports three colors:
  * ```.BLACK```
  * ```.WHITE```
  * ```.RED```
  
* The adafruit ```fillScreen()``` method will work just fine, however it is more efficient to use ```setDefaultColor()```

* A few handy methods have been added to find screen points:
  * right()
  * bottom()
  * centerX()
  * centerY()

### Screen Update
The E-INK display will begin to update as soon as the ```while ( calculating() )``` loop has finished. By default, this process blocks any further code execution until complete. 

It is possible to instead update the display in the background by running the paging loop as ```while ( calculating( false ) )```. It is very important to be aware that if this option is used,  the display will not respond to any further commands until it has completed its background update process. 
This can be checked by calling ```busy()```, which will return false once the the panel is once again free to respond.

## Wiring
### The display is 3.3V, do not connect it directly to  an Arduino.

All warnings aside, connection isn't all that hard. Just be sure to implement some sort of level-shifter. I can can confirm that a simple voltage divider is perfectly adequate, for example: 

![voltage-divider example](https://github.com/todd-herbert/Heltec-213R-V2/blob/main/docs/wiring_example.png?raw=true)
### Display Pins:
* **VCC**: 3.3V Power In
* **GND**: Ground
* **D/C**: Data / Command
 	* Tells panel whether incoming serial data is a command, or is image data.
	* Can connect to any available digital pin on Arduino
	* 3.3V Logic Input, needs level shifter
	
* **SDI**: Serial Data Input
	* SPI *MOSI* pin
	* On Arduino UNO, *must connect to pin 11*
	* 3.3V Logic Input, needs level shifter
* **CS**: Chip Select
	* SPI *SS* Pin
	* Can connect to any available digital pin on Arduino, however *pin 10* is traditional. 
	* 3.3V Logic Input, needs level shifter
* **CLK**: Clock
	* SPI *SCK* pin
	* On Arduino UNO, *must connect to pin 13*
	* 3.3V Logic Input, needs level shifter
* **BUSY**
	* Pin is LOW when screen is ready to accept commands
	* Can connect to any available digital pin on Arduino
	* 3.3V Logic Output, **level shifter not required** as 3.3V is a valid level for a HIGH signal on Arduino UNO

Make sure to specify the location of your *D/C, CS* and *BUSY* pins in the constructor.

## Configuration
### Constructor
```c++
Heltec_213R_V2  panel(dc, cs, busy);
```
Pass the Arduino digital pin numbers where the *D/C*, *CS*, and *BUSY* pins from the display are connected.

### .begin()
It is possible to change the *speed vs. memory* tradeoff while calling begin.
```c++
panel.begin(panel.PAGESIZE_TINY); 	//64kb of SRAM, 4% of total (Arduino UNO)
panel.begin(panel.PAGESIZE_SMALL); 	//160kb of SRAM, 8% of total (Arduino UNO)
panel.begin(panel.PAGESIZE_MEDIUM); 	//320kb of SRAM, 16% of total (Arduino UNO)
panel.begin(panel.PAGESIZE_LARGE); 	//800kb of SRAM, 40% of total (Arduino UNO)
``` 
If `begin()` is called with no parameters, `PAGESIZE_MEDIUM` is selected.

### External Libraries
A known working copy of GFX_Root is bundled with this library, however if for some reason you need to use an external version of GFX_Root or Adafruit_GFX, this is possible by using one the following two macros before the first include:
```c++
//#define  HELTEC_USE_EXTERNAL_GFX_ROOT
//#define  HELTEC_USE_EXTERNAL_ADAFRUIT_GFX

#include "heltec_213r_v2.h"
```

## Is my display supported?
This particular library supports one single model. At some stage, I may develop further libraries for various other Heltec displays.

#### This library supports only the *Heltec 2.13"* Red V2 Display.

There are two versions of the 2.13" display available. Only the V2 display is supported. You can identify this by the pin labels on the front, and the text on the rear.

Front | Rear
----|----
![V1 Front](https://github.com/todd-herbert/Heltec-213R-V2/blob/main/docs/V1-Front.png?raw=true)|![V1 Front](https://github.com/todd-herbert/Heltec-213R-V2/blob/main/docs/V1-Rear.png?raw=true)
![V2 Front](https://github.com/todd-herbert/Heltec-213R-V2/blob/main/docs/V2-Front.png?raw=true)| ![V2 Front](https://github.com/todd-herbert/Heltec-213R-V2/blob/main/docs/V2-Rear.png?raw=true)
# Acknowledgements
This library is inspired by [GxEPD2](https://github.com/ZinggJM/GxEPD2), a similar project for Goodisplay and Waveshare displays.

Information on how to correctly communicate with the display hardware was provided by on the [official Heltec library](https://github.com/HelTecAutomation/e-ink).

The drawing functions are provided by [GFX Root](https://github.com/ZinggJM/GFX_Root), which itself is a stripped down version of [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library).
