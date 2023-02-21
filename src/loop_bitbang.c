#include "loop_bitbang.h"
#include "blinker.h"
#include "motor.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"

#define TIMEOUT_MIN 300
#define TIMEOUT_MAX 500000
#define DSTEP 1.2

void menu_bitbang(void)
{
    printf("------------------------------------\n");
    printf("Bitbang\n");
    printf("1 Full stepping\n");
    printf("2 Half Stepping\n");
    printf("4 Half Stepping\n");
    printf("+ Increase Frequency\n");
    printf("- Decrease Frequency\n");
    printf("r Reverse direction\n");
    printf("c Countdown\n");
    printf("ESC  exit\n");
    printf("press key to select\n");
    printf("------------------------------------\n");
}

const int pins[4] =
{
COIL1,
COIL2,
COIL3,
COIL4, };

// mode step pin
int outs[4][8][4] =
{
{
{ false, false, false, false },
{ false, false, false, false },
{ false, false, false, false },
{ false, false, false, false },
{ false, false, false, false },
{ false, false, false, false },
{ false, false, false, false },
{ false, false, false, false } },
{
{ true, true, false, false },
{ true, true, false, false },
{ false, true, true, false },
{ false, true, true, false },
{ false, false, true, true },
{ false, false, true, true },
{ true, false, false, true },
{ true, false, false, true } },
{
{ true, false, false, false },
{ true, true, false, false },
{ false, true, false, false },
{ false, true, true, false },
{ false, false, true, false },
{ false, false, true, true },
{ false, false, false, true },
{ true, false, false, true } },
{
{ true, false, false, false },
{ true, false, false, false },
{ false, true, false, false },
{ false, true, false, false },
{ false, false, true, false },
{ false, false, true, false },
{ false, false, false, true },
{ false, false, false, true }

} };

void bitbang_init(void)
{
    for (int i = 0; i < 4; i++)
    {
        int pin = pins[i];

        gpio_init(pin);
        //gpio_set_function(pin, GPIO_FUNC_);
        gpio_set_dir(pin, GPIO_OUT);
        gpio_put(pin, false);
    }
}

void bitbang_deinit(void)
{
    for (int i = 0; i < 4; i++)
    {
        int pin = pins[i];

        gpio_init(pin);
        gpio_set_function(pin, GPIO_FUNC_NULL);
        gpio_set_dir(pin, GPIO_IN);
        gpio_put(pin, false);
    }
}

void loop_bitbang(void)
{
    volatile int c; // make visible in debugger; avoid optimize out
    int menu_counter = 0;
    int delay = TIMEOUT_MAX;
    int forward = true;
    int countdown = 0;
    int mode = 0;
    int step = 0;

    menu_bitbang();
    bitbang_init();

    for (;;)
    {
        c = getchar_timeout_us(delay);
        blinker_toggle();

        if (c == PICO_ERROR_TIMEOUT)
        {
            //printf("Bitbang Loop Counter %i\n", counter);
            menu_counter++;
            for (int i = 0; i < 4; i++)
            {
                volatile int p = pins[i];
                volatile int v = outs[mode][step][i];
                gpio_put(p, v);
                //gpio_put(pins[i], outs[mode][step][i]);
            }
            step = (forward ? ++step : --step) & 7;
            if(countdown != 0)
            {
                countdown--;
                if(countdown == 0)
                {
                    mode = 0;
                }
            }
        }
        else
        {
            switch (c)
            {
            case '1':
                mode = 1;
                break;
            case '2':
                mode = 2;
                break;
            case '4':
                mode = 3;
                break;
            case '+':
                delay = (int) ((double) delay / DSTEP);
                if (delay < TIMEOUT_MIN)
                    delay = TIMEOUT_MIN;
                break;
            case '-':
                delay = (int) ((double) delay * DSTEP);
                if (delay > TIMEOUT_MAX)
                    delay = TIMEOUT_MAX;
                break;
            case 'r':
                forward = !forward;
                break;
            case 'c':
                countdown = 64 *64;
                break;
            case ' ':
            case '0':
                mode = 0;
                break;
            case 27:
                goto deinit;
                break;
            default:
                menu_bitbang();
                break;
            }
        }
    }
    deinit: bitbang_deinit();
}
