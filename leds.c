/**
 * ******************************************************************************
 * @file    : leds.c
 * @brief   : Initiates leds
 * 
 * @author Leo Mai
 * @date 9/2/2025
 * ******************************************************************************
*/

#include "ti/devices/msp/msp.h"
#include "leds.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/peripherals/hw_gpio.h" 
#include "ti/devices/msp/peripherals/hw_iomux.h" 

void LED1_init(void) {
    if (!(GPIOA->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK)) {
				GPIOA->GPRCM.RSTCTL = (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT);
        GPIOA->GPRCM.PWREN = (GPIO_PWREN_KEY_UNLOCK_W  | GPIO_PWREN_ENABLE_ENABLE);
    }
    
		IOMUX->SECCFG.PINCM[IOMUX_PINCM1] |= IOMUX_PINCM_PC_MASK;
		IOMUX->SECCFG.PINCM[IOMUX_PINCM1] |= IOMUX_PINCM_DRV_DRVVAL1;
		IOMUX->SECCFG.PINCM[IOMUX_PINCM1] |= IOMUX_PINCM_INV_ENABLE;
		IOMUX->SECCFG.PINCM[IOMUX_PINCM1] |= 0x01;
    
    
    GPIOA->DOESET31_0 = 0x01;
	}
void LED2_init(void) {
    if (!(GPIOB->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK)) {
				GPIOB->GPRCM.RSTCTL = (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT);
        GPIOB->GPRCM.PWREN = (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE);
    }
		
    IOMUX->SECCFG.PINCM[IOMUX_PINCM57] |= IOMUX_PINCM_PC_MASK; // Red
		IOMUX->SECCFG.PINCM[IOMUX_PINCM57] |= IOMUX_PINCM_DRV_DRVVAL1;
		IOMUX->SECCFG.PINCM[IOMUX_PINCM57] |= 0x01;
		GPIOB->DOESET31_0 |= (1U << 26);
    
    IOMUX->SECCFG.PINCM[IOMUX_PINCM58] |= IOMUX_PINCM_PC_MASK; // Green
		IOMUX->SECCFG.PINCM[IOMUX_PINCM58] |= IOMUX_PINCM_DRV_DRVVAL1;
		IOMUX->SECCFG.PINCM[IOMUX_PINCM58] |= 0x01;
		GPIOB->DOESET31_0 |= (1U << 27);
		
		IOMUX->SECCFG.PINCM[IOMUX_PINCM50] |= IOMUX_PINCM_PC_MASK; // Blue
		IOMUX->SECCFG.PINCM[IOMUX_PINCM50] |= IOMUX_PINCM_DRV_DRVVAL1;
		IOMUX->SECCFG.PINCM[IOMUX_PINCM50] |= 0x01;
		GPIOB->DOESET31_0 |= (1U << 22);
}

void LED1_set(int i) {
    if( i == 1){
			GPIOA->DOUTSET31_0 |= GPIO_DOESET31_0_DIO0_SET;
		}
			else{
				GPIOA->DOUTCLR31_0 |= GPIO_DOECLR31_0_DIO0_CLR;
}
			}

void LED2_set(char color) {
	GPIOB->DOUTCLR31_0 |= GPIO_DOESET31_0_DIO26_SET; // clear red
	GPIOB->DOUTCLR31_0 |= GPIO_DOESET31_0_DIO27_SET; // clear green
	GPIOB->DOUTCLR31_0 |= GPIO_DOESET31_0_DIO22_SET; // clear blue
	
	switch(color){
		case 'r':
				GPIOB->DOUTSET31_0 |= GPIO_DOESET31_0_DIO26_SET;
        break;
		case 'g':
				GPIOB->DOUTSET31_0 |= GPIO_DOESET31_0_DIO27_SET;
        break;
		case 'b':
				GPIOB->DOUTSET31_0 |= GPIO_DOESET31_0_DIO22_SET;
        break;
		case 'c':
				GPIOB->DOUTSET31_0 |= GPIO_DOESET31_0_DIO27_SET;
				GPIOB->DOUTSET31_0 |= GPIO_DOESET31_0_DIO22_SET;
        break;
		case 'm':
				GPIOB->DOUTSET31_0 |= GPIO_DOESET31_0_DIO26_SET;
				GPIOB->DOUTSET31_0 |= GPIO_DOESET31_0_DIO22_SET;
        break;
		case 'y':
				GPIOB->DOUTSET31_0 |= GPIO_DOESET31_0_DIO26_SET;
				GPIOB->DOUTSET31_0 |= GPIO_DOESET31_0_DIO27_SET;
        break;
		case 'w':
				GPIOB->DOUTSET31_0 |= GPIO_DOESET31_0_DIO27_SET;
				GPIOB->DOUTSET31_0 |= GPIO_DOESET31_0_DIO22_SET;
				GPIOB->DOUTSET31_0 |= GPIO_DOESET31_0_DIO26_SET;
        break;
		default:
			break;
	}
}
