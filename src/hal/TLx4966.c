/** 
 * @file        TLx4966.c
 * @brief       TLx4966 C library Hardware Abstraction Layer 
 *              Features: 
 *              - Polling or interrupt based measuring mode
 *              - GPIO controlled power-on/off interface avilable
 *              - Flexible hardware platform configuration
 * @date        April 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#include <stdlib.h>
#include <stdbool.h>
#include "TLx4966.h"

/**
 * @defgroup TLx4966 C library HAL
 * @{
 */

/**
 * @defgroup Speed unit product coefficients
 * @{
 */
#define TLx4966_SPEED_COEF_HZ      1000.0      /**< Hertz - cps*/
#define TTLx4966_SPEED_COEF_RADS   6283.2      /**< Rad/s */
#define TTLx4966_SPEED_COEF_RPM    60000.0     /**< RPM  */


/**
 * @brief Look-up array for Speed Unit Coeffients
 */
STATIC double TLx4966_SpeedCoeffient[3] = 
{
    TLx4966_SPEED_COEF_HZ,
    TTLx4966_SPEED_COEF_RADS,
    TTLx4966_SPEED_COEF_RPM
};

/** @} */


/**
 * @brief           Configures the sensor handle attributes
 *                  Mandatory hw interfaces: dir, speed, timer.
 *                  Optional  hw interfaces: power (only for switch mode platform).
 * 
 * @param[in/out]   handle      Pointer to the sensor instance handle
 * @param[in]       polesPair   Rotor poles pair number     
 * @param[in]       powMode     Power mode   
 * @param[in]       measMode    Measuring mode 
 * @param[in]       speedUnit   Speed unit        
 * @param[in]       *hwIntf     Pointer to hardware interface handle
 * @return          void         
 */
void TLx4966_Config(TLx4966_Handle_t       *handle, 
                     uint8_t                polesPair,
                     TLx4966_PowerMode_t    powMode,
                     TLx4966_MeasMode_t     measMode,  
                     TLx4966_SpeedUnit_t    speedUnit,    
                     TLx4966_HwIntf_t       *hwIntf)
{
    handle->status    = TLx4966_UNINITED;
    handle->polesPair = polesPair;
    handle->powMode   = powMode;
    handle->measMode  = measMode;
    handle->speedUnit = speedUnit;

    handle->direction = TLx4966_DIR_UNDEF; 
    handle->speed     = 0; 
    handle->hwIntf    = hwIntf;
}

/**
 * @brief       Initilializes the hardware interfaces
 * @param[in]   handle  Pointer to the sensor entity handle
 * @return      TLx4966 error code    
 * @retval      TLx4966_OK if success
 * @retval      TLx4966_INTF_ERROR if hardware interface error            
 */
TLx4966_Error_t TLx4966_Init(TLx4966_Handle_t *handle)
{
    TLx4966_Error_t err = TLx4966_OK;

    if(handle->powMode == TLx4966_POWMODE_SWITCH)
    {
        INTF_ASSERT(handle->hwIntf->power->init());
    }   

    INTF_ASSERT(handle->hwIntf->dir->init());
    INTF_ASSERT(handle->hwIntf->speed->init());
    INTF_ASSERT(handle->hwIntf->timer->init());

    handle->status    = TLx4966_INITED;
    return err;
}

/**
 * @brief       Direction interrupt callback
 *              The sensor instance direction is updated upon interrupt event: rising or falling edge.
 * 
 * @param[in]   handle      Pointer to the sensor entity handle
 * @return      void    
 */
STATIC void TLx4966_DirectionCallback(TLx4966_Handle_t *handle)
{
    TLx4966_IntEvent_t event = handle->hwIntf->dir->intEvent();

    if(event == TLx4966_INT_FALLING_EDGE)
    {
        handle->direction = TLx4966_DIR_LOW;
    }
    else if(event == TLx4966_INT_RISING_EDGE)
    {
        handle->direction = TLx4966_DIR_HIGH;
    }
}

/**
 * @brief       Calculates the speed             
 *              The rising edge of the speed GPIO signal determines the rotating 
 *              period between a pair of poles. The speed is then obtained by applying 
 *              the selected speed unit product coefficient, and dividing between the 
 *              number of poles pairs. 
 * 
 * @note        To be called upon speed GPIO rising edge. 
 * 
 * @param[in]   handle      Pointer to the sensor entity handle
 * @return      void    
 */
STATIC INLINE void TLx4966_CalculateSpeed(TLx4966_Handle_t *handle)
{
    handle-> speed = TLx4966_SpeedCoeffient[handle->speedUnit]/((double)handle->polesPair * (double)handle->hwIntf->timer->elapsed());
}

/**
 * @brief       Speed interrupt callback
 *              The sensor instance instant speed is updated upon rising edge. 
 * 
 * @param[in]   handle      Pointer to the sensor entity handle
 * @return      void    
 */
STATIC void TLx4966_SpeedCallback(TLx4966_Handle_t *handle)
{
    TLx4966_IntEvent_t event = handle->hwIntf->speed->intEvent();

    if(event == TLx4966_INT_RISING_EDGE)
    {
        TLx4966_CalculateSpeed(handle);
        handle->hwIntf->timer->stop();
        handle->hwIntf->timer->start();
    }
}

