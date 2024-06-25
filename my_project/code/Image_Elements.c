/*
 * Image_Elements.c
 *
 *  Created on: 2024年4月2日
 *      Author: lilinyang
 */

#include "zf_common_headfile.h"
#include "Image_Process.h"
#include "Image_Elements.h"

uint8 Huandao_L = 0, Huandao_R = 0, Cross = 0, Follow_Mode = 0;                 //Follow_Mode:   0:摄像头寻中线  1:无效帧

extern uint16 Points_L[MT9V03X_W * 2][2];
extern uint16 Points_R[MT9V03X_W * 2][2];
extern uint8 Mpu_Integral_Flag;
extern float mpu_z;

uint16 Width0=51;                                                                //0行赛道的宽度
float Width_Step=1.54;                                                             //下一行宽-上一行宽

/**
 * \brief 校正赛道宽度
 *
 */
void Width_Reinit(void)
{
    uint16 w9=Img_TestWidth(9);
    uint16 w69=Img_TestWidth(69);
    Width_Step=(float)(w69-w9)/60;
    Width0=limit_a_b(w9-Width_Step*9, 0, 180);
}

/**
 * \brief 获取赛道宽度
 *
 * \param y
 * \return 宽度
 */
uint16 Width(uint8 y)
{
    return Width0+(uint16)(Width_Step*y);
}

/**
 * \brief 找十字拐点
 *
 * \param form 1:Leftdown   2:Leftup   3:Rightdown    4:Rightup
 * \return 1:找到 0:未找到
 */
