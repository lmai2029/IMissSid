#include "lab5/adc12.h"
#include "sysctl.h"
#include "ti/devices/msp/msp.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/peripherals/hw_gptimer.h" 
#include <stdint.h>

/**
 * @brief Initialize ADC0
*/
void ADC0_init(void){
	//i
	if (!(ADC0->ULLMEM.GPRCM.PWREN & ADC12_PWREN_ENABLE_ENABLE)) {
				ADC0->ULLMEM.GPRCM.RSTCTL |= (ADC12_RSTCTL_KEY_UNLOCK_W | ADC12_RSTCTL_RESETASSERT_ASSERT);
        ADC0->ULLMEM.GPRCM.PWREN |= (ADC12_PWREN_KEY_UNLOCK_W | ADC12_PWREN_ENABLE_ENABLE); 
    }
	//ii
	ADC0->ULLMEM.GPRCM.CLKCFG |= ADC12_CLKCFG_SAMPCLK_ULPCLK;
	ADC0->ULLMEM.CLKFREQ |= ADC12_CLKFREQ_FRANGE_RANGE40TO48;
	//iii
	ADC0->ULLMEM.CTL0 |= ADC12_CTL0_PWRDN_MANUAL;
	//iv
	ADC0->ULLMEM.CTL0 |= ADC12_CTL0_SCLKDIV_DIV_BY_8;
	//v
	ADC0->ULLMEM.CTL1 |= ADC12_CTL1_CONSEQ_SINGLE;
	ADC0->ULLMEM.CTL2 |= ADC12_CTL2_STARTADD_ADDR_00;
	ADC0->ULLMEM.MEMCTL[0] |= ADC12_MEMCTL_CHANSEL_CHAN_0;
	ADC0->ULLMEM.CTL1 |= ADC12_CTL1_TRIGSRC_SOFTWARE;
	ADC0->ULLMEM.CTL1 |= ADC12_CTL1_SAMPMODE_AUTO;
	ADC0->ULLMEM.MEMCTL[0] |= ADC12_MEMCTL_STIME_SEL_SCOMP0;
	// setting SCOMP bit
	ADC0->ULLMEM.CTL0 |= ADC12_CTL0_ENC_ON;
	
}


/**
 * @brief Retrieve a the value from the ADC0
 * @note The ADC channel in use is set during initialization
 *       The channel is not the same as where the module stores the value
 * @return ADC0 processed value
*/
uint32_t ADC0_getVal(void){
		//i
		ADC0->ULLMEM.CTL1 |= ADC12_CTL1_SC_START;
    // iii. 
    while (ADC0->ULLMEM.STATUS & ADC12_STATUS_BUSY_MASK);
    // iv. 
    return (ADC0->ULLMEM.MEMRES[0] & ADC12_PERIPHERALREGIONSVT_SVTMEM_MEMRES_DATA_MASK);
}
