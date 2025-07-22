#!/usr/bin/env python

"""
Simple test program to send notes to the midi stick.
"""

import time
import rtmidi

midiout = rtmidi.MidiOut()
available_ports = midiout.get_ports()

portNum = 0
for port in available_ports:
   if "Stick" in port:
      midiout.open_port(portNum)
      print("Opened port:", port)
      break
   portNum = portNum + 1
else:
   print("Didn't fin any valid port.")
   exit(-1)



with midiout:
    note_on = [0x90, 60, 112] # channel 1, middle C, velocity 112
    note_off = [0x80, 60, 0]
    print("Start sending notes.")
    while True:
       midiout.send_message(note_on)
       time.sleep(1)
       midiout.send_message(note_off)
       time.sleep(1)

del midiout