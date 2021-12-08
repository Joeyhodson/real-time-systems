#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "msp430.h"
#include "main.h"
#include "uart_config.h"
#include "lcd.h"

#define RED_LED BIT0
#define GREEN_LED BIT7

#define RM_TASK1_PERIOD (pdMS_TO_TICKS(5000))
#define RM_TASK1_BUDGET (pdMS_TO_TICKS(1000))
#define RM_TASK1_PRIORITY (tskIDLE_PRIORITY + 4)

#define RM_TASK2_PERIOD (pdMS_TO_TICKS(10000))
#define RM_TASK2_BUDGET (pdMS_TO_TICKS(4000))
#define RM_TASK2_PRIORITY (tskIDLE_PRIORITY + 3)

#define RM_TASK3_PERIOD (pdMS_TO_TICKS(15000))
#define RM_TASK3_BUDGET (pdMS_TO_TICKS(4000))
#define RM_TASK3_PRIORITY (tskIDLE_PRIORITY + 2)

#define RM_TASK4_PERIOD (pdMS_TO_TICKS(25000))
#define RM_TASK4_BUDGET (pdMS_TO_TICKS(5000))
#define RM_TASK4_PRIORITY (tskIDLE_PRIORITY + 1)

void vRMTask1(void *pvParameters);
void vRMTask2(void *pvParameters);
void vRMTask3(void *pvParameters);
void vRMTask4(void *pvParameters);

TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;
TaskHandle_t xTask3Handle = NULL;
TaskHandle_t xTask4Handle = NULL;

TickType_t xTotalTickCount = 0;
TickType_t xTaskTick1 = 0;
TickType_t xTaskTick2 = 0;
TickType_t xTaskTick3 = 0;
TickType_t xTaskTick4 = 0;

void main_app_rm(void) {

    P1DIR |= RED_LED;
    P9DIR |= GREEN_LED;

    P1OUT &= ~RED_LED;
    P9OUT &= ~GREEN_LED;

    initialize_UART();
    initialize_lcd();

    xTaskCreate( vRMTask1, "task1", configMINIMAL_STACK_SIZE, NULL, RM_TASK1_PRIORITY, &xTask1Handle);
    xTaskCreate( vRMTask2, "task2", configMINIMAL_STACK_SIZE, NULL, RM_TASK2_PRIORITY, &xTask2Handle);
    xTaskCreate( vRMTask3, "task3", configMINIMAL_STACK_SIZE, NULL, RM_TASK3_PRIORITY, &xTask3Handle);
    xTaskCreate( vRMTask4, "task4", configMINIMAL_STACK_SIZE, NULL, RM_TASK4_PRIORITY, &xTask4Handle);

    vTaskStartScheduler();

    while(1);
}

void vRMTask1(void *pvParameters ) {

    const TickType_t xRdLEDFreq = RM_TASK1_PERIOD;
    TickType_t xLastWakeTime;

    char task_start[] = "S1 ";
    char task_end[] = "F1 ";

    TickType_t capacity = RM_TASK1_BUDGET;

    xLastWakeTime = xTaskGetTickCount();

    while(1) {

        display_num_lcd((unsigned int)xLastWakeTime);
        uart_write_string(task_start);
        uart_write_uint32(xLastWakeTime);

        P1OUT ^= RED_LED;

        while(xTaskTick1 != capacity) {
            /*
             * update_task_tick_count() updates the xTaskTick1 value at every tick
             */
        }

        xTaskTick1 = 0;

        P1OUT ^= RED_LED;

        TickType_t xFinishedTime = xTaskGetTickCount();
        display_num_lcd((unsigned int)xFinishedTime);
        uart_write_string(task_end);
        uart_write_uint32(xFinishedTime);
        uart_write_char('\n');

        vTaskDelayUntil(&xLastWakeTime, xRdLEDFreq);
    }
}

