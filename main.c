#include "ti/devices/msp/msp.h"
#include "Lab5/camera.h"
#include "Lab5/uart.h"
#include "sysctl.h"
#include "sysctl.c"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static uint16_t* cameraData;

int main(void){
	UART0_init();
	Camera_init();
	while(1){
		if(Camera_isDataReady()){
			UART0_put((uint8_t *)"-1\n\r");
			cameraData = Camera_getData();
			for(int i = 0; i < 128; i++){
				// each data printed to PuTTY
				UART0_put((uint8_t *)&cameraData[i]);
				UART0_put((uint8_t *)"\n\r");
			}
		UART0_put((uint8_t *)"-2\n\r");
		}
	}
}
