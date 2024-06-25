#include "CUG_Headfile.h"

#define PIT1  	(TIM6_PIT)
#define PIT2    (TIM7_PIT)

Pid_posi_Set pid_img =               //图像环，由偏差得到目标角速度
{
	.kp = 31,
	.ki = 0.1,
	.kd = 0,
	.pid_amplitude_limitdown = -3000,
	.pid_amplitude_limitup = 3000,
	.inte_limitup = 400,
	.inte_limitdown = -400
};

Pid_posi_Set pid_ang =               //角速度环，得到左右电机差速
{
	.kp = 8.5, //8.4,
	.ki = 0.1, //0,
	.kd = 0,
	.pid_amplitude_limitdown = -8000,
	.pid_amplitude_limitup = 8000
};

Pid_posi_Set pid_speed =             //速度环，得到电机基准速度
{
	.kp = 35,
	.ki = 6,
	.kd = 0,
	.pid_amplitude_limitdown = -7500,
	.pid_amplitude_limitup = 7500,
	.inte_limitup = 600,
	.inte_limitdown = -600
};

//速度和目标速度
int16 Speed = 0;
int16 TargetSpeed = 250;
//图像偏差    船体左偏为负
int16 ImgError = 0;
//目标角速度
int16 TargetAngS = 0;
//实际角速度  角加速度向左为负 向右为正
int16 AngS = 0;
//左右差速
int16 DifferS = 0;
//电机基准速度
int16 BaseS = 0;
//电机设置的速度
int16 SpeedLB = 0, SpeedRB = 0, SpeedL = 0, SpeedR = 0;
uint8 Mpu_Integral_Flag = 0;
float mpu_z = 0;
//弯道弯曲程度，处理完一帧图像后自动更新
float Curve = 0;

/* notes:
 * 终点停车待完成
 * */
int main(void)
{
	//必要的初始化步骤
	CUG_Init();

	// 设置无刷电机速度
	set_blmotor_speed(BLMotor_L, 9000);
	set_blmotor_speed(BLMotor_R, 9000);

	// 延时一定时间后开始前进
	system_delay_ms(2000);

	// 定时器用于转向控制
	pit_ms_init(PIT1, 20, (1<<5) | 2);
	pit_ms_init(PIT2, 2, (0<<5) | 2);
	key_init(2);

//	set_motor_speed(Motor_LB, motor_speed_limit(3000));
//	set_motor_speed(Motor_RB, motor_speed_limit(3000));
//	set_motor_speed(Motor_L, motor_speed_limit(3000));
//	set_motor_speed(Motor_R, motor_speed_limit(3000));

	// 所有显示函数放在主循环中
    while (1)
    {
    	Menu_Display();
    	Img_Display(); //显示图像

    	// 图像偏差 期望角速度 实际角速度 转向差速 侧向速度
    	wireless_printf("%d,%d,%d\n", ImgError, Speed, TargetSpeed);
    }
}

void pit1_hanlder (void)
{
	// 图像处理
	Img_FinalProcess();
	// 获取编码器计数
	Speed = get_basic_speed();
	// 图像偏差
	ImgError = Img_GetError();
	// 由图像偏差得到期望角速度 船体偏左中点偏右时error为负 此时期望的角速度为正 向右
	TargetAngS = (int16)pid_positional(0-ImgError, &pid_img);
	// 前进速度
	TargetSpeed = Judge_Speed(); //速度决策
	//速度环PID
	BaseS = (int16)pid_positional(TargetSpeed-Speed, &pid_speed) + 2000;
}

//滑动窗口滤波
#define Win_Length 10 //窗口大小
int16 filter_line[Win_Length]; //滑动窗口
int16 Sliding_Window_Filter(int16 angs)
{
    for(uint8 i = 0;i < Win_Length-1;i++)
    {
        filter_line[i] = filter_line[i+1]; //数据前移一位
    }
    filter_line[Win_Length-1] = angs; //放入新的数据

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
    imu963ra_get_gyro();  // 获取 IMU963RA 的角速度测量数值

//    //角速度均值滤波
//    for(uint8 i=0;i<9;i++)
//    {
//        filter_line[i]=filter_line[i+1];//数据前移一位
//    }
//    filter_line[9]=imu963ra_gyro_z;//放入新的数据
//    int16 sum=0;
//    for(uint8 i=0;i<10;i++)
//    {
//        sum+=filter_line[i];
//    }
//    AngS=sum/10;

    // 角速度向左为负 向右为正
    //滑动串口滤波
    AngS = Sliding_Window_Filter(imu963ra_gyro_z);

    //环岛状态下对角速度进行积分辅助出环
    if(Mpu_Integral_Flag)
    {
        mpu_z += ((float)imu963ra_gyro_z)/500/14.3+0.2/500;
    }

    // 用于转向的差速大小
    DifferS = (int16)pid_positional(TargetAngS-AngS, &pid_ang);
    SpeedLB = BaseS + DifferS/2;
    SpeedRB = BaseS - DifferS/2;

    // 速度大小正比于 （船体角速度 + 图像偏差）
//    SpeedL = (0.9*AngS - 30*ImgError);
    SpeedL = 2*AngS - 40*ImgError;

    // 同一时刻两侧电机的吹风方向一致
    SpeedR = -SpeedL;

    set_motor_speed(Motor_LB, motor_speed_limit(SpeedLB));
    set_motor_speed(Motor_RB, motor_speed_limit(SpeedRB));
    set_motor_speed(Motor_L, motor_speed_limit(1.3*SpeedL));
    set_motor_speed(Motor_R, motor_speed_limit(SpeedR));
}

/*
- 过弯道时应判断船体是否超出弯道内侧或外侧，以此为依据确定两侧风扇的转向
*/



////////////////////////////////////////////////////////////////
// 此处为测试用代码
//int main(void)
//{
//    clock_init(SYSTEM_CLOCK_120M);      // 初始化芯片时钟 工作频率为 120MHz
//    debug_init();                       // 务必保留，本函数用于初始化MPU 时钟 调试串口
//
////    wireless_uart_init();
//
//    // 无刷 有刷电机接口初始化
//    blmotor_init();
//	motor_init();
//
//	// 编码器接口初始化
////	basic_speed_encoder_init();
//
//	// 设置无刷电机速度
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
////            printf("\r\nIMU963RA init error.");                                 // IMU963RA 初始化失败
////        else
////            break;
////    }
//
//
////	// 编码器接口初始化
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
//	// 获取编码器计数 船体前进为正 反之为负
//	Speed = -encoder_get_count(TIM10_ENCOEDER);
//	encoder_clear_count(TIM10_ENCOEDER);
//
////	imu963ra_get_gyro();
////	imu963ra_get_acc();
//}






