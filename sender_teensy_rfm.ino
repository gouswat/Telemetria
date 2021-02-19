#include <Wire.h> 
#include <Adafruit_MLX90614.h>
#include <SPI.h>
#include <RH_RF95.h>

#define RF95_FREQ 433.0

#define RFM95_CS 15
#define RFM95_INT 16
#define RFM95_RST 9

uint8_t firstAddress = 0x5A;
uint8_t secondAddress = 0x50;
uint8_t thirdAddress = 0x55;

long firstTemp = 0;
long secondTemp = 0;
long thirdTemp = 0;
long singleStringTemps;
char finalTemp[1];

int sensorPin = A0;
int ledPin = 7;
int sensorValue = -1;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
RH_RF95 rf95(RFM95_CS, RFM95_INT);


void setup()
{

 
 pinMode(ledPin, OUTPUT);
 digitalWrite(ledPin, LOW);

 
 pinMode(RFM95_RST, OUTPUT);
 digitalWrite(RFM95_RST, HIGH);

 while (!Serial);
 Serial.begin(9600);
 delay(100);
 Serial.println("Arduino LoRa TX Test!");
 
 digitalWrite(RFM95_RST, LOW);
 delay(10);
 digitalWrite(RFM95_RST, HIGH);
 delay(10);
 while (!rf95.init()) {
 Serial.println("LoRa radio init failed");
 while (1);
 }
 Serial.println("LoRa radio init OK!");
 
 if (!rf95.setFrequency(RF95_FREQ)) {
 Serial.println("setFrequency failed");
 while (1);
 }
 Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
 mlx.begin();
}
  //int16_t packetnum = 0;

void loop() 
{
  mlx = Adafruit_MLX90614();
  firstTemp = mlx.readObjectTempC();


  mlx = Adafruit_MLX90614(secondAddress);
  secondTemp = mlx.readObjectTempC();


  mlx = Adafruit_MLX90614(thirdAddress);
  thirdTemp = mlx.readObjectTempC();

  singleStringTemps = (firstTemp + (secondTemp * 100) + (thirdTemp * 10000)); 
  //Serial.println(singleStringTemps);

  sprintf(finalTemp, "%li\0", singleStringTemps);
  //Serial.println(finalTemp);

  rf95.send((uint8_t*)finalTemp, strlen (finalTemp));
  rf95.waitPacketSent();
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
 uint8_t len = sizeof(buf);
   if (rf95.waitAvailableTimeout(100))
 {
 // Should be a reply message for us now
 if (rf95.recv(buf, &len))
 {
 //Serial.print("Got reply: ");
 //Serial.println((char*)buf);
 //Serial.print("RSSI: ");
 //Serial.println(rf95.lastRssi(), DEC);
 }
 else
 {
 //Serial.println("Receive failed");
 }
 }
 else
 {
 //serial.println("No reply, is there a listener around?");
 }
  delay(1000);
}



