/** 
 * @file        TLx4966-platf-ard.h
 * @brief       TLx4966 Hardware Platform Arduino
 *              - Shield2Go polling mode
 * @date        April 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#ifndef TLx4966_PLATF_ARD_H_
#define TLx4966_PLATF_ARD_H_

#include "../hal/TLx4966-types.h"

extern TLx4966_HwIntf_t TLx4966_Shield2Go_poll;       /**< Shield2Go Hardware Interface Arduino - Polling Mode*/
extern TLx4966_HwIntf_t TLx4966_Shield2Go_interrupt;  /**< Not available. @todo TODO */

#endif /** TLx4966_PLATF_ARD_H_ **/