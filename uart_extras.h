/**
 * ******************************************************************************
 * @file    : uart_extras.h
 * @brief   : UART Extras header file
 * @details : Header instantiation for extra UART functions
 * 
 * @author tlk1160
 * @date 3/14/2025
 * ******************************************************************************
*/

#ifndef _UART_EXTRAS_H_
#define _UART_EXTRAS_H_
#include <stdint.h>

void UART0_printDec(int num);
void UART0_printFloat(double num);
void UART0_printHex(uint32_t num);
void UART0_printUDec(uint32_t num);
#endif // _UART_EXTRAS_H_
