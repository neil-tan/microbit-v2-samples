/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "main.h"
// #include "../../lib/codal/libraries/codal-microbit-v2/inc/compat/mbed.h"
// #include "TensorFlowLite.h"
#include "tflite/model.h"
#include "tflite/tflite_predict.h"
#include "tflite/main_functions.h"
#include "input_data.h"


extern "C" void mp_main(void);
extern "C" void m_printf(...);
// extern const float arr_input_data[3200];

void printFloat(float f) {
  int int_part = (int)f;

  uBit.serial.send(int_part);
  uBit.serial.send(".");

  float frac_part = f - (float) int_part;
  if (frac_part < 0) {
    frac_part = -frac_part;
  }

  for(int i = 0; i <= 3; i++) {
    frac_part *= 10;
    int digit = (int) frac_part;
    uBit.serial.send(digit);
    frac_part -= (float) digit;
  }
}

int argmax(float* arr, int len) {
    int max_idx = 0;
    float max_val = arr[0];
    for (int i = 1; i < len; i++) {
        if (arr[i] > max_val) {
            max_idx = i;
            max_val = arr[i];
        }
    }
    return max_idx;
}


MicroBit uBit;

int main() {
    uBit.init();

    // As well as configuring a larger RX buffer, this needs to be called so it
    // calls Serial::initialiseRx, to set up interrupts.
    uBit.serial.setRxBufferSize(128);

    uBit.serial.send("Init...");
    uBit.display.setBrightness(255);

    setup();

    for (int sample_counter=0;sample_counter<num_samples;sample_counter++) {
        const float *input_data = arr_input_data[sample_counter];

        uBit.display.print("x");
        unsigned long elapsed_time = uBit.systemTime();
        float* output = predict(input_data);
        elapsed_time = uBit.systemTime() - elapsed_time;
        uBit.display.print("O");

        int max_index = argmax(output, 41);

        uBit.serial.send("\n");
        uBit.serial.send("Label:");
        uBit.serial.send((int) arr_input_label[sample_counter]);
        uBit.serial.send("\n");

        uBit.serial.send("Predicted: ");
        uBit.serial.send(max_index);
        uBit.serial.send("\n");

        uBit.serial.send("Time: ");
        uBit.serial.send((int)elapsed_time);
        uBit.serial.send("ms\n\n");

    }

    uBit.serial.send("\nDone.");
    uBit.sleep(1000);

    return 0;
}