Turn_Point Find_Cross_TP(uint8 form)
{
    uint8 startx = 0, starty = 0;
    uint16 Points_Single[MT9V03X_W * 2][2] = {0};
    uint16 Points_Single_Count = 0;
    int16 Angle[MT9V03X_H] = {0};
    uint8 Angle_Count=0;
    Turn_Point TP = { 0,0,0,0 };
    switch (form)
    {
    case 1:
        if (Img_FindStartPoint_Single(7, 25, 3, &startx, &starty))
        {
            Img_SearchLine_Righthand(startx, starty, Points_Single, &Points_Single_Count);
            Img_CalulateAngle_Single(Points_Single, Points_Single_Count, Angle, &Angle_Count);
            if (Angle_Count >= 3)
            {
                for (uint8 i = 0; i < Angle_Count - 1; i++)
                {
                    if ((Angle[i] < 20 || Angle[i] > 340) && (Angle[i + 2] > 210 && Angle[i + 2] < 270))
                    {
                        TP.find_flag = 1;
                        TP.x = Points_Single[(i + 1) * 6 + 5][0];
                        TP.y = Points_Single[(i + 1) * 6 + 5][1];
                        TP.k = limit_a_b_f(my_k((i + 1) * 6 + 5, Points_Single_Count, Points_Single),-2.3,-0.5);
                        return TP;
                    }
                }
            }
        }
        break;
    case 2:
        if (Img_FindStartPoint_Single(7, 78, 2, &startx, &starty))
        {
            Img_SearchLine_Lefthand(startx, starty, Points_Single, &Points_Single_Count);
            Img_CalulateAngle_Single(Points_Single, Points_Single_Count, Angle, &Angle_Count);
            if (Angle_Count >= 3)
            {
                for (uint8 i = 0; i < Angle_Count - 1; i++)
                {
                    if ((Angle[i] < 20 || Angle[i] > 340) && (Angle[i + 2] > 30 && Angle[i + 2] < 90))
                    {
                        TP.find_flag = 1;
                        TP.x = Points_Single[(i + 1) * 6 + 5][0];
                        TP.y = Points_Single[(i + 1) * 6 + 5][1];
                        TP.k = limit_a_b_f(my_k((i + 1) * 6 + 5, limit_a_b((i + 1) * 6 + 15, 0, Points_Single_Count), Points_Single),-2.3,-0.5);
                        return TP;
                    }
                }
            }
        }
        else if (Img_FindStartPoint_Single(35, 78, 2, &startx, &starty))
        {
            Img_SearchLine_Lefthand(startx, starty, Points_Single, &Points_Single_Count);
            Img_CalulateAngle_Single(Points_Single, Points_Single_Count, Angle, &Angle_Count);
            if (Angle_Count >= 3)
            {
                for (uint8 i = 0; i < Angle_Count - 1; i++)
                {
                    if ((Angle[i] < 20 || Angle[i] > 340) && (Angle[i + 2] > 30 && Angle[i + 2] < 90))
                    {
                        TP.find_flag = 1;
                        TP.x = Points_Single[(i + 1) * 6 + 5][0];
                        TP.y = Points_Single[(i + 1) * 6 + 5][1];
                        TP.k = limit_a_b_f(my_k((i + 1) * 6 + 5, limit_a_b((i + 1) * 6 + 15, 0, Points_Single_Count), Points_Single),-2.3,-0.5);
                        return TP;
                    }
                }
            }
        }
        break;
    case 3:
        if (Img_FindStartPoint_Single(180, 25, 3, &startx, &starty))
        {
            Img_SearchLine_Lefthand(startx, starty, Points_Single, &Points_Single_Count);
            Img_CalulateAngle_Single(Points_Single, Points_Single_Count, Angle, &Angle_Count);
            if (Angle_Count >= 3)
            {
                for (uint8 i = 0; i < Angle_Count - 1; i++)
                {
                    if ((Angle[i] < 190 || Angle[i] > 170) && (Angle[i + 2] > 270 && Angle[i + 2] < 340))
                    {
                        TP.find_flag = 1;
                        TP.x = Points_Single[(i + 1) * 6 + 5][0];
                        TP.y = Points_Single[(i + 1) * 6 + 5][1];
                        TP.k = limit_a_b_f(my_k((i + 1) * 6 + 5, Points_Single_Count, Points_Single),0.5,2.3);
                        return TP;
                    }
                }
            }
        }
        break;
    case 4:
        if (Img_FindStartPoint_Single(180, 78, 2, &startx, &starty))
        {
            Img_SearchLine_Righthand(startx, starty, Points_Single, &Points_Single_Count);
            Img_CalulateAngle_Single(Points_Single, Points_Single_Count, Angle, &Angle_Count);
            if (Angle_Count >= 3)
            {
                for (uint8 i = 0; i < Angle_Count - 1; i++)
                {
                    if ((Angle[i] < 200 && Angle[i] > 160) && (Angle[i + 2] > 90 && Angle[i + 2] < 150) && (Angle[i] - Angle[i + 2]) > 30)
                    {
                        TP.find_flag = 1;
                        TP.x = Points_Single[(i + 1) * 6 + 5][0];
                        TP.y = Points_Single[(i + 1) * 6 + 5][1];
                        TP.k = limit_a_b_f(my_k((i + 1) * 6 + 5, limit_a_b((i + 1) * 6 + 15, 0, Points_Single_Count), Points_Single),0.5,2.3);
                        return TP;
                    }
                }
            }
        }
        else if (Img_FindStartPoint_Single(153, 78, 2, &startx, &starty))
        {
            Img_SearchLine_Righthand(startx, starty, Points_Single, &Points_Single_Count);
            Img_CalulateAngle_Single(Points_Single, Points_Single_Count, Angle, &Angle_Count);
            if (Angle_Count >= 3)
            {
                for (uint8 i = 0; i < Angle_Count - 1; i++)
                {
                    if ((Angle[i] < 200 && Angle[i] > 160) && (Angle[i + 2] > 90 && Angle[i + 2] < 150) && (Angle[i] - Angle[i + 2]) > 30)
                    {
                        TP.find_flag = 1;
                        TP.x = Points_Single[(i + 1) * 6 + 5][0];
                        TP.y = Points_Single[(i + 1) * 6 + 5][1];
                        TP.k = limit_a_b_f(my_k((i + 1) * 6 + 5, limit_a_b((i + 1) * 6 + 15, 0, Points_Single_Count), Points_Single),0.5,2.3);
                        return TP;
                    }
                }
            }
        }
        break;
    }
    return TP;
}

