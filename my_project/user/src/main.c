#include "CUG_Headfile.h"

#define PIT1  	(TIM6_PIT)
#define PIT2    (TIM7_PIT)

Pid_posi_Set pid_img =               //ͼ�񻷣���ƫ��õ�Ŀ����ٶ�
{
	.kp = 31,
	.ki = 0.1,
	.kd = 0,
	.pid_amplitude_limitdown = -3000,
	.pid_amplitude_limitup = 3000,
	.inte_limitup = 400,
	.inte_limitdown = -400
};

Pid_posi_Set pid_ang =               //���ٶȻ����õ����ҵ������
{
	.kp = 8.5, //8.4,
	.ki = 0.1, //0,
	.kd = 0,
	.pid_amplitude_limitdown = -8000,
	.pid_amplitude_limitup = 8000
};

Pid_posi_Set pid_speed =             //�ٶȻ����õ������׼�ٶ�
{
	.kp = 35,
	.ki = 6,
	.kd = 0,
	.pid_amplitude_limitdown = -7500,
	.pid_amplitude_limitup = 7500,
	.inte_limitup = 600,
	.inte_limitdown = -600
};

//�ٶȺ�Ŀ���ٶ�
int16 Speed = 0;
int16 TargetSpeed = 250;
//ͼ��ƫ��    ������ƫΪ��
int16 ImgError = 0;
//Ŀ����ٶ�
int16 TargetAngS = 0;
//ʵ�ʽ��ٶ�  �Ǽ��ٶ�����Ϊ�� ����Ϊ��
int16 AngS = 0;
//���Ҳ���
int16 DifferS = 0;
//�����׼�ٶ�
int16 BaseS = 0;
//������õ��ٶ�
int16 SpeedLB = 0, SpeedRB = 0, SpeedL = 0, SpeedR = 0;
uint8 Mpu_Integral_Flag = 0;
float mpu_z = 0;
//��������̶ȣ�������һ֡ͼ����Զ�����
float Curve = 0;

/* notes:
 * �յ�ͣ�������
 * */
int main(void)
{
	//��Ҫ�ĳ�ʼ������
	CUG_Init();

	// ������ˢ����ٶ�
	set_blmotor_speed(BLMotor_L, 9000);
	set_blmotor_speed(BLMotor_R, 9000);

	// ��ʱһ��ʱ���ʼǰ��
	system_delay_ms(2000);

	// ��ʱ������ת�����
	pit_ms_init(PIT1, 20, (1<<5) | 2);
	pit_ms_init(PIT2, 2, (0<<5) | 2);
	key_init(2);

//	set_motor_speed(Motor_LB, motor_speed_limit(3000));
//	set_motor_speed(Motor_RB, motor_speed_limit(3000));
//	set_motor_speed(Motor_L, motor_speed_limit(3000));
//	set_motor_speed(Motor_R, motor_speed_limit(3000));

	// ������ʾ����������ѭ����
    while (1)
    {
    	Menu_Display();
    	Img_Display(); //��ʾͼ��

    	// ͼ��ƫ�� �������ٶ� ʵ�ʽ��ٶ� ת����� �����ٶ�
    	wireless_printf("%d,%d,%d\n", ImgError, Speed, TargetSpeed);
    }
}

void pit1_hanlder (void)
{
	// ͼ����
	Img_FinalProcess();
	// ��ȡ����������
	Speed = get_basic_speed();
	// ͼ��ƫ��
	ImgError = Img_GetError();
	// ��ͼ��ƫ��õ��������ٶ� ����ƫ���е�ƫ��ʱerrorΪ�� ��ʱ�����Ľ��ٶ�Ϊ�� ����
	TargetAngS = (int16)pid_positional(0-ImgError, &pid_img);
	// ǰ���ٶ�
	TargetSpeed = Judge_Speed(); //�ٶȾ���
	//�ٶȻ�PID
	BaseS = (int16)pid_positional(TargetSpeed-Speed, &pid_speed) + 2000;
}

//���������˲�
#define Win_Length 10 //���ڴ�С
int16 filter_line[Win_Length]; //��������
int16 Sliding_Window_Filter(int16 angs)
{
    for(uint8 i = 0;i < Win_Length-1;i++)
    {
        filter_line[i] = filter_line[i+1]; //����ǰ��һλ
    }
    filter_line[Win_Length-1] = angs; //�����µ�����

    int16 sum = 0;
    for(uint8 i = 0;i < Win_Length;i++)
    {
        sum += filter_line[i];
    }
    return sum/Win_Length;
}

