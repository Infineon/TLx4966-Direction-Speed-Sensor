/** 
 * @file        TLx4966-ino.h
 * @brief       TLx4966 C++ Class
 * @date        April 2019
 * @copyright   Copyright (c) 2019 Infineon Technologies AG
 */

#ifndef TLx4966_INO_H_
#define TLx4966_INO_H_

extern "C"
{
#include "TLx4966/TLx4966-types.h"
#include "TLx4966/TLx4966.h"
}

/**
 * @class TLx4966 
 */
class TLx4966
{
    private:
        TLx4966_Handle_t handle;   /**< TLx4966 attributes handle */

    public:
        /**
         * @brief Arduino hardware platforms
         */
        enum ArduinoHwPlatf 
        { 
            Shield_2Go      /**< Shield 2Go */
        };

        TLx4966                            (uint8_t              polesPair,
                                            TLx4966_SpeedUnit_t  speedUnit  = TLx4966_SPEED_RPM,
                                            ArduinoHwPlatf       ardHwPlatf = Shield_2Go,  
                                            TLx4966_MeasMode_t   measMode   = TLx4966_MEASMODE_POLLING);
        TLx4966                            (uint8_t              polesPair,
                                            TLx4966_PowerMode_t  powMode,
                                            TLx4966_MeasMode_t   measMode,
                                            TLx4966_SpeedUnit_t  speedUnit,
                                            TLx4966_HwIntf_t     *hwIntf);
        ~TLx4966                           ();
        TLx4966_Error_t    begin           ();
        TLx4966_Error_t    enable          ();
        TLx4966_Error_t    disable         ();
        void               measureLoop     ();
        TLx4966_Status_t   readStatus      ();
        TLx4966_Dir_t      readDirection   ();
        double             readSpeed       ();
};

#endif /** TLx4966_INO_H_ **/