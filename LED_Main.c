 /*********************************************************************************************************************/
 /*\title Blinky LED
 * \abstract An LED is blinking based on the timing given by a wait function.
 * \description A wait function is used to add delays between switching on and switching off an LED on port pin P33.8.
 * \name Blinky_LED_1_KIT_TC265_TFT
 * \version V1.0.1
 * \board APPLICATION KIT TC2x5 V2.0, KIT_AURIX_TC265_TFT_BC-Step, TC26xD_B-Step
 * \keywords LED, blinky, GPIO, General Port Input Output, AURIX, Blinky_LED_1
 * \documents https://www.infineon.com/aurix-expert-training/Infineon-AURIX_Blinky_LED_1_KIT_TC265_TFT-TR-v01_00_01-EN.pdf
 * \documents https://www.infineon.com/aurix-expert-training/TC26B_iLLD_UM_1_0_1_12_0.chm
 * \lastUpdated 2020-12-18
 *********************************************************************************************************************/

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "Blinky_LED.h"

IfxCpu_syncEvent g_cpuSyncEvent = 0;

int core0_main(void)
{
    IfxCpu_enableInterrupts();

    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    initLED();  /* Initialize the LED port pin      */

    while(1)
    {
        blinkLED(); /* Make the LED blink           */
    }
    return (1);
}
