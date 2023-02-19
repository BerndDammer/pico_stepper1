#include <stdbool.h>

#include "pico/stdlib.h"
#include "hardware/pwm.h"

#include "motor.h"

// Pin 21 Slice 0 Channel A GPIO 16
// Pin 22 Slice 0 Channel B GPIO 17
// Pin 19 Slice 7 Channel A GPIO 14
// Pin 20 Slice 7 Channel B GPIO 15

#define COIL1 16
#define COIL2 17
#define COIL3 14
#define COIL4 15

#define SLICE1 0
#define SLICE2 7

#define WRAP 3000
#define OFF_CHANNEL 0
#define ON_CHANNEL 3000

typedef struct
{
    uint slice;
    int moth;
    int motl;
    pwm_config pc;

} s_cs_mot;

s_cs_mot cs_mot1 =
{ SLICE1, COIL1, COIL2,
{

} };

s_cs_mot cs_mot2 =
{ SLICE2, COIL3, COIL4,
{

} };

void motor_init2(s_cs_mot *s)
{
    // Tell GPIO they are allocated to the PWM
    gpio_set_function(s->moth, GPIO_FUNC_PWM);
    gpio_set_function(s->motl, GPIO_FUNC_PWM);

    s->pc = pwm_get_default_config();
    pwm_config_set_clkdiv_mode(&s->pc, PWM_DIV_FREE_RUNNING);
    pwm_config_set_clkdiv(&s->pc, WRAP);

    s->pc.div = 0XFFFFFF;
    s->pc.top = WRAP;

    pwm_init(s->slice, &s->pc, false);

    pwm_set_chan_level(s->slice, PWM_CHAN_A, OFF_CHANNEL);
    pwm_set_chan_level(s->slice, PWM_CHAN_B, OFF_CHANNEL);

    pwm_set_enabled(s->slice, true);
}

void motor_init()
{
    motor_init2(&cs_mot1);
    motor_init2(&cs_mot2);
}

void motor_set_single(int coil_number)
{
    switch (coil_number)
    {
    case 1:
        pwm_set_chan_level(cs_mot1.slice, PWM_CHAN_A, ON_CHANNEL);
        pwm_set_chan_level(cs_mot1.slice, PWM_CHAN_B, ON_CHANNEL);
        pwm_set_chan_level(cs_mot2.slice, PWM_CHAN_A, OFF_CHANNEL);
        pwm_set_chan_level(cs_mot2.slice, PWM_CHAN_B, OFF_CHANNEL);
        break;
    case 2:
        pwm_set_chan_level(cs_mot1.slice, PWM_CHAN_A, OFF_CHANNEL);
        pwm_set_chan_level(cs_mot1.slice, PWM_CHAN_B, ON_CHANNEL);
        pwm_set_chan_level(cs_mot2.slice, PWM_CHAN_A, ON_CHANNEL);
        pwm_set_chan_level(cs_mot2.slice, PWM_CHAN_B, OFF_CHANNEL);
        break;
    case 3:
        pwm_set_chan_level(cs_mot1.slice, PWM_CHAN_A, OFF_CHANNEL);
        pwm_set_chan_level(cs_mot1.slice, PWM_CHAN_B, OFF_CHANNEL);
        pwm_set_chan_level(cs_mot2.slice, PWM_CHAN_A, ON_CHANNEL);
        pwm_set_chan_level(cs_mot2.slice, PWM_CHAN_B, ON_CHANNEL);
        break;
    case 4:
        pwm_set_chan_level(cs_mot1.slice, PWM_CHAN_A, ON_CHANNEL);
        pwm_set_chan_level(cs_mot1.slice, PWM_CHAN_B, OFF_CHANNEL);
        pwm_set_chan_level(cs_mot2.slice, PWM_CHAN_A, OFF_CHANNEL);
        pwm_set_chan_level(cs_mot2.slice, PWM_CHAN_B, ON_CHANNEL);
        break;
    default:
        pwm_set_chan_level(cs_mot1.slice, PWM_CHAN_A, OFF_CHANNEL);
        pwm_set_chan_level(cs_mot1.slice, PWM_CHAN_B, OFF_CHANNEL);
        pwm_set_chan_level(cs_mot2.slice, PWM_CHAN_A, OFF_CHANNEL);
        pwm_set_chan_level(cs_mot2.slice, PWM_CHAN_B, OFF_CHANNEL);
        break;
    }
}

void motor_set(int pwm_val)
{
    if (pwm_val > 0)
    {
        pwm_set_chan_level(cs_mot1.slice, PWM_CHAN_A, pwm_val);
        pwm_set_chan_level(cs_mot1.slice, PWM_CHAN_B, OFF_CHANNEL);
    }
    else
    {
        pwm_set_chan_level(cs_mot1.slice, PWM_CHAN_A, OFF_CHANNEL);
        pwm_set_chan_level(cs_mot1.slice, PWM_CHAN_B, -pwm_val);
    }
}
