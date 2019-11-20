/** 
 * @file        TLx4966-platf-ard.c
 * @brief       TLx4966 Hardware Platform Arduino
 * 
 *              The hardware interfaces for the specific Arduino hardware platforms.
 *              As Arduino platform is considered any interface implementation using the 
 *              Arduino Functions set (https://www.arduino.cc/reference/en/).  
 *              Platforms:
 *              - Shield2Go (polling mode)
 * @date        April 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#include <Arduino.h>
#include <stdlib.h>
#include "TLx4966-platf-ard.h"
#include "../TLx4966/TLx4966-types.h"

/**
 * @addtogroup tlx4966ardhp
 * @{
 * @addtogroup tle4966k2go 
 * @{
 */

//#define CUSTOM_ARDUINO_HARDWARE   /**< Uncomment this line for MH_ET_LIVE_ESP32MINIKIT usage (Not defined as during compilation)*/

#ifdef XMC1100_XMC2GO                                                   /**< xmc2go + Shield2Go  */    
STATIC const int    speedGPIO = 4;                                      /**< Pin connected to Q2 */
STATIC const int    dirGPIO   = 9;                                      /**< Pin connected to Q1 */
#elif ((XMC1100_Boot_Kit)  || (XMC4700_Relax_Kit) || (ARDUINO_AVR_UNO)) /**< xmc arduino and arduino uno boards */  
STATIC const int    speedGPIO = 7;                                      /**< Pin connected to Q2 */
STATIC const int    dirGPIO   = 3;                                      /**< Pin connected to Q1 */
#elif defined(CUSTOM_ARDUINO_HARDWARE)                                  /**< Custom Arduino Configuration*/ 
STATIC const int    speedGPIO = x;                                      /**< Pin connected to Q2 */
STATIC const int    dirGPIO   = y;                                      /**< Pin connected to Q1 */
#else
# error "Board not yet defined. Please define the specific Arduino boards Pins"
#endif

STATIC uint32_t      curTime   = 0;     /**< Current time */

STATIC TLx4966_Error_t GPIO_Speed_Init()
{
    pinMode(speedGPIO, INPUT_PULLUP);
    return TLx4966_OK;
};

STATIC TLx4966_GPIOLevel_t GPIO_Speed_Read()
{
    return digitalRead(speedGPIO);
}

TLx4966_GPIO_t speed =
{
    &GPIO_Speed_Init,
    NULL,
    NULL,
    NULL,
    &GPIO_Speed_Read,
    NULL,
    NULL
};

STATIC TLx4966_Error_t GPIO_Dir_Init(void)
{
    pinMode(dirGPIO, INPUT_PULLUP);
    return TLx4966_OK;
};

STATIC TLx4966_GPIOLevel_t GPIO_Dir_Read()
{
    return digitalRead(dirGPIO);
}

TLx4966_GPIO_t direction =
{
    GPIO_Dir_Init,
    NULL,
    NULL,
    NULL,
    GPIO_Dir_Read,
    NULL,
    NULL
};

STATIC TLx4966_Error_t Timer_Init()
{
    //Nothing
}

STATIC void Timer_Start()
{
    curTime = millis();
}

STATIC uint32_t Timer_Elapsed()
{
    return (millis() - curTime);
}

STATIC void Timer_Stop()
{
    //Nothing
}

STATIC TLx4966_Error_t Timer_Delay(uint32_t timeout)
{
    delay(timeout);
    return TLx4966_OK;
}

TLx4966_Timer_t timer = 
{
    &Timer_Init,
    &Timer_Start,
    &Timer_Elapsed,
    &Timer_Stop,
    &Timer_Delay
};

TLx4966_HwIntf_t TLx4966_Shield2Go_poll =
{
    NULL,
    &direction,
    &speed,
    &timer
};


/**< Not available. TODO */
TLx4966_HwIntf_t TLx4966_Shield2Go_interrupt =
{
    NULL,
    NULL,
    NULL,
    NULL
};

/** @} */
/** @} */