/**
 * @file traffic_light.h
 * @brief Traffic Light Controller Data Structures
 * @author Jorge Minjares (https://github.com/JorgeMinjares)
 * @brief Traffic Light Controller custom data structures
 * @version 0.1
 * @date 2022-11-24
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include "driver/gpio.h"

/******************************************************************
 * \enum direction_t traffic_light.h
 * \brief Direction enumeration
 *
 * ### Example
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.c
 * typedef enum{
 *      NONE = 0x00,
 *      NORTH = 0x01,
 *      EAST = 0x02,
 *      SOUTH = 0x03,
 *      WEST = 0x04,
 * }direction_t;
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *******************************************************************/
typedef enum
{
    NONE = 0x00,  /*!< Default */
    NORTH = 0x01, /*!< North Direction */
    EAST = 0x02,  /*!< East Direction */
    SOUTH = 0x03, /*!< South Direction */
    WEST = 0x04,  /*!< West Direction */
} direction_t;

/******************************************************************
 * \struct tlc_t traffic_light.h
 * \brief Traffic Light Controller structure
 *
 * ### Example
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.c
 * typedef struct{
 *      direction_t direction;
 *      gpio_num_t led[3];
 *      gpio_num_t button[2];
 *      gpio_num_t buzzer;
 *      gpio_num_t walkingSignal;
 * }tlc_t;
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *******************************************************************/
typedef struct
{
    direction_t direction;    /*!< Direction */
    gpio_num_t led[3];        /*!< LEDs  */
    gpio_num_t button[2];     /*!< Pedestrian Buttons */
    gpio_num_t buzzer;        /*!< Sound Queue */
    gpio_num_t walkingSignal; /*!< Walking LED Signal */
} tlc_t;

/******************************************************************
 * \enum state_t traffic_light.h
 * \brief State enumeration
 *
 * ### Example
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~.c
 * typedef enum{
 *      GREEN,
 *      YELLOW,
 *      RED,
 * }state_t;
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *******************************************************************/
typedef enum
{
    GREEN = 0x00, /*!< Green state */
    YELLOW = 0x01, /*!< Yellow state */
    RED = 0x02, /*!< Red state */
} state_t;

#endif