/**
 * \brief 找左环岛拐点
 *
 * \param form 1:Leftdown   2:Leftup   3:Midguaidian   4:Right_out
 * \return TP
 */
Turn_Point Find_Huandao_L_TP(uint8 form)
{
    uint8 startx = 0, starty = 0;
    uint16 Points_Single[MT9V03X_W * 2][2] = {0};
    uint16 Points_Single_Count = 0;
    int16 Angle[MT9V03X_H] = {0};
    uint8 Angle_Count=0;
    Turn_Point TP = { 0,0,0,0 };
    switch (form)
    {
    case 1:
        if (Img_FindStartPoint_Single(7, 25, 3, &startx, &starty))
        {
            Img_SearchLine_Righthand(startx, starty, Points_Single, &Points_Single_Count);
            Img_CalulateAngle_Single(Points_Single, Points_Single_Count, Angle, &Angle_Count);
            if (Angle_Count >= 4)
            {
                for (uint8 i = 0; i < Angle_Count - 1; i++)
                {
                    if ((Angle[i] < 45 || Angle[i] > 355) && (Angle[i + 2] > 200 && Angle[i + 2] < 270))
                    {
                        TP.find_flag = 1;
                        TP.x = Points_Single[(i + 1) * 6 + 5][0];
                        TP.y = Points_Single[(i + 1) * 6 + 5][1];
                        TP.k = 0;
                        return TP;
                    }
                }
            }
        }
        break;
    case 2:
        for (uint8 i = 79; i >= 9; i --)
        {
            if (((int16)Points_L[i-1][0] - (int16)Points_L[i][0]) > 20)
            {
                if (my_abs(((int16)Points_L[i - 1][0] - (int16)Points_L[i - 3][0])) >= 12)
                    continue;
                TP.find_flag = 1;
                TP.x = Points_L[i - 1][0];
                TP.y = i;
                TP.k= my_k_twopoints(TP.x, TP.y, 186, 78);
                return TP;
            }
        }
        for (uint8 i = 79; i >= 9; i--)
        {
            if (((int16)Points_R[i][0] - (int16)Points_R[i-1][0]) > 20)
            {
                if (my_abs(((int16)Points_R[i - 1][0] - (int16)Points_R[i - 3][0])) >= 12)
                    continue;
                TP.find_flag = 1;
                TP.x = Points_R[i - 1][0];
                TP.y = i;
                TP.k = my_k_twopoints(TP.x, TP.y, 186, 78);
                return TP;
            }
        }
        if (Img_FindStartPoint_Single(7, 180, 0, &startx, &starty))
        {
            Img_SearchLine_Righthand(startx, starty, Points_Single, &Points_Single_Count);
            Img_CalulateAngle_Single(Points_Single, Points_Single_Count, Angle, &Angle_Count);
            if (Angle_Count >= 4)
            {
                for (uint8 i = 0; i < Angle_Count - 1; i++)
                {
                    if (my_jiajiao(Angle[i], Angle[i+2])>80)
                    {
                        TP.find_flag = 1;
                        TP.x = Points_Single[(i + 1) * 6 + 5][0];
                        TP.y = Points_Single[(i + 1) * 6 + 5][1];
                        TP.k = my_k_twopoints(TP.x, TP.y, 186, 78);
                        return TP;
                    }
                }
            }
        }
        break;
    case 3:
        for (uint8 i = 70; i >= 15; i--)
        {
            if ((Points_L[i + 5][0] < Points_L[i + 1][0]) && (Points_L[i + 4][0] < Points_L[i + 1][0]) && (Points_L[i + 3][0] < Points_L[i + 1][0])
                && (Points_L[i + 5][0] <= Points_L[i + 4][0]) && (Points_L[i + 4][0] <= Points_L[i + 3][0]) && (Points_L[i - 3][0] <= Points_L[i - 2][0])
                && (Points_L[i - 4][0] <= Points_L[i - 1][0]) && (Points_L[i - 3][0] <= Points_L[i - 1][0]) && (Points_L[i - 2][0] <= Points_L[i - 1][0])
                && (Points_L[i - 4][0] <= Points_L[i - 3][0]) && (Points_L[i - 3][0] <= Points_L[i - 2][0]))
            {
                TP.x = Points_L[i][0];
                TP.y = Points_L[i][1];
                TP.find_flag = 1;
                return TP;
            }
        }
        break;
    case 4:
        if (Img_FindStartPoint_Single(180, 20, 3, &startx, &starty))
        {
            Img_SearchLine_Lefthand(startx, starty, Points_Single, &Points_Single_Count);
            Img_CalulateAngle_Single(Points_Single, Points_Single_Count, Angle, &Angle_Count);
            if (Angle_Count >= 4)
            {
                for (uint8 i = 0; i < Angle_Count - 1; i++)
                {
                    if (my_jiajiao(Angle[i], Angle[i + 2]) > 60)
                    {
                        TP.find_flag = 1;
                        TP.x = Points_Single[(i + 1) * 6 + 5][0];
                        TP.y = Points_Single[(i + 1) * 6 + 5][1];
                        TP.k = 0;
                        return TP;
                    }
                }
            }
        }
        break;
    }
    return TP;
}

