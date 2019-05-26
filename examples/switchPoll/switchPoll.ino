#include <Arduino.h>
#include <stdint.h>
#include <TLx4966-ino.h>

uint8_t           polesN    = 1;                  /**< Number of poles pair*/
TLx4966_Status_t  sSt       = TLx4966_UNINITED;   /**< Switch status */
TLx4966_Dir_t     sDir      = TLx4966_DIR_UNDEF;  /**< Direction of movement */ 
double            sSpd      = 0;                  /**< Speed */
   
TLx4966          s(polesN);                       /**< Switch object */

/**
 * @brief       Prints the sensor data in JSON format 
 * @param[in]   stat  status
 * @param[in]   dir   direction
 * @param[]     spd   speed
 */
void JSONPrint(TLx4966_Status_t stat,
               TLx4966_Dir_t    dir,
               double            spd)
{
  Serial.print("{ \"status\" : ");
  Serial.print(stat);
  Serial.print(", \"direction\" : ");
  Serial.print(dir);
  Serial.print(", \"speed\" : ");
  Serial.print(spd);
  Serial.println(" }");
}


void setup()
{    
  TLx4966_Error_t  errCode   = TLx4966_OK;     

  delay(1000);

  Serial.begin(115200);
  Serial.println("Serial initialized.");

  /* Begin switch hardware interfaces */
  errCode = s.begin();
  if(errCode != TLx4966_OK)
  {
      Serial.print("Double Switch error: ");
      Serial.println(errCode);
  }

  /* Enable the switch */
  errCode = s.enable();
  if(errCode != TLx4966_OK)
  {
      Serial.print("Double Switch error: ");
      Serial.println(errCode);
  }

  JSONPrint(sSt, sDir, sSpd);
}

void loop()
{
  /* Super-loop switch measurements */
  s.measureLoop();

  TLx4966_Status_t cursSt   = s.readStatus();
  TLx4966_Dir_t cursDir     = s.readDirection(); 
  double cursSpd            = s.readSpeed(); 

  /* Print switch data if any variable has suffered changes */
  if(cursSt != sSt || cursDir != sDir || cursSpd != sSpd )  
  {
    JSONPrint(cursSt, cursDir, cursSpd);
  }

  sSt  = cursSt;
  sDir = cursDir;
  sSpd = cursSpd; 
}