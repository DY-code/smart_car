#include "motor.h"

// brush less motor
#define BLM_L_Pin          (TIM3_PWM_MAP3_CH1_C6)
#define BLM_R_Pin          (TIM3_PWM_MAP3_CH2_C7)


//-------------------------------------------------------------------------------------------------------------------
//��飺��ˢ����ӿڳ�ʼ��
//-------------------------------------------------------------------------------------------------------------------
void blmotor_init(void)
{
    pwm_init(BLM_L_Pin, 50, 0);
    pwm_init(BLM_R_Pin, 50, 0);
}

//-------------------------------------------------------------------------------------------------------------------
//��飺������ˢ����ٶ�
//duty: ��ˢ����źŰٷֱ� ��Χ��0~10000
//-------------------------------------------------------------------------------------------------------------------
void set_blmotor_speed(blmotor_index blm_index, uint32 duty)
{
	//�޷�
	if (duty > 10000)
		duty = 10000;

	//��ˢ���pwm��Χ500~1000
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

// ĸ��ӿڣ���ˢ���1
#define DIR_L 	D12						// 0-���� 1-����
#define DIR_LB 	D15						// 0-���� 1-����
#define L_dir_out	(1)					// �� ����
#define LB_dir_out	(1)					// ��� ����
#define PWM_L 	TIM4_PWM_MAP1_CH2_D13  	// ��
#define PWM_LB 	TIM4_PWM_MAP1_CH3_D14	// ���


// ĸ��ӿڣ���ˢ���2
#define DIR_R 	A1 						// 0-���� 1-����
#define DIR_RB 	A2						// 0-���� 1-����
#define R_dir_out	(0)					// �� ����
#define RB_dir_out	(0)					// �Һ� ����
#define PWM_R 	TIM2_PWM_MAP0_CH1_A0  	// ��
#define PWM_RB 	TIM2_PWM_MAP0_CH4_A3 	// �Һ�


//-------------------------------------------------------------------------------------------------------------------
//��飺����ӿڳ�ʼ��
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
//��飺���õ���ٶ�
//mindex: ָ�����
//speed: �����ٶ� >0��ʾ���ⴵ�� ��Χ��0~7500
//-------------------------------------------------------------------------------------------------------------------
void set_motor_speed(motor_index mindex, int32 speed)
{
	uint8 dir_val;
	//�޷�
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

//��ˢ�������
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

// �������ӿڳ�ʼ�� ��������ǰ���ٶ�
void basic_speed_encoder_init(void)
{
	encoder_dir_init(TIM10_ENCOEDER, TIM10_ENCOEDER_MAP3_CH1_D1, TIM10_ENCOEDER_MAP3_CH2_D3);
}

// ��ȡ����������
int16 get_basic_speed(void)
{
	int16 speed_val = -encoder_get_count(TIM10_ENCOEDER);
	encoder_clear_count(TIM10_ENCOEDER);
	return speed_val;
}










