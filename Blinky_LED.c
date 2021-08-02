/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "IfxPort.h"
#include "Bsp.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define LED_D107    &MODULE_P33,8                                           /* LED D107: Port, Pin definition       */
#define WAIT_TIME   500                                                     /* Wait time constant in milliseconds   */

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
/* This function initializes the port pin which drives the LED */
void initLED(void)
{
    /* Initialization of the LED used in this example */
    IfxPort_setPinModeOutput(LED_D107, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

    /* Switch OFF the LED (low-level active) */
    IfxPort_setPinHigh(LED_D107);
}

/* This function toggles the port pin and wait 500 milliseconds */
void blinkLED(void)
{
    IfxPort_togglePin(LED_D107);                                                /* Toggle the state of the LED      */
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME));    /* Wait 500 milliseconds            */
}
