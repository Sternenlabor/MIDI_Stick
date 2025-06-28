#ifndef DEBUG_SERIAL_H
#define DEBUG_SERIAL_H

#include <SoftwareSerial.h>
#include <Config.h>

/**
 * @file DebugSerial.h
 * @brief Provides helper macros and functions for debug logging via SoftwareSerial.
 *
 * This header declares a global SoftwareSerial instance named `SerialDebug` and provides
 * template functions and macros for debug logging. The logging functions are enabled only if
 * the preprocessor symbol `DEBUG_SERIAL` is defined.
 */

/* ============================================================================
 *                          GLOBAL INSTANCE DECLARATION
 * ==========================================================================*/

/**
 * @brief Global SoftwareSerial instance for debug logging.
 *
 * The instance should be defined and initialized elsewhere in your code.
 */
extern SoftwareSerial SerialDebug;

/* ============================================================================
 *                           DEBUG LOGGING FUNCTIONS & MACROS
 * ==========================================================================*/

#ifdef DEBUG_SERIAL
// Debug logging enabled

/**
 * @brief Variadic template function to print multiple arguments without a newline.
 *
 * @tparam Args Types of the arguments.
 * @param args A list of arguments to be printed via SerialDebug.
 */
template <typename... Args>
void debugLog(Args... args)
{
    // Fold expression to call SerialDebug.print on each argument.
    (SerialDebug.print(args), ...);
}

/**
 * @brief Variadic template function to print multiple arguments followed by a newline.
 *
 * @tparam Args Types of the arguments.
 * @param args A list of arguments to be printed via SerialDebug.
 */
template <typename... Args>
void debugLogLn(Args... args)
{
    (SerialDebug.print(args), ...);
    SerialDebug.println();
}

// Macros to simplify debug logging calls.
#define DEBUG_LOG(...) debugLog(__VA_ARGS__)
#define DEBUG_LOG_LN(...) debugLogLn(__VA_ARGS__)
#define DEBUG_LOG_PRINTF(...) SerialDebug.printf(__VA_ARGS__)
#define DEBUG_PRINT(value, format)            \
    do                                        \
    {                                         \
        if ((format) == HEX && (value) < 16)  \
        {                                     \
            SerialDebug.print("0");           \
        }                                     \
        SerialDebug.print((value), (format)); \
    } while (0)

#else
// Debug logging disabled: define the macros as no-operations.
#define DEBUG_LOG(...)
#define DEBUG_LOG_LN(...)
#define DEBUG_LOG_PRINTF(...)
#define DEBUG_PRINT(value, format)

#endif

/* ============================================================================
 *                          SERIAL DEBUG SETUP
 * ==========================================================================*/

/**
 * @brief Initializes the SerialDebug interface.
 *
 * This function should be implemented to configure and begin the SoftwareSerial port
 * for debug logging. It is optional and can be used to centralize the initialization
 * of the debug serial port.
 */
void setupSerialDebug();

#endif // DEBUG_SERIAL_H
