/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/*
Changes from V1.2.3

	+ portCPU_CLOSK_HZ definition changed to 8MHz base 10, previously it
	  base 16.
*/

#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
#define portCHAR	char
#define portFLOAT	float
#define portDOUBLE	double
#define portLONG	long
#define portSHORT	int
#define portSTACK_TYPE	uint8_t
#define portBASE_TYPE	uint8_t

typedef portSTACK_TYPE StackType_t;
typedef signed char BaseType_t;
typedef unsigned char UBaseType_t;

#if( configUSE_16_BIT_TICKS == 1 )
	typedef uint16_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffffU
#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL
#endif
/*-----------------------------------------------------------*/

/* Critical section management. */
extern volatile uint8_t _sreg_store;

#define portENTER_CRITICAL()    __asm__ __volatile__ (					        \
                                        "in %0, __SREG__"               "\n\t"  \
                                        "cli" 					        "\n\t"	\
                                        :"=r" (_sreg_store) :: "memory"         \
                                        )


#define portEXIT_CRITICAL()     __asm__ __volatile__ (					        \
                                        "out __SREG__, %0"              "\n\t"  \
                                        :: "r"(_sreg_store) : "memory"          \
                                        )


#define portDISABLE_INTERRUPTS()        __asm__ __volatile__ ( "cli" ::: "memory")
#define portENABLE_INTERRUPTS()         __asm__ __volatile__ ( "sei" ::: "memory")

/* Architecture specifics. */
#define portSTACK_GROWTH                ( -1 )
#define portBYTE_ALIGNMENT              1
#define portNOP()                       __asm__ __volatile__ ( "nop" );

#define sleep_reset()                   do { _SLEEP_CONTROL_REG = 0; } while(0) // reset all sleep_mode() configurations.

/* Timing for the scheduler.
 * Watchdog Timer is 128kHz nominal,
 * but 120 kHz at 5V DC and 25 degrees is actually more accurate,
 * from data sheet.
 */
#define portTICK_PERIOD_MS              (1000 / configTICK_RATE_HZ)

/*-----------------------------------------------------------*/

/* Kernel utilities. */
extern void vPortYield( void )          __attribute__ ( ( naked ) );
#define portYIELD()                     vPortYield()

extern volatile uint8_t isYieldFromIsrPending;

#define portYIELD_FROM_ISR(flag) if (flag) { isYieldFromIsrPending = 1; }
#define portIS_IN_ISR (interruptStackReentryCounter)
/*-----------------------------------------------------------*/

#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)
/* Task function macros as described on the FreeRTOS.org WEB site. */
// This changed to add .lowtext tag for the linker for ATmega2560 and ATmega2561. To make sure they are loaded in low memory.
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters ) __attribute__ ((section (".lowtext")))
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

#else
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

#endif

extern volatile void * userCodeStackPointer;
extern volatile void * const interruptStackBase;
extern volatile uint8_t interruptStackReentryCounter;  // Reentry counter for nested interrupts

void vApplicationStackOverflowHook( void * xTask, portCHAR *pcTaskName );
extern const char * ISR_NAME;
extern volatile uint8_t interruptStack[configINTERRUPT_STACK_SIZE];
#if configCHECK_FOR_STACK_OVERFLOW > 1
#define portCHECK_ISR_STACK_WATERMARK()                             \
            "push XL                                        \n\t"   \
            "push XH                                        \n\t"   \
            "push r19                                       \n\t"   \
            "ldi XL, lo8(interruptStack)                    \n\t"   \
            "ldi XH, hi8(interruptStack)                    \n\t"   \
            "ldi r19, 9                                     \n\t"   \
            "10:                                            \n\t"   \
            "dec r19                                        \n\t"   \
            "breq 11f                                       \n\t"   \
            "ld r18, X+                                     \n\t"   \
            "cpi r18, 0xa5                                  \n\t"   \
            "breq 10b                                       \n\t"   \
            " /* Stack overflow detected! */                \n\t"   \
            "ldi r24, 0                                     \n\t"   \
            "ldi r25, 0                                     \n\t"   \
            "lds r22, ISR_NAME                              \n\t"   \
            "lds r23, ISR_NAME + 1                          \n\t"   \
            "call vApplicationStackOverflowHook             \n\t"   \
            "11:                                            \n\t"   \
            "pop    r19                                     \n\t"   \
            "pop    XH                                      \n\t"   \
            "pop    XL                                      \n\t"
