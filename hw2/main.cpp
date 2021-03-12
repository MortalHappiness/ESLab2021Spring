/*
 * Copyright (c) 2014-2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "PinNames.h"
#include "ThisThread.h"
#include "mbed.h"

// Adjust pin name to your board specification.
// You can use LED1/LED2/LED3/LED4 if any is connected to PWM capable pin,
// or use any PWM capable pin, and see generated signal on logical analyzer.

PwmOut led(PWM_OUT);
// PwmOut led(LED1);

#define SLEEP_TIME 0.05
#define DUTY_CYCLE_DIFF 0.1f

int main()
{
    led.period(0.05f);
    float duty_cycle = 0.1f;

    while (1) {
        led.write(duty_cycle);
        rtos::ThisThread::sleep_for(SLEEP_TIME);
        duty_cycle += DUTY_CYCLE_DIFF;
        if ( duty_cycle >= 1.0f ) {
            duty_cycle = 0.1f;
        }
    }
}
