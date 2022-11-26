/**
 * @file tlc_bsp.c
 * @brief Traffic Light Controller Board Support Package (BSP) source code
 * @author Jorge Minjares (https://github.com/JorgeMinjares)
 * @brief BSP are custom function calls to add abstraction to the software.
 * @version 0.1
 * @date 2022-11-24
 * 
 * @copyright Copyright (c) 2022
 */
#include "tlc_bsp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>
#include <driver/dac.h>
#include <driver/adc.h>
#include "driver/uart.h"
#include <string.h>

/**
 * @brief Initialize bsp LEDs
 * 
 * @param tlc pointer to a tlc structure
 * @note Call it once per tlc_t pointer
 * @warning Make sure to pass an intialize tlc
 * 
 * @return None
 */
void tlc_bsp_led_init(tlc_t * const tlc){
    for(int i = 0; i < 3; i++){
        gpio_pad_select_gpio(tlc->led[i]);
        gpio_set_direction(tlc->led[i], GPIO_MODE_OUTPUT);
        gpio_set_level(tlc->led[i], LOW);
    }
}

/**
 * @brief Turn on Green LED
 * 
 * @param tlc pointer to a tlc structure
 * @return None
 */
void tlc_bsp_green_led_on(tlc_t * const tlc){
    gpio_set_level(tlc->led[0], HIGH);
}
/**
 * @brief Turn off Green LED
 * 
 * @param tlc pointer to a tlc structure
 * @return None
 */
void tlc_bsp_green_led_off(tlc_t * const tlc){
    gpio_set_level(tlc->led[0], LOW);
}

/**
 * @brief Turn on Yellow LED
 * 
 * @param tlc pointer to a tlc structure
 * @return None
 */
void tlc_bsp_yellow_led_on(tlc_t * const tlc){
    gpio_set_level(tlc->led[1], HIGH);
}

/**
 * @brief Turn off Yellow LED
 * 
 * @param tlc pointer to a tlc structure
 * @return None
 */
void tlc_bsp_yellow_led_off(tlc_t * const tlc){
    gpio_set_level(tlc->led[1], LOW);
}
/**
 * @brief Toggle Yellow LED
 * 
 * @param tlc pointer to a tlc structure
 * @return None
 */
void tlc_bsp_yellow_toggle(tlc_t * const tlc_0, tlc_t * const tlc_1){
    tlc_bsp_green_led_off(tlc_0);
    tlc_bsp_green_led_off(tlc_1);
    for (int i = 0; i < 10; i++)
        {
            tlc_bsp_yellow_led_on(tlc_0);
            tlc_bsp_yellow_led_on(tlc_1);
            vTaskDelay(250 / portTICK_PERIOD_MS);
            tlc_bsp_yellow_led_off(tlc_0);
            tlc_bsp_yellow_led_off(tlc_1);
            vTaskDelay(250 / portTICK_PERIOD_MS);
        }
}

/**
 * @brief Turn on Red LED
 * 
 * @param tlc pointer to a tlc structure
 * @return None
 */
void tlc_bsp_red_led_on(tlc_t * const tlc){
    gpio_set_level(tlc->led[2], HIGH);
}
/**
 * @brief Turn off Red LED
 * 
 * @param tlc pointer to a tlc structure
 * @return None
 */
void tlc_bsp_red_led_off(tlc_t * const tlc){
    gpio_set_level(tlc->led[2], LOW);
}


/**
 * @brief Initialize bsp buttons
 * 
 * @param tlc pointer to a tlc structure
 * @note Call it once per tlc_t pointer
 * @warning Make sure to pass an intialize tlc
 * @return None
 */
void tlc_bsp_button_init(tlc_t * const tlc){
    for(int i = 0; i < 2; i++){
        gpio_pad_select_gpio(tlc->button[i]);
        gpio_set_direction(tlc->button[i], GPIO_MODE_INPUT);
        gpio_set_pull_mode(tlc->button[i], GPIO_PULLDOWN_ONLY);
    }    
}
/**
 * @brief Read bsp buttons
 * 
 * @param tlc pointer to a tlc structure
 * @return gpio level of buttons
 */
uint8_t tlc_bsp_button_read(tlc_t * const tlc){
   return gpio_get_level(tlc->button[0]) | gpio_get_level(tlc->button[1]);

}
/**
 * @brief Initialize bsp buzzer
 * 
 * @param tlc pointer to a tlc structure
 * @note Call it once per tlc_t pointer
 * @warning Make sure to pass an intialize tlc
 * @return None
 */
void tlc_bsp_buzzer_init(tlc_t * const tlc){
    dac_channel_t channel = tlc->buzzer == (gpio_num_t)25 ? DAC_CHANNEL_1 : DAC_CHANNEL_2;
    
    dac_output_enable(channel);
    dac_output_voltage(channel, 0);
}

