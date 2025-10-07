#include "Lab5/camera.h"
#include "Lab5/adc12.h"
#include "Lab5/timers.h"
#include "sysctl.h"
#include "ti/devices/msp/msp.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"


void TIMG6_IRQHandler(void);
void TIMG0_IRQHandler(void);
static bool cameraData_complete = false;
static uint16_t cameraData[128];
static uint32_t pixelCounter = 0;

void Camera_init(void){
	if (!(GPIOA->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK)) {
				GPIOA->GPRCM.RSTCTL = (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT);
        GPIOA->GPRCM.PWREN = (GPIO_PWREN_KEY_UNLOCK_W  | GPIO_PWREN_ENABLE_ENABLE);
	}
	// initialize iomux for SI and CLK
	IOMUX->SECCFG.PINCM[IOMUX_PINCM3]|= IOMUX_PINCM_PC_MASK|IOMUX_PINCM_PIPD_DISABLE|IOMUX_PINCM_PIPU_DISABLE|IOMUX_PINCM3_PF_TIMG6_CCP0 ;
	IOMUX->SECCFG.PINCM[IOMUX_PINCM34]|= IOMUX_PINCM_PC_MASK|IOMUX_PINCM_PIPD_DISABLE|IOMUX_PINCM_PIPU_DISABLE|IOMUX_PINCM34_PF_TIMG6_CCP0 ;
	
	// set and clear DIOs SI - 28 CLK - 12
	GPIOA->DOESET31_0 |= GPIO_DOESET31_0_DIO28_SET;
	GPIOA->DOUTCLR31_0 = GPIO_DOUTCLR31_0_DIO28_CLR;
	GPIOA->DOESET31_0 |= GPIO_DOESET31_0_DIO12_SET;
	GPIOA->DOUTCLR31_0 = GPIO_DOUTCLR31_0_DIO12_CLR;
	
	//GPIOB->DOUTSET31_0 |= GPIO_DOESET31_0_DIO22_SET; to enable
	ADC0_init();
	TIMG0_init(100000, 1);
	TIMG6_init((uint32_t)7.5, 256);
}



uint8_t Camera_isDataReady(void){
	if(cameraData_complete){
		//when data is ready, reset it for next capture.
		cameraData_complete = false;
		return true;
	}
	return false;
}


uint16_t* Camera_getData(void){
	return cameraData;
}

void TIMG6_IRQHandler(void){
	TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_MASK; // disable timer0
	if(cameraData_complete){
		TIMG6-> CPU_INT.ICLR = GPTIMER_GEN_EVENT1_ICLR_Z_CLR; 
		return;
	}
		
		GPIOA->DOUTCLR31_0 = GPIO_DOUTCLR31_0_DIO28_CLR; // SI low
		GPIOA->DOUTCLR31_0 = GPIO_DOUTCLR31_0_DIO12_CLR; // CLK low
		GPIOA->DOUTSET31_0 = GPIO_DOUTSET31_0_DIO28_SET; // SI high
		GPIOA->DOUTSET31_0 = GPIO_DOUTSET31_0_DIO12_SET; // CLK high
		GPIOA->DOUTCLR31_0 = GPIO_DOUTCLR31_0_DIO28_CLR; // SI low
		GPIOA->DOUTCLR31_0 = GPIO_DOUTCLR31_0_DIO12_CLR; // CLK low
	
		TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED; //reenable timer0
	
		TIMG6->CPU_INT.ICLR = GPTIMER_GEN_EVENT1_ICLR_Z_CLR; // clear interrupt flag
	
}

void TIMG0_IRQHandler(void){
	// pulsing clock
	GPIOA->DOUTSET31_0 = GPIO_DOUTSET31_0_DIO12_SET; //CLK high
	// store data in cameradata by pixel counter index
	cameraData[pixelCounter] = (uint16_t) ADC0_getVal();
	pixelCounter++;
	
	// the index exceeds the array bounds (there are 128 values to read), set the cameraData_complete flag
	if (pixelCounter >= 128) {
		cameraData_complete = true; 
		//disable the CLK timer, and reset the index
		TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_MASK;
		pixelCounter = 0;
	}
	// clear interrupt
	TIMG0->CPU_INT.ICLR = GPTIMER_GEN_EVENT1_ICLR_Z_CLR;
}


