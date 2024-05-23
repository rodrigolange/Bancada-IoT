// this must be done before indirectly including "pico/stdio_usb.h":
#ifndef PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS
#define PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS (5000)
#endif

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int main() {

    printf("teste\n");

    _Bool result = stdio_usb_init();

    result = stdio_init_all();

    result = cyw43_arch_init();

    while (true) {
 	printf("Hello\n");
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(2000);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(2000);
    }

}
