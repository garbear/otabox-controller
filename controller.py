#!/usr/bin/env python
################################################################################
#
#    Copyright (c) 2014 Garrett Brown
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
#    The above copyright notice and this permission notice shall be included in
#    all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
################################################################################

################################################################################
# controller.py
#
# Control power to otabox. Call with -f to force pipe re-creation if pipe
# already exists.
################################################################################

import os
import serial
import subprocess
import sys

################################################################################
# PIPE_NAME
#
# This script creates the following pipe. To control otabox, echo 0 or 1 to this
# pipe.
################################################################################
PIPE_NAME = '/tmp/otabox'

################################################################################
# SERIAL_PORT_PATTERN
#
# Search for serial ports using this pattern.
################################################################################
SERIAL_PORTS = '/dev/ttyACM*'

################################################################################
# GET_DEVICE_PROPS_CMD
#
# Command to print the device's information.
#
# %s - Device path (e.g. '/dev/ttyACM0')
################################################################################
GET_DEVICE_PROPS_CMD = 'udevadm info -q all -n %s --attribute-walk'

################################################################################
# ARDUINO_SEARCH_STRING
#
# Device is an Arduino if the output of GET_DEVICE_PROPS_CMD contains this
# string.
################################################################################
ARDUINO_SEARCH_STRING = 'Arduino'

################################################################################
# BAUD_RATE
#
# Set to the baud rate used in AVR source.
################################################################################
BAUD_RATE = 115200

################################################################################
# main()
#
# Entry point of the program. Initialize serial communication with the Arduino
# and create a named pipe for communication with Linux processes, then enter the
# main loop. The pipe is polled for '0' and '1', used to power otabox off and
# on, respectively.
################################################################################
def main():
  # Establish communication with the Arduino
  ser = None
  devices = glob.glob(SERIAL_PORTS)
  for device in devices:
    # Get device information
    deviceInfo = subprocess.check_output(GET_DEVICE_PROPS_CMD % device, shell=True)
    # Check if the device is an Arduino
    if ARDUINO_SEARCH_STRING in deviceInfo:
      ser = serial.Serial(port=device, \
                          baudrate=BAUD_RATE, \
                          parity=serial.PARITY_NONE, \
                          stopbits=serial.STOPBITS_ONE, \
                          bytesize=serial.EIGHTBITS)
      break
  if not ser:
    print('No Arduino detected')
    sys.exit(0)

  # Check if the pipe exists, and re-create if -f was passed as an argument
  if os.path.exists(PIPE_NAME):
    param = sys.argv[1] if len(sys.argv) > 1 else ''
    if '-f' in param:
      os.unlink(PIPE_NAME)
    else:
      print('Pipe exists (%s), run with -f to unlink' % PIPE_NAME)
      sys.exit(0)

  os.mkfifo(PIPE_NAME)
  try:
    with open(PIPE_NAME) as pipein:
      while True:
        linein = pipein.read()
        try:
          command = int(linein)
          if command == 0:
            print('Command: off')
          elif command == 1:
            print('Command: on')
        except:
          pass
  except:
    pass
  os.unlink(PIPE_NAME)

if __name__ == '__main__':
  main()

