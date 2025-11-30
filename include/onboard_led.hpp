/*
 * onboard_led.hpp
 *
 *  Created on: 30.10.2025
 *      Author: christophoberle
 *
 * this work is licenced under the Apache 2.0 licence
 */

#ifndef ONBOARD_LED_HPP_
#define ONBOARD_LED_HPP_

#include <stdio.h>

#include "led_strip.h"
#include <cstdint>
#include <string>

class OnBoardLed {
public:
	OnBoardLed(std::string tag, gpio_num_t ledPin, std::string colorOrder,
	           std::string stripBackend, led_model_t ledModel, int blinkPeriod);
	OnBoardLed(std::string tag, gpio_num_t ledPin, uint8_t activeLevel, int blinkPeriod);
	virtual ~OnBoardLed();
    void setLedPixelColor(uint32_t index, uint32_t red, uint32_t green, uint32_t blue);
    void show(void);
    void setLedState(bool state);
    bool getLedState();
    void blink();

private:	
    std::string tag = "OnBoardLed";
	gpio_num_t ledPin; // the pin number for this LED
	std::string ledType; // the LED type {"GPIO"|"LED strip"}
	std::string colorOrder; // the LED color_order {"RGB"|"GRB"}
	std::string stripBackend; // the LED strip backend {"RMT"|"SPI"}
	led_model_t ledModel; // the LED model { LED_MODEL_WS2812 | LED_MODEL_SK6812 }
	int blinkPeriod; // blinking period in milliseconds
	
	// LedPixelColor settings
	uint32_t index;
	uint32_t red;
	uint32_t green;
	uint32_t blue;
	
	// Led State {ON|OFF}
    uint8_t state = 1;

    led_strip_handle_t led_strip;
    led_strip_config_t strip_config;
    led_strip_rmt_config_t rmt_config;
    led_strip_spi_config_t spi_config;

    // ledType GPIO activeLevel: low (0) or high (1)
    uint8_t activeLevel;
};

#endif /* ONBOARD_LED_HPP_ */
