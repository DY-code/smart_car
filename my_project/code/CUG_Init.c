/*
 * CUG_Init.c
 *
 *  Created on: 2024��6��7��
 *      Author: lilinyang
 */

#include "CUG_Headfile.h"

void CUG_Init(void)
{
    clock_init(SYSTEM_CLOCK_120M);      // ��ʼ��оƬʱ�� ����Ƶ��Ϊ 120MHz
    debug_init();                       // ��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
    ips200_init(IPS200_TYPE_PARALLEL8);
    while(1)
    {
        if(imu963ra_init())
            printf("\r\nIMU963RA init error."); // IMU963RA ��ʼ��ʧ��
        else
            break;
    }

    // ���ߴ��� ����ʹ��wireless_printf����
	wireless_uart_init();

    // ����ͷ��ʼ��
    ips200_show_string(0, 0, "mt9v03x init.");
    while(1)
    {
        if(mt9v03x_init())
            ips200_show_string(0, 80, "mt9v03x reinit.");
        else
            break;
        system_delay_ms(500); // ����ʱ�������Ʊ�ʾ�쳣
    }
    //ͼ�����ʼ��
    Img_ProcessInit();

    // ��ˢ ��ˢ����ӿڳ�ʼ��
    blmotor_init();
    motor_init();
    // �������ӿڳ�ʼ��
    basic_speed_encoder_init();
}