/**
 * \brief 找右环岛拐点
 *
 * \param form 1:Rightdown   2:Rightup   3:Midguaidian   4:Left_out
 * \return 1:找到 0:未找到
 */
Turn_Point Find_Huandao_R_TP(uint8 form)
{
    uint8 startx = 0, starty = 0;
    uint16 Points_Single[MT9V03X_W * 2][2] = {0};
    uint16 Points_Single_Count = 0;
    int16 Angle[MT9V03X_H] = {0};
    uint8 Angle_Count=0;
    Turn_Point TP = { 0,0,0,0 };
    switch (form)
    {
    case 1:
        if (Img_FindStartPoint_Single(180, 25, 3, &startx, &starty))
        {
            Img_SearchLine_Lefthand(startx, starty, Points_Single, &Points_Single_Count);
            Img_CalulateAngle_Single(Points_Single, Points_Single_Count, Angle, &Angle_Count);
            if (Angle_Count >= 4)
            {
                for (uint8 i = 0; i < Angle_Count - 1; i++)
                {
                    if ((Angle[i] < 180 && Angle[i] > 90) && (Angle[i + 2] > 260))
                    {
                        TP.find_flag = 1;
                        TP.x = Points_Single[(i + 1) * 6 + 5][0];
                        TP.y = Points_Single[(i + 1) * 6 + 5][1];
                        TP.k = 0;
                        return TP;
                    }
                }
            }
        }
        break;
    case 2:
        for (uint8 i = 79; i >= 9; i--)
        {
            if (((int16)Points_R[i][0] - (int16)Points_R[i-1][0]) > 20)
            {
                if (my_abs(((int16)Points_R[i - 1][0] - (int16)Points_R[i - 3][0])) >= 12)
                    continue;
                TP.find_flag = 1;
                TP.x = Points_R[i - 1][0];
                TP.y = i;
                TP.k = my_k_twopoints(TP.x, TP.y, 2, 78);
                return TP;
            }
        }
        for (uint8 i = 79; i >= 9; i--)
        {
            if (((int16)Points_L[i-1][0] - (int16)Points_L[i][0]) > 20)
            {
                if (my_abs(((int16)Points_L[i - 1][0] - (int16)Points_L[i - 3][0])) >= 12)
                    continue;
                TP.find_flag = 1;
                TP.x = Points_L[i - 1][0];
                TP.y = i;
                TP.k = my_k_twopoints(TP.x, TP.y, 2, 78);
                return TP;
            }
        }
        if (Img_FindStartPoint_Single(7, 8, 1, &startx, &starty))
        {
            Img_SearchLine_Lefthand(startx, starty, Points_Single, &Points_Single_Count);
            Img_CalulateAngle_Single(Points_Single, Points_Single_Count, Angle, &Angle_Count);
            if (Angle_Count >= 4)
            {
                for (uint8 i = 0; i < Angle_Count - 1; i++)
                {
                    if (my_jiajiao(Angle[i], Angle[i+2])>80)
                    {
                        TP.find_flag = 1;
                        TP.x = Points_Single[(i + 1) * 6 + 5][0];
                        TP.y = Points_Single[(i + 1) * 6 + 5][1];
                        TP.k = my_k_twopoints(TP.x, TP.y, 2, 78);
                        return TP;
                    }
                }
            }
        }
        break;
    case 3:
        for (uint8 i = 70; i >= 15; i--)
        {
            if ((Points_R[i + 5][0] > Points_R[i + 1][0]) && (Points_R[i + 4][0] > Points_R[i + 1][0]) && (Points_R[i + 3][0] > Points_R[i + 1][0])
                && (Points_R[i + 5][0] >= Points_R[i + 4][0]) && (Points_R[i + 4][0] >= Points_R[i + 3][0]) && (Points_R[i - 3][0] >= Points_R[i - 2][0])
                && (Points_R[i - 4][0] >= Points_R[i - 1][0]) && (Points_R[i - 3][0] >= Points_R[i - 1][0]) && (Points_R[i - 2][0] >= Points_R[i - 1][0])
                && (Points_R[i - 4][0] >= Points_R[i - 3][0]) && (Points_R[i - 3][0] >= Points_R[i - 2][0]))
            {
                TP.x = Points_R[i][0];
                TP.y = Points_R[i][1];
                TP.find_flag = 1;
                return TP;
            }
        }
        break;
    case 4:
        if (Img_FindStartPoint_Single(7, 20, 3, &startx, &starty))
        {
            Img_SearchLine_Righthand(startx, starty, Points_Single, &Points_Single_Count);
            Img_CalulateAngle_Single(Points_Single, Points_Single_Count, Angle, &Angle_Count);
            if (Angle_Count >= 4)
            {
                for (uint8 i = 0; i < Angle_Count - 1; i++)
                {
                    if (my_jiajiao(Angle[i], Angle[i + 2]) > 60)
                    {
                        TP.find_flag = 1;
                        TP.x = Points_Single[(i + 1) * 6 + 5][0];
                        TP.y = Points_Single[(i + 1) * 6 + 5][1];
                        TP.k = 0;
                        return TP;
                    }
                }
            }
        }
        break;
    }
    return TP;
}

