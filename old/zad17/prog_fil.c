////////////////////////////////////////
//	blinkLED.c
//      Blinks the USR3 LED.
//	Wiring:	None
//	Setup:	
//	See:	
////////////////////////////////////////
#include <stdio.h>
#include <unistd.h>

int main() {
	FILE * trigger = fopen("/sys/class/leds/beaglebone:green:usr3/trigger", "w");
	FILE * brightness = fopen("/sys/class/leds/beaglebone:green:usr3/brightness", "w");
	int on = 0;
	
	fprintf(trigger, "none\n");
    
	while(1) {
  
  	//F (..-.)
		fprintf(brightness, "1\n");
		fflush(brightness);
		usleep(500000);
    fprintf(brightness, "0\n");
		fflush(brightness);
    usleep(500000);
    fprintf(brightness, "1\n");
		fflush(brightness);
    usleep(500000);
    fprintf(brightness, "0\n");
		fflush(brightness);
    usleep(500000);
    fprintf(brightness, "1\n");
		fflush(brightness);
    usleep(1000000);
    fprintf(brightness, "0\n");
		fflush(brightness);
    usleep(500000);
    fprintf(brightness, "1\n");
		fflush(brightness);
    usleep(1500000);
    fprintf(brightness, "0\n");
		fflush(brightness);
    usleep(500000);
     //W((.--))
     fprintf(brightness, "1\n");
		fflush(brightness);
		usleep(500000);
    fprintf(brightness, "0\n");
		fflush(brightness);
    usleep(500000);
    fprintf(brightness, "1\n");
		fflush(brightness);
    usleep(1000000);
    fprintf(brightness, "0\n");
		fflush(brightness);
    usleep(500000);
    fprintf(brightness, "1\n");
		fflush(brightness);
    usleep(1000000);
    fprintf(brightness, "0\n");
		fflush(brightness);
    usleep(500000);
}     
     
     
    
	}
}
