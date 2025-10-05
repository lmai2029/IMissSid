/**
 * ******************************************************************************
 * @file    : uart.c
 * @brief   : uart file which dictates how the peripheral initializes, 
 *            recieves, and trasmits dats.
 * 
 * @author Leo Mai
 * @date 9/6/2025
 * ******************************************************************************
*/

#include "ti/devices/msp/msp.h"
#include "uart.h"
#include "sysctl.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/peripherals/hw_uart.h" 
#include "ti/devices/msp/peripherals/hw_iomux.h" 

//RX = PINCM22 PA11
//TX = PINCM21 PA10

float static BRD = 0;

// Function for initializing the UART
void UART0_init(void){
	if (!(UART0->GPRCM.PWREN & UART_PWREN_ENABLE_MASK)) {
				UART0->GPRCM.RSTCTL = (UART_RSTCTL_KEY_UNLOCK_W | UART_RSTCTL_RESETASSERT_ASSERT);
        UART0->GPRCM.PWREN = (UART_PWREN_KEY_UNLOCK_W | UART_PWREN_ENABLE_ENABLE); 
    }
	IOMUX->SECCFG.PINCM[IOMUX_PINCM22] |= IOMUX_PINCM_PC_MASK;
	IOMUX->SECCFG.PINCM[IOMUX_PINCM22] |= IOMUX_PINCM_INENA_ENABLE;
	IOMUX->SECCFG.PINCM[IOMUX_PINCM22] |= 0x02;
	
	IOMUX->SECCFG.PINCM[IOMUX_PINCM21] |= IOMUX_PINCM_PC_MASK;
	IOMUX->SECCFG.PINCM[IOMUX_PINCM21] |= 0x02;
	
	UART0->CLKSEL |= UART_CLKSEL_BUSCLK_SEL_MASK;
	UART0->CLKDIV |= UART_CLKDIV_RATIO_DIV_BY_1;
	UART0->CTL0 &= ~UART_CTL0_ENABLE_ENABLE;
	UART0->CTL0 |= UART_CTL0_HSE_OVS16;
	UART0->CTL0 |= UART_CTL0_RXE_ENABLE;
	UART0->CTL0 |= UART_CTL0_TXE_ENABLE;
	UART0->CTL0 |= UART_CTL0_FEN_ENABLE;
	uint32_t freq = SYSCTL_SYSCLK_getULPCLK();
	// Baud rate calculations
	BRD = ((float)freq)/ (BAUD_RATE * OVS);
	uint16_t ibrd = (uint16_t)BRD;
  uint8_t fbrd = (uint8_t)(((BRD - ibrd) * 64.0f) + 0.5f);
  UART0->IBRD = ibrd;
  UART0->FBRD = fbrd;

	UART0->LCRH |= UART_LCRH_PEN_DISABLE;
	UART0->LCRH |= UART_LCRH_STP2_DISABLE;
	UART0->LCRH |= UART_LCRH_WLEN_DATABIT8;
	 
	UART0->CTL0 |= UART_CTL0_ENABLE_ENABLE;
}

void UART0_putchar(uint8_t ch){
	while((UART0->STAT & UART_STAT_TXFF_MASK) != 0){}
	UART0->TXDATA = ch;
}

char UART0_getchar(void){
	while((UART0->STAT & UART_STAT_RXFE_MASK) != 0){}
    return (UART0->RXDATA & UART_RXDATA_DATA_MASK);
}

void UART0_put(uint8_t *ptr_str){
	while (*ptr_str) {
        UART0_putchar(*ptr_str++);
    }
}

void UART1_init(void){
		if (!(UART1->GPRCM.PWREN & UART_PWREN_ENABLE_MASK)) {
				UART1->GPRCM.RSTCTL = (UART_RSTCTL_KEY_UNLOCK_W | UART_RSTCTL_RESETASSERT_ASSERT);
        UART1->GPRCM.PWREN = (UART_PWREN_KEY_UNLOCK_W | UART_PWREN_ENABLE_ENABLE); 
    }
	IOMUX->SECCFG.PINCM[IOMUX_PINCM20] |= IOMUX_PINCM_PC_MASK;
	IOMUX->SECCFG.PINCM[IOMUX_PINCM20] |= IOMUX_PINCM_INENA_ENABLE;
	IOMUX->SECCFG.PINCM[IOMUX_PINCM20] |= 0x02;
	
	IOMUX->SECCFG.PINCM[IOMUX_PINCM19] |= IOMUX_PINCM_PC_MASK;
	IOMUX->SECCFG.PINCM[IOMUX_PINCM19] |= 0x02;
	
	UART1->CLKSEL |= UART_CLKSEL_BUSCLK_SEL_MASK;
	UART1->CLKDIV |= UART_CLKDIV_RATIO_DIV_BY_1;
	UART1->CTL0 &= ~UART_CTL0_ENABLE_ENABLE;
	UART1->CTL0 |= UART_CTL0_HSE_OVS16;
	UART1->CTL0 |= UART_CTL0_RXE_ENABLE;
	UART1->CTL0 |= UART_CTL0_TXE_ENABLE;
	UART1->CTL0 |= UART_CTL0_FEN_ENABLE;
	uint32_t freq = SYSCTL_SYSCLK_getULPCLK();
	// Baud rate calculations
	BRD = ((float)freq)/ (BAUD_RATE * OVS);
	uint16_t ibrd = (uint16_t)BRD;
  uint8_t fbrd = (uint8_t)(((BRD - ibrd) * 64.0f) + 0.5f);
  UART1->IBRD = ibrd;
  UART1->FBRD = fbrd;

	UART1->LCRH |= UART_LCRH_PEN_DISABLE;
	UART1->LCRH |= UART_LCRH_STP2_DISABLE;
	UART1->LCRH |= UART_LCRH_WLEN_DATABIT8;
	 
	UART1->CTL0 |= UART_CTL0_ENABLE_ENABLE;
}


void UART1_putchar(uint8_t ch){
	while((UART1->STAT & UART_STAT_TXFF_MASK) != 0){}
	UART1->TXDATA = ch;
	}

char UART1_getchar(void){
	while((UART1->STAT & UART_STAT_RXFE_MASK) != 0){}
    return (UART1->RXDATA & UART_RXDATA_DATA_MASK);
}

void UART1_put(uint8_t *ptr_str){
	while (*ptr_str) {
        UART1_putchar(*ptr_str++);
    }
}

bool UART1_isDataAvailable(void){
	return ((UART1->STAT & UART_STAT_RXFE_MASK) == 0);
}

bool UART0_isDataAvailable(void){
	return ((UART0->STAT & UART_STAT_RXFE_MASK) == 0);
}
