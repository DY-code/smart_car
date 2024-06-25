#include "motor.h"

// brush less motor
#define BLM_L_Pin          (TIM3_PWM_MAP3_CH1_C6)
#define BLM_R_Pin          (TIM3_PWM_MAP3_CH2_C7)


//-------------------------------------------------------------------------------------------------------------------
//简介：无刷电机接口初始化
//-------------------------------------------------------------------------------------------------------------------
void blmotor_init(void)
{
    pwm_init(BLM_L_Pin, 50, 0);
    pwm_init(BLM_R_Pin, 50, 0);
}

//-------------------------------------------------------------------------------------------------------------------
//简介：设置无刷电机速度
//duty: 无刷电机信号百分比 范围：0~10000
//-------------------------------------------------------------------------------------------------------------------
void set_blmotor_speed(blmotor_index blm_index, uint32 duty)
{
	//限幅
	if (duty > 10000)
		duty = 10000;

	//无刷电机pwm范围500~1000
	if (blm_index == BLMotor_L)
	{
		pwm_set_duty(BLM_L_Pin, 500 + (float)duty/10000 * 500);
	}
	else if (blm_index == BLMotor_R)
	{
		pwm_set_duty(BLM_R_Pin, 500 + (float)duty/10000 * 500);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 母板接口：有刷电机1
#define DIR_L 	D12						// 0-向内 1-向外
#define DIR_LB 	D15						// 0-向内 1-向外
#define L_dir_out	(1)					// 左 向外
#define LB_dir_out	(1)					// 左后 向外
#define PWM_L 	TIM4_PWM_MAP1_CH2_D13  	// 左
#define PWM_LB 	TIM4_PWM_MAP1_CH3_D14	// 左后


// 母板接口：有刷电机2
#define DIR_R 	A1 						// 0-向外 1-向内
#define DIR_RB 	A2						// 0-向外 1-向内
#define R_dir_out	(0)					// 右 向外
#define RB_dir_out	(0)					// 右后 向外
#define PWM_R 	TIM2_PWM_MAP0_CH1_A0  	// 右
#define PWM_RB 	TIM2_PWM_MAP0_CH4_A3 	// 右后


//-------------------------------------------------------------------------------------------------------------------
//简介：电机接口初始化
//-------------------------------------------------------------------------------------------------------------------
void motor_init(void)
{
	gpio_init(DIR_L, GPO, L_dir_out, GPO_PUSH_PULL);
	gpio_init(DIR_LB, GPO, LB_dir_out, GPO_PUSH_PULL);

	gpio_init(DIR_R, GPO, R_dir_out, GPO_PUSH_PULL);
    gpio_init(DIR_RB, GPO, RB_dir_out, GPO_PUSH_PULL);

    pwm_init(PWM_L, 17000, 0);
	pwm_init(PWM_LB, 17000, 0);

	pwm_init(PWM_R, 17000, 0);
	pwm_init(PWM_RB, 17000, 0);
}


//-------------------------------------------------------------------------------------------------------------------
//简介：设置电机速度
//mindex: 指定电机
//speed: 设置速度 >0表示向外吹风 范围：0~7500
//-------------------------------------------------------------------------------------------------------------------
void set_motor_speed(motor_index mindex, int32 speed)
{
	uint8 dir_val;
	//限幅
	if (speed > 7500)
		speed = 7500;
	else if (speed < -7500)
		speed = -7500;
	if (mindex == Motor_L)
	{
		if (speed > 0) dir_val = L_dir_out;
		else dir_val = (L_dir_out+1) % 2;

		pwm_set_duty(PWM_L, abs(speed));
		gpio_set_level(DIR_L, dir_val);
	}
	else if (mindex == Motor_LB)
	{
		if (speed > 0) dir_val = LB_dir_out;
		else dir_val = (LB_dir_out+1) % 2;

		pwm_set_duty(PWM_LB, abs(speed));
		gpio_set_level(DIR_LB, dir_val);
	}
	else if (mindex == Motor_R)
	{
		if (speed > 0) dir_val = R_dir_out;
		else dir_val = (R_dir_out+1) % 2;

		pwm_set_duty(PWM_R, abs(speed));
		gpio_set_level(DIR_R, dir_val);
	}
	else if (mindex == Motor_RB)
	{
		if (speed > 0) dir_val = RB_dir_out;
		else dir_val = (RB_dir_out+1) % 2;

		pwm_set_duty(PWM_RB, abs(speed));
		gpio_set_level(DIR_RB, dir_val);
	}
}

//有刷电机限速
int32 motor_speed_limit(int32 speed)
{
	static int32 speed_limit = 7200;
	if (speed < -speed_limit)
		return -speed_limit;
	else if (speed > speed_limit)
		return speed_limit;
	else
		return speed;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 编码器接口初始化 测量基础前进速度
void basic_speed_encoder_init(void)
{
	encoder_dir_init(TIM10_ENCOEDER, TIM10_ENCOEDER_MAP3_CH1_D1, TIM10_ENCOEDER_MAP3_CH2_D3);
}

// 获取编码器计数
int16 get_basic_speed(void)
{
	int16 speed_val = -encoder_get_count(TIM10_ENCOEDER);
	encoder_clear_count(TIM10_ENCOEDER);
	return speed_val;
}










