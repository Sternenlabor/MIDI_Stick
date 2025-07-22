#include <Arduino.h>
#include <Adafruit_TinyUSB.h> // For USB MIDI communication
#include <Config.h>
#include <SerialDebug.h>
#include <MIDIHandler.h>

void ledInit()
{
   pinMode(LED_TRANSMITTER_PIN, OUTPUT);
   digitalWrite(LED_TRANSMITTER_PIN, LOW); // LOW = LED off

   pinMode(LED_POWER_PIN, OUTPUT);
   digitalWrite(LED_POWER_PIN, HIGH); // HIGH = LED off

   pinMode(LED_COM_PIN, OUTPUT);
   digitalWrite(LED_COM_PIN, HIGH); // HIGH = LED off

   pinMode(LED_SIGNAL_PIN, OUTPUT);
   digitalWrite(LED_SIGNAL_PIN, HIGH); // HIGH = LED off
}

/**
 * First Core
 */

void setup()
{
   // Manual begin() for boards that do not have built-in USB support
   if (!TinyUSBDevice.isInitialized())
   {
      TinyUSBDevice.begin(0);
   }

   // clear configuration will remove all USB interfaces including CDC (Serial)
   // This is needed to get USB MIDI running on Windows. On macOS both will work at the same time.
   TinyUSBDevice.clearConfiguration();

   // Set the USB MIDI device string descriptor.
   USBDevice.setManufacturerDescriptor("Sternenlabor");
   USBDevice.setProductDescriptor("MIDI Stick");

   // If already enumerated, additional class driver begin() e.g msc, hid, midi won't take effect until re-enumeration
   if (TinyUSBDevice.mounted())
   {
      TinyUSBDevice.detach();
      delay(10);
      TinyUSBDevice.attach();
   }

   setupSerialDebug();

   DEBUG_LOG_LN("Device starting...");

   ledInit();

   MIDIHandler::getInstance()->begin();
}

void loop()
{
   MIDIHandler::getInstance()->read();
}

/**
 * Second Core
 */

void setup1()
{
}

uint8_t loopCounter = 0;

void loop1()
{
   //DEBUG_LOG("Loop count: ");
   //DEBUG_LOG_LN(loopCounter++);

   // Blink all LEDs

   digitalWrite(LED_POWER_PIN, LOW); // LOW = LED on

   delay(200);

   digitalWrite(LED_POWER_PIN, HIGH); // HIGH = LED off
   digitalWrite(LED_COM_PIN, LOW);    // LOW = LED on

   delay(200);

   digitalWrite(LED_COM_PIN, HIGH);   // HIGH = LED off
   digitalWrite(LED_SIGNAL_PIN, LOW); // LOW = LED on

   delay(200);

   digitalWrite(LED_SIGNAL_PIN, HIGH); // HIGH = LED off

   // Test the Transmitter LED
   digitalWrite(LED_TRANSMITTER_PIN, HIGH); // LED on
   delay(10);
   digitalWrite(LED_TRANSMITTER_PIN, LOW); // LED off

   delay(190);
}
