#ifndef _PID_POSITIONAL_H
#define _PID_POSITIONAL_H	 

/*
2023.7.30
增加了pid输出量限幅，用于并级pid
*/

//pid参数结构体
typedef struct
{
	float kp;
	float ki;
	float kd;	
	
	float error; //误差 = 目标值 - 实际值
	float last_error; //上次偏差
	
	float integral; //积分项
	float inte_limitup;//积分限幅上限
	float inte_limitdown;//积分限幅下限
	float inte_control; //积分分离点 将积分项清零
	
	float pid_value; //pid输出量
	float pid_amplitude_limitup; //pid输出量限幅
	float pid_amplitude_limitdown;
}Pid_posi_Set;

//pid参数结构体初始化示例
// Pid_posi_Set pid_info = 
// {
// 	.kp = 1,
// 	.ki = 0,
// 	.kd = 0,
// };

//位置式pid运算
//float pid_positional(float target, float measure_val, Pid_posi_Set* pid);
//error = target - measure_val
float pid_positional(float error, Pid_posi_Set* pid);
//pid转换值限幅，常用于并级pid运算
float pid_val_limit(float val, float limit);
		 				    
#endif
