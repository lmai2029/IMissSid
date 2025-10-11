#include "ti/devices/msp/msp.h"
#include "camera.h"
#include "..\Lab05\uart.h"
#include "sysctl.h"
#include "sysctl.c"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "uart_extras.h"

static uint16_t* cameraData;

int main(void){
	UART0_init();
	Camera_init();
	while(1){
		if(Camera_isDataReady()){
			UART0_put((uint8_t *)"-1\r\n");
			cameraData = Camera_getData();
			for(int i = 0; i < 128; i++){
				// each data pinted to PuTTY
				UART0_printDec((int)cameraData[i] );
				UART0_put((uint8_t *) "\r\n");
				//char buffer[128];
        //sprintf(buffer, "%u\r\n", cameraData[i]);
        //UART0_put((uint8_t *)buffer);
				
				//UART0_put((uint8_t *)&cameraData[i]);
				//UART0_put((uint8_t *)"\n\r");
			}
		UART0_put((uint8_t *)"-2\r\n");
		}
	}
}

