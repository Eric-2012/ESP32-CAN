/******************************************************************************
 * func_HMI.cpp
 * This file has the code for the functions that prepares CAN packets to send to the HMI
 *****************************************************************************/

#include "func_HMI.h"

// Prepare the 1 CAN packet to send via can to the HMI

void prepareCANHMI1Out () 
{

// Resets HMI 1 CAN Packet
	HMISend1.resetData();

// Send Speed (byte 0,1)
//  split the Speed in high and low byte 
   uint16_t auxS = vehicleInfo.speedKmh;
   HMISend1.writeByte(HMISend1_byteSpeedHighByte, (auxS >> 8));
   HMISend1.writeByte(HMISend1_byteSpeedLowByte, (auxS & 0xFF)); 
    

// Send Voltage (byte 2,3)
// split the Voltage in high and low byte 
// The Value of the voltage has been multiplied by 10
   uint16_t auxV = MCUData.voltage*10; 
   HMISend1.writeByte(HMISend1_byteVoltageHighByte, (auxV >> 8));
   HMISend1.writeByte(HMISend1_byteVoltageLowByte, (auxV & 0xFF));

// Send Current (byte 4,5)
// split the Current in high and low byte (added offset of 2000)
   int32_t auxC = BMSData.current+2000;
   HMISend1.writeByte(HMISend1_byteCurrentHighByte, (auxC >> 8));
   HMISend1.writeByte(HMISend1_byteCurrentLowByte,  (auxC & 0xFF));
   

// Send Speed Unit (byte 6)
// If the speed unit is KM, shows the screen with KM
 if (vehicleParam.speedUnit == 0)
   HMISend1.writeByte(HMISend1_byteSpeed_UnitSettings,0);
// If the speed unit is miles, shows the screen with miles
 else if (vehicleParam.speedUnit == 1)
   HMISend1.writeByte(HMISend1_byteSpeed_UnitSettings,1);                  

} 

// Prepare the 2 CAN packet to send via can to the HMI

void prepareCANHMI2Out () 
{
// Resets HMI 2 CAN Packet
	HMISend2.resetData();

// Send BMS Average temperature  (byte 0 )  
// Define values below 100 like negatives values of temperature and above 100
// like positives values (100 = 0 degree)
int16_t auxT = BMSData.avgBatteryTemperature + 100; 

    HMISend2.writeByte(HMISend2_byteBMS_avg_temp, auxT);
    
// Send Motor temperature (byte 1)
    HMISend2.writeByte(HMISend2_byteMoto_temp, MCUData.motorTemperature);


// Send Controller temperature (byte 2)
    HMISend2.writeByte(HMISend2_byteController_temp, MCUData.controllerTemperature);

// Send Trip_meter (byte 3)
// Calculated in the HMI side
    HMISend2.writeByte(HMISend2_byteTrip_meter, vehicleInfo.tripKm);   

// Send SOC (byte 4)
   HMISend2.writeByte(HMISend2_byteSOC, BMSData.SOC);

// Send Remaining Range  (byte 5,6)  
// to be implemented


}


// Prepare the 3 CAN packet to send via can to the HMI

