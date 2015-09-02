/*
 * FreeRTOS_Leds_Example.c
 *
 * Created: 28-Aug-15 00:20:41
 *  Author: Денис Васильковский
 *
 *   About: Пример использования операционной системы реального времени
 *          FreeRTOS для того, чтобы поморгать светодиодами, подключенными
 *          к PA0 и PA1. Некратные промежутки моргания демонстрируют
 *          одну из ключевых возможностей операционной системы - создание
 *          независимых потоков вычислений.
 *
 *          Кроме этого, третий процесс используется для выполнения условной
 *          "сложной и длинной задачи", требующей несколько секунд
 *          процессорного времени для полного исполнения цикла. Светодиод PA3,
 *          а также ЖКИ информируют пользователя тогда, когда эта задача
 *          находится в активной стадии вычислений. Поскольку эта задача
 *          имеет более низкий приоритет, чем задачи моргания светодиодами,
 *          то стадия вычислений математической задачи (активна/ждет) не влияет
 *          на частоту моргания светодиодов PA0, PA1.
 *
 *          В ОС включена функция, индикации простоя, поэтому если при
 *          переключении контекста будет обнаружено, что ни одна задача не
 *          готова к исполнению, будет вызвана функция vApplicationIdleHook.
 *          Внутри этой функции происходит переключение светодиода PA7. Таким
 *          образом, интервалы переключения PA7 можно трактовать как свободное
 *          процессорное время. При работе данной программы на плате можно
 *          визуально наблюдать, что свободное процессорное время есть только
 *          при спящей "сложной и длинной" задаче.
 */ 

#include <avr/io.h>
#include <stddef.h>
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hd44780.h"

/* Код задачи моргания светодиодом PA0.*/
static void task_blink_pa0(void *pvParameters) {
    // эта переменная хранит количество байт ни разу
    // не использованного стека задачи
    static volatile UBaseType_t blink_pa0_stack_size;
    
    for(;;)
    {
        // пересчет верхней границы использования стека
        blink_pa0_stack_size = uxTaskGetStackHighWaterMark(NULL);
        
        // основной код задачи
        taskENTER_CRITICAL();
        PORTA ^= 1 << PA0;
        taskEXIT_CRITICAL();
        
        // блокировка задачи
        vTaskDelay( 667 );
    }
    (void)blink_pa0_stack_size;
}

/* Код задачи моргания светодиодом PA1.*/
static void task_blink_pa1(void *pvParameters) {
    // эта переменная хранит количество байт ни разу
    // не использованного стека задачи
    static volatile UBaseType_t blink_pa1_stack_size;
    
    for(;;)
    {
        // пересчет верхней границы использования стека
        blink_pa1_stack_size = uxTaskGetStackHighWaterMark(NULL);
        
        // основной код задачи
        taskENTER_CRITICAL();
        PORTA ^= 1 << PA1;
        taskEXIT_CRITICAL();
        
        // блокировка задачи
        vTaskDelay( 900 );
    }
    
    (void)blink_pa1_stack_size;
}

/* Код задачи, связанный с повышенной вычислительной нагрузкой.

Время исполнения задачи очень велико, поэтому желательно назначить
ей низкий приоритет.*/
static void task_heavy_math(void *pvParameters) {
    // эта переменная хранит количество байт ни разу
    // не использованного стека задачи
    static volatile UBaseType_t heavy_math_stack_size;
    
    // инициализация жки и вывод первоначальной информации
    hd44780_init(Phys2Row5x8, OutNorm, CursorOff);
    hd44780_gotoxy(0, 0);
    hd44780_puts(pcTaskGetTaskName(NULL));
    
    for(;;)
    {
        // пересчет верхней границы использования стека
        heavy_math_stack_size = uxTaskGetStackHighWaterMark(NULL);
        
        // индикация занятости светодиодом
        taskENTER_CRITICAL();
        PORTA |= 1 << PA2;
        // сбрасываем также светодиод индикации простоя
        PORTA &=~(1 << PA7);
        taskEXIT_CRITICAL();
        
        // индикация занятости на жки
        hd44780_gotoxy(0, 1);
        hd44780_puts("Working...");
        
        // длинная и трудная задача
        volatile uint32_t sum = 0;
        for (volatile uint32_t i = 0; i < 524288; i++) {
            sum += i;
        }
        
        // индикация освобождения на жки
        hd44780_gotoxy(0, 1);
        hd44780_puts("Sleeps... ");
        
        // индикация освобождения на светодиоде
        taskENTER_CRITICAL();
        PORTA &=~(1 << PA2);
        taskEXIT_CRITICAL();
        
        // блокировка задачи
        vTaskDelay( 5000 );
    }
    (void)heavy_math_stack_size;
}

/* Эта функция вызывается, если есть свободное процессорное
время. Она никогда не должна приводить к блокировке, поскольку
не выгружается при системном тике. */
void vApplicationIdleHook(void *pvParameters) {
    // если ни одна задача не потребляет
    // процессорного времени, то происходит
    // переключение светодиода
    taskENTER_CRITICAL();
    PORTA ^= 1 << PA7;
    taskEXIT_CRITICAL();
}

int main(void)
{
    // создание задач
    xTaskCreate(task_blink_pa0,  "Blink PA0",  200, NULL, tskIDLE_PRIORITY + 3, NULL);
    xTaskCreate(task_blink_pa1,  "Blink PA1",  200, NULL, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(task_heavy_math, "Heavy math", 400, NULL, tskIDLE_PRIORITY + 1, NULL);
    
    // настройка оборудования
    DDRA |= (1 << PA7) | (1 << PA2) | (1 << PA1) | (1 << PA0);
    
    // запуск операционной системы
    vTaskStartScheduler();
    
    // на эту строчку программа перейдет только в случае ошибки
    return 0;
}