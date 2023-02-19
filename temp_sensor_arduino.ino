/*
A small Temperature sensing program using Round
Robin Interrupt schema and console output.

Credit for how to utilize Timer1 of Atmel chip
for time based interrupts goes to:
https://microcontrollerslab.com/arduino-timer-interrupts-tutorial/

Conversion formulae for TMP36 retrieved from datasheet:
https://www.analog.com/media/en/technical-documentation/data-sheets/tmp35_36_37.pdf

Author: Niko Tubach
Class: EN.605.715.81
*/

#include <avr/interrupt.h>

// Pin for sensor analog output
const uint16_t ktemp_out_pin = 0;
// Voltage (or reference) supplied to sensor for readout conversion
const float aref_voltage = 3.3;
// Storage for temperature reading
uint16_t reading = 0;
// Storage for time
uint32_t time = 0;

void setup()
{
  Serial.begin(9600);

  // Set up a timer based interrupt per link above
  // from microcontrollerslab
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  // 16 MHz clock / 256 prescaler (below) = 16us
  // 1s / 16us = 62500 = 0xF424
  OCR1A = 0xF424;
  TCCR1B = (1 << WGM12) | (1 << CS12);
  TIMSK1 = (1 << OCIE1A);
  sei();

  // Utilize external reference (3.3V) for
  // further stability in readout
  analogReference(EXTERNAL);

  // Call a temp sensor read to prime the sensor data
  readTempSenor();

  // Program welcome
  // Serial.println("Module 2: Temperature reader");
  // Setup headers for data
  Serial.println("Time (s),Temperature (F)");
}

void loop()
{
  // Prevent interrupts while we print out
  noInterrupts();

  // Convert raw value read to voltage using
  // supplied reference voltage over raw range
  float voltage = reading * (aref_voltage / 1024);

  // Convert from 10 mV per degree with 500 mV offset
  // to celsius ((voltage - 500mV) times 100) per
  // TMP36 datasheet
  float tempC = (voltage - 0.5) * 100;

  // Convert our Celsius value to Fahrenheit
  float tempF = (tempC * 9.0 / 5.0) + 32.0;

  // Check time for output
  time = millis() / 1000;

  // Print out current time , tempF reading
  Serial.print(time);
  Serial.print(" , ");
  Serial.println(tempF);

  // Re-enable Interrupts
  interrupts();

  // Delay 10 seconds for next read
  delay(10000);
}

void readTempSenor()
{
  // Getting the voltage reading from the temperature sensor
  reading = analogRead(ktemp_out_pin);
}

// Built-in ISR from Atmel chipset
// Addresses Timer1 interrupts
ISR(TIMER1_COMPA_vect)
{
  // Call our reader function to
  // bring in the new temperature
  readTempSenor();
}