void prepareCANHMI3Out () 
{

   HMISend3.resetData();

   uint8_t aux_icon_lowByte = 0;

 // Send Icon Signal Low byte (byte 0)

 if (inputs.headlightHighBeam==true)
 {
   bitWrite(aux_icon_lowByte, 0, 1);  
 }
 else
 {
   bitWrite(aux_icon_lowByte, 0, 0);
 }

 if (inputs.turnLeftSwitch==true)
 {
   bitWrite(aux_icon_lowByte, 1, 1);
 }
 else
 {
   bitWrite(aux_icon_lowByte, 1, 0);
 }

 if (inputs.turnRightSwitch==true)
 {
   bitWrite(aux_icon_lowByte, 2, 1);
 }
 else
 {
   bitWrite(aux_icon_lowByte, 2, 0);
 }

 if (vehicleInfo.isInReverseMode==true)
 {
   bitWrite(aux_icon_lowByte, 3, 1);
 }
 else
 {
   bitWrite(aux_icon_lowByte, 3, 0);
 }

 if (vehicleInfo.Neutral==true)
 {
   bitWrite(aux_icon_lowByte, 4, 1);
 }
 else
 {
   bitWrite(aux_icon_lowByte, 4, 0);
 }

 if (inputs.killswitch==true)    
 {
   bitWrite(aux_icon_lowByte, 5, 1);
 }
 else
 {
   bitWrite(aux_icon_lowByte, 5, 0);
 }

 if (inputs.kickstand==true)   
 {
   bitWrite(aux_icon_lowByte, 6, 1);
 }
 else
 {
   bitWrite(aux_icon_lowByte, 6, 0);
 }

 if (vehicleInfo.WinterMode==true)
 {
   bitWrite(aux_icon_lowByte, 7, 1);
 }
 else
 {
   bitWrite(aux_icon_lowByte, 7, 0);
 }

 HMISend3.writeByte(HMISend3_byteIconSignalLowByte, aux_icon_lowByte);

 // Send Icon Signal Mid byte (byte 1)

 uint8_t aux_icon_MidByte = 0;

 if (vehicleInfo.Positionlight==true)
 {
   bitWrite(aux_icon_MidByte, 0, 1);
 }
 else
 {
   bitWrite(aux_icon_MidByte, 0, 0);
 }

 // High Temp Warning  (bit 1) 
 // Trigger by three possible conditions: Motor temp Limit (from MCU), Controller temp Limit (from MCU), 
 // BMS Avg Temperature > 60
 if (bitRead(MCUData.lowByteError, 4)==1 || bitRead(MCUData.lowByteError, 6)==1 || BMSData.avgBatteryTemperature > 60 || tempIcon)
 {
 bitWrite(aux_icon_MidByte, 1, 1);
 }
 else
 {
 bitWrite(aux_icon_MidByte, 1, 0);
 }	

// OBD Icon
 if (vehicleInfo.errorsDetected==true)
 {
   bitWrite(aux_icon_MidByte, 7, 1);
 }
 else
 {
   bitWrite(aux_icon_MidByte, 7, 0);
 }
	
 HMISend3.writeByte(HMISend3_byteIconSignalMidByte, aux_icon_MidByte);

 // Send Icon Signal High byte (byte 2)

 uint8_t aux_icon_HighByte = 0;


 if (vehicleInfo.isChargerConnected== true) 
 {
   bitWrite(aux_icon_HighByte, 0, 1);
 }
 else
 {
   bitWrite(aux_icon_HighByte, 0, 0);
 }

 if (vehicleInfo.isFastChargeOn== true) 
 {
   bitWrite(aux_icon_HighByte, 2, 1);
 }
 else
 {
   bitWrite(aux_icon_HighByte, 2, 0);
 }

 HMISend3.writeByte(HMISend3_byteIconSignalHighByte, aux_icon_HighByte);

 // Send Riders Mode (byte 3)

 // 0 = Eco, 1 = City, 2 = Pro. 5= NO TEXT. Starts in City
 
 switch (vehicleInfo.ridingMode) {
 case 0:   
   HMISend3.writeByte(HMISend3_byteRidersMode, 0);
   break;
 case 1:   
   HMISend3.writeByte(HMISend3_byteRidersMode, 1);
   break;
 case 2:
 HMISend3.writeByte(HMISend3_byteRidersMode, 2);
  break;

 }
 
 // Send Odometer  (byte 4,5,6)
 }

 // Prepare the 4 CAN packet to send via can to the HMI

 void prepareCANHMI4Out () 
 {
  // Errors frame
  HMISend4.resetData();
 

    // Auxiliary variable used to send errors by CAN
    static uint16_t auxScan = 0;
    uint16_t auxSendError[4] = {0};
    uint16_t indexError = 0;
    
    if(vehicleInfo.errorsDetected) {
       
        while (auxScan < 1000)
        {
            if (errorArray.get(auxScan))
            {
                auxSendError[indexError]=auxScan;
                Serial.printf("auxScan = %d\n",auxScan);
                indexError++;
                auxScan++;
                // break;
                continue;
            }
 
            auxScan++;
        }

    } 
  if(auxScan == 1000)
    auxScan = 0;
   uint16_t auxE1 = auxSendError[0];
   auxE1 = HexToDex(auxE1);
   HMISend4.writeByte(HMISend4_byteError1HighByte, (auxE1 & 0xFF));
   HMISend4.writeByte(HMISend4_byteError1LowByte, (auxE1 >> 8));

   uint16_t auxE2 = auxSendError[1]; 
   auxE2 = HexToDex(auxE2);
   HMISend4.writeByte(HMISend4_byteError2HighByte, (auxE2 & 0xFF));
   HMISend4.writeByte(HMISend4_byteError2LowByte, (auxE2 >> 8));

   uint16_t auxE3 = auxSendError[2]; 
   auxE3 = HexToDex(auxE3);
   HMISend4.writeByte(HMISend4_byteError3HighByte, (auxE3 & 0xFF));
   HMISend4.writeByte(HMISend4_byteError3LowByte, (auxE3 >> 8));

   uint16_t auxE4 = auxSendError[3]; 
   auxE4 = HexToDex(auxE4);
   HMISend4.writeByte(HMISend4_byteError4HighByte, (auxE4 & 0xFF));
   HMISend4.writeByte(HMISend4_byteError4LowByte, (auxE4 >> 8));
  
 }

//
 uint16_t HexToDex(uint16_t auxE)
 {
    uint8_t remainder;
    uint16_t DexNum = 0;
    int i = 0, m = 0;
    while(auxE > 9)
    {
      remainder = auxE % 10;
      auxE /= 10;
      m = i;
      while(m--)
      remainder *= 16;
      DexNum += remainder;
      i++;
    }
    while(i--)
     auxE *= 16;
    DexNum += auxE;

    return DexNum;
 }