/**
 * @brief Turn on buzzer
 * 
 * @param tlc pointer to a tlc structure
 * @return None
 */
void tlc_bsp_buzzer_on(tlc_t * const tlc, uint8_t volume){
    dac_channel_t channel = tlc->buzzer == (gpio_num_t)25 ? DAC_CHANNEL_1 : DAC_CHANNEL_2;
    dac_output_voltage(channel, volume);
}

/**
 * @brief Turn off buzzer
 * 
 * @param tlc pointer to a tlc structure
 * @return None
 */
void tlc_bsp_buzzer_off(tlc_t * const tlc){
    dac_channel_t channel = tlc->buzzer == (gpio_num_t)25 ? DAC_CHANNEL_1 : DAC_CHANNEL_2;
    dac_output_voltage(channel, 0);
}

/**
 * @brief Initialize bsp walk signal
 * 
 * @param tlc pointer to a tlc structure
 * @note Call it once per tlc_t pointer
 * @warning Make sure to pass an intialize tlc
 * @return None
 */
void tlc_bsp_walk_init(tlc_t * const tlc){
    gpio_pad_select_gpio(tlc->walkingSignal);
    gpio_set_direction(tlc->walkingSignal, GPIO_MODE_OUTPUT);
    gpio_set_level(tlc->walkingSignal, LOW);
}

/**
 * @brief Turn on walk signal
 * 
 * @param tlc pointer to a tlc structure
 * @return None
 */
void tlc_bsp_walk_on(tlc_t * const tlc){
    gpio_set_level(tlc->walkingSignal, HIGH);
}

/**
 * @brief Turn off walk signal
 * 
 * @param tlc pointer to a tlc structure
 * @return None
 */
void tlc_bsp_walk_off(tlc_t * const tlc){
    gpio_set_level(tlc->walkingSignal, LOW);
}

/**
 * @brief Toggle walk signal
 * 
 * @param tlc pointer to a tlc structure
 * @return None
 */
void tlc_bsp_walk_warning(tlc_t * const tlc){
    for(int i = 0; i < 4; i++){
        tlc_bsp_walk_on(tlc);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        tlc_bsp_walk_off(tlc);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Initialize bsp hardware
 * 
 * @param tlc pointer to a tlc structure @see traffic_light.h
 * @note Call it once per tlc_t pointer
 */
void tlc_bsp_init(tlc_t * const tlc){
    tlc_bsp_led_init(tlc);
    tlc_bsp_button_init(tlc);
    tlc_bsp_buzzer_init(tlc);
    tlc_bsp_walk_init(tlc);
}

/**
 * @brief Update traffic light LEDs
 * 
 * @param tlc pointer to a tlc structure
 * @param state current state
 * @return None
 */
void tlc_bsp_lights(state_t state, tlc_t * const tlc)
{
    if (state == GREEN)
    {
        tlc_bsp_green_led_on(tlc);
        tlc_bsp_yellow_led_off(tlc);
        tlc_bsp_red_led_off(tlc);
    }
    else if (state == YELLOW)
    {
        tlc_bsp_green_led_off(tlc);
        tlc_bsp_yellow_led_on(tlc);
        tlc_bsp_red_led_off(tlc);
    }
    else if (state == RED)
    {
        tlc_bsp_green_led_off(tlc);
        tlc_bsp_yellow_led_off(tlc);
        tlc_bsp_red_led_on(tlc);
    }
}

/**
 * @brief Initialize bsp adc
 * 
 * @return None
 */
void tlc_bsp_adc_init(void){
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
}
/**
 * @brief Read adc channel
 * 
 * @return raw adc value
 * @note ADC is set at 12 bits resolution: 0-4096
 */
uint32_t tlc_bsp_adc_read(void){
    return adc1_get_raw(ADC1_CHANNEL_6);  
}

/**
 * @brief Initialize bsp uart
 * 
 * @return None
 */
void tlc_bsp_uart_init(void){
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits =  UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };

    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, (gpio_num_t)1, (gpio_num_t)3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_0, 1024 * 2, 1024 * 2, 0, NULL, 0);

}

/**
 * @brief Write byte to uart driver
 * 
 * @param str message to be sent
 * @note Utilze UART-0 at 115200 b/s 
 */
void tlc_bsp_uart_write_byte(char*str){
    uart_write_bytes(UART_NUM_0, (const char *)str, strlen(str));
}
/**
 * @brief Read single byte from uart driver
 * 
 * @param c character to store byte
 * @return int total bytes read
 */
int tlc_bsp_uart_read_byte(char *c){
    return uart_read_bytes(UART_NUM_0, (void *)c, 1, portMAX_DELAY);
}
/**
 * @brief Map value
 * 
 * @param x         value to be mapped
 * @param in_min    input minimum
 * @param in_max    input maximum
 * @param out_min   output minimum
 * @param out_max   output maximum
 * @return uint16_t value mapped
 */
uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