/**
 * \brief 改变左边线
 *
 * \param x 经过点的x坐标
 * \param y 经过点的y坐标
 * \param k 斜率
 * \param start_y 开始的行数
 * \param stop_y 结束的行数
 */
void Img_ChangePoints_Left(uint16 x, uint16 y, float k, uint16 start_y, uint16 stop_y)
{
    float b = 0;
    b = (float)y - k * (float)x;
    for (int i = my_min(start_y, stop_y); i <= my_max(start_y, stop_y); i++)
    {
        if (Points_L[i][0] < (int16)(Points_L[i][1] - b) / k)
        {
            Points_L[i][0] = limit_a_b((int16)(Points_L[i][1] - b) / k, 1, 186);
        }
    }
}

/**
 * \brief 改变右边线
 *
 * \param x 经过点的x坐标
 * \param y 经过点的y坐标
 * \param k 斜率
 * \param start_y 开始的行数
 * \param stop_y 结束的行数
 */
void Img_ChangePoints_Right(uint16 x, uint16 y, float k, uint16 start_y, uint16 stop_y)
{
    float b = 0;
    b = (float)y - k * (float)x;
    for (uint16 i = my_min(start_y, stop_y); i <= my_max(start_y, stop_y); i++)
    {
        if (Points_R[i][0] > (int16)(Points_R[i][1] - b) / k)
        {
            Points_R[i][0] = limit_a_b((int16)(Points_R[i][1] - b) / k, 1, 186);
        }
    }
}

