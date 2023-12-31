#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <Adafruit_NeoPixel.h>

MAX30105 particleSensor;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, 2, NEO_GRB + NEO_KHZ800);

const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;
long lastPrintTime = 0;  // Variable para realizar un seguimiento del último momento en que se imprimieron los datos
float beatsPerMinute;
int beatAvg;

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");
  strip.begin();
  strip.show();


  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);
}

void loop()
{


  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;

      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  // Imprimir datos cada medio minuto
  if (millis() - lastPrintTime >= 15000)
  {
    if (irValue < 50000)
      Serial.print(" No finger?");
    if (beatsPerMinute > 120)
    {
      Serial.println("Warning: High heart rate! Consult a doctor. Your values are: ");
      Serial.print("IR=");
      Serial.print(irValue);
      Serial.print(", BPM=");
      Serial.print(beatsPerMinute);
      Serial.print(", Avg BPM=");
      Serial.print(beatAvg);
      strip.setPixelColor(0, strip.Color(255, 0, 0));
      strip.show();
    }
    else if (beatsPerMinute >= 30 && beatsPerMinute <= 120)
    {
      Serial.println("Heart rate is stable. Your values are: ");
      Serial.print("IR=");
      Serial.print(irValue);
      Serial.print(", BPM=");
      Serial.print(beatsPerMinute);
      Serial.print(", Avg BPM=");
      Serial.print(beatAvg);
      strip.setPixelColor(0, strip.Color(0, 255, 0));
      strip.show();
    }
    else
    {
      Serial.println("Warning: Low heart rate! Consult a doctor. Your values are: ");
      Serial.print("IR=");
      Serial.print(irValue);
      Serial.print(", BPM=");
      Serial.print(beatsPerMinute);
      Serial.print(", Avg BPM=");
      Serial.print(beatAvg);
      strip.setPixelColor(0, strip.Color(255, 255, 0));  // Establece el color a amarillo
      strip.show();

    }

    Serial.println();

    // Actualizar el tiempo de impresión
    lastPrintTime = millis();
  }
}
