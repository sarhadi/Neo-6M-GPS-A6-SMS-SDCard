/*   

  Getting data from GPS module
 
 This is a program that can used with Neo-6M-GPS module & Arduino Uno Board
 Neo-6M-GPS module uses the latest technology to give the best possible position information
 with 4 connections: RX, TX, VCC and GND

 Created 4 September. 2018
 by Ehsan Sarhadi

/*********************
 * 
 * GPS attached to SPI bus as follows:
 * GPS VCC to 3.3V
 * GND to GND
 * Pin 3 to GPS Module RX
 * Pin 4 to GPS Module TX
 * Default Baud Rate: 9600 bps 
 * 
 * SD card attached to SPI bus as follows:
 * SD Card VCC to 3.3V
 * MOSI - pin 11 on Arduino Uno/Duemilanove/Diecimila
 * MISO - pin 12 on Arduino Uno/Duemilanove/Diecimila
 * SCK - pin 13 on Arduino Uno/Duemilanove/Diecimila
 * CS - depends on your SD card shield or module. 
 * Pin 5 used here for consistency with other Arduino examples
 * 
 * GSM attached to SPI bus as follows:
 * Pin 6 (TX)       U_RXD
 * Pin 7 (RX)       U_TXD 
 * GPRS baud rate: 9600 bps 
 * 
 * Disconnect the GPS TX & RX from the Arduino during the Uploading
 *********************/

#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <SPI.h>
#include <SD.h>
const int chipSelect = 5;
char filename[15] = "data_##.log";
long startLoggingTime = 1;
char phone_no[]="9129378659";

SoftwareSerial SMS_PORT(7, 6); // TX-Pin7, RX-Pin6
SoftwareSerial mySerial(4, 3); // TX, RX

TinyGPS gps;

  // Deployment Position "Bandar Abbas"
  static const double release_LAT = 27.1141515, release_LON = 56.0614786; 

void setup()  
{

  delay(2000);
  // Open serial communications and wait for port to open:
  // set the data rate for the SoftwareSerial port
  
//  Serial.begin(9600);
//  delay(1000);
  
  SMS_PORT.begin(9600);
  delay(1000);
  
  mySerial.begin(9600);
  
  delay(1000);
//  while (!mySerial)
  
//  SMS_PORT.begin(9600);
//  while (!SMS_PORT)

//  Serial.println("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
 //   Serial.println("Card failed, or not present");

  } else {
//  Serial.println("card initialized.");
}
}

void loop() // run over and over

