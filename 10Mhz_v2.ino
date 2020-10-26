/*
    Ref 10Mhz Version 2

     This project is a 10Mhz Generateur Frequency base on GPS Signal
     it show the numbers of Satelites LOCKED and the status of the Signal 10Mhz

     BarGarph show the number of satelittes ( 1 to 10 )
     BlueLED show the Status Signal
        Blink - Long +  Short ( N in MORSE Code )  -> GPS OK
        Blink - Short -> NO GPS

    GPS use the TP5 Mode ; the GPS can't save the parameters in memory , so we have to send it each soft reset.
    See the UBLOX_INIT variable
    i use U-center for testing GPS
    Link to ALI GPS : 
      https://fr.aliexpress.com/item/1765564932.html?spm=a2g0o.productlist.0.0.6e94679cZWlbDe&algo_pvid=e9d4234c-de1a-4d32-8f51-4e942e9d9913&algo_expid=e9d4234c-de1a-4d32-8f51-4e942e9d9913-16&btsid=2100bdca16036931553453127e7419&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_
    The GPS is modify to use the LED Signal for 10Mhz ( need to take off the led and take the signal )




    ARDUINO MICRO PRO
    PINOUT :  8-TX
              9-RX
              6-  BARGraph 9
              7-  BARGraph 10
              10- BARGraph 1 
              16- BARGraph 2
              14- BARGraph 3
              15- BARGraph 4
              18- BARGraph 5
              19- BARGraph 6
              20- BARGraph 7
              21- BARGraph 8
              5-  SATLED

    
    version  by SHM (see variable )
    date : 2020-10-26
*/


// ===============================================================================================================================================
//                                                                   INCLUDE
// ===============================================================================================================================================
#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial Gps_Serial(8, 9);                  // define the serial port for GPS ( baud : 9600 )


// ===============================================================================================================================================
//                                                                   DEFINE
// ===============================================================================================================================================

#define Version  "1.0a"
#define SATLED  5
#define LED_BAR0  10
#define LED_BAR1  16
#define LED_BAR2  14
#define LED_BAR3  15
#define LED_BAR4  18
#define LED_BAR5  19
#define LED_BAR6  20
#define LED_BAR7  21
#define LED_BAR8  7
#define LED_BAR9  6
#define PAUSE     300

// ===============================================================================================================================================
//                                                                   VARIABLE
// ===============================================================================================================================================

int sats = 0 ;
const char UBLOX_INIT[] PROGMEM = { 
 
  0xB5, 0x62, 0x06, 0x31, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x80, 0x96, 0x98, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x6F, 0x00, 0x00, 0x00, 0xA8, 0x08,

};


// ===============================================================================================================================================
//                                                                   Self_Test
// ===============================================================================================================================================
void Self_Test(int delay_ms)                          // increase/decrease LED like K2000 
{
  digitalWrite(LED_BAR0, HIGH);
  delay(delay_ms);
  digitalWrite(LED_BAR1, HIGH);
  delay(delay_ms);
  digitalWrite(LED_BAR2, HIGH);
  delay(delay_ms);
  digitalWrite(LED_BAR3, HIGH);
  delay(delay_ms);
  digitalWrite(LED_BAR4, HIGH);
  delay(delay_ms);
  digitalWrite(LED_BAR5, HIGH);
  delay(delay_ms);
  digitalWrite(LED_BAR6, HIGH);
  delay(delay_ms);
  digitalWrite(LED_BAR7, HIGH);
  delay(delay_ms);
  digitalWrite(LED_BAR8, HIGH);
  delay(delay_ms);
  digitalWrite(LED_BAR9, HIGH);

  delay(delay_ms*4);
  digitalWrite(LED_BAR9, LOW);
  delay(delay_ms);
  digitalWrite(LED_BAR8, LOW);
  delay(delay_ms);
  digitalWrite(LED_BAR7, LOW);
  delay(delay_ms);
  digitalWrite(LED_BAR6, LOW);
  delay(delay_ms);
  digitalWrite(LED_BAR5, LOW);
  delay(delay_ms);
  digitalWrite(LED_BAR4, LOW);
  delay(delay_ms);
  digitalWrite(LED_BAR3, LOW);
  delay(delay_ms);
  digitalWrite(LED_BAR2, LOW);
  delay(delay_ms);
  digitalWrite(LED_BAR1, LOW);
  delay(delay_ms);
  digitalWrite(LED_BAR0, LOW);
  delay(delay_ms*2);
}

