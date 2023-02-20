#define COIL1 16
#define COIL2 17
#define COIL3 14
#define COIL4 15

void motor_init();
void motor_set(int pwm_val);
void motor_set_single(int coil_number);



