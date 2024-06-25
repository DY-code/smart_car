#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "zf_common_headfile.h"

typedef enum
{
	BLMotor_L,
	BLMotor_R
}blmotor_index;

typedef enum
{
	Motor_L,
	Motor_LB,
	Motor_R,
	Motor_RB
}motor_index;


void blmotor_init(void);
void set_blmotor_speed(blmotor_index blm_index, uint32 duty);

void motor_init(void);
void set_motor_speed(motor_index mindex, int32 speed);
int32 motor_speed_limit(int32 speed);

void basic_speed_encoder_init(void);
int16 get_basic_speed(void);

#endif
