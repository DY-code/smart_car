#ifndef _PID_POSITIONAL_H
#define _PID_POSITIONAL_H	 

/*
2023.7.30
������pid������޷������ڲ���pid
*/

//pid�����ṹ��
typedef struct
{
	float kp;
	float ki;
	float kd;	
	
	float error; //��� = Ŀ��ֵ - ʵ��ֵ
	float last_error; //�ϴ�ƫ��
	
	float integral; //������
	float inte_limitup;//�����޷�����
	float inte_limitdown;//�����޷�����
	float inte_control; //���ַ���� ������������
	
	float pid_value; //pid�����
	float pid_amplitude_limitup; //pid������޷�
	float pid_amplitude_limitdown;
}Pid_posi_Set;

//pid�����ṹ���ʼ��ʾ��
// Pid_posi_Set pid_info = 
// {
// 	.kp = 1,
// 	.ki = 0,
// 	.kd = 0,
// };

//λ��ʽpid����
//float pid_positional(float target, float measure_val, Pid_posi_Set* pid);
//error = target - measure_val
float pid_positional(float error, Pid_posi_Set* pid);
//pidת��ֵ�޷��������ڲ���pid����
float pid_val_limit(float val, float limit);
		 				    
#endif
