/*
 * CUG_Init.c
 *
 *  Created on: 2024年6月7日
 *      Author: lilinyang
 */

#include "CUG_Headfile.h"

void CUG_Init(void)
{
    clock_init(SYSTEM_CLOCK_120M);      // 初始化芯片时钟 工作频率为 120MHz
    debug_init();                       // 务必保留，本函数用于初始化MPU 时钟 调试串口
    ips200_init(IPS200_TYPE_PARALLEL8);
    while(1)
    {
        if(imu963ra_init())
            printf("\r\nIMU963RA init error."); // IMU963RA 初始化失败
        else
            break;
    }

    // 无线串口 可以使用wireless_printf函数
	wireless_uart_init();

    // 摄像头初始化
    ips200_show_string(0, 0, "mt9v03x init.");
    while(1)
    {
        if(mt9v03x_init())
            ips200_show_string(0, 80, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500); // 短延时快速闪灯表示异常
    }
    //图像处理初始化
    Img_ProcessInit();

    // 无刷 有刷电机接口初始化
    blmotor_init();
    motor_init();
    // 编码器接口初始化
    basic_speed_encoder_init();
}
