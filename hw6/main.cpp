#include "mbed.h"

InterruptIn button(USER_BUTTON);
DigitalOut led(LED1);
Timeout press_threhold;
EventQueue queue(32 * EVENTS_EVENT_SIZE);
static int num = 0;

void button_release_detecting() { button.enable_irq(); }

void button_pressed() {
    button.disable_irq();

    press_threhold.attach(&button_release_detecting, 1s);
}

void button_released() {
    led = !led;
    ++num;
    queue.call(printf, "Current long button press times = %d\n", num);
}

// main() runs in its own thread in the OS
int main() {
    button.fall(&button_pressed);
    button.rise(&button_released);
    queue.dispatch_forever();
}
