#include "pid_positional.h"

//λ��ʽpid����
float pid_positional(float error, Pid_posi_Set* pid)
{
	//�ϴ�������
	pid->last_error = pid->error;
	//���´˴����
	pid->error = error;

	//�ۻ�����
	pid->integral += pid->error;
	//�����޷�
	if(pid->integral > pid->inte_limitup)
	    pid->integral = pid->inte_limitup;
	if(pid->integral < pid->inte_limitdown)
	    pid->integral = pid->inte_limitdown;
	//���ַ���
	
	//����pid���
	float pid_val = pid->kp * pid->error + 
		pid->ki * pid->integral + 
		pid->kd * (pid->error - pid->last_error);
	
	//pidת��ֵ�޷�
	if (pid_val < pid->pid_amplitude_limitdown)
		pid_val = pid->pid_amplitude_limitdown;
	else if (pid_val > pid->pid_amplitude_limitup)
		pid_val = pid->pid_amplitude_limitup;
	
	pid->pid_value = pid_val;
	
	return pid_val;
}

////λ��ʽpid����
//float pid_positional(float target, float measure_val, Pid_posi_Set* pid)
//{
//	//�ϴ�������
//	pid->last_error = pid->error;
//	//���´˴����
//	pid->error = target - measure_val;
//	//�ۻ�����
//	pid->integral += pid->error;
//
//	//���ַ���
//
//	//����pid���
//	float pid_val = pid->kp * pid->error +
//		pid->ki * pid->integral +
//		pid->kd * (pid->error - pid->last_error);
//
//	//pidת��ֵ�޷�
//	if (pid_val < -pid->pid_amplitude_limit)
//		pid_val = -pid->pid_amplitude_limit;
//	else if (pid_val > pid->pid_amplitude_limit)
//		pid_val = pid->pid_amplitude_limit;
//
//	pid->pid_value = pid_val;
//
//	return pid_val;
//}

//pidת��ֵ�޷��������ڲ���pid����
float pid_val_limit(float val, float limit)
{
	if (val < -limit)
		return -limit;
	else if (val > limit)
		return limit;
	else
		return val;
}
