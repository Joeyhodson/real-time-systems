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
#define BUT1 BIT1
#define BUT2 BIT2

#define RM_TASK1_PERIOD (pdMS_TO_TICKS( 3000))
#define RM_TASK1_BUDGET (pdMS_TO_TICKS( 1000))
#define RM_TASK1_PRIORITY (tskIDLE_PRIORITY + 4)

#define RM_TASK2_PERIOD (pdMS_TO_TICKS( 6000))
#define RM_TASK2_BUDGET (pdMS_TO_TICKS( 1000))
#define RM_TASK2_PRIORITY (tskIDLE_PRIORITY + 3)

#define RM_SPORADIC_TASK1_PERIOD (pdMS_TO_TICKS( 8000))
#define RM_SPORADIC_TASK1_BUDGET (pdMS_TO_TICKS( 1000))
#define RM_SPORADIC_TASK1_PRIORITY (tskIDLE_PRIORITY + 2)

#define RM_SPORADIC_TASK2_PERIOD (pdMS_TO_TICKS( 9000))
#define RM_SPORADIC_TASK2_BUDGET (pdMS_TO_TICKS( 2000))
#define RM_SPORADIC_TASK2_PRIORITY (tskIDLE_PRIORITY + 1)

void vRMTask1(void *pvParameters);
void vRMTask2(void *pvParameters);
void vRMSporadicTask1(void *pvParameters);
void vRMSporadicTask2(void *pvParameters);

TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;
TaskHandle_t xSporadicTask1Handle = NULL;
TaskHandle_t xSporadicTask2Handle = NULL;

TickType_t xTotalTickCount = 0;
TickType_t xTaskTick1 = 0;
TickType_t xTaskTick2 = 0;
TickType_t xSporadicTaskTick1 = 0;
TickType_t xSporadicTaskTick2 = 0;

void main_app_rm_sporadic(void) {

    P1DIR |= RED_LED;
    P9DIR |= GREEN_LED;

    P1OUT &= ~RED_LED;
    P9OUT &= ~GREEN_LED;

    P1DIR &= ~(BUT1|BUT2);
    P1REN |= (BUT1|BUT2);
    P1OUT |= (BUT1|BUT2);
    P1IE |= (BUT1|BUT2);
    P1IES |= (BUT1|BUT2);
    P1IFG &= ~(BUT1|BUT2);

    _enable_interrupts();

    initialize_UART();
    initialize_lcd();

    xTaskCreate( vRMTask1, "task1", configMINIMAL_STACK_SIZE, NULL, RM_TASK1_PRIORITY, &xTask1Handle);
    xTaskCreate( vRMTask2, "task2", configMINIMAL_STACK_SIZE, NULL, RM_TASK2_PRIORITY, &xTask2Handle);
    xTaskCreate( vRMSporadicTask1, "sporadic_task1", configMINIMAL_STACK_SIZE, NULL, RM_SPORADIC_TASK1_PRIORITY, &xSporadicTask1Handle);
    xTaskCreate( vRMSporadicTask2, "sporadic_task2", configMINIMAL_STACK_SIZE, NULL, RM_SPORADIC_TASK2_PRIORITY, &xSporadicTask2Handle);

    // Suspend sporadic tasks before starting scheduler
    vTaskSuspend(xSporadicTask1Handle);
    vTaskSuspend(xSporadicTask2Handle);

    vTaskStartScheduler();
    while(1);
}

void update_task_tick_counts(char * taskName, TickType_t currentTick) {

    if (strcmp(taskName, "task1") == 0)
        xTaskTick1 += (currentTick - xTotalTickCount);
    else if (strcmp(taskName, "task2") == 0)
        xTaskTick2 += (currentTick - xTotalTickCount);
    else if (strcmp(taskName, "sporadictask1") == 0)
        xSporadicTaskTick1 += (currentTick - xTotalTickCount);
    else if (strcmp(taskName, "sporadictask2") == 0)
        xSporadicTaskTick2 += (currentTick - xTotalTickCount);

    xTotalTickCount = currentTick;
}

