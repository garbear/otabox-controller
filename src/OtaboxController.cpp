/*
 * Copyright (c) 2014 Garrett Brown
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *    The above copyright notice and this permission notice shall be included in
 *    all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "OtaboxController.h"

#include <Arduino.h>

#define LED_PIN   13
#define BAUD_RATE 115200

OtaboxController::OtaboxController(void)
 : m_serial(NULL)
{
}

void OtaboxController::Init(void)
{
  pinMode(LED_PIN, OUTPUT);
  
#if defined(USBCON)
  m_serial = &Serial1;
#else
  m_serial = &Serial;
#endif

  m_serial->begin(BAUD_RATE);
  m_serial->setTimeout(250); // ms
}

void OtaboxController::Spin(void)
{
  char buffer;
  for (;;)
  {
    while (m_serial->available())
    {
      m_serial->readBytes(&buffer, sizeof(buffer));
      if (buffer == '0')
        digitalWrite(LED_PIN, LOW);
      else if (buffer == '1')
        digitalWrite(LED_PIN, HIGH);
    }
  }
}

