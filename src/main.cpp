

#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif
#define PIN 32
#define NUMPIXELS 12

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 5000

// Listado de colores (verde, azul, naranja, rojo)
int COLORS [4][3] {
    {0, 255, 0}, 
    {0, 0, 255}, 
    {255, 159, 35},
    {255, 0, 0}
  };

  void setup()
  {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif

    pixels.begin();
  }

  void loop()
  {

    // Cada 5 segundos en ciende un led (12*5 = 1 minuto)
    // Cada bloque de 3 led cambiará de color (verde, azul, naranja, rojo)

    pixels.clear();

    for (int i = 0; i < NUMPIXELS; i++)
    {
      if (i < 3) {
        pixels.setPixelColor(i, pixels.Color(COLORS[0][0], COLORS[0][1], COLORS[0][2]));
      } else if (i < 6) {
        pixels.setPixelColor(i, pixels.Color(COLORS[1][0], COLORS[1][1], COLORS[1][2]));
      } else if (i < 9) {
        pixels.setPixelColor(i, pixels.Color(COLORS[2][0], COLORS[2][1], COLORS[2][2]));
      } else {
        pixels.setPixelColor(i, pixels.Color(COLORS[3][0], COLORS[3][1], COLORS[3][2]));
      }

      pixels.show();
      delay(DELAYVAL);
    }
  }