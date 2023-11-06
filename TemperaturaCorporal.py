#include <Adafruit_CircuitPlayground.h>

void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
}

void loop() {
  float temperatureC = CircuitPlayground.temperature();
  float temperatureF = temperatureC * 1.8 + 32;
  
  Serial.print("Temperature C: ");
  Serial.println(temperatureC);
  Serial.print("Temperature F: ");
  Serial.println(temperatureF);

  if (temperatureC > 38.0) {
    CircuitPlayground.setPixelColor(0xFF0000);  // LED rojo
  } else {
    CircuitPlayground.clearPixels();  // Apagar el LED
  }

  delay(1000);  // Actualiza cada segundo
}
