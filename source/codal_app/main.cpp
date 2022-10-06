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
extern const float arr_input_data[3200];

MicroBit uBit;

int main() {
    uBit.init();


    // As well as configuring a larger RX buffer, this needs to be called so it
    // calls Serial::initialiseRx, to set up interrupts.
    uBit.serial.setRxBufferSize(128);

    uBit.display.setBrightness(255);


    setup();

    uBit.display.print("x");

    float* output = predict(arr_input_data);

    float max_value = -100.0;
    int8_t max_index = -1;
    for(int i = 0; i<8; i++){
        if(output[i] > max_value){
            max_value = output[i];
            max_index = i;
        }
    }
        
    uBit.display.print(max_index);
    uBit.sleep(1000);

    return 0;
}
