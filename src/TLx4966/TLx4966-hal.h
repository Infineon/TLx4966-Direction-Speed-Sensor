/** 
 * @file        TLx4966-hal.h
 * @brief       TLx4966 Hardware Abstraction Layer
 * @date        November 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#ifndef TLx4966_HAL_H_
#define TLx4966_HAL_H_

#include <stdint.h>
#include "TLx4966-types.h"

/**
 * @addtogroup tlx4966hal
 * @{
 */

/**
 * @brief   GPIO hardware interface
 */
typedef struct 
{
    /**
     * @brief   Initialiazes the GPIO
     * @return  TLx4966 error code
     * @retval  TLx4966_OK if success
     * @retval  TLx4966_INTF_ERROR if hardware interface error
     */
    TLx4966_Error_t        (*init)         (void);
    /**
     * @brief       Enables hardware interrupt
     * @param[in]   *cback  Function pointer to the interrupt callback function   
     * @param[in]   *fargs  Callback arguments
     * @return      TLx4966 error code
     * @retval      TLx4966_OK if success
     * @retval      TLx4966_INTF_ERROR if hardware interface error
     */
    TLx4966_Error_t        (*enableInt)    (TLx4966_GPIO_IntCallback cback, void * fargs);
     /**
     * @brief   Disables hardware interrupt
     * @return  TLx4966 error code
     * @retval  TLx4966_OK if success
     * @retval  TLx4966_INTF_ERROR if hardware interface error
     */
    TLx4966_Error_t        (*disableInt)   (void);
     /**
     * @brief       Gets the latest interrupt event type
     * @return      TLx4966 interruption event
     * @retval      TLx4966_INT_RISING_EDGE if rising edge interrupt
     * @retval      TLx4966_INT_FALLING_EDGE if falling edge interrupt
     */
    TLx4966_IntEvent_t     (*intEvent)     (void);
    /**
     * @brief   Reads the GPIO voltage level
     * @return  TLx4966 GPIO voltage level
     * @retval  TLx4966_GPIO_LOW if voltage low
     * @retval  TLx4966_GPIO_HIGH if voltage high
     */
    TLx4966_GPIOLevel_t    (*read)         (void);
    /**
     * @brief   Enables the GPIO (logic high)
     * @return  TLx4966 error code
     * @retval  TLx4966_OK if success
     * @retval  TLx4966_INTF_ERROR if hardware interface error
     */
    TLx4966_Error_t        (*enable)       (void);
    /**
     * @brief   Disables the GPIO (logic low)
     * @return  TLx4966 error code
     * @retval  TLx4966_OK if success
     * @retval  TLx4966_INTF_ERROR if hardware interface error
     */
    TLx4966_Error_t        (*disable)      (void);
}TLx4966_GPIO_t;

/**
 * @brief   Timer hardware interface
 */
typedef struct
{
    /**
     * @brief   Initialiazes the timer
     * @return  TLx4966 error code
     * @retval  TLx4966_OK if success
     * @retval  TLx4966_INTF_ERROR if hardware interface error
     */
    TLx4966_Error_t     (*init)    (void);
    /**
     * @brief   Starts the timer
     * @return  void
     */
    void                (*start)   (void);
    /**
     * @brief   Elapsed time since the timer was started 
     * @return  Elapsed time in milliseconds
     */
    uint32_t            (*elapsed) (void);
    /**
     * @brief   Stops the timer
     * @return  void
     */
    void                (*stop)    (void);
    /**
     * @brief       Introduces a delay during the specified time    
     * @param[in]   timeout    Delay time in milliseconds   
     * @return      TLx4966 error code    
     * @retval      TLx4966_OK if success
     * @retval      TLx4966_INTF_ERROR if hardware interface error
     */
    TLx4966_Error_t    (*delay)   (uint32_t timeout);
}TLx4966_Timer_t;

/**
 * @brief   Hardware interface
 */
typedef struct 
{
    TLx4966_GPIO_t     *power;     /**< Power On/Off. For GPIO switch power controlled platforms */
    TLx4966_GPIO_t     *dir;       /**< Direction */ 
    TLx4966_GPIO_t     *speed;     /**< Speed */
    TLx4966_Timer_t    *timer;     /**< Timer */
}TLx4966_HwIntf_t;

/** @} */


#endif /** TLx4966_HAL_H_ **/