/**
 * \brief 以右边线拟合左边线
 *
 * \param start_y 开始的行数
 * \param stop_y 结束的行数
 */
void Img_Adapt_Right_To_Left(uint16 start_y, uint16 stop_y)
{
    for (uint16 i = my_min(start_y, stop_y); i <= my_max(start_y, stop_y); i++)
    {
        Points_L[i][0] = my_max(1, Points_R[i][0] - Width(i));
    }
}

/**
 * \brief 以左边线拟合右边线
 *
 * \param start_y 开始的行数
 * \param stop_y 结束的行数
 */
void Img_Adapt_Left_To_Right(uint16 start_y, uint16 stop_y)
{
    for (uint16 i = my_min(start_y, stop_y); i <= my_max(start_y, stop_y); i++)
    {
        Points_R[i][0] = my_min(186, Points_L[i][0] + Width(i));
    }
}

/**
 * \brief 处理十字
 *
 */
void Cross_Handler(void)
{
    switch (Cross)
    {
    case 1:
    {
        Turn_Point leftdown, rightdown;
        leftdown = Find_Cross_TP(1);
        rightdown = Find_Cross_TP(3);
        if (leftdown.find_flag == 1 && rightdown.find_flag == 1)
        {
            Img_ChangePoints_Left(leftdown.x, leftdown.y, leftdown.k, leftdown.y, 2);
            Img_ChangePoints_Right(rightdown.x, rightdown.y, rightdown.k, rightdown.y, 2);
            Follow_Mode = 0;
        }
        else
            Follow_Mode = 1;
        break;
    }
    case 2:
    {
        Turn_Point leftup, rightup;
        leftup = Find_Cross_TP(2);
        rightup = Find_Cross_TP(4);
        if (leftup.find_flag == 1 && rightup.find_flag == 1)
        {
            Img_ChangePoints_Left(leftup.x, leftup.y, leftup.k, leftup.y, 78);
            Img_ChangePoints_Right(rightup.x, rightup.y, rightup.k, rightup.y, 78);
            Follow_Mode = 0;
        }
        else
            Follow_Mode = 1;
        break;
    }
    }
}

/**
 * \brief 处理左环岛
 *
 */
void Huandao_L_Handler(void)
{
    switch (Huandao_L)
    {
    case 1:
        Img_Adapt_Right_To_Left(75, 10);
        Follow_Mode = 0;
        break;
    case 2:
    {
        Turn_Point Midguaidian;
        Midguaidian = Find_Huandao_L_TP(3);
        if (Midguaidian.find_flag == 1)
        {
            Img_Adapt_Right_To_Left(75, Midguaidian.y);
            Follow_Mode=0;
        }
        else
        {
            Follow_Mode = 1;
        }
        break;
    }
    case 3:
    {
        Turn_Point Leftup;
        Leftup=Find_Huandao_L_TP(2);
        if(Leftup.find_flag==1)
        {
            Follow_Mode = 0;
            Img_ChangePoints_Right(Leftup.x,Leftup.y,Leftup.k,Leftup.y,78);
        }
        else
        {
            Follow_Mode=1;
        }
        break;
    }
    case 4:
        Follow_Mode = 0;
        break;
    case 5:
    {
        Turn_Point Rightout;
        Rightout = Find_Huandao_L_TP(4);
        if (Rightout.find_flag == 1)
        {
            Follow_Mode = 0;
            Img_ChangePoints_Right(Rightout.x, Rightout.y, 0.8, Rightout.y, 2);
        }
        else
        {
            Follow_Mode = 0;
            Img_ChangePoints_Right(186, 77, 0.9, 77, 2);
        }
        break;
    }
    case 6:
        Img_Adapt_Right_To_Left(75, 10);
        Follow_Mode = 0;
        break;
    case 7:
        Img_Adapt_Right_To_Left(75, 10);
        Follow_Mode = 0;
        break;
    }
}

/**
 * \brief 处理右环岛
 *
 */
