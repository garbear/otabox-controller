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
#define BAUD_RATE 9600

OtaboxController::OtaboxController(void)
 : m_serial(NULL)
{
}

void OtaboxController::Init(void)
{
  pinMode(LED_PIN, OUTPUT);

  m_serial = &Serial;
  m_serial->begin(BAUD_RATE);
}

void OtaboxController::Spin(void)
{
  for (;;)
  {
    while (m_serial->available())
    {
      int byteIn = m_serial->read();
      if (byteIn == '0')
        digitalWrite(LED_PIN, LOW);
      else if (byteIn == '1')
        digitalWrite(LED_PIN, HIGH);
    }
  }
}