void vRMTask1(void *pvParameters) {

    const TickType_t xRdLEDFreq = RM_TASK1_PERIOD;
    TickType_t xLastWakeTime;

    char task_start[] = "S1 ";
    char task_end[] = "F1 ";

    TickType_t capacity = RM_TASK1_BUDGET;

    while(1) {

        xLastWakeTime = xTaskGetTickCount();
        display_num_lcd((unsigned int)xLastWakeTime);
        uart_write_string(task_start);
        uart_write_uint32(xLastWakeTime);

        P1OUT ^= RED_LED;

        while(xTaskTick1 != capacity) {
            update_task_tick_counts("task1", xTaskGetTickCount());
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

void vRMTask2(void *pvParameters) {

    const TickType_t xRdLEDFreq = RM_TASK2_PERIOD;
    TickType_t xLastWakeTime;

    char task_start[] = "S2 ";
    char task_end[] = "F2 ";

    TickType_t capacity = RM_TASK2_BUDGET;

    while(1) {

        xLastWakeTime = xTaskGetTickCount();
        display_num_lcd((unsigned int)xLastWakeTime);
        uart_write_string(task_start);
        uart_write_uint32(xLastWakeTime);

        P1OUT ^= RED_LED;

        while(xTaskTick2 != capacity) {
            update_task_tick_counts("task2", xTaskGetTickCount());
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

void vRMSporadicTask1(void *pvParameters) {

    const TickType_t xRdLEDFreq = RM_SPORADIC_TASK1_PERIOD;
    TickType_t xLastWakeTime;

    char task_start[] = "S3 ";
    char task_end[] = "F3 ";

    TickType_t capacity = RM_SPORADIC_TASK1_BUDGET;

    while(1) {

        xLastWakeTime = xTaskGetTickCount();
        display_num_lcd((unsigned int)xLastWakeTime);
        uart_write_string(task_start);
        uart_write_uint32(xLastWakeTime);

        P9OUT ^= GREEN_LED;

        while(xSporadicTaskTick1 != capacity) {
            update_task_tick_counts("sporadictask1", xTaskGetTickCount());
        }

        xSporadicTaskTick1 = 0;
        P9OUT ^= GREEN_LED;

        TickType_t xFinishedTime = xTaskGetTickCount();
        display_num_lcd((unsigned int)xFinishedTime);
        uart_write_string(task_end);
        uart_write_uint32(xFinishedTime);
        uart_write_char('\n');

        vTaskDelayUntil(&xLastWakeTime, xRdLEDFreq);
        vTaskSuspend(xSporadicTask1Handle);
    }
}

void vRMSporadicTask2(void *pvParameters) {

    const TickType_t xRdLEDFreq = RM_SPORADIC_TASK2_PERIOD;
    TickType_t xLastWakeTime;

    char task_start[] = "S4 ";
    char task_end[] = "F4 ";

    TickType_t capacity = RM_SPORADIC_TASK2_BUDGET;

    while(1) {

        xLastWakeTime = xTaskGetTickCount();
        display_num_lcd((unsigned int)xLastWakeTime);
        uart_write_string(task_start);
        uart_write_uint32(xLastWakeTime);

        P9OUT ^= GREEN_LED;

        while(xSporadicTaskTick2 != capacity) {
            update_task_tick_counts("sporadictask2", xTaskGetTickCount());
        }

        xSporadicTaskTick2 = 0;
        P9OUT ^= GREEN_LED;

        TickType_t xFinishedTime = xTaskGetTickCount();
        display_num_lcd((unsigned int)xFinishedTime);
        uart_write_string(task_end);
        uart_write_uint32(xFinishedTime);
        uart_write_char('\n');

        vTaskDelayUntil(&xLastWakeTime, xRdLEDFreq);
        vTaskSuspend(xSporadicTask2Handle);
    }
}


#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR() {

    BaseType_t xHigherPriorityTaskWoken;

    if ((P1IFG & BUT1) == BUT1) {
        xHigherPriorityTaskWoken = xTaskResumeFromISR(xSporadicTask1Handle);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        P1IFG &= ~BUT1;
    }
    if ((P1IFG & BUT2) == BUT2) {
        xHigherPriorityTaskWoken = xTaskResumeFromISR(xSporadicTask2Handle);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        P1IFG &= ~BUT2;
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