void Huandao_R_Handler(void)
{
    switch (Huandao_R)
    {
    case 1:
        Img_Adapt_Left_To_Right(75, 10);
        Follow_Mode = 0;
        break;
    case 2:
    {
        Turn_Point Midguaidian;
        Midguaidian = Find_Huandao_R_TP(3);
        if (Midguaidian.find_flag == 1)
        {
            Img_Adapt_Left_To_Right(75, Midguaidian.y);
            Follow_Mode=0;
        }
        else
        {
            Follow_Mode = 1;
        }
        break;
    }
    case 3:
    {
        Turn_Point Rightup;
        Rightup=Find_Huandao_R_TP(2);
        if(Rightup.find_flag==1)
        {
            Follow_Mode = 0;
            Img_ChangePoints_Left(Rightup.x,Rightup.y,Rightup.k,Rightup.y,78);
        }
        else
        {
            Follow_Mode=1;
        }
        break;
    }
    case 4:
        Follow_Mode = 0;
        break;
    case 5:
    {
        Turn_Point Leftout;
        Leftout = Find_Huandao_R_TP(4);
        if (Leftout.find_flag == 1)
        {
            Follow_Mode = 0;
            Img_ChangePoints_Left(Leftout.x, Leftout.y, -0.8, Leftout.y, 2);
        }
        else
        {
            Follow_Mode = 0;
            Img_ChangePoints_Left(2, 77, -0.9, 77, 2);
        }
        break;
    }
    case 6:
        Img_Adapt_Left_To_Right(75, 10);
        Follow_Mode = 0;
        break;
    case 7:
        Img_Adapt_Left_To_Right(75, 10);
        Follow_Mode = 0;
        break;
    }
}

extern float Curve;
extern uint16 Midline_Points[MT9V03X_H][2];
float curve_filter_line[5];
/**
 * \brief 计算中线的方差，判断弯道弯曲程度
 *
 */
void Img_UpdateCurve(void)
{
    //拟合中线   x=by+a,y=(x-a)/b
    int32 xsum = 0, ysum = 0, xysum = 0, x2sum = 0;
    int16 x, y, xy, x2;
    uint8 i = 0;
    float b = 0;
    float a = 0;
    for (i = 20; i < 80; i++)
    {
        x = Midline_Points[i][1];
        y = Midline_Points[i][0];
        xy = x * y;
        x2 = x * x;
        xsum += x;
        ysum += y;
        xysum += xy;
        x2sum += x2;
    }
    if (60 * x2sum - xsum * xsum) //判断除数是否为零
        b = (float)(60 * xysum - xsum * ysum) / (float)(60 * x2sum - xsum * xsum);
    else//除数为零则为直角
        b = 200;
    if(abs(b)>200)
        b=200;
    a=(float)ysum/60-b*(float)xsum/60;

    float curve_sum=0,curve_cul=0;
    for(i=0;i<80;i++)
    {
        curve_sum+=pow((float)i*b+a-(float)Midline_Points[i][0],2);
    }
    curve_cul=curve_sum/80;

    //对数据进行均值滤波
    for(i=0;i<4;i++)
    {
        curve_filter_line[i]=curve_filter_line[i+1];//数据前移一位
    }
    curve_filter_line[4]=curve_cul;//放入新的数据
    float sum1=0;
    for(uint8 i=0;i<5;i++)
    {
        sum1+=curve_filter_line[i];
    }
    Curve=limit_a_b_f(pow(sum1/5,0.5), 0, 10);
}

/**
 * \brief 决定中线是否有效
 *
 */
void Running_Handler(void)
{
    switch (Follow_Mode)
    {
    case 0://寻中线
        Img_FindMidline();
        Img_UpdateCurve();
        break;
    case 1://无效帧
        break;
    }
}

/**
 * \brief 元素判断、状态切换、元素处理
 *
 */
