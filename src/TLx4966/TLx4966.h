/** 
 * @file        TLx4966.h
 * @brief       TLx4966 C library Hardware Abstraction Layer 
 * @date        April 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#ifndef TLx4966_H_
#define TLx4966_H_

#include "TLx4966-types.h"
#include "TLx4966-hal.h"

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



void                TLx4966_Config         (TLx4966_Handle_t      *handle, 
                                            uint8_t               polesPair,
                                            TLx4966_PowerMode_t   powMode,
                                            TLx4966_MeasMode_t    measMode,
                                            TLx4966_SpeedUnit_t   speedUnit,   
                                            TLx4966_HwIntf_t      *hwIntf);
TLx4966_Error_t     TLx4966_Init           (TLx4966_Handle_t      *handle);
TLx4966_Error_t     TLx4966_Enable         (TLx4966_Handle_t *handle);
TLx4966_Error_t     TLx4966_Disable        (TLx4966_Handle_t *handle);
void                TLx4966_UpdateSpeed    (TLx4966_Handle_t *handle);
void                TLx4966_UpdateDirection(TLx4966_Handle_t *handle);
void                TLx4966_UpdateValues   (TLx4966_Handle_t *handle);
void                TLx4966_GetValues      (TLx4966_Handle_t *handle, 
                                            TLx4966_Params_t *params);
TLx4966_Status_t    TLx4966_GetStatus      (TLx4966_Handle_t *handle);
double              TLx4966_GetSpeed       (TLx4966_Handle_t *handle);
TLx4966_Dir_t       TLx4966_GetDirection   (TLx4966_Handle_t *handle);

#endif /** TLx4966_H_ **/
