/**
 * ******************************************************************************
 * @file    : leds.h
 * @brief   : LEDs module header file
 * @details : LED initialization and interaction
 * 
 * @author 
 * @date 
 * ******************************************************************************
*/

#ifndef _LEDS_H_
#define _LEDS_H_

#define LED1_PIN        0
#define LED2_R_PIN      26
#define LED2_G_PIN      27
#define LED2_B_PIN      22

/**
 * @brief Initialze LED1
 * @hint You might want to check out the schematics in the MSP User Guide
 *       The IOMUX has a hardware inversion bit
*/
void LED1_init(void);


/**
 * @brief Initialize LED2
 * @note You must account for each LED color
*/
void LED2_init(void);


/**
 * @brief Set LED1 output state
 * @note ON, OFF, TOGGLE
 * input paramter marks on or off
*/
void LED1_set(int i);


/**
 * @brief Set LED2 output state
 * @note RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, OFF
*/
void LED2_set(char color);

#endif // _LEDS_H_
