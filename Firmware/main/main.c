/**
 * @file main.c
 * @brief main application
 * @author Jorge Minjares (https://github.com/JorgeMinjares)
 * @brief Main code
 * @version 0.1
 * @date 2022-11-24
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include "esp_timer.h"

/*FreeRTOS files*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

/* Configs files*/
#include <tlc_config.h>
#include <traffic_light.h>
#include "bsp/tlc_bsp.h"
#include "timer.h"

#include <driver/gpio.h>
#include <driver/dac.h>

#include "esp_log.h"
static const char* STATE_TAG = "STATE: "; /*!< String Tag to check current state*/
static const char* BUTTON_TAG = "BUTTON: "; /*!< String Tag to check button event */


esp_timer_handle_t timer_yellow_handle; /*!< One shot timer handle to yellow callback*/

TaskHandle_t light_task_handle = NULL; /*!< Task handle for Light Task*/
TaskHandle_t button_task_handle = NULL; /*!< Task handle for Button Task*/
TaskHandle_t yellow_task_handle = NULL; /*!< Task handle for Yellow Task*/
TaskHandle_t walk_task_handle = NULL; /*!< Task handle for Walk Task*/

QueueHandle_t adc_queue = NULL; /*!< Queue Variable to send data between tasks*/

SemaphoreHandle_t walk_semaphore = NULL; /*!< Semaphore to synchronize tasks*/

state_t state = GREEN; /*!< Global state_t variable to keep track of lights' state*/
uint8_t pedestrainTime = 0; /*!< Global variable that increase time for pedestrain walk*/
static bool isPressed = false; /*!< Boolean variable to check amount of times button event occurs*/
static bool isPressedOnce = false; /*!< Boolean variable to check if amount of times button event is not greater than 1*/
static bool halt = false; /*!< Halt if button event occurs*/

static char *banner="\033[1;33m   __  __________________ \r\n"
                                  "  / / / /_  __/ ____/ __ \\ \r\n"
                                 " / / / / / / / __/ / /_/ / \r\n"
                                "/ /_/ / / / / /___/ ____/ \r\n"
                                "\\____/ /_/ /_____/_/ \r\n \033[1;39m \r\n"; /*!< Banner for UTEP*/

/**
 * @brief Default tlc configuration
 */
tlc_t tlc[2] = {
    {
        .direction = DIRECTION_0,
        .led = {LED_0, LED_1, LED_2},
        .button = {BUTTON_0, BUTTON_1},
        .buzzer = BUZZER_0,
        .walkingSignal = WALK_0,
    },
    {
        .direction = DIRECTION_1,
        .led = {LED_3, LED_4, LED_5},
        .button = {BUTTON_2, BUTTON_3},
        .buzzer = BUZZER_1,
        .walkingSignal = WALK_1,
    },
};

/**
 * @brief Timer callback to trigger yellow state
 * 
 * @param arg generic argument
 */
void timer_yellow_callback(void *arg)
{
    /* Change state for the system */
    state = YELLOW;
    /* Display event with ESP_LOGGER */
    ESP_LOGI(STATE_TAG, "YELLOW");
    /* Send Notification */
    xTaskNotifyGive(yellow_task_handle);
}

/**
 * @brief Light task manages the traffic light states changes 
 * 
 * @param pvParameters generic argument 
 */
