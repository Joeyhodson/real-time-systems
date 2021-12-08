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


#define EDF_TASK1_PERIOD (pdMS_TO_TICKS( 500))
#define EDF_TASK2_PERIOD (pdMS_TO_TICKS( 1000))
#define EDF_TASK3_PERIOD (pdMS_TO_TICKS( 1500))
#define EDF_TASK4_PERIOD (pdMS_TO_TICKS( 3000))

#define EDF_TASK1_BUDGET (pdMS_TO_TICKS( 200))
#define EDF_TASK2_BUDGET (pdMS_TO_TICKS( 300))
#define EDF_TASK3_BUDGET (pdMS_TO_TICKS( 500))
#define EDF_TASK4_BUDGET (pdMS_TO_TICKS( 600))

void vEDFTask1(void *pvParameters);
void vEDFTask2(void *pvParameters);
void vEDFTask3(void *pvParameters);
void vEDFTask4(void *pvParameters);

void log_task_to_ready_state(char *taskName);
void log_task_switched_out(char *taskName);
void log_task_switched_in(char *taskName);

TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;
TaskHandle_t xTask3Handle = NULL;
TaskHandle_t xTask4Handle = NULL;

TickType_t xTotalTickCount = 0;
TickType_t xTaskTick1 = 0;
TickType_t xTaskTick2 = 0;
TickType_t xTaskTick3 = 0;
TickType_t xTaskTick4 = 0;

void main_app_edf(void) {

    P1DIR |= RED_LED;
    P9DIR |= GREEN_LED;

    P1OUT &= ~RED_LED;
    P9OUT &= ~GREEN_LED;

    initialize_UART();
    initialize_lcd();
    
    xTaskPeriodicCreate( vEDFTask1, "task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle, EDF_TASK1_PERIOD);
    xTaskPeriodicCreate( vEDFTask2, "task2", configMINIMAL_STACK_SIZE, NULL, 1, &xTask2Handle, EDF_TASK2_PERIOD);
    xTaskPeriodicCreate( vEDFTask3, "task3", configMINIMAL_STACK_SIZE, NULL, 1, &xTask3Handle, EDF_TASK3_PERIOD);
    xTaskPeriodicCreate( vEDFTask4, "task4", configMINIMAL_STACK_SIZE, NULL, 1, &xTask4Handle, EDF_TASK4_PERIOD);

    vTaskStartScheduler();

    while(1);
}

void vEDFTask1(void *pvParameters) {

    const TickType_t xTask1Freq = EDF_TASK1_PERIOD;
    TickType_t xLastWakeTime;

    char task_start[] = "S1 ";
    char task_end[] = "F1 ";

    TickType_t capacity = EDF_TASK1_BUDGET;
    // TickType_t count = capacity;

    xLastWakeTime = xTaskGetTickCount();

    while(1) {

        display_num_lcd((unsigned int)xTotalTickCount);
        uart_write_string(task_start);
        uart_write_uint32(xTotalTickCount);

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

        vTaskDelayUntil(&xLastWakeTime, xTask1Freq);
    }
}

void vEDFTask2(void *pvParameters) {

       const TickType_t xTask2Freq = EDF_TASK2_PERIOD;
       TickType_t xLastWakeTime;

       char task_start[] = "S2 ";
       char task_end[] = "F2 ";

       TickType_t capacity = EDF_TASK2_BUDGET;

       xLastWakeTime = xTaskGetTickCount();

       while(1) {

           display_num_lcd((unsigned int)xTotalTickCount);
           uart_write_string(task_start);
           uart_write_uint32(xTotalTickCount);

           P9OUT ^= GREEN_LED;

           while(xTaskTick2 != capacity) {
            /*
             * update_task_tick_count() updates the xTaskTick1 value at every tick
             */
           }

           xTaskTick2 = 0;

           P9OUT ^= GREEN_LED;

           TickType_t xFinishedTime = xTaskGetTickCount();
           display_num_lcd((unsigned int)xFinishedTime);
           uart_write_string(task_end);
           uart_write_uint32(xFinishedTime);
           uart_write_char('\n');

           vTaskDelayUntil(&xLastWakeTime, xTask2Freq);
       }
}

void vEDFTask3(void *pvParameters)
{
    const TickType_t xTask3Freq = EDF_TASK3_PERIOD;
    TickType_t xLastWakeTime;

    char task_start[] = "S3 ";
    char task_end[] = "F3 ";

    TickType_t capacity = EDF_TASK3_BUDGET;

    xLastWakeTime = xTaskGetTickCount();

    while(1) {

        display_num_lcd((unsigned int)xTotalTickCount);
        uart_write_string(task_start);
        uart_write_uint32(xTotalTickCount);

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

        vTaskDelayUntil(&xLastWakeTime, xTask3Freq);
    }
}

void vEDFTask4(void *pvParameters) {

    const TickType_t xTask4Freq = EDF_TASK4_PERIOD;
    TickType_t xLastWakeTime;

    char task_start[] = "S4 ";
    char task_end[] = "F4 ";

    TickType_t capacity = EDF_TASK4_BUDGET;

    xLastWakeTime = xTaskGetTickCount();

    while(1) {

        display_num_lcd((unsigned int)xTotalTickCount);
        uart_write_string(task_start);
        uart_write_uint32(xTotalTickCount);

        P9OUT ^= GREEN_LED;

        while(xTaskTick4 != capacity) {
            /*
             * update_task_tick_count() updates the xTaskTick1 value at every tick
             */
        }

        xTaskTick4 = 0;

        P9OUT ^= GREEN_LED;

        TickType_t xFinishedTime = xTaskGetTickCount();
        display_num_lcd((unsigned int)xFinishedTime);
        uart_write_string(task_end);
        uart_write_uint32(xFinishedTime);
        uart_write_char('\n');

        vTaskDelayUntil(&xLastWakeTime, xTask4Freq);
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