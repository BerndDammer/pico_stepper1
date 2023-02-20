#include "loop.h"
#include "blinker.h"
#include "motor.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"

#define CONSOLE_TIMEOUT 1000000
#define STEP_MAX 3000
#define STEP_STEP 300
#define STEP_START 300

void menu(void)
{
    printf("------------------------------------\n");
    printf("1 2 3 4 switch coil on \n");
    printf("0 all coils off\n");
    printf("ESC exit\n");
    printf("press key to select\n");
    printf("------------------------------------\n");
}

void loop_it()
{
    int c;
    int step = 1;
    printf("Stepping\n");
    for (;;)
    {
        c = getchar_timeout_us(50 * 1000);
        blinker_toggle();

        if (c != PICO_ERROR_TIMEOUT)
        {
            return;
        }
        else
        {
            step++;
            if (step > 4)
                step = 1;
            motor_set_single(step);
        }
    }
    printf("Stepping stopped\n");
}

void loop()
{
    volatile int c; // make visible in debugger; avoid optimize out
    int counter = 0;

    motor_init();
    int step_value = STEP_START;
    motor_set(step_value);

    menu();

    for (;;)
    {
        c = getchar_timeout_us(CONSOLE_TIMEOUT);
        blinker_toggle();

        if (c == PICO_ERROR_TIMEOUT)
        {
            printf("Microstepping Loop Counter %i\n", counter);
            counter++;
        }
        else
        {
            switch (c)
            {
            case '1':
                motor_set_single(1);
                break;
            case '2':
                motor_set_single(2);
                break;
            case '3':
                motor_set_single(3);
                break;
            case '4':
                motor_set_single(4);
                break;
            case 'l':
                loop_it();
                break;
            case ' ':
            case '0':
                motor_set_single(0);
                break;
            case 27:
                return;
                break;
            default:
                menu();
                break;
            }
        }
    }
}
