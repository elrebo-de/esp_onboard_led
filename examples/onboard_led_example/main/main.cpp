/*
 * Example program to use the onboard led with elrebo/onboard_led
 */

#include <string>
#include "onboard_led.hpp"
#include "esp_log.h"

static const char *tag = "onboard_led";

extern "C" void app_main(void)
{
    /* Configure the onboard led */
    OnBoardLed onBoardLed(
	    #if defined(CONFIG_IDF_TARGET_ESP32C6)
 	        /* ESP32-C6-DevKitM-1 V1.0 */
	        std::string("ESP32-C6-DevKitM-1 V1.0 onBoardLed"),
		    (gpio_num_t) 8,
		    std::string("GRB"),
		    std::string("RMT"),
		    LED_MODEL_WS2812,
	    #elif defined(CONFIG_IDF_TARGET_ESP32C3)
	        /* Waveshare ESP32-C3-Zero
		    std::string("Waveshare ESP32-C3-Zero onBoardLed"),
		    (gpio_num_t) 10,
		    std::string("GPIO"),
		    std::string("RGB"),
		    std::string("RMT"),
		    LED_MODEL_WS2812, */

	        /* ESP32-C3 w/ LCD display */
		    std::string("ESP32-C3 w/ LCD display onBoardLed"),
		    (gpio_num_t) 8,
            (uint8_t) 0, // activeLevel
	    #elif defined(CONFIG_IDF_TARGET_ESP32)
	        /* M5 Atom Lite */
		    std::string("M5 Atom Lite onBoardLed"),
		    (gpio_num_t) 27,
		    std::string("GRB"),
		    std::string("RMT"),
		    LED_MODEL_WS2812,
	    #endif
		500);

    onBoardLed.setLedState(1);

	for(int i=0; i<10;i++) {
        if (i % 3 == 0) onBoardLed.setLedPixelColor(0, 16, 0, 0); // pixel 0, color red, intensity 16/256
        if (i % 3 == 1) onBoardLed.setLedPixelColor(0, 0, 16, 0); // pixel 0, color green, intensity 16/256
        if (i % 3 == 2) onBoardLed.setLedPixelColor(0, 0, 0, 16); // pixel 0, color blue, intensity 16/256
        onBoardLed.blink();
        onBoardLed.blink();
    }
}
