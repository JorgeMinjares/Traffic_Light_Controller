/**
 * @file tlc_config.h
 * @brief Traffic Light Controlller Configuration
 * @author Jorge Minjares (https://github.com/JorgeMinjares)
 * @brief This is the configuration for the Traffic Light Controller (TLC).
 * @version 0.1
 * @date 2022-11-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef TLC_CONFIG_H
#define TLC_CONFIG_H

#define NORTH_SOUTH 1 /*!< Enable North|South or East|West */

/*NORTH & SOUTH Configuration*/
#if NORTH_SOUTH

/*Directions*/
#define DIRECTION_0 0x01 /*!< North direction  */
#define DIRECTION_1 0x03 /*!< South direction */

/*Direction 0 Buttons*/
#define BUTTON_0 14 /*!< Pedestrian Button 0 */
#define BUTTON_1 15 /*!< Pedestrian Button 1 */
/*Direction 1 Buttons*/
#define BUTTON_2 12 /*!< Pedestrian Button 2 */
#define BUTTON_3 13 /*!< Pedestrian Button 3 */

#else

/*EAST & WEST Configuration*/
#define DIRECTION_0 0x02 /*!< East direction  */
#define DIRECTION_1 0x04 /*!< West direction  */

/*Direction 0 Buttons*/
#define BUTTON_0 12      /*!< Pedestrian Button 0 */
#define BUTTON_1 15      /*!< Pedestrian Button 1 */
/*Direction 1 Buttons*/
#define BUTTON_2 13      /*!< Pedestrian Button 2 */
#define BUTTON_3 14      /*!< Pedestrian Button 3 */

#endif

/*Direction 0 LEDs*/
#define LED_0 16 /*!< Green Led Direction 0 */
#define LED_1 17 /*!< Yellow Led Direction 0 */
#define LED_2 18 /*!< Red Led Direction 0 */
/*Direction 1 LEDs*/
#define LED_3 19 /*!< Green Led Direction 1 */
#define LED_4 21 /*!< Yellow Led Direction 1 */
#define LED_5 22 /*!< Red Led Direction 1 */

/*Direction 0 Buzzer*/
#define BUZZER_0 25 /*!< Sound Queue Direction 0 */
/*Direction 1 Buzzer*/
#define BUZZER_1 26 /*!< Sound Queue Direction 1 */
/*Direction 0 Walking Signal*/
#define WALK_0 32 /*!< Walk Signal Direction 0 */
/*Direction 1 Walking Signal*/
#define WALK_1 33 /*!< Walk Signal Direction 1 */
/*Traffic Density ADC Channel*/
#define TRAFFIC_DENSITY 34 /*!< Traffic Congestion */


/*Min and Max Values*/
#define MIN_ADC_VAL 0  /*!< Minimum adc value */
#define MAX_ADC_VAL 4096 /*!< Maximum adc value */
#define MIN_CARS 0 /*!< Minimum cars */
#define MAX_CARS 25  /*!< Maximum cars */

/* Logic Level */
#define LOW 0  /*!< Logic Level Low */
#define HIGH 1 /*!< Logic Level High */
#endif