void light_task(void *pvParameters){

    while(1){
        /* If state is either GREEN or RED turn on the LED */
        if(state == GREEN || state == RED){
            tlc_bsp_lights(state, &tlc[0]);
            tlc_bsp_lights(state, &tlc[1]);
        }
        /* Toggle YELLOW LED on YELLOW State */
        else{
            tlc_bsp_yellow_toggle(&tlc[0], &tlc[1]);
        }
        /* Avoid WDT */
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Halt task changes the state of traffic light
 * 
 * @param pvParameters generic argument 
 */
void halt_light_task(void *pvParameters){
    while(1){
        /* Halt system if both buttons are pressed */
        if(tlc_bsp_button_read(&tlc[0]) && tlc_bsp_button_read(&tlc[1]) && halt == false){
            state = RED;
            halt = true;
            isPressed = false;
            isPressedOnce = false;
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        /* Restart system if both buttons are pressed again */
        if(tlc_bsp_button_read(&tlc[0]) && tlc_bsp_button_read(&tlc[1]) && halt == true){
            state = GREEN;
            halt = false;
            isPressed = false;
            isPressedOnce = false;
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        /* Avoid WDT */
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Button task starts one-shot timer and update pedestrian time
 * 
 * @param pvParameters generic argument
 */
void button_task(void *pvParameters)
{
    /* Set arguments for timer*/
    esp_timer_create_args_t timer_args = {
        .callback = timer_yellow_callback,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "Yellow Timer Trigger",
        .skip_unhandled_events = false,
    };
    /* Create timer with arguemnt and */
    esp_timer_create(&timer_args, &timer_yellow_handle);
    /* Default time period */
    uint32_t period = ONE_SECOND;

    while (1)
    {
        /* While system state is green and button read, start light sequence  */
        if (state == GREEN && (tlc_bsp_button_read(&tlc[0]) || tlc_bsp_button_read(&tlc[1])) && isPressedOnce == false)
        {
            /* Start One shot timer after 3 seconds */
            esp_timer_start_once(timer_yellow_handle, period * 3);
            /* Display event with ESP_LOGGER */
            ESP_LOGI(BUTTON_TAG, "PRESSED ONCE");
            isPressedOnce = true; /* Set event true */
            pedestrainTime = 15; /* Set time to 15 seconds */
        }
        /* If button is pressed and hold enable disability */
        if ((tlc_bsp_button_read(&tlc[0]) || tlc_bsp_button_read(&tlc[1])) && isPressed == false)
        {
            /* Wait 2 seconds and recheck buttons */
            vTaskDelay(2000 / portTICK_PERIOD_MS); 
            if((tlc_bsp_button_read(&tlc[0]) || tlc_bsp_button_read(&tlc[1]))){
                /* Increase time */
                pedestrainTime += 15;     
                /* Set event true*/
                isPressed = true; 
                /* Display event with ESP_LOGGER */
                ESP_LOGI(BUTTON_TAG, "PRESSED & HOLD");         
            }
        }
        else
        {
            /* Avoid WDT */
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
}

/**
 * @brief Yellow task controls yellow->red transition 
 * 
 * @param pvParameters generic argument
 */
void yellow_task(void *pvParameters)
{
    while (1)
    {   
        /* Wait for notification */
        uint32_t count = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        vTaskDelay( 5000 / portTICK_PERIOD_MS);
        /* Update state after 5 seconds */
        state = RED;
        /* Display event in ESP_LOGGER */
        ESP_LOGI(STATE_TAG, "RED");
        /* Send semaphore */
        xSemaphoreGive(walk_semaphore);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Walk task manages red state transition and display walking signals (LEDs, Buzzer)
 * 
 * @param pvParameters 
 */
void walk_task(void *pvParameters)
{
    while (1)
    {   /* Wait to receive semaphore */
        if (xSemaphoreTake(walk_semaphore, portMAX_DELAY) == pdTRUE)
        {
            /* Iterate time */
            while (pedestrainTime-- > 0)
            {
                /* Trigger Walk Signal */
                if (pedestrainTime > 10)
                {
                    tlc_bsp_walk_on(&tlc[0]);
                    tlc_bsp_walk_on(&tlc[1]);
                }
                /* Trigger Walk Signal Warning */
                else
                {   /* Disability Enable */
                    if(isPressed == true){
                        tlc_bsp_buzzer_on(&tlc[0], 126);
                        vTaskDelay(100 / portTICK_PERIOD_MS);
                        tlc_bsp_buzzer_off(&tlc[0]);
                        tlc_bsp_buzzer_on(&tlc[1], 126);
                        vTaskDelay(100 / portTICK_PERIOD_MS);
                        tlc_bsp_buzzer_off(&tlc[1]);
                    }
                    tlc_bsp_walk_warning(&tlc[0]);
                    tlc_bsp_walk_warning(&tlc[1]);
                }
                vTaskDelay(500 / portTICK_PERIOD_MS);
            };
            /* Update State */
            state = GREEN;
            /* Display event in ESP_LOGGER */
            ESP_LOGI(STATE_TAG, "GREEN");
            /* Restart events to false*/
            isPressed = false;
            isPressedOnce = false;
        }
        /* Avoid WDT */
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief ADC task read traffic congestions and sends data through queue
 * 
 * @param pvParameters generic argument 
 */
void adc_task(void *pvParameters)
{
    while (1)
    {
        /* Read ADC channel */
        uint32_t adc = tlc_bsp_adc_read();
        /* Map ADC values 
           MIN_ADC_VAL - MAX_ADC_VAL (0 - 4096)
           MIN_CARS - MAX_CARS (0 - 25)
        */
        uint16_t adc_map = map(adc, MIN_ADC_VAL, MAX_ADC_VAL, MIN_CARS, MAX_CARS);
        /* Send Mapped Values through queue */
        xQueueSendToBack(adc_queue, &adc_map, 0);
        /* Avoid WDT */
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief UART task receives queue and display message via serial port
 * 
 * @param pvParameters generic argument 
 */
void uart_task(void *pvParamters){
    /* Variable to store queue information */
    uint16_t adc_map = 0;
    while(1){
        /* Receive queue information and store it in variable */
        if(xQueueReceive(adc_queue, &adc_map, (TickType_t)100) == pdPASS){
            /* Create buffer array for messages */
            char buffer[32];
            /* Set meesage to send */
            sprintf(buffer, "Traffic Congestion: %d\r\n", adc_map);
            /* Send message to UART */
            tlc_bsp_uart_write_byte(buffer);
            /* Send message if exceeds threshold */
            if((MAX_CARS / 2) < adc_map){
                tlc_bsp_uart_write_byte("\033[1;31m Whoa Traffic is Heavy\033[1;39m\r\n");
            }
        }
    }
}

void app_main(void)
{
    /* Initialize TLC hardware */
    tlc_bsp_init(&tlc[0]);
    tlc_bsp_init(&tlc[1]);
    /* Initialize TLC UART communication */
    tlc_bsp_uart_init();
    /* Default State based on Direction_0 
       North-South -> GREEN
       East-West -> RED
     */
    if(DIRECTION_0 == NORTH){
        state = GREEN;
        ESP_LOGI(STATE_TAG, "GREEN");
    }
    else{
        state = RED;
        ESP_LOGI(STATE_TAG, "RED");
    }
    /* Display Banner through UART */
    tlc_bsp_uart_write_byte(banner);
    /* Create Binary Semaphore */    
    walk_semaphore = xSemaphoreCreateBinary();
    /* Create Queue of size 2 */
    adc_queue = xQueueCreate(2, sizeof(uint16_t));
    /* Wait until Semaphore is created */
    if (walk_semaphore != NULL)
    {
        /* Create Tasks */
        xTaskCreate(&halt_light_task, "Halt Lights Task", 2048, NULL, 15, NULL);
        xTaskCreate(&light_task, "light_task", 2048, NULL, 5, &light_task_handle);
        xTaskCreate(&button_task, "button_task", 2048, NULL, 10, &button_task_handle);
        xTaskCreate(&yellow_task, "yellow_task", 2048, NULL, 5, &yellow_task_handle);
        xTaskCreate(&walk_task, "walk_task", 2048, NULL, 5, &walk_task_handle);
        xTaskCreate(&adc_task, "adc_task", 2048, NULL, 10, NULL);
        xTaskCreate(&uart_task, "uart_task", 2048, NULL, 10, NULL);

    }
}