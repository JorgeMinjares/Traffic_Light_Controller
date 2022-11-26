/**
 * @file tlc_bsp.h
 * @brief Traffic Light Controller Board Support Package (BSP)
 * @author Jorge Minjares (https://github.com/JorgeMinjares)
 * @brief BSP are custom function calls to add abstraction to the software.
 * @version 0.1
 * @date 2022-11-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef TLC_BSP_H
#define TLC_BSP_H
#include "../tlc_config.h"
#include "../traffic_light.h"

void tlc_bsp_led_init(tlc_t * const tlc);
void tlc_bsp_green_led_on(tlc_t * const tlc);
void tlc_bsp_green_led_off(tlc_t * const tlc);
void tlc_bsp_yellow_led_on(tlc_t * const tlc);
void tlc_bsp_yellow_led_off(tlc_t * const tlc);
void tlc_bsp_yellow_toggle(tlc_t * const tlc_0, tlc_t * const tlc_1);
void tlc_bsp_red_led_on(tlc_t * const tlc);
void tlc_bsp_red_led_off(tlc_t * const tlc);
void tlc_bsp_button_init(tlc_t * const tlc);
void tlc_bsp_buzzer_init(tlc_t * const tlc);
uint8_t tlc_bsp_button_read(tlc_t * const tlc);
void tlc_bsp_buzzer_on(tlc_t * const tlc, uint8_t volume);
void tlc_bsp_buzzer_off(tlc_t * const tlc);
void tlc_bsp_walk_init(tlc_t * const tlc);
void tlc_bsp_walk_on(tlc_t * const tlc);
void tlc_bsp_walk_off(tlc_t * const tlc);
void tlc_bsp_walk_warning(tlc_t * const tlc);
void tlc_bsp_init(tlc_t * const tlc);
void tlc_bsp_lights(state_t state, tlc_t * const tlc);
void tlc_bsp_adc_init(void);
uint32_t tlc_bsp_adc_read(void);
void tlc_bsp_uart_init(void);
void tlc_bsp_uart_write_byte(char*str);
int tlc_bsp_uart_read_byte(char *c);
uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

#endif