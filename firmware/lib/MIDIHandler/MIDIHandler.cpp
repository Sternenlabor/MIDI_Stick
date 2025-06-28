/**
 * @brief Implementation of the MIDIHandler class methods.
 *
 * processes incoming MIDI messages (including SysEx and NRPN commands).
 */

#include "MIDIHandler.h"
#include <cstdint> // For fixed-width integer types (e.g., uint8_t)
#include <SerialDebug.h>

// USB MIDI object declaration using the Adafruit USBD MIDI implementation.
Adafruit_USBD_MIDI usb_midi;

// Create a new instance of the Arduino MIDI Library for USB using the Adafruit_USBD_MIDI object.
// The created instance is referred to as usbMidi.
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, usbMidi);

// Define and initialize the static instance pointer for the MIDIHandler singleton.
MIDIHandler *MIDIHandler::instance = nullptr;

/**
 * @brief Constructor for MIDIHandler.
 *
 * Stores a reference to the configuration object and sets the static instance pointer.
 */
MIDIHandler::MIDIHandler()
{
    // Set the singleton instance pointer to this object.
    instance = this;
}

/**
 * @brief Initializes all MIDI interfaces and communication settings.
 *
 * This method sets up USB MIDI descriptors, initializes serial ports (DIN and RS485),
 * begins listening on MIDI channels, disables MIDI Thru, assigns callback handlers, and
 * initializes the file system.
 *
 * @param config Reference to a Config object containing device-specific configuration.
 */
void MIDIHandler::begin()
{
    usbMidi.begin(MIDI_CHANNEL_OMNI); // Listen on all MIDI channels
    usbMidi.turnThruOff();

    usbMidi.setHandleNoteOn(handleNoteOn);
    usbMidi.setHandleNoteOff(handleNoteOff);
    usbMidi.setHandleControlChange(handleControlChange);
    usbMidi.setHandleSystemExclusive(handleSysEx);
}

/**
 * @brief Decodes two 7-bit MIDI bytes into a single 14-bit integer.
 *
 * This method ensures that the provided bytes are within valid 7-bit ranges before combining them.
 *
 * @param msb Most significant 7-bit byte.
 * @param lsb Least significant 7-bit byte.
 * @return The combined 14-bit integer, or -1 if either byte is invalid.
 */
int MIDIHandler::decode14Bit(byte msb, byte lsb)
{
    if (msb > 0x7F || lsb > 0x7F)
    {
        return -1; // Invalid input: bytes must be 7-bit.
    }
    return (msb << 7) | lsb;
}

/**
 * @brief Reads incoming MIDI messages from all active MIDI interfaces.
 *
 * This function calls the read() method on RS485, DIN (if enabled), and USB interfaces.
 */
void MIDIHandler::read()
{
    if (TinyUSBDevice.mounted())
    {
        usbMidi.read();
    }
}

/**
 * @brief Static callback for handling Note On events.
 *
 * This method is invoked by the MIDI library when a Note On event is received.
 *
 * @param channel  The MIDI channel on which the note was triggered.
 * @param pitch    The MIDI note number.
 * @param velocity The velocity of the note.
 */
void MIDIHandler::handleNoteOn(byte channel, byte pitch, byte velocity)
{
    // Log the Note On event details for debugging.
    DEBUG_LOG_LN("Note On: Channel = ", channel, ", Pitch = ", pitch, ", Velocity = ", velocity);

    // A Note On with velocity 0 is equivalent to a Note Off.
    if (velocity > 0)
    {
        // TODO: Add code here to trigger sound or instrument action.
    }
    else
    {
        handleNoteOff(channel, pitch, velocity);
    }
}

/**
 * @brief Static callback for handling Note Off events.
 *
 * This method is invoked by the MIDI library when a Note Off event is received.
 *
 * @param channel  The MIDI channel on which the note was released.
 * @param pitch    The MIDI note number.
 * @param velocity The release velocity.
 */
void MIDIHandler::handleNoteOff(byte channel, byte pitch, byte velocity)
{
    // Log the Note Off event details for debugging.
    DEBUG_LOG_LN("Note Off: Channel = ", channel, ", Pitch = ", pitch, ", Velocity = ", velocity);

    // TODO: Add code here to stop the sound associated with the note.
}

/**
 * @brief Static callback for handling Control Change events.
 *
 * This method processes control change messages to capture NRPN parts and, when complete,
 * combines them to form a full NRPN message.
 *
 * @param channel    The MIDI channel on which the control change occurred.
 * @param identifier The controller number.
 * @param value      The value of the controller.
 */
void MIDIHandler::handleControlChange(byte channel, byte identifier, byte value)
{
    DEBUG_LOG_LN("Control Change: Channel = ", channel, " Control = ", identifier, " Value = ", value);

    // TODO: Add code here to handle control changes.
}

/**
 * @brief Static callback for handling System Exclusive (SysEx) messages.
 *
 * This method validates the SysEx message length and manufacturer ID, then
 * dispatches the message to the appropriate handler based on the command type.
 *
 * @param data   Pointer to the SysEx data.
 * @param length The length of the SysEx message.
 */
void MIDIHandler::handleSysEx(byte *data, unsigned int length)
{
    DEBUG_LOG("Received SysEx data: ");
    for (unsigned int i = 0; i < length; i++)
    {
        DEBUG_PRINT(data[i], HEX);
        DEBUG_LOG(" ");
    }
    DEBUG_LOG_LN();

    // TODO: Validate SysEx message length and manufacturer ID.
}

bool MIDIHandler::decodeSysExData(const byte *encodedData, unsigned encodedLength, byte *decodedData, unsigned &decodedLength)
{
    unsigned decodedIndex = 0;
    unsigned encodedIndex = 0;

    while (encodedIndex < encodedLength)
    {
        // Read the MSB byte
        byte msbByte = encodedData[encodedIndex++];

        // Decode up to 7 data bytes
        for (int i = 0; i < 7 && encodedIndex < encodedLength; i++)
        {
            if (decodedIndex >= decodedLength)
            {
                // Not enough space in the decodedData buffer
                return false;
            }

            // Read the next encoded data byte
            byte dataByte = encodedData[encodedIndex++];

            // Restore the 8th bit if it was set
            if (msbByte & (1 << i))
            {
                dataByte |= 0x80;
            }

            // Store the decoded byte
            decodedData[decodedIndex++] = dataByte;
        }
    }

    // Update the decoded length to reflect the actual number of decoded bytes
    decodedLength = decodedIndex;
    return true;
}
