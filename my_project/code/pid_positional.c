#include "pid_positional.h"

//位置式pid运算
float pid_positional(float error, Pid_posi_Set* pid)
{
	//上次误差更新
	pid->last_error = pid->error;
	//更新此次误差
	pid->error = error;

	//累积积分
	pid->integral += pid->error;
	//积分限幅
	if(pid->integral > pid->inte_limitup)
	    pid->integral = pid->inte_limitup;
	if(pid->integral < pid->inte_limitdown)
	    pid->integral = pid->inte_limitdown;
	//积分分离
	
	//计算pid结果
	float pid_val = pid->kp * pid->error + 
		pid->ki * pid->integral + 
		pid->kd * (pid->error - pid->last_error);
	
	//pid转换值限幅
	if (pid_val < pid->pid_amplitude_limitdown)
		pid_val = pid->pid_amplitude_limitdown;
	else if (pid_val > pid->pid_amplitude_limitup)
		pid_val = pid->pid_amplitude_limitup;
	
	pid->pid_value = pid_val;
	
	return pid_val;
}

////位置式pid运算
//float pid_positional(float target, float measure_val, Pid_posi_Set* pid)
//{
//	//上次误差更新
//	pid->last_error = pid->error;
//	//更新此次误差
//	pid->error = target - measure_val;
//	//累积积分
//	pid->integral += pid->error;
//
//	//积分分离
//
//	//计算pid结果
//	float pid_val = pid->kp * pid->error +
//		pid->ki * pid->integral +
//		pid->kd * (pid->error - pid->last_error);
//
//	//pid转换值限幅
//	if (pid_val < -pid->pid_amplitude_limit)
//		pid_val = -pid->pid_amplitude_limit;
//	else if (pid_val > pid->pid_amplitude_limit)
//		pid_val = pid->pid_amplitude_limit;
//
//	pid->pid_value = pid_val;
//
//	return pid_val;
//}

//pid转换值限幅，常用于并级pid运算
float pid_val_limit(float val, float limit)
{
	if (val < -limit)
		return -limit;
	else if (val > limit)
		return limit;
	else
		return val;
}