// ===============================================================================================================================================
//                                                                   Print_SatLED
// ===============================================================================================================================================
void Print_SatLED(int Number_SAT)                       // Print the bargraph with the number of Satelitte
{
  if (Number_SAT>0)  {digitalWrite(LED_BAR0, HIGH);} else {digitalWrite(LED_BAR0, LOW);}
  if (Number_SAT>1)  {digitalWrite(LED_BAR1, HIGH);} else {digitalWrite(LED_BAR1, LOW);}
  if (Number_SAT>2)  {digitalWrite(LED_BAR2, HIGH);} else {digitalWrite(LED_BAR2, LOW);}
  if (Number_SAT>3)  {digitalWrite(LED_BAR3, HIGH);} else {digitalWrite(LED_BAR3, LOW);}
  if (Number_SAT>4)  {digitalWrite(LED_BAR4, HIGH);} else {digitalWrite(LED_BAR4, LOW);}
  if (Number_SAT>5)  {digitalWrite(LED_BAR5, HIGH);} else {digitalWrite(LED_BAR5, LOW);}
  if (Number_SAT>6)  {digitalWrite(LED_BAR6, HIGH);} else {digitalWrite(LED_BAR6, LOW);}
  if (Number_SAT>7)  {digitalWrite(LED_BAR7, HIGH);} else {digitalWrite(LED_BAR7, LOW);}
  if (Number_SAT>8)  {digitalWrite(LED_BAR8, HIGH);} else {digitalWrite(LED_BAR8, LOW);}
  if (Number_SAT>9)  {digitalWrite(LED_BAR9, HIGH);} else {digitalWrite(LED_BAR9, LOW);}
}


// ===============================================================================================================================================
//                                                                   Blink
// ===============================================================================================================================================
void Blink(int delay_ms)                              // Blink the Blue LED
{
  digitalWrite(SATLED, HIGH);
  delay(delay_ms);
  digitalWrite(SATLED, LOW);
  delay(delay_ms);
  Serial.println("Blink");
}

// ===============================================================================================================================================
//                                                                   SETUP
// ===============================================================================================================================================
void setup()
{ 
  Serial.begin(115200);                                // Start the Serial port of the Arduino
  Serial.println();
  Serial.println("-------------- START --------------------");
  Serial.print(" Version "); Serial.print(Version); Serial.println(" - Date : 2020-10-26 by SHM  ");
  Serial.println();
  Gps_Serial.begin(9600);
  pinMode(SATLED, OUTPUT);                             // define the PORT and Fix it to LOW
  pinMode(LED_BAR0, OUTPUT); 
  digitalWrite(LED_BAR0, LOW);
  pinMode(LED_BAR1, OUTPUT); 
  digitalWrite(LED_BAR1, LOW);
  pinMode(LED_BAR2, OUTPUT); 
  digitalWrite(LED_BAR2, LOW);
  pinMode(LED_BAR3, OUTPUT); 
  digitalWrite(LED_BAR3, LOW);
  pinMode(LED_BAR4, OUTPUT); 
  digitalWrite(LED_BAR4, LOW);
  pinMode(LED_BAR5, OUTPUT); 
  digitalWrite(LED_BAR5, LOW);
  pinMode(LED_BAR6, OUTPUT); 
  digitalWrite(LED_BAR6, LOW);
  pinMode(LED_BAR7, OUTPUT); 
  digitalWrite(LED_BAR7, LOW);
  pinMode(LED_BAR8, OUTPUT); 
  digitalWrite(LED_BAR8, LOW);
  pinMode(LED_BAR9, OUTPUT); 
  digitalWrite(LED_BAR9, LOW);


  // test Unit LED
  digitalWrite(SATLED, HIGH);                         // BlueLED ON
  delay(2000);
  Self_Test(PAUSE);                                   // Self Test
  Self_Test(PAUSE/2);                                 // Selft Test *2 ( more speed )  
  Self_Test(PAUSE/4);                                 // Self Test  *4 ( see K2000 )
  Blink(5000);                                        // Stop BLEU LED - End of test
  Serial.println("Init GPS for 10Mhz");               // Init the GPS
  Serial.println("Send Trame to GPS");
  for(int i = 0; i < sizeof(UBLOX_INIT); i++) {       // see the vairiable and U-center ( 1Hz is GPS not LOCKED - 10Mhz if LOCKED )                
     Gps_Serial.write( pgm_read_byte(UBLOX_INIT+i) );
      delay(25); 
  }
}


// ===============================================================================================================================================
//                                                                   LOOP
// ===============================================================================================================================================
void loop()
{
  int delay_SATLED=100;                               // use for the delay of BLUELED
  bool newData = false;                               // use to check if new data arrive
  unsigned long chars;                                // use for GPS.stat
  unsigned short sentences, failed;                   // use for GPS.stat
  Blink(delay_SATLED);                                // blink short
  for (unsigned long start = millis(); millis() - start < 1000;)      // check every second
  {
    while (Gps_Serial.available())                                    // if data available
    {
      char c = Gps_Serial.read();                                     // read the data
      if (gps.encode(c)){ newData = true;}                            // decrypt the data
    }
  }
  if (newData)
  {
    float flat, flon;                                 // Latittude and Longitude  ; just use for debug
    unsigned long age;
    Serial.print(".");
    delay_SATLED=250;                                 // fix long Blink is NEW data
    gps.f_get_position(&flat, &flon, &age);           // get position ; just for debug
    Serial.print("LAT=");                             // print position and satelitte numbers
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.println();
    sats = gps.satellites();                          // number of satellites
    Print_SatLED(sats);                               // print satelittes on BarGraph
    Blink(delay_SATLED);                              // Blink ( long of course )
  } else {delay_SATLED=100;}                          // fix short Blink if not
 
  gps.stats(&chars, &sentences, &failed);               
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
}
