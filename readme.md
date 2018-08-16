This is a fork of Phillip Stevens' port of Richard Berry's freeRTOS, optimised for the Arduino AVR devices. This fork adds support for a separate stack and reentrancy for interrupt service routines (ISRs).

It has been created to provide access to FreeRTOS capabilities, with full compatibility to the Arduino environment.
It does this by keeping hands off almost everything, and only touching the minimum of hardware to be successful.

## Further Reading

The canonical source for information is the [FreeRTOS Web Site](http://www.freertos.org/ "FreeRTOS").
Within this site, the [Getting Started](http://www.freertos.org/FreeRTOS-quick-start-guide.html "Quick Start Guide") page is very useful.
It is worth having a view from a user, and [manicbug](https://maniacbug.wordpress.com/2012/01/31/freertos/) has some interesting examples.
Phillip Stevens' [AVRfreeRTOS Repository](https://sourceforge.net/projects/avrfreertos/) has plenty of examples,
ranging from [blink](https://sourceforge.net/projects/avrfreertos/files/MegaBlink/) through to a [synthesiser](https://sourceforge.net/projects/avrfreertos/files/GA_Synth/).

## General

FreeRTOS has a multitude of configuration options, which can be specified from within the FreeRTOSConfig.h file.
To keep commonality with all of the Arduino hardware options, some sensible defaults have been selected.

The AVR Timer5 is used to generate 10ms time slices, but Tasks that finish before their allocated time will hand execution back to the Scheduler.
This does not affect the use of any other Timer functions in Arduino.

Time slices can be selected by setting the configTICK_RATE_HZ macro. Slower time slicing can allow the Arduino MCU to sleep for longer, without the complexity of a Tickless idle.

Note that Timer resolution is affected by integer math division and the time slice selected. Trying to measure 100ms, using a 60ms time slice for example, won't work.

Stack for the loop() function has been set at 192 bytes. This can be configured by adjusting the configIDLE_STACK_SIZE parameter.
It should not be less than the configMINIMAL_STACK_SIZE. If you have stack overflow issues, just increase it.
Users should prefer to allocate larger structures, arrays, or buffers using pvPortMalloc(), rather than defining them locally on the stack.

Memory for the heap is allocated by the normal malloc() function, wrapped by pvPortMalloc().
This option has been selected because it is automatically adjusted to use the capabilities of each device.
Other heap allocation schemes are supported by FreeRTOS, and they can used with additional configuration.

In the default configuration, only static memory allocation is enabled. This means that an array has to be allocated to hold each task's stack beforehand, instead of relying on dynamic allocation. Dynamic allocation can be easily enabled in the configuration as described in the FreeRTOS documentation.

## Errors

* Stack Overflow: If any stack (for the loop() or) for any Task overflows, there will be a slow LED blink, with 4 second cycle.
* Heap Overflow: If any Task tries to allocate memory and that allocation fails, there will be a fast LED blink, with 100 millisecond cycle.

## Errata

Testing with the Software Serial library shows some incompatibilities at low baud rates (9600), due to the extended time this library disables the global interrupt.

## Compatibility

  * ATmega328 @ 16MHz : Arduino UNO, Arduino Duemilanove, Arduino Diecimila, etc.
  * ATmega328 @ 16MHz : Adafruit Pro Trinket 5V, Adafruit Metro 328, Adafruit Metro Mini
  * ATmega328 @ 16MHz : Seeed Studio Stalker
  * ATmega328 @ 16MHz : Freetronics Eleven
  * ATmega328 @ 12MHz : Adafruit Pro Trinket 3V
  * ATmega32u4 @ 16MHz : Arduino Leonardo, Arduino Micro, Arduino Yun, Teensy 2.0
  * ATmega32u4 @ 8MHz : Adafruit Flora, Bluefruit Micro
  * ATmega1284p @ 24.576MHz : Seeed Studio Goldilocks, Seeed Studio Goldilocks Analogue
  * ATmega2560 @ 16MHz : Arduino Mega, Arduino ADK
  * ATmega2560 @ 16MHz : Seeed Studio ADK
  * ATmegaXXXX @ XXMHz : Anything with an ATmega MCU, really.

## Files & Configuration

* FreeRTOS.h : Must always be #include first. It references other configuration files, and sets defaults where necessary.
* FreeRTOSConfig.h : Contains a multitude of API and environment configurations.
* FreeRTOSVariant.h : Contains the AVR specific configurations for this port of freeRTOS.
* heap_3.c : Contains the heap allocation scheme based on malloc(). Other schemes are available, but depend on user configuration for specific MCU choice.

## Reentrant Interrupt Service Routines (ISR)

In contrast to the original work by Phillip Stevens, this fork adds support for reentrant ISRs. This means that you can reenable interrupt handling within an ISR to allow other interrupts to be handled whithout causing any issue for the scheduler. The only caveat is that the ISRs where you want to reenable interrupts **must** be defined using the *portRTOS_ISR* macro instead of the usual *ISR* macro. The *portRTOS_ISR* macro is defined in portmacro.h, which is already included in FreeRTOS.h, so no additional header needs to be included.


