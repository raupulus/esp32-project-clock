
// RGB leds
#include <Adafruit_NeoPixel.h>

// RTC DS1307 (RELOJ)
#include <Wire.h>

// RGB leds
#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 32
#define NUMPIXELS 12
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 5000
// Listado de colores (verde, azul, naranja, rojo)
int COLORS[4][3]{
    {0, 255, 0},
    {0, 0, 255},
    {255, 159, 35},
    {255, 0, 0}
};


// RTC DS1307 (RELOJ)
#define DS3231_I2C_ADDRESS 0x68

// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}

// Agrega timestamp al módulo RTC DS1307 (RELOJ)
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year){
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

// Lee el módulo RTC DS1307 (RELOJ)
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

// Muestr datos por consola del módulo RTC DS1307 (RELOJ)
void displayTime(){
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10){
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10){
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Día de la semana: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Domingo");
    break;
  case 2:
    Serial.println("Lunes");
    break;
  case 3:
    Serial.println("Martes");
    break;
  case 4:
    Serial.println("Miércoles");
    break;
  case 5:
    Serial.println("Jueves");
    break;
  case 6:
    Serial.println("Viernes");
    break;
  case 7:
    Serial.println("Sábado");
    break;
  }
}

void setup() {
  // Inicio i2c
  Wire.begin(21, 22);

  // Inicio Serial
  Serial.begin(9600);

  // RGB
  pixels.begin();

  // RTC DS1307 (RELOJ)
  // Inicia el reloj: DS1307 seconds, minutes, hours, day, date, month, year
  //setDS3231time(30,33,23,5,30,1,20);
}

void loop() {
  Serial.println("--- Comienza todo el loop ---");

  // RGB

  // Cada 5 segundos en ciende un led (12*5 = 1 minuto)
  // Cada bloque de 3 led cambiará de color (verde, azul, naranja, rojo)

  pixels.clear();

  // TODO → Colores según el segundo actual de la lectura del reloj
  for (int i = 0; i < NUMPIXELS; i++) {
    Serial.println("--- Iteración en el for ---");
    if (i < 3)
    {
      pixels.setPixelColor(i, pixels.Color(COLORS[0][0], COLORS[0][1], COLORS[0][2]));
    }
    else if (i < 6)
    {
      pixels.setPixelColor(i, pixels.Color(COLORS[1][0], COLORS[1][1], COLORS[1][2]));
    }
    else if (i < 9)
    {
      pixels.setPixelColor(i, pixels.Color(COLORS[2][0], COLORS[2][1], COLORS[2][2]));
    }
    else
    {
      pixels.setPixelColor(i, pixels.Color(COLORS[3][0], COLORS[3][1], COLORS[3][2]));
    }

    pixels.show();
    delay(DELAYVAL);

    // RTC
    displayTime();
  }
}

/**
   Funcion auxiliar para imprimir siempre 2 digitos
*/
void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}