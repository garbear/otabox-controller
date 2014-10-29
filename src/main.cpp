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

/**
 * Control of the Arduino is delegated to the OtaboxController class.
 */
OtaboxController controller;

/**
 * Called by Arduino's main() function before loop().
 */
void setup()
{
  // Set up our serial communications
  controller.Init();
}

/**
 * OtaboxController::Spin() uses an infinite loop to bypass Arduino's main loop.
 * The advantages here are twofold: we avoid the overhead of a function call,
 * and we skip Arduino's call to "if (serialEventRun) serialEventRun();" betwixt
 * every call to loop(). This is desirable because we are using master to
 * process serial data instead of the serialEvent() callback provided by
 * Arduino.
 */
void loop()
{
  controller.Spin();
}
