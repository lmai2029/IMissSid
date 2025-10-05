/**
 * ******************************************************************************
 * @file    : switches.c
 * @brief   : Initiates switches
 * 
 * @author Leo Mai
 * @date 9/2/2025
 * ******************************************************************************
*/

#include "ti/devices/msp/msp.h"
#include "switches.h" 
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/peripherals/hw_gpio.h" 
#include "ti/devices/msp/peripherals/hw_iomux.h" 

void S1_init(void) {
    if (!(GPIOA->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK)) {
				GPIOA->GPRCM.RSTCTL = (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT);// 26H key mask also allows write access + reset assert. (pg. 1025)
        GPIOA->GPRCM.PWREN = (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE); // the 26H key allows write access, enable, enables power. (pg. 1024)
    }
    
		
    IOMUX->SECCFG.PINCM[IOMUX_PINCM40] |= IOMUX_PINCM_PC_MASK; //(pg. 1011)
    IOMUX->SECCFG.PINCM[IOMUX_PINCM40] |= IOMUX_PINCM_INENA_ENABLE;
		IOMUX->SECCFG.PINCM[IOMUX_PINCM40] |= IOMUX_PINCM_INV_ENABLE;
    IOMUX->SECCFG.PINCM[IOMUX_PINCM40] |= IOMUX_PINCM_PIPD_ENABLE;
		IOMUX->SECCFG.PINCM[IOMUX_PINCM40] |= 0x01;
}

void S2_init(void) {
    if (!(GPIOB->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK)) {
				GPIOB->GPRCM.RSTCTL = (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT);
        GPIOB->GPRCM.PWREN = (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE);
    }
    
    IOMUX->SECCFG.PINCM[IOMUX_PINCM49] |= IOMUX_PINCM_PC_MASK; //(pg. 1011)
    IOMUX->SECCFG.PINCM[IOMUX_PINCM49] |= IOMUX_PINCM_INENA_ENABLE;
    IOMUX->SECCFG.PINCM[IOMUX_PINCM49] |= IOMUX_PINCM_PIPU_ENABLE;
		IOMUX->SECCFG.PINCM[IOMUX_PINCM49] |= 0x01;
}

int S1_pressed(void) {
    return !(GPIOA->DIN31_0 & GPIO_DIN31_0_DIO18_MASK); //(pg.1106)
}

int S2_pressed(void) {
    return !(GPIOB->DIN31_0 & (GPIO_DIN31_0_DIO21_MASK));
}

void S1_init_interrupt(void){
	__disable_irq();
	S1_init();
	GPIOA-> CPU_INT.ICLR = GPIO_GEN_EVENT1_ICLR_DIO18_CLR; 
  GPIOA-> CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;
	GPIOA-> POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;
	
	NVIC_EnableIRQ(GPIOA_INT_IRQn);
	__enable_irq();
}


void S2_init_interrupt(void){
	__disable_irq();
	S2_init();
	GPIOB-> CPU_INT.ICLR = GPIO_GEN_EVENT1_ICLR_DIO21_CLR; 
  GPIOB-> CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO21_SET;
	GPIOB-> POLARITY31_16 = GPIO_POLARITY31_16_DIO21_FALL;
	
	NVIC_EnableIRQ(GPIOB_INT_IRQn);
	__enable_irq();
}

