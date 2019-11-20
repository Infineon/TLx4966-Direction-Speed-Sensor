/** 
 * @file        TLx4966-ino.cpp
 * @brief       TLx4966 C++ Class
 *          
 *              Infineon TLx4966 Double hall switch sensor C++ class:
 *               - C++ wrapper for TLx4966 C HAL library
 *               - Polling or interrupt based measuring mode
 *               - GPIO controlled power-on/off interface avilable
 *               - Flexible hardware platform configuration
 *               - Avaiable configured platforms: 
 *                  - xmc2go + shield2go in polling mode     
 *  
 * @date        April 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#include <stdlib.h>
#include <Arduino.h>
#include "TLx4966-ino.h"

extern "C" 
{
#include "tlx4966/TLx4966.h"
#include "hw-platf/TLx4966-platf-ard.h"
} 

/**
 * @addtogroup arduinoAPI
 * @{
 */

/**
 * @brief           Double hall switch instance constructor
 *                  Mandatory hw interfaces: dir, speed, timer.
 *                  Optional  hw interfaces: power (only for switch mode platform).
 * 
 * @param[in]       polesPair   Rotor poles pair number     
 * @param[in]       speedUnit   Speed unit 
 * @param[in]       ardHwPlatf  Arduino hardware platform
 * @param[in]       measMode    Measuring mode 
 * @return          void         
 */
TLx4966::TLx4966(uint8_t              polesPair,
                 TLx4966_SpeedUnit_t  speedUnit,
                 ArduinoHwPlatf       ardHwPlatf,   
                 TLx4966_MeasMode_t   measMode)
{
    TLx4966_HwIntf_t *hw = NULL; 

    if(ardHwPlatf == Shield_2Go)
    {
        if(measMode == TLx4966_MEASMODE_POLLING)
        {
            hw = &TLx4966_Shield2Go_poll;
        }
        else if(measMode == TLx4966_MEASMODE_INTERRUPT)
        {
            hw = &TLx4966_Shield2Go_interrupt;
        }
    }
    else
    {
        /* Other arduino platfroms*/
    }

    TLx4966_Config(&(this->handle), polesPair, TLx4966_POWMODE_MAIN, measMode, speedUnit, hw);
}

/**
 * @brief           Double hall switch instance constructor
 *                  Mandatory hw interfaces: dir, speed, timer.
 *                  Optional  hw interfaces: power (only for switch mode platform).
 * 
 * @param[in]       polesPair   Rotor poles pair number     
 * @param[in]       powMode     Power mode   
 * @param[in]       measMode    Measuring mode 
 * @param[in]       speedUnit   Speed unit        
 * @param[in]       *hwIntf     Pointer to hardware interface handle
 * @return          void         
 */
TLx4966::TLx4966(uint8_t              polesPair,
                 TLx4966_PowerMode_t  powMode,   
                 TLx4966_MeasMode_t   measMode,
                 TLx4966_SpeedUnit_t  speedUnit,  
                 TLx4966_HwIntf_t     *hwIntf)
{
    TLx4966_HwIntf_t *hw = NULL; 

    if(measMode == TLx4966_MEASMODE_POLLING)
    {
        hw = &TLx4966_Shield2Go_poll;
    }
    else if(measMode == TLx4966_MEASMODE_INTERRUPT)
    {
        hw = &TLx4966_Shield2Go_interrupt;
    }

    TLx4966_Config(&(this->handle), polesPair, powMode, measMode, speedUnit, hw);
}

/**
 * @brief       Initilializes the hardware interfaces
 * @return      TLx4966 error code    
 * @retval      TLx4966_OK if success
 * @retval      TLx4966_INTF_ERROR if hardware interface error            
 */
TLx4966_Error_t TLx4966::begin(void)
{
    return TLx4966_Init(&(this->handle));
}

/**
 * @brief   Double hall switch instance destructor    
 *          Disables the switch   
 * @return  void
 */
TLx4966::~TLx4966(void)
{
   TLx4966_Disable(&(this->handle)); 
}

/**
 * @brief       Enables the sensor
 *              - If the "Switch power mode" is configured, the sensor is powered up.
 *              - If the "Interrupt measuring mode" is configured, the interrupt are enabled. 
 *              - The measuring speed timer is started
 * @return      TLx4966 error code    
 * @retval      TLx4966_OK if success
 * @retval      TLx4966_INTF_ERROR if hardware interface error            
 */
TLx4966_Error_t TLx4966::enable(void)
{
    return TLx4966_Enable(&(this->handle));
}

/**
 * @brief       Disables the sensor
 *              - If the "Interrupt measuring mode" is configured, the interrupt are enabled. 
 *              - If the "Switch power mode" is configured, the sensor is powered off.
 *              - The measuring speed timer is stopped. 
 * @return      TLx4966 error code    
 * @retval      TLx4966_OK if success
 * @retval      TLx4966_INTF_ERROR if hardware interface error            
 */
TLx4966_Error_t TLx4966::disable()
{
    return TLx4966_Disable(&(this->handle));    
}

/**
 * @brief       Updates the rotation direction and speed. 
 * @note        To be used in super-loop. 
 * @return      void        
 */
void TLx4966::measureLoop()
{
    TLx4966_UpdateValues(&(this->handle));
}

/**
 * @brief       Reads the sensor status
 * @return      TLx4966 status         
 */
TLx4966_Status_t TLx4966::readStatus()
{
    return TLx4966_GetStatus(&(this->handle));
}

/**
 * @brief       Gets the rotation direction
 *              The direction definition (clock-wise/anticlock-wise, left/rigth, positive/negative...)
 *              is dependant of the mechanical configuration of the sensor with repect to the
 *              rotor. The library just provide voltage-low/voltage-high directions.
 * @return      TLx4966 direction
 * @retval      TLx4966_DIR_UNDEF prior initial rotor movement (after initialization)             
 */
TLx4966_Dir_t TLx4966::readDirection()
{   
    return TLx4966_GetDirection(&(this->handle));
}

/**
 * @brief       Gets the rotation speed
 * @return      Speed in the unit specified in the configuration         
 */
double TLx4966::readSpeed()
{
    return TLx4966_GetSpeed(&(this->handle));
}

/** @} */