//#define isrCHECK_FOR_STACK_OVERFLOW()
#else
#define portCHECK_ISR_STACK_WATERMARK()
#endif

#define portENTER_INTERRUPT()	\
	__asm__ __volatile__ (                                          \
	        " /* Push R18 and SREG */                       \n\t"   \
            "push r18                                       \n\t"   \
            "in r18, __SREG__                               \n\t"   \
            "push r18                                       \n\t"   \
	        " /* Increase the reentry counter */            \n\t"   \
	        "lds r18, interruptStackReentryCounter          \n\t"   \
	        "inc r18                                        \n\t"   \
	        "sts interruptStackReentryCounter, r18          \n\t"   \
	        " /* Check whether we need to store status */   \n\t"   \
	        "cpi r18, 1                                     \n\t"   \
	        "brne 0f                                        \n\t"   \
	        " /* Save user-mode stack */                    \n\t"   \
            "in  r18, __SP_L__                              \n\t"   \
            "sts userCodeStackPointer, r18                  \n\t"   \
            "in  r18, __SP_H__                              \n\t"   \
            "sts userCodeStackPointer + 1, r18              \n\t"   \
	        " /* Enter interrupt stack */                   \n\t"   \
            "lds r18, interruptStackBase                    \n\t"   \
            "out __SP_L__, r18                              \n\t"   \
            "lds r18, interruptStackBase + 1                \n\t"   \
            "out __SP_H__, r18                              \n\t"   \
            "0:                                             \n\t"   \
	::: "memory");

#define portEXIT_INTERRUPT()	\
	__asm__ __volatile__ (									        \
	        "cli                                            \n\t"   \
	        " /* Decrease the reentry counter */            \n\t"   \
	        "lds r18, interruptStackReentryCounter          \n\t"   \
	        "dec r18                                        \n\t"   \
	        "sts interruptStackReentryCounter, r18          \n\t"   \
	        "brne 0f                                        \n\t"   \
	        portCHECK_ISR_STACK_WATERMARK()                         \
	        " /* Load user-mode stack */                    \n\t"   \
            "lds r18, userCodeStackPointer                  \n\t"   \
            "out __SP_L__, r18                              \n\t"   \
            "lds r18, userCodeStackPointer + 1              \n\t"   \
            "out __SP_H__, r18                              \n\t"   \
	        " /* Check for pending yield */                 \n\t"   \
	        "lds r18, isYieldFromIsrPending                 \n\t"   \
	        "cpi r18, 0                                     \n\t"   \
	        "breq 0f                                        \n\t"   \
	        "ldi r18, 0                                     \n\t"   \
	        "sts isYieldFromIsrPending, r18                 \n\t"   \
	        "call vPortYield                                \n\t"   \
            "0:                                             \n\t"   \
            "pop    r18                                     \n\t"   \
            "out __SREG__, r18                              \n\t"   \
            "pop    r18                                     \n\t"   \
	::: "memory");

#define portRTOS_ISR(_vec_name_)                                    \
    static void __real_ ## _vec_name_();                            \
    ISR(_vec_name_) {                                               \
        portENTER_INTERRUPT();                                      \
        __real_ ## _vec_name_();                                    \
        portEXIT_INTERRUPT();                                       \
    }                                                               \
    static void __real_ ## _vec_name_()
//    static void __attribute__((noinline)) __real_ ## _vec_name_()


#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */

