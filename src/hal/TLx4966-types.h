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
#elif
    #define INLINE 
    #define STATIC 
#endif

/**
 * @defgroup    TLx4966 types
 * @{
 */

#define TLx4966_STARTUP_TIME   1 /**< Start-up time (ms). Minimum 45 us. Source: Innovative Features Integrated in Hall Switches - App Note) */

/**
 * @name    TLx4966 error codes
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
 */
typedef enum
{
    TLx4966_MEASMODE_INTERRUPT = 1,    /**< Interruption mode */
    TLx4966_MEASMODE_POLLING   = 2,    /**< Polling mode */
}TLx4966_MeasMode_t;
/** @} */

/**
 * @name    TLx4966 power modes
 */
typedef enum
{
    TLx4966_POWMODE_MAIN       = 1,    /**< Same as MCU VDD supply signal */
    TLx4966_POWMODE_SWITCH     = 2,    /**< Switched controlled power from MCU */
}TLx4966_PowerMode_t;
/** @} */

/**
 * @name    TLx4966 direction 
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
 */
typedef enum
{
    TLx4966_SPEED_HERTZ = 0,     /**< cps-Hertz */
    TLx4966_SPEED_RADS  = 1,     /**< rad/s */
    TLx4966_SPEED_RPM   = 2,     /**< RPM */
}TLx4966_SpeedUnit_t;
/** @} */

/**
 * @name    TLx4966 measuring parameters
 */
typedef struct 
{
    TLx4966_Dir_t  direction;       /**< Moving direction */
    double          speed;          /**< Rotation speed */    
}TLx4966_Params_t;
/** @} */

/**
 * @name    TLx4966 interruption events
 */
typedef enum
{
    TLx4966_INT_RISING_EDGE    = 0,    /**< Rising edge interruption */
    TLx4966_INT_FALLING_EDGE   = 1     /**< Falling edge interruption */
}TLx4966_IntEvent_t;
/** @} */

/**
 * @name    TLx4966 GPIO levels
 */
typedef enum
{
    TLx4966_GPIO_LOW   = 0,        /**< GPIO voltage low. Positive logic */
    TLx4966_GPIO_HIGH  = 1         /**< GPIO voltage high. Positive logic */
}TLx4966_GPIOLevel_t;
/** @} */

/**
 * @defgroup TLx4966 hardware interface
 */

/**
 * @brief       Interrupt callback
 * @param[in]   void *  Generic function argument
 * @return      void
 */
typedef void (*TLx4966_GPIO_IntCallback)(void *);  

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

/**
 * @brief Instance handle
 */
typedef struct
{
    TLx4966_Status_t       status;            /**< Switch status */
    TLx4966_PowerMode_t    powMode;           /**< Power mode */
    TLx4966_MeasMode_t     measMode;          /**< Measure mode */
    TLx4966_SpeedUnit_t    speedUnit;         /**< Speed measure units */
    uint8_t                polesPair;         /**< Rotor poles pair number */
    TLx4966_HwIntf_t       *hwIntf;           /**< Hardware interface */
    TLx4966_Dir_t          direction;         /**< Direction. Last value measured */
    double                 speed;             /**< Speed. Last value measured */
}TLx4966_Handle_t;

/**
 * @brief       Asserts the TLx4966 interface functions 
 * @param[in]   _f  Function call
 * @return      TLx4966 error code if not TLx4966_OK
 */ 
#define INTF_ASSERT(_f)  {TLx4966_Error_t err = _f; \
                    if(err != TLx4966_OK) return err;}

/** @} TLx4966 types*/

#endif /** TLx4966_TYPES_H_ **/