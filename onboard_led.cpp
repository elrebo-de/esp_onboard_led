/*
 * onboard_led.cpp
 *
 *  Created on 01.11.2025
 *      Author: christophoberle
 *
 * this work is licenced under the Apache 2.0 licence
 */

#include "onboard_led.hpp"
#include "esp_log.h"
#include "driver/gpio.h"
#include "soc/clk_tree_defs.h"

OnBoardLed::OnBoardLed(std::string tag, gpio_num_t ledPin, std::string colorOrder,
                       std::string stripBackend, led_model_t ledModel, int blinkPeriod) {
	this->tag = tag;
    this->ledPin = ledPin;
    this->ledType = std::string("LED strip");
    this->colorOrder = colorOrder;
    this->stripBackend = stripBackend;
    this->ledModel = ledModel;
    this->blinkPeriod = blinkPeriod;

    // configure LED according to type and strip_backend
    // ToDo: additional parameters are needed to deal with different LED strips
    ESP_LOGI(this->tag.c_str(), "configured to use addressable LED strip!");
    /* LED strip initialization with the GPIO and pixels number*/
    if (this->colorOrder == "RGB") {
        this->strip_config = {
            .strip_gpio_num = this->ledPin,
            .max_leds = 1,
            .led_model = this->ledModel,
            .color_component_format = {
                .format = {.r_pos = 0,
                           .g_pos = 1,
                           .b_pos = 2,
                           .w_pos = 3,
                           .reserved = 0,
                           .num_components = 3
                }
            },
                .flags = {
	            .invert_out = false
            }
        };
    }
	else {
        this->strip_config = {
            .strip_gpio_num = this->ledPin,
            .max_leds = 1,
            .led_model = this->ledModel,
            .color_component_format = {
                .format = {.r_pos = 1,
                           .g_pos = 0,
                           .b_pos = 2,
                           .w_pos = 3,
                           .reserved = 0,
                           .num_components = 3
                }
            },
            .flags = {
   	            .invert_out = false
            }
        };
    }
	if (this->stripBackend == "RMT") {
        this->rmt_config = {
	        .clk_src = RMT_CLK_SRC_DEFAULT,
            .resolution_hz = 10 * 1000 * 1000, // 10MHz
            .mem_block_symbols = 0,
            .flags = {
	            .with_dma = false
	        }
        };
        ESP_ERROR_CHECK(led_strip_new_rmt_device(&this->strip_config, &this->rmt_config, &this->led_strip));
    }
    else if (this->stripBackend == "SPI") {
        this->spi_config = {
	        .clk_src = SPI_CLK_SRC_DEFAULT,
            .spi_bus = SPI2_HOST,
            .flags = {
	            .with_dma = true
	        }
        };
        ESP_ERROR_CHECK(led_strip_new_spi_device(&this->strip_config, &this->spi_config, &this->led_strip));
	   /* Set all LED off to clear all pixels */
	}
    led_strip_clear(this->led_strip);
}

OnBoardLed::OnBoardLed(std::string tag, gpio_num_t ledPin, uint8_t activeLevel, int blinkPeriod) {
	this->tag = tag;
    this->ledPin = ledPin;
    this->ledType = std::string("GPIO");
    this->activeLevel = activeLevel;
    this->blinkPeriod = blinkPeriod;

    ESP_LOGI(this->tag.c_str(), "configured to use GPIO LED!");
    const gpio_config_t ledGpioConfig =
        {
            .pin_bit_mask = (1ULL << this->ledPin),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_ENABLE,
            .intr_type = GPIO_INTR_DISABLE
        };
    gpio_config(&ledGpioConfig);

    gpio_dump_io_configuration(stdout, (1ULL << this->ledPin));

}

OnBoardLed::~OnBoardLed() {
	// TODO Auto-generated destructor stub
}

void OnBoardLed::setLedState(bool state) {
	this->state = state;
}

bool OnBoardLed::getLedState() {
	return this->state;
}

void OnBoardLed::setLedPixelColor(uint32_t index, uint32_t red, uint32_t green, uint32_t blue) {
	this->index = index;
	this->red = red;
	this->green = green;
	this->blue = blue;
}

void OnBoardLed::show(void) {
	if (this->ledType == "LED strip") {
        /* If the addressable LED is enabled */
        if (this->state) {
            /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
            led_strip_set_pixel(this->led_strip, this->index, this->red, this->green, this->blue);
            /* Refresh the strip to send data */
            led_strip_refresh(this->led_strip);
        } 
        else {
            /* Set all LED off to clear all pixels */
            led_strip_clear(this->led_strip);
        }
    } else {
        if (this->ledType == "GPIO") {
            /* Set the GPIO level according to the state (LOW or HIGH)*/
            if(this->activeLevel == 1) {
                gpio_set_level((gpio_num_t)this->ledPin, (uint32_t) this->state);
            }
            else {
                gpio_set_level((gpio_num_t)this->ledPin, (uint32_t) !this->state);
            }
        }
    }
}

void OnBoardLed::blink(void) {
	ESP_LOGI(this->tag.c_str(), "Turning the LED %s!", this->state == true ? "ON" : "OFF");
    /* show LED in current color and state {ON|OFF} */
    this->show();
    /* Toggle the LED state */
    this->state = !this->state;
    vTaskDelay(this->blinkPeriod / portTICK_PERIOD_MS);
}

