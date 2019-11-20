/** 
 * @file        TLx4966-types.h
 * @brief       TLx4966 Types
 * @date        April 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#ifndef TLx4966_TYPES_H_
#define TLx4966_TYPES_H_

#include <stdint.h>

#ifndef TEST
    #define INLINE inline
    #define STATIC static
#else
    #define INLINE 
    #define STATIC 
#endif

/**
 * @addtogroup  tlx4966typ
 * @{
 */

#define TLx4966_STARTUP_TIME   1 /**< Start-up time (ms). Minimum 45 us. Source: Innovative Features Integrated in Hall Switches - App Note) */

/**
 * @name    TLx4966 error codes
 * @{
 */
typedef enum
{
    TLx4966_OK          =  0,  /**< No error */
    TLx4966_INTF_ERROR  = -1,  /**< Interface error */
    TLx4966_CONF_ERROR  = -2,  /**< Configuration error*/
}TLx4966_Error_t;
/** @} */

/**
 * @name    TLx4966 status
 * @{
 */
typedef enum
{
    TLx4966_UNINITED   = 0,   /**< Not initialized */
    TLx4966_INITED     = 1,   /**< Initialized (external interfaces initialized) */
    TLx4966_OFF        = 2,   /**< Power off */
    TLx4966_ON         = 3,   /**< Power on */
}TLx4966_Status_t;
/** @} */

/**
 * @name    TLx4966 measuring modes
 * @{
 */
typedef enum
{
    TLx4966_MEASMODE_INTERRUPT = 1,    /**< Interruption mode */
    TLx4966_MEASMODE_POLLING   = 2,    /**< Polling mode */
}TLx4966_MeasMode_t;
/** @} */

/**
 * @name    TLx4966 power modes
 * @{
 */
typedef enum
{
    TLx4966_POWMODE_MAIN       = 1,    /**< Same as MCU VDD supply signal */
    TLx4966_POWMODE_SWITCH     = 2,    /**< Switched controlled power from MCU */
}TLx4966_PowerMode_t;
/** @} */

/**
 * @name    TLx4966 direction 
 * @{
 */
typedef enum
{
    TLx4966_DIR_UNDEF = -1,      /**< Direction Q1 output default high. Undefined. */
    TLx4966_DIR_LOW   =  0,      /**< Direction Q1 output low */
    TLx4966_DIR_HIGH  =  1       /**< Direction Q1 output high */
}TLx4966_Dir_t;
/** @} */

/**
 * @name    TLx4966 speed units
 * @{
 */
typedef enum
{
    TLx4966_SPEED_HERTZ = 0,     /**< cps-Hertz */
    TLx4966_SPEED_RADS  = 1,     /**< rad/s */
    TLx4966_SPEED_RPM   = 2,     /**< RPM */
}TLx4966_SpeedUnit_t;
/** @} */

/**
 * @name TLx4966 speed unit product coefficients
 * @{
 */
#define TLx4966_SPEED_COEF_HZ      1000.0      /**< Hertz - cps (1000 ms)*/
#define TLx4966_SPEED_COEF_RADS    6283.2      /**< Rad/s (2pi x 1000 ms) */
#define TLx4966_SPEED_COEF_RPM     60000.0     /**< RPM  (60 x 1000 ms)*/

/** @} */
/** @} */

/**
 * @name    TLx4966 measuring parameters
 * @{
 */
typedef struct 
{
    TLx4966_Dir_t  direction;       /**< Moving direction */
    double          speed;          /**< Rotation speed */    
}TLx4966_Params_t;
/** @} */

/**
 * @name    TLx4966 interruption events
 * @{
 */
typedef enum
{
    TLx4966_INT_RISING_EDGE    = 0,    /**< Rising edge interruption */
    TLx4966_INT_FALLING_EDGE   = 1     /**< Falling edge interruption */
}TLx4966_IntEvent_t;
/** @} */

/**
 * @name    TLx4966 GPIO levels
 * @{
 */
typedef enum
{
    TLx4966_GPIO_LOW   = 0,        /**< GPIO voltage low. Positive logic */
    TLx4966_GPIO_HIGH  = 1         /**< GPIO voltage high. Positive logic */
}TLx4966_GPIOLevel_t;
/** @} */

/** @} TLx4966 types*/

/**
 * @brief       Asserts the TLx4966 interface functions 
 * @param[in]   _f  Function call
 * @return      TLx4966 error code if not TLx4966_OK
 */ 
#define INTF_ASSERT(_f)  {TLx4966_Error_t err = _f; \
                    if(err != TLx4966_OK) return err;}



#endif /** TLx4966_TYPES_H_ **/