{
  
//  while(startLoggingTime < 604800)
//    {
  unsigned short sentences, failed;
  bool newData = false;

  // parse GPS data for 1 second and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {

    while (mySerial.available())
    {

      char c = mySerial.read();
      // Serial.write(c); // uncomment this line if you want to see the raw GPS data flowing
      //gps.encode(c); // Did a new valid sentence come in?
      if (gps.encode(c)) 
        newData = true;

  }
  }
    
  // Every 1 seconds print an update on SD card

    if (startLoggingTime == 1) {

    // Pick a numbered filename, 00 to 99.

    for (uint8_t i=0; i<100; i++) {
      filename[5] = '0' + i/10;
      filename[6] = '0' + i%10;
      if (!SD.exists(filename)) {
        // Use this one!
        break;
      }
    }

  File myFile = SD.open(filename, FILE_WRITE);

    if (myFile) {

    myFile.println( F("Time(UTC): Latitude(°N): Longitude(°E):  Alt(m): Speed(m/s): Satellites: HDOP: Errors: Deployment(km): \n" ) );
//    Serial.pintln( F("Time(UTC): Latitude(°N): Longitude(°E):  Alt(m): Speed(m/s): Satellites: HDOP: Errors: Deployment(km): \n" ) );

    } else {
    // if the file didn't open, print an error:
//    Serial.pintln("error opening data.log");
  }

  myFile.close();

    }

    File myFile = SD.open(filename, FILE_WRITE);

      //    Serial.print( F("Date & Time(UTC EPOCH):") );

    float flat, flon;
    unsigned long age, date, time, chars;
    int year;
    byte month, day, hour, minute, second, hundredths;
    unsigned long epoch;
    gps.f_get_position(&flat, &flon, &age);
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);

    startLoggingTime = startLoggingTime + 1 ;

    if (newData)
  {

    myFile.print(static_cast<int>(day)); myFile.print("/"); 
    myFile.print(static_cast<int>(month)); myFile.print("/"); 
    myFile.print(year); myFile.print( ' ' );
    myFile.print(static_cast<int>(hour));  myFile.print(":"); 
    myFile.print(static_cast<int>(minute)); myFile.print(":"); 
    myFile.print(static_cast<int>(second));  myFile.print("."); 
    myFile.print(static_cast<int>(hundredths)); myFile.print( ' ' );

//    Serial.pint(static_cast<int>(day)); Serial.print("/"); 
//    Serial.pint(static_cast<int>(month)); Serial.print("/"); 
//    Serial.pint(year); Serial.print( ' ' );
//    Serial.pint(static_cast<int>(hour));  Serial.print(":"); 
//    Serial.pint(static_cast<int>(minute)); Serial.print(":"); 
//    Serial.pint(static_cast<int>(second));  Serial.print("."); 
//    Serial.pint(static_cast<int>(hundredths)); Serial.print( ' ' );

      myFile.print( ' ' );
  //    Serial.pint( ' ' );  

//    Serial.print( F("Latitude: ") );


      myFile.print( flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6 );
  //    Serial.pint( flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6 );
      
      myFile.print( ' ' );
  //    Serial.pint( ' ' );
      
//    Serial.print( F("Longitude: ") );
      myFile.print( flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6 );
  //    Serial.pint( flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6 );
 
      myFile.print( ' ' );
  //    Serial.pint( ' ' );
      myFile.print( ' ' );
  //    Serial.pint( ' ' );  

//    Serial.print( F("Altitude(m) : ") );

      myFile.print( gps.f_altitude() == TinyGPS::GPS_INVALID_F_ALTITUDE? 0 : gps.f_altitude() );
  //    Serial.pint( gps.f_altitude() == TinyGPS::GPS_INVALID_F_ALTITUDE? 0 : gps.f_altitude() );
    
      myFile.print( ' ' );
  //    Serial.pint( ' ' );
      myFile.print( ' ' );
  //    Serial.pint( ' ' );  

//    Serial.print( F("Speed (mps): ") );

      myFile.print( gps.f_speed_mps() == TinyGPS::GPS_INVALID_F_SPEED? 0 : gps.f_speed_mps() );
  //    Serial.pint( gps.f_speed_mps() == TinyGPS::GPS_INVALID_F_SPEED? 0 : gps.f_speed_mps() );

      myFile.print( ' ' );
  //    Serial.pint( ' ' );
      myFile.print( ' ' );
  //    Serial.pint( ' ' );  

//    Serial.print( F("Satellites: ") );

      myFile.print( gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites() );
  //    Serial.pint( gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites() );
    
      myFile.print( ' ' );
  //    Serial.pint( ' ' );
      myFile.print( ' ' );
  //    Serial.pint( ' ' );  

//    Serial.print( F("HDOP: ") );

      myFile.print( gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop() );
  //    Serial.pint( gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop() );

      myFile.print( ' ' );
  //    Serial.pint( ' ' );
      myFile.print( ' ' );
  //    Serial.pint( ' ' );  

//    Serial.print( F("Errors: ") );

      gps.stats(&chars, &sentences, &failed);

      myFile.print( failed );
  //    Serial.pint( failed );
      
      myFile.print( ' ' );
  //    Serial.pint( ' ' );
      myFile.print( ' ' );
  //    Serial.pint( ' ' );  

      //    Serial.print( F("Deployment-GPS Distance(km): ") );

      myFile.print( TinyGPS::distance_between(flat, flon, release_LAT, release_LON) / 1000 );
  //    Serial.pint( TinyGPS::distance_between(flat, flon, release_LAT, release_LON) / 1000 );

      myFile.print( ' ' );
  //    Serial.pint( ' ' );
      myFile.print( '@' );
  //    Serial.pint( '@' );
      myFile.print( ' ' );
  //    Serial.pint( ' ' );

      myFile.print( TinyGPS::cardinal (TinyGPS::course_to(release_LAT, release_LON, flat, flon)) );
  //    Serial.pint( TinyGPS::cardinal (TinyGPS::course_to(release_LAT, release_LON, flat, flon)) );

      myFile.println();
  //    Serial.pintln();

  }

      myFile.flush();

      myFile.close();

        // Every 30 Min send an update through SMS

      if ( (startLoggingTime % 1800) == 0 || startLoggingTime == 60 || startLoggingTime == 120 || startLoggingTime == 300 ) {

 //     SoftwareSerial Serial(7, 6); // TX-Pin7, RX-Pin6

  //    Serial.pintln("Send Data through SMS");

      SMS_PORT.println("AT+CMGF=1");  
      delay(2000);  
      SMS_PORT.print("AT+CMGS=\"");
      SMS_PORT.print(phone_no); 
      SMS_PORT.write(0x22);
      SMS_PORT.write(0x0D);  // hex equivalent of Carraige return    
      SMS_PORT.write(0x0A);  // hex equivalent of newline
      delay(2000);
      SMS_PORT.println("Time (UTC):");
      SMS_PORT.print(static_cast<int>(hour));  SMS_PORT.print(':'); 
      SMS_PORT.print(static_cast<int>(minute)); SMS_PORT.print(':'); 
      SMS_PORT.print(static_cast<int>(second));  SMS_PORT.println(' '); 
 
      SMS_PORT.println("Lat & Long (°):");
      SMS_PORT.print( flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6 );
      SMS_PORT.print(' '); 
      SMS_PORT.print( flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6 );
      SMS_PORT.println(' '); 
      
      SMS_PORT.println("Distance (km):");
      SMS_PORT.print( TinyGPS::distance_between(flat, flon, release_LAT, release_LON) / 1000 );
      SMS_PORT.print(' '); 
      SMS_PORT.print('@'); 
      SMS_PORT.print(' '); 
      SMS_PORT.println( TinyGPS::cardinal (TinyGPS::course_to(release_LAT, release_LON, flat, flon)) );

      SMS_PORT.println("Satellites (#):");
      SMS_PORT.print( gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites() );
      SMS_PORT.println(' '); 

      if (!SD.begin(chipSelect))
      SMS_PORT.println("No SD Card");
      if (!mySerial)
      SMS_PORT.println("No GPS Signal");

      delay(2000);
      
      SMS_PORT.println (char(26));//the ASCII code of the ctrl+z is 26

//      SMS_PORT.end();

//      // Switch off mobile station
//      SMS_PORT.println("AT+CPOF");

    }

//  }

}
