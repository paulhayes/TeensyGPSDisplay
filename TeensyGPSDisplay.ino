#include <elapsedMillis.h>
#include <LedControl.h>
#include <Adafruit_GPS.h>

#define GPSSerial Serial1

LedControl lc=LedControl(12,13,10,1);
elapsedMillis lastUpdate;
unsigned int updateInterval = 1000;
Adafruit_GPS GPS(&GPSSerial);

char numBuf[16];
int numBufLen = 0;
void setup() {

  lc.shutdown(0,false);

  /* Set the brightness to a medium values */
  lc.setIntensity(0,4);

  /* and clear the display */
  lc.clearDisplay(0);

  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);

  Serial.begin(9600);
}

void loop() {
  char c = GPS.read();
  if (false)
    if (c) Serial.print(c);

    
  if( GPS.newNMEAreceived() ){
    GPS.parse(GPS.lastNMEA());
  }
  if(lastUpdate>=updateInterval){
    lastUpdate = 0;
    if(GPS.fix){

      int t = millis() / 4000;
      sprintf(numBuf,"%8.4f   ",((t%2)==0) ? GPS.latitudeDegrees : GPS.longitudeDegrees);
      
      //sprintf(numBuf,"%4.2f %4.2f   ",GPS.latitude/100,GPS.longitude/100);
      Serial.println(numBuf);
      displayValue(0);
      
      
      //dtostrf(GPS.latitude,5, 3, numBuf);
      //displayValue(0);
      //dtostrf(GPS.longitude,5, 3, numBuf);
      //displayValue(4);
    } else {
      //sprintf(numBuf,"%5.1f%5.1f   ",50.8f,-0.1);
      //Serial.println(numBuf);
      sprintf(numBuf, "%02d-%02d-%02d         ", GPS.hour, GPS.minute, GPS.seconds);  
      displayValue(0);
      
      //displayValue(0);
      //Serial.println(numBuf);
      //dtostrf(-0.1f,5, 3, numBuf);
      //displayValue(0);
      //dtostrf(33.1,5, 3, numBuf);
      //displayValue(4);
    }
  }
}

void displayValue(int digitOffset){
  
    
    int digit = 0;
    for(int i=0;digit<8;i++){
      bool nextIsDot = ( (i<15) && numBuf[i+1]=='.');
      bool currentIsDot = (numBuf[i]=='.');
       
       lc.setChar( 0,7-digit+digitOffset, numBuf[i], nextIsDot );      
       if( !currentIsDot && nextIsDot  ){
        i++;
       } 
       digit++;
    }
  }
