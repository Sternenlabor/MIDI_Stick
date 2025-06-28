/**
 * @file SerialDebug.cpp
 * @brief Instantiates and sets up the global SoftwareSerial instance for debug logging.
 *
 * This file defines the global SoftwareSerial object used for debug logging and provides a
 * helper function to initialize the debug serial port.
 */

#include "SerialDebug.h"

// Instantiate the global SoftwareSerial object for debug logging.
// The constructor parameters are defined in Config.h.
SoftwareSerial SerialDebug(SERIAL_DEBUG_RX_PIN, SERIAL_DEBUG_TX_PIN);

/**
 * @brief Initializes the debug serial port.
 *
 * This function sets the proper pin modes for the debug serial RX and TX pins, and
 * initializes the SoftwareSerial port using the baud rate defined in the configuration.
 * Debug logging is enabled only if the preprocessor symbol DEBUG_SERIAL is defined.
 */
void setupSerialDebug()
{
#ifdef DEBUG_SERIAL
    // Set the RX pin as an input.
    pinMode(SERIAL_DEBUG_RX_PIN, INPUT);
    // Set the TX pin as an output.
    pinMode(SERIAL_DEBUG_TX_PIN, OUTPUT);

    // Begin the debug serial communication at the defined baud rate.
    SerialDebug.begin(SERIAL_DEBUG_BAUD_RATE);
#endif
}