void pit2_hanlder(void)
{
    key_scanner();
    imu963ra_get_gyro();  // ��ȡ IMU963RA �Ľ��ٶȲ�����ֵ

//    //���ٶȾ�ֵ�˲�
//    for(uint8 i=0;i<9;i++)
//    {
//        filter_line[i]=filter_line[i+1];//����ǰ��һλ
//    }
//    filter_line[9]=imu963ra_gyro_z;//�����µ�����
//    int16 sum=0;
//    for(uint8 i=0;i<10;i++)
//    {
//        sum+=filter_line[i];
//    }
//    AngS=sum/10;

    // ���ٶ�����Ϊ�� ����Ϊ��
    //���������˲�
    AngS = Sliding_Window_Filter(imu963ra_gyro_z);

    //����״̬�¶Խ��ٶȽ��л��ָ�������
    if(Mpu_Integral_Flag)
    {
        mpu_z += ((float)imu963ra_gyro_z)/500/14.3+0.2/500;
    }

    // ����ת��Ĳ��ٴ�С
    DifferS = (int16)pid_positional(TargetAngS-AngS, &pid_ang);
    SpeedLB = BaseS + DifferS/2;
    SpeedRB = BaseS - DifferS/2;

    // �ٶȴ�С������ ��������ٶ� + ͼ��ƫ�
//    SpeedL = (0.9*AngS - 30*ImgError);
    SpeedL = 2*AngS - 40*ImgError;

    // ͬһʱ���������Ĵ��緽��һ��
    SpeedR = -SpeedL;

    set_motor_speed(Motor_LB, motor_speed_limit(SpeedLB));
    set_motor_speed(Motor_RB, motor_speed_limit(SpeedRB));
    set_motor_speed(Motor_L, motor_speed_limit(1.3*SpeedL));
    set_motor_speed(Motor_R, motor_speed_limit(SpeedR));
}

/*
- �����ʱӦ�жϴ����Ƿ񳬳�����ڲ����࣬�Դ�Ϊ����ȷ��������ȵ�ת��
*/



////////////////////////////////////////////////////////////////
// �˴�Ϊ�����ô���
//int main(void)
//{
//    clock_init(SYSTEM_CLOCK_120M);      // ��ʼ��оƬʱ�� ����Ƶ��Ϊ 120MHz
//    debug_init();                       // ��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
//
////    wireless_uart_init();
//
//    // ��ˢ ��ˢ����ӿڳ�ʼ��
//    blmotor_init();
//	motor_init();
//
//	// �������ӿڳ�ʼ��
////	basic_speed_encoder_init();
//
//	// ������ˢ����ٶ�
////    set_blmotor_speed(BLMotor_L, 9000);
////    set_blmotor_speed(BLMotor_R, 9000);
//
//    set_motor_speed(Motor_L, 3000);
//    set_motor_speed(Motor_R, 3000);
//	set_motor_speed(Motor_LB, 3000);
//	set_motor_speed(Motor_RB, 3000);
////    while(1)
////    {
////        if(imu963ra_init())
////            printf("\r\nIMU963RA init error.");                                 // IMU963RA ��ʼ��ʧ��
////        else
////            break;
////    }
//
//
////	// �������ӿڳ�ʼ��
////	encoder_dir_init(TIM10_ENCOEDER, TIM10_ENCOEDER_MAP3_CH1_D1, TIM10_ENCOEDER_MAP3_CH2_D3);
////	pit_ms_init(PIT, 10);
//
//
////	blmotor_init();
////	motor_init();
//
//    while (1)
//    {
////		printf("%d\r\n", imu963ra_gyro_z);
////
////    	wireless_printf("%d\n\r", Speed);
//////    	wireless_printf("%d\n\r", imu963ra_acc_y);
////    	system_delay_ms(10);
//    }
//}
//
//void pit_hanlder (void)
//{
//	// ��ȡ���������� ����ǰ��Ϊ�� ��֮Ϊ��
//	Speed = -encoder_get_count(TIM10_ENCOEDER);
//	encoder_clear_count(TIM10_ENCOEDER);
//
////	imu963ra_get_gyro();
////	imu963ra_get_acc();
//}