void vRMTask2(void *pvParameters ) {

    const TickType_t xRdLEDFreq = RM_TASK2_PERIOD;
    TickType_t xLastWakeTime;

    char task_start[] = "S2 ";
    char task_end[] = "F2 ";

    TickType_t capacity = RM_TASK2_BUDGET;

    xLastWakeTime = xTaskGetTickCount();

    while(1) {

        display_num_lcd((unsigned int)xLastWakeTime);
        uart_write_string(task_start);
        uart_write_uint32(xLastWakeTime);

        P1OUT ^= RED_LED;

        while(xTaskTick2 != capacity) {
            /*
             * update_task_tick_count() updates the xTaskTick1 value at every tick
             */
        }

        xTaskTick2 = 0;

        P1OUT ^= RED_LED;

        TickType_t xFinishedTime = xTaskGetTickCount();
        display_num_lcd((unsigned int)xFinishedTime);
        uart_write_string(task_end);
        uart_write_uint32(xFinishedTime);
        uart_write_char('\n');

        vTaskDelayUntil(&xLastWakeTime, xRdLEDFreq);
    }
}

void vRMTask3(void *pvParameters ) {

    const TickType_t xRdLEDFreq = RM_TASK3_PERIOD;
    TickType_t xLastWakeTime;

    char task_start[] = "S3 ";
    char task_end[] = "F3 ";

    TickType_t capacity = RM_TASK3_BUDGET;

    xLastWakeTime = xTaskGetTickCount();

    while(1) {

        display_num_lcd((unsigned int)xLastWakeTime);
        uart_write_string(task_start);
        uart_write_uint32(xLastWakeTime);

        P1OUT ^= RED_LED;

        while(xTaskTick3 != capacity) {
            /*
             * update_task_tick_count() updates the xTaskTick1 value at every tick
             */
        }

        xTaskTick3 = 0;

        P1OUT ^= RED_LED;

        TickType_t xFinishedTime = xTaskGetTickCount();
        display_num_lcd((unsigned int)xFinishedTime);
        uart_write_string(task_end);
        uart_write_uint32(xFinishedTime);
        uart_write_char('\n');

        vTaskDelayUntil(&xLastWakeTime, xRdLEDFreq);
    }
}

void vRMTask4(void *pvParameters ) {

    const TickType_t xRdLEDFreq = RM_TASK4_PERIOD;
    TickType_t xLastWakeTime;

    char task_start[] = "S4 ";
    char task_end[] = "F4 ";

    TickType_t capacity = RM_TASK4_BUDGET;

    xLastWakeTime = xTaskGetTickCount();

    while(1) {

        display_num_lcd((unsigned int)xLastWakeTime);
        uart_write_string(task_start);
        uart_write_uint32(xLastWakeTime);

        P1OUT ^= RED_LED;

        while(xTaskTick4 != capacity) {
            /*
             * update_task_tick_count() updates the xTaskTick1 value at every tick
             */
        }

        xTaskTick4 = 0;

        P1OUT ^= RED_LED;

        TickType_t xFinishedTime = xTaskGetTickCount();
        display_num_lcd((unsigned int)xFinishedTime);
        uart_write_string(task_end);
        uart_write_uint32(xFinishedTime);
        uart_write_char('\n');

        vTaskDelayUntil(&xLastWakeTime, xRdLEDFreq);
    }
}

void log_task_switched_in(char *taskName) {
    display_task_running_state(taskName);
}

void log_task_switched_out(char *taskName) {
    display_task_switched_out_state(taskName);
}

void log_task_to_ready_state(char *taskName) {
    display_task_ready_state(taskName);
}

void update_task_tick_counts(char * taskName, TickType_t currentTick) {
    if (strcmp(taskName, "task1") == 0)
        xTaskTick1 += (currentTick - xTotalTickCount);
    else if (strcmp(taskName, "task2") == 0)
        xTaskTick2 += (currentTick - xTotalTickCount);
    else if (strcmp(taskName, "task3") == 0)
        xTaskTick3 += (currentTick - xTotalTickCount);
    else if (strcmp(taskName, "task4") == 0)
        xTaskTick4 += (currentTick - xTotalTickCount);

    xTotalTickCount = currentTick;
}