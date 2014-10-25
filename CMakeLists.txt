# Copyright (c) 2014 Garrett Brown
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


############################################################################
# Arduino Toolchain. See https://github.com/queezythegreat/arduino-cmake
############################################################################
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/arduino-cmake/cmake/ArduinoToolchain.cmake)

cmake_minimum_required(VERSION 2.8)
project(otabox)

# If None is specified, the debug flag -g causes debug info to be included in the hex
set(CMAKE_BUILD_TYPE Release)

# Board configuration
set(ARDUINO_BOARD    leonardo)
set(ARCH             AVR8)
set(F_CPU            16000000)
#set(ARDUINO_PROGRAMMER usbtinyisp)

# Options for flashing
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  set(ARDUINO_DEFAULT_PORT "/dev/ttyACM0")
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
  set(ARDUINO_DEFAULT_PORT "/dev/tty.usbmodem1451")
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  set(ARDUINO_DEFAULT_PORT )
endif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")

# Tell CMake where to search for libraries use the link_directories command.
# The command has to be used before defining any firmware or libraries
# requiring those libraries. An Arduino library is ***any directory which
# contains a header named after the directory.***
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/libraries)

# A udev rule, placed in a new rule file (named whatever you'd like) in
# /etc/udev/rules.d/ (or wherever you distro will expect it) will set the
# permissions for the USBtinyISP correctly.
# https://learn.adafruit.com/usbtinyisp/avrdude#for-linux
#
# SUBSYSTEM=="usb", ATTR{product}=="USBtiny", ATTR{idProduct}=="0c9f", ATTRS{idVendor}=="1781", MODE="0660", GROUP="dialout"


############################################################################
# otabox
############################################################################

# Hack so that Eclipse can find the pins_arduino.h file specific to the mega2560
# The "mega" part of this directory is determined by looking in boards.txt (see
# the arduino sdk path) and finding the boardID.build.variant setting.
include_directories("${ARDUINO_VARIANTS_PATH}/leonardo")

# Public include files so that programs can interface with the Arduino hardware
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

# Local include files so that public include files can find their referenced classes
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Weffc++ -pedantic -std=c++0x")

set(OTABOX_SRCS src/main.cpp
                src/OtaboxController.cpp
)

generate_arduino_firmware(otabox
    BOARD ${ARDUINO_BOARD}
    SRCS ${OTABOX_SRCS}
    #LIBS lib/main_lib
    #LIBS lufa-core-device
)

