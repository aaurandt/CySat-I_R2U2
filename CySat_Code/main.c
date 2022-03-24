#include "main.h"
#include "MCU_Init.h"
#include "fatfs.h"
#include "version.h"
#include "Payload.h"

#include "helper_functions.h"
#include "EPS.h"
#include "UHF.h"
#include "test.h"

#include "AppTasks.h"

#include  <errno.h>
#include  <sys/unistd.h>
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>

osMutexId EPS_I2C_Mutex;
osMutexId UART_Mutex;
osMutexId ADCS_Active_Mutex;
osMutexId Low_Power_Mode_Mutex;
osThreadId myUHFTask;
osThreadId myADCSTask;
osThreadId myMainTask;

int main(void)
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_UART4_Init();
    MX_USART6_UART_Init();
    MX_SDIO_SD_Init();
    MX_FATFS_Init();


    osMutexDef(EPS_I2C_Mutex);
    EPS_I2C_Mutex = osMutexCreate(osMutex(EPS_I2C_Mutex));
    osMutexDef(UART_Mutex);
    UART_Mutex = osMutexCreate(osMutex(UART_Mutex));
    osMutexDef(ADCS_Active_Mutex);
    ADCS_Active_Mutex = osMutexCreate(osMutex(ADCS_Active_Mutex));
    osMutexDef(Low_Power_Mode_Mutex);
    Low_Power_Mode_Mutex = osMutexCreate(osMutex(Low_Power_Mode_Mutex));

    osThreadDef(myMainTask, Main_Task, osPriorityAboveNormal, 0, 512);
    osThreadCreate(osThread(myMainTask), NULL);

    osThreadDef(myUHFTask, UHF_Task, osPriorityNormal, 0, 512);
    osThreadCreate(osThread(myUHFTask), NULL);

    osThreadDef(myR2U2Task, R2U2_Task, osPriorityLow, 0, 1024);
    osThreadCreate(osThread(myR2U2Task), NULL);

    osThreadDef(myADCSTask, ADCS_Task, osPriorityHigh, 0, 1024);
    osThreadCreate(osThread(myADCSTask), NULL);

    osThreadDef(myBatteryCapacityTask, BatteryCapacity_Task, osPriorityRealtime, 0, 512);
    osThreadCreate(osThread(myBatteryCapacityTask), NULL);

    /* Start scheduler */
    osKernelStart();

    while(1){

    }
}
