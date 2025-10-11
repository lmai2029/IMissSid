/**
 * ******************************************************************************
 * @file    : main.c
 * @brief   : Lab 5 Part 1 main file.
 * 
 * @author Leo Mai, Job Sava
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
#include "uart_extras.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static volatile uint32_t g12counter = 0;
static int LED1counter = 0;
static int LED2counter = 0;
static bool sw1_state = false;
static bool sw2_state = false;
void GROUP1_IRQHandler(void);
void TIMG6_IRQHandler(void);
void TIMG12_IRQHandler(void);

void TIMG6_IRQHandler(void){
	//UART0_put((uint8_t *)"Hello timer 6");
	if(LED1counter == 0){
		LED1_set(1);
		LED1counter = 1;
	}
	else if(LED1counter == 1){
		LED1_set(0);
		LED1counter = 0;
	}
	//TIMG6-> CPU_INT.ICLR = GPTIMER_GEN_EVENT1_ICLR_Z_CLR; 
}

void TIMG12_IRQHandler(void){
	//UART0_put((uint8_t *)"Hello timer 12");
	if(sw2_state){
			g12counter++; 
			//UART0_printDec((int) g12counter);
	}
	//TIMG12-> CPU_INT.ICLR = GPTIMER_GEN_EVENT1_ICLR_Z_CLR; 
}

void GROUP1_IRQHandler(void){
	GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
	GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO21_CLR;
	
	switch(CPUSS->INT_GROUP[1].IIDX){
			case CPUSS_INT_GROUP_IIDX_STAT_INT0:
					if (!sw1_state) {
						TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
						//LED1_set(1);
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
						if(LED2counter==0){LED2_set('r'); LED2counter++; sw2_state=true; break;}
						if(LED2counter==1){LED2_set('g'); LED2counter++; sw2_state=true; break;}
						if(LED2counter==2){LED2_set('b'); LED2counter++; sw2_state=true; break;}
						if(LED2counter==3){LED2_set('c'); LED2counter++; sw2_state=true; break;}
						if(LED2counter==4){LED2_set('m'); LED2counter++; sw2_state=true; break;}
						if(LED2counter==5){LED2_set('y'); LED2counter++; sw2_state=true; break;}
						if(LED2counter==6){LED2_set('w'); LED2counter++; sw2_state=true; break;}
						if(LED2counter==7){LED2_set('o'); LED2counter=0; sw2_state=true; break;}
						sw2_state = true;
					}else{
						TIMG12->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_MASK;
						UART0_put((uint8_t *)"Elapsed Time: ");
						UART0_printDec((int) g12counter);
						//UART0_put((uint8_t *)g12counter);
						UART0_put((uint8_t *)"\r\n");
						sw2_state = false;
					}
					GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO21_CLR;
					break;
	}
	
}

int main(void){
	S1_init_interrupt();
	S2_init_interrupt();
	LED1_init();
	LED2_init();
	UART0_init();
	TIMG6_init(2, 256);
	TIMG12_init(1000);
	TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
	while(1){__WFI();}
}
