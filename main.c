/**
 * ******************************************************************************
 * @file    : main.c
 * @brief   : Lab 5 Part 1 main file.
 * 
 * @author Leo Mai
 * @date 9/26/2025
 * ******************************************************************************
*/

#include "ti/devices/msp/msp.h"
#include "uart.h"
#include "leds.h"
#include "sysctl.h"
#include "sysctl.c"
#include "switches.h"
#include "timers.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static volatile uint32_t g12counter = 0;
static int LED1counter = 0;
static bool sw1_state = false;
static bool sw2_state = false;
void GROUP1_IRQHandler(void);
void TIMG6_IRQHandler(void);
void TIMG12_IRQHandler(void);

int main(void){
	S1_init_interrupt();
	S2_init_interrupt();
	LED1_init();
	LED2_init();
	UART0_init();
	TIMG6_init(2, 255);
	TIMG12_init(1000);
	while(1){}
}

void TIMG6_IRQHandler(void){
		if (LED1counter == 0){
			LED1_set(1); 
			LED1counter = 1;
		} else{
			LED1_set(0);
			LED1counter = 0;
		}
    TIMG6-> CPU_INT.ICLR = GPTIMER_GEN_EVENT1_ICLR_Z_CLR; 
}

void TIMG12_IRQHandler(void){
		if(sw2_state){
			g12counter++; 
		}
    TIMG12-> CPU_INT.ICLR = GPTIMER_GEN_EVENT1_ICLR_Z_CLR; 
}

void GROUP1_IRQHandler(void){
	
		switch(CPUSS->INT_GROUP[1].IIDX){
			case CPUSS_INT_GROUP_IIDX_STAT_INT0:
					if (!sw1_state) {
						TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
            sw1_state = true;
					}else{
						TIMG6->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_MASK;
            LED1_set(0); 
            sw1_state = false;
					}
					GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
				break;
			
			case CPUSS_INT_GROUP_IIDX_STAT_INT1:
				if(!sw2_state){
					g12counter = 0;
					TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
					LED2_set('r');
					LED2_set('g');
					LED2_set('b');
					LED2_set('c');
					LED2_set('m');
					LED2_set('y');
					LED2_set('w');
					LED2_set('o');
          sw2_state = true;
				}else{
					TIMG12->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_MASK;
					UART0_put((uint8_t *)"Elapsed Time: ");
					UART0_put((uint8_t *)g12counter);
					UART0_put((uint8_t *)"\r\n");
				}
				GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO21_CLR;
				
				break;
		}
}
