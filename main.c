/**
 * ******************************************************************************
 * @file    : main.c
 * @brief   : Lab 5 Part 2 main file.
 * 
 * @author Leo Mai, Job Sava
 * @date 9/26/2025
 * ******************************************************************************
*/
#include "ti/devices/msp/msp.h"
#include "..\Lab05\uart.h"
#include "uart_extras.h"
#include "..\Lab05\leds.h"
#include "sysctl.h"
#include "sysctl.c"
#include "switches.h"
#include "timers.h"
#include "adc12.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t adc_value;
void TIMG6_IRQHandler(void);


int main(void){
	UART0_init();
	TIMG6_init(2,256);
	ADC0_init();
	while(1){__WFI();}
}

void TIMG6_IRQHandler(void){
	adc_value = ADC0_getVal();
	UART0_put((uint8_t *)"Beginning conversion\r\n");
	UART0_put((uint8_t *)"Decimal value: ");
	UART0_printUDec(adc_value);
	UART0_put((uint8_t *)"Hexdecimal value: 0x");
	UART0_printHex(adc_value);
	UART0_put((uint8_t *)"\r\n");
	TIMG6-> CPU_INT.ICLR = GPTIMER_GEN_EVENT1_ICLR_Z_CLR;
}
