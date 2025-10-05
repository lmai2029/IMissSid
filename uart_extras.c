/**
 * ******************************************************************************
 * @file    : uart_extras.c
 * @brief   : Useful extra UART functions
 * @details : Provides functions for decimal and float value printing (UART0)
 * @note    : Provided as sprintf behaves stragely when printing floats on the MSP
 *            - Requires completed UART
 * 
 * @author tlk1160
 * @date 3/14/2025
 * ******************************************************************************
*/

#include "uart_extras.h"
#include "uart.h"

/**
 * @brief Prints integer value over UART0
 * @param[in] num - Value to print
 */
void UART0_printDec(int num) {
    int neg = 0;
    char String[12];
    char *StringPtr = &String[11]; // Point to the end of the buffer
    *StringPtr = '\0'; // Null terminator for the string

    // Handle zero case directly
    if (num == 0) {
        *(--StringPtr) = '0';
    } else {
        if (num < 0) {
            neg = 1;
            num = -num; // Convert to positive value
        }
        // Convert number to string
        while (num > 0) {
            *(--StringPtr) = (num % 10) + '0';
            num /= 10;
        }
        if (neg) *(--StringPtr) = '-';
    }
    UART0_put((uint8_t *)StringPtr);
}

void UART0_printUDec(uint32_t num) {
    char String[12];
    char *StringPtr = &String[11]; 
    *StringPtr = '\0';             
    if (num == 0) {
        *(--StringPtr) = '0';
    } else {
        while (num > 0) {
            *(--StringPtr) = (num % 10) + '0';
            num /= 10;
        }
    }
    UART0_put((uint8_t *)StringPtr);
		UART0_put((uint8_t *)"\n");
}

/**
 * @brief Prints float/double value over UART0
 * @param[in] num - Value to print
 * @note Will have some precision loss due to print method
 */
void UART0_printFloat(double num) {
    if (num < 0) {
        UART0_putchar('-');
        num = -num;
    }
    int integerPart = (int) num;
    double fractionalPart = num - integerPart;

    UART0_printDec(integerPart);
    UART0_putchar('.');

    int precision = 0;
    while (fractionalPart > 1e-6 && precision++ < 6) {
        fractionalPart *= 10;
        UART0_putchar((char) ('0' + (int) fractionalPart));
        fractionalPart -= (int) fractionalPart;
    }
}

void UART0_printHex(uint32_t num) {
    char hexString[9]; 
    char *ptr = hexString;
    int i;

    for (i = 28; i >= 0; i -= 4) {
        uint8_t chr = (num >> i) & 0x0F;
        if (chr < 10) {
            *ptr++ = chr + '0';
        } else {
            *ptr++ = (chr - 10) + 'A';
        }
    }
    *ptr = '\0';
    UART0_put((uint8_t *)hexString);
		UART0_put((uint8_t *)"\n");
}
