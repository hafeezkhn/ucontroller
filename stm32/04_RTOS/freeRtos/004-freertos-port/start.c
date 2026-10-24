#include "FreeRTOS.h"
#include "task.h"


static void vTask1(void *pvParameters)
{
   volatile int a = 0;
    while(1)
    {
        a++;// Task 1 code
    };
}

static void vTask2(void *pvParameters)
{
   volatile int b = 0;
    while(1)
    {
        b++;// Task 2 code
    };
}

extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _sidata;

void start(void)
{


    uint32_t *src = &_sidata;
    uint32_t *dest = &_sdata;
    while (dest < &_edata)
    {
        *dest++ = *src++;
    }

    // Initialize the BSS section
    uint32_t *bss_ptr = &_sbss;
    while (bss_ptr < &_ebss)
    {
        *bss_ptr++ = 0;
    }

   xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
   xTaskCreate(vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
   vTaskStartScheduler();
   return;
}

