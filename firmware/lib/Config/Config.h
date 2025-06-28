#ifndef CONFIG_H
#define CONFIG_H

/* ============================================================================
 *                          DEBUG OPTIONS
 * ==========================================================================*/
// Enable serial debugging (uncomment to enable)
#define DEBUG_SERIAL

/* ============================================================================
 *                    SERIAL PORT CONFIGURATION
 * ==========================================================================*/
// Serial Debug Port (for general debugging)
#define SERIAL_DEBUG_RX_PIN 0
#define SERIAL_DEBUG_TX_PIN 1
#define SERIAL_DEBUG_BAUD_RATE 115200

/* ============================================================================
 *                         LED PIN ASSIGNMENTS
 * ==========================================================================*/
#define LED_TRANSMITTER_PIN 11
#define LED_POWER_PIN 26
#define LED_COM_PIN 27
#define LED_SIGNAL_PIN 28

#endif // CONFIG_H