void Img_ElementsProcess(void)
{
    Follow_Mode = 0;
    switch (Img_Judge_LostLine(30))
    {
    case 0:
        if ((Cross == 2 || Cross == 1) && (Img_Judge_LostLine(65) != 3))  Cross = 0;
        if (Huandao_L == 1 && Find_Huandao_L_TP(3).find_flag==1)  Huandao_L = 2;
        if ((Huandao_L == 7) && Img_Judge_LostLine(50) == 0 && Img_Judge_StraightLine_Right(65, 10) == 1
            && Img_Judge_StraightLine_Left(65, 10) == 1)
        {
            Mpu_Integral_Flag = 0;
            mpu_z = 0;
            Huandao_L = 0;
        }
        if (Huandao_R == 1 && Find_Huandao_R_TP(3).find_flag==1)  Huandao_R = 2;
        if ((Huandao_R == 7) && Img_Judge_LostLine(50) == 0 && Img_Judge_StraightLine_Right(65, 10) == 1
            && Img_Judge_StraightLine_Left(65, 10) == 1)
        {
            Mpu_Integral_Flag = 0;
            mpu_z = 0;
            Huandao_R = 0;
        }
        break;
    case 1:
        if ((Cross == 2 || Cross == 1) && (Img_Judge_LostLine(65) != 3))  Cross = 0;
        if (Huandao_L == 0 && Huandao_R == 0 && Img_Judge_StraightLine_Right(70, 10) == 1
            && Img_Judge_StraightLine_Left(70, 10) == 0 && Cross == 0
            && Find_Huandao_L_TP(1).find_flag == 1 && Find_Cross_TP(3).find_flag == 0)
        {
            Mpu_Integral_Flag = 1;
            mpu_z = 0;
            Huandao_L = 1;
        }
        if (Huandao_L == 2 && Img_Judge_LostLine(40)==0)  Huandao_L = 3;
        break;
    case 2:
        if ((Cross == 2 || Cross == 1) && (Img_Judge_LostLine(65) != 3))  Cross = 0;
        if (Huandao_R == 0 && Huandao_L == 0 && Img_Judge_StraightLine_Left(70, 10) == 1
            && Img_Judge_StraightLine_Right(70, 10) == 0 && Cross == 0
            && Find_Huandao_R_TP(1).find_flag == 1 && Find_Cross_TP(1).find_flag == 0)
        {
            Mpu_Integral_Flag = 1;
            mpu_z = 0;
            Huandao_R = 1;
        }
        if (Huandao_R == 2 && Img_Judge_LostLine(40) == 0)  Huandao_R = 3;
        if (Huandao_R == 6)  Huandao_R = 7;
        break;
    case 3:
        if (Cross == 0 && Img_Judge_LostLine(60) != 3
            && (Find_Cross_TP(1).find_flag == 1 || Find_Cross_TP(3).find_flag == 1)
            && Huandao_L != 4 && Huandao_L != 5 && Huandao_L != 6 && Huandao_L != 7 &&Huandao_L!=3
            && Huandao_R != 4 && Huandao_R != 5 && Huandao_R != 6 && Huandao_R != 7&& Huandao_R != 3)
        {
            Huandao_L = 0;
            Huandao_R = 0;
            Cross = 1;
        }
        if (Cross == 1 && Img_Judge_LostLine(65) != 0 && Img_Judge_LostLine(4)!=3)
            Cross = 2;
        break;
    }
    if (Huandao_L == 3 && mpu_z < -45)
        Huandao_L = 4;
    if (Huandao_L == 4 && mpu_z < -250)
        Huandao_L = 5;
    if (Huandao_L == 5 && mpu_z < -350)
        Huandao_L = 6;
    if (Huandao_R == 3 && mpu_z > 45)
        Huandao_R = 4;
    if (Huandao_R == 4 && mpu_z > 250)
        Huandao_R = 5;
    if (Huandao_R == 5 && mpu_z > 350)
        Huandao_R = 6;
    if (Huandao_L == 6 && Find_Huandao_L_TP(2).find_flag == 1)
        Huandao_L = 7;
    if (Huandao_R == 6 && Find_Huandao_R_TP(2).find_flag == 1)
        Huandao_R = 7;
    Cross_Handler();
    Huandao_L_Handler();
    Huandao_R_Handler();
    Running_Handler();
}