/**
 * @brief       Enables the sensor
 *              - If the "Switch power mode" is configured, the sensor is powered up.
 *              - If the "Interrupt measuring mode" is configured, the interrupt are enabled. 
 *              - The measuring speed timer is started. 
 *              
 * @param[in]   handle  Pointer to the sensor entity handle
 * @return      TLx4966 error code    
 * @retval      TLx4966_OK if success
 * @retval      TLx4966_INTF_ERROR if hardware interface error            
 */
TLx4966_Error_t TLx4966_Enable(TLx4966_Handle_t *handle)
{
    TLx4966_Error_t err = TLx4966_OK;

    if(handle->powMode == TLx4966_POWMODE_SWITCH)
        INTF_ASSERT(handle->hwIntf->power->enable());

    INTF_ASSERT(handle->hwIntf->timer->delay(TLx4966_STARTUP_TIME));

    if(handle->measMode == TLx4966_MEASMODE_INTERRUPT)
    {
        INTF_ASSERT(handle->hwIntf->dir->enableInt((TLx4966_GPIO_IntCallback)TLx4966_DirectionCallback,handle));
        INTF_ASSERT(handle->hwIntf->speed->enableInt((TLx4966_GPIO_IntCallback)TLx4966_SpeedCallback,handle));
    }

    handle->hwIntf->timer->start();

    handle->status = TLx4966_ON;

    return err;
}

/**
 * @brief       Disables the sensor
 *              - If the "Interrupt measuring mode" is configured, the interrupt are enabled. 
 *              - If the "Switch power mode" is configured, the sensor is powered off.
 *              - The measuring speed timer is stopped. 
 *              
 * @param[in]   handle  Pointer to the sensor entity handle
 * @return      TLx4966 error code    
 * @retval      TLx4966_OK if success
 * @retval      TLx4966_INTF_ERROR if hardware interface error            
 */
TLx4966_Error_t TLx4966_Disable(TLx4966_Handle_t *handle)
{
    if(handle->measMode == TLx4966_MEASMODE_INTERRUPT)
    {
        INTF_ASSERT(handle->hwIntf->dir->disableInt());
        INTF_ASSERT(handle->hwIntf->speed->disableInt());
    }
    if(handle->powMode == TLx4966_POWMODE_SWITCH)
        INTF_ASSERT(handle->hwIntf->power->disable());

    handle->hwIntf->timer->stop();

    return TLx4966_OK;
}

/**
 * @brief       Updates the sensor speed. 
 *              A rising edge is detected when the gpio voltage level changes from low to high.
 * 
 * @note        To be used in super-loop.           
 * @param[in]   handle  Pointer to the sensor entity handle
 * @return      void        
 */
void TLx4966_UpdateSpeed(TLx4966_Handle_t *handle)
{
    static bool waitingRisingEdge = true;

    if(waitingRisingEdge && (handle->hwIntf->speed->read() == TLx4966_GPIO_HIGH))
    {
        TLx4966_CalculateSpeed(handle);
        handle->hwIntf->timer->stop();
        handle->hwIntf->timer->start();
        waitingRisingEdge = false;
    }
    else if(!waitingRisingEdge && (handle->hwIntf->speed->read() == TLx4966_GPIO_LOW))
    {
        waitingRisingEdge = true;
    }
}

/**
 * @brief       Updates the rotation direction 
 *              A rising edge is detected when the gpio voltage level changes from low to high.
 * 
 * @note        To be used in super-loop. 
 * @param[in]   handle  Pointer to the sensor entity handle
 * @return      void        
 */
INLINE void TLx4966_UpdateDirection(TLx4966_Handle_t *handle)
{
    handle->direction =  handle->hwIntf->dir->read();
}

/**
 * @brief       Updates the rotation direction and speed. 
 * @note        To be used in super-loop. 
 * @param[in]   handle  Pointer to the sensor entity handle
 * @return      void        
 */
INLINE void TLx4966_UpdateValues(TLx4966_Handle_t *handle)
{
    TLx4966_UpdateSpeed(handle);
    TLx4966_UpdateDirection(handle);
}

/**
 * @brief       Gets the rotation direction and speed parameters
 * @param[in]   handle  Pointer to the sensor entity handle
 * @param[out]  *params Pointer to the struct to store the parameter values
 * @return      void        
 */
INLINE void TLx4966_GetValues(TLx4966_Handle_t *handle, TLx4966_Params_t *params)
{
    params->direction = handle->direction;
    params->speed     = handle->speed;
}

/**
 * @brief       Gets the sensor status
 * @param[in]   handle  Pointer to the sensor entity handle
 * @return      TLx4966 status         
 */
INLINE TLx4966_Status_t TLx4966_GetStatus(TLx4966_Handle_t *handle)
{
    return handle->status;
}

/**
 * @brief       Gets the rotation speed
 * @param[in]   handle  Pointer to the sensor entity handle
 * @return      Speed in the unit specified in the configuration         
 */
INLINE double TLx4966_GetSpeed(TLx4966_Handle_t *handle)
{
    return handle->speed;
}

/**
 * @brief       Gets the rotation direction
 *              The direction definition (clock-wise/anticlock-wise, left/rigth, positive/negative...)
 *              is dependant of the mechanical configuration of the sensor with repect to the
 *              rotor. The library just provide voltage-low/voltage-high directions.
 * @param[in]   handle  Pointer to the sensor entity handle
 * @return      TLx4966 direction
 * @retval      TLx4966_DIR_UNDEF prior initial rotor movement (after initialization)             
 */
INLINE TLx4966_Dir_t TLx4966_GetDirection(TLx4966_Handle_t *handle)
{
    return handle->direction;
}

/** @} */