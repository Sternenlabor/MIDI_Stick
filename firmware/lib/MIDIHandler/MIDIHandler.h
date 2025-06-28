/**
 * @file MIDIHandler.h
 * @brief Declaration of the MIDIHandler class which manages MIDI I/O (USB),
 *        System Exclusive (SysEx) processing, and NRPN handling.
 */

#ifndef MIDIHANDLER_H
#define MIDIHANDLER_H

#include <Arduino.h>
#include <MIDI.h>             // Arduino MIDI library (Adafruit_USBD_MIDI used here)
#include <Adafruit_TinyUSB.h> // For USB MIDI communication

/**
 * @brief External MIDI interface objects for different communication interfaces.
 *
 * These objects are declared as extern so that they can be defined in a corresponding
 * source file and shared among modules that need MIDI I/O functionality.
 */

// USB MIDI interface using Adafruit_USBD_MIDI.
extern MIDI_NAMESPACE::MidiInterface<MIDI_NAMESPACE::SerialMIDI<Adafruit_USBD_MIDI>> usbMidi;

/**
 * @brief The MIDIHandler class manages MIDI input/output, SysEx command processing,
 *        and Non-Registered Parameter Number (NRPN) handling.
 *
 * This class provides an interface for initializing MIDI ports, reading incoming
 * messages, sending standard messages (Note On/Off, NRPN), and processing SysEx messages.
 */
class MIDIHandler
{
public:
    /**
     * @brief Retrieves a pointer to the singleton instance of the MIDIHandler.
     * @return Pointer to the MIDIHandler instance.
     */
    static MIDIHandler *getInstance() { return instance; }

    /**
     * @brief Constructor.
     */
    explicit MIDIHandler();

    /**
     * @brief Initializes all MIDI ports (USB, DIN, RS485, etc.) and prepares the system
     *        for MIDI I/O operations.
     * @param config Reference to a Config object that holds user/device configuration.
     */
    void begin();

    /**
     * @brief Reads incoming MIDI messages from all configured interfaces (USB, DIN, etc.).
     */
    void read();

    /**
     * @brief Decodes two 7-bit MIDI bytes into a single 14-bit integer.
     * @param msb Most significant 7-bit byte.
     * @param lsb Least significant 7-bit byte.
     * @return Decoded 14-bit integer value, or -1 if inputs are invalid.
     */
    int decode14Bit(byte msb, byte lsb); // Non-static member function

    /**
     * @brief Pointer to the singleton instance of the MIDIHandler.
     *
     * This static member allows global access to the single MIDIHandler instance.
     */
    static MIDIHandler *instance;

    //
    // Public static callback methods for the Arduino MIDI Library.
    // These callbacks must be static because they are assigned to the MIDI library's
    // event handling mechanisms.
    //

    /**
     * @brief Callback for handling Note On messages.
     * @param channel  MIDI channel on which the note is played.
     * @param pitch    MIDI note number.
     * @param velocity MIDI note velocity.
     */
    static void handleNoteOn(byte channel, byte pitch, byte velocity);

    /**
     * @brief Callback for handling Note Off messages.
     * @param channel  MIDI channel on which the note is released.
     * @param pitch    MIDI note number.
     * @param velocity MIDI note release velocity.
     */
    static void handleNoteOff(byte channel, byte pitch, byte velocity);

    /**
     * @brief Callback for handling Control Change messages.
     * @param channel    MIDI channel of the control change.
     * @param identifier Controller number.
     * @param value      Controller value.
     */
    static void handleControlChange(byte channel, byte identifier, byte value);

    /**
     * @brief Callback for handling System Exclusive (SysEx) messages.
     * @param data   Pointer to the received SysEx data.
     * @param length Length of the SysEx message.
     */
    static void handleSysEx(byte *data, unsigned int length);

    static bool decodeSysExData(const byte *encodedData, unsigned encodedLength, byte *decodedData, unsigned &decodedLength);

private:
    //
    // NRPN tracking variables.
    //
    // These variables temporarily store the separate MSB and LSB parts of the NRPN
    // identifier and value until a full NRPN message can be constructed and processed.
    //
    int currentNrpnMsb = -1; ///< Most significant byte of the current NRPN identifier
    int currentNrpnLsb = -1; ///< Least significant byte of the current NRPN identifier
    int nrpnValueMsb = -1;   ///< Most significant byte of the current NRPN value
    int nrpnValueLsb = -1;   ///< Least significant byte of the current NRPN value
};

#endif // MIDIHANDLER_H
