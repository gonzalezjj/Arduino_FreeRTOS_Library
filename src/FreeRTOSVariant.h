/* freeRTOSVariant.h
 *
 * Board variant (hardware) specific definitions for the AVR boards that I use regularly.
 *
 * This file is NOT part of the FreeRTOS distribution.
 *
 */

#ifndef freeRTOSVariant_h
#define freeRTOSVariant_h

#include <avr/io.h>
#include <avr/wdt.h>

#ifdef __cplusplus
extern "C" {
#endif

// System Tick - Scheduler timer
// Use the Watchdog timer, and choose the rate at which scheduler interrupts will occur.

#define configTICK_RATE_HZ		100
    
#define portTIMER_PRESCALER     (_BV(CS11))
#define portTIMER_PRESCALER_DIV 8
#define portTIMER_PERIOD ((((uint32_t)(F_CPU) / portTIMER_PRESCALER_DIV) / configTICK_RATE_HZ) - 1)

/*-----------------------------------------------------------*/

void initVariant(void);

void vApplicationIdleHook( void );

void vApplicationMallocFailedHook( void );


/*-----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif // freeRTOSVariant_h
