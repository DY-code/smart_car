/*
 * Speed_Judge.c
 *
 *  Created on: 2024��6��9��
 *      Author: lilinyang
 */
#include "CUG_Headfile.h"

extern uint8 Huandao_L, Huandao_R;
extern float Curve;

int16 Judge_Speed(void)
{
    if(Huandao_L > 0 && Huandao_L < 6) //Բ��״̬�¼���
        return 250;
    if(Huandao_R > 0 && Huandao_R < 6)
        return 250;
    return 250 + 100*(1 - Curve/10); //��׼�ٶ� �� ֱ����������ֵ*�����̶�
}
