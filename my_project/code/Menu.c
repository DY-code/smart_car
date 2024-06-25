/*
 * Menu.c
 *
 *  Created on: 2024年4月6日
 *      Author: lilinyang
 */
#include "CUG_Headfile.h"
uint8 PageFlag = 0, ArrowFlag=1;

typedef struct{
    uint8 pagenumber;//页码标号
    uint8 arrownumber;//选项个数
    uint8 lastpage;//上一级的页码编号
    uint8 nextpage[5];//下一级的页码编号
}Page;

Page Main={0,3,0,{0,1,5,8}};
Page PID_Main={1,3,0,{0,2,3,4}};
Page PID_Img={2,0,1};
Page PID_Ang={3,0,1};
Page PID_Speed={4,0,1};
Page Img_Main={5,3,0,{0,6,7,9}};
Page Img_Contrast={6,1,5};
Page Img_Pianzhi={7,1,5};
Page Speed_Page={8,1,0};
Page Width_Reset={9,0,5};

void Menu_Clear(void)
{
    for(uint8 i=0;i<188;i++)
    {
        for(uint8 j=80;j<240;j++)
            ips200_draw_point(i,j,RGB565_WHITE);
    }
}

void Menu_ShowArrow(void)
{
    for(uint8 line=1;line<=5;line++)
    {
        if(line==ArrowFlag)
        {
            ips200_show_char(0,90+20*((uint16)line-1),'>');
        }
        else
        {
            ips200_show_char(0,90+20*((uint16)line-1),' ');
        }
    }
}

void arrowadd(Page x)
{
    if(ArrowFlag<x.arrownumber)
        ArrowFlag++;
}

void arrowsub(void)
{
    if(ArrowFlag>1)
        ArrowFlag--;
}

void Main_Handler(void)
{
    ips200_show_string(10,90,"PID");
    ips200_show_string(10,110,"Image");
    ips200_show_string(10,130,"Speed");
    Menu_ShowArrow();
    if(key_get_state(KEY_2)==KEY_SHORT_PRESS)
    {
        arrowadd(Main);
        key_clear_state(KEY_2);
    }
    if(key_get_state(KEY_1)==KEY_SHORT_PRESS)
    {
        arrowsub();
        key_clear_state(KEY_1);
    }
    if(key_get_state(KEY_3)==KEY_SHORT_PRESS)
    {
        PageFlag=Main.nextpage[ArrowFlag];
        ArrowFlag=1;
        key_clear_state(KEY_3);
    }
}

extern Pid_posi_Set pid_img;
extern Pid_posi_Set pid_ang;
extern Pid_posi_Set pid_speed;

void PID_Main_Handler(void)
{
    ips200_show_string(10,90,"PID_Img");
    ips200_show_string(10,110,"PID_Ang");
    ips200_show_string(10,130,"PID_SPeed");
    Menu_ShowArrow();
    if(key_get_state(KEY_2)==KEY_SHORT_PRESS)
    {
        arrowadd(PID_Main);
        key_clear_state(KEY_2);
    }
    if(key_get_state(KEY_1)==KEY_SHORT_PRESS)
    {
        arrowsub();
        key_clear_state(KEY_1);
    }
    if(key_get_state(KEY_3)==KEY_SHORT_PRESS)
    {
        PageFlag=PID_Main.nextpage[ArrowFlag];
        ArrowFlag=0;
        key_clear_state(KEY_3);
    }
    if(key_get_state(KEY_4)==KEY_SHORT_PRESS)
    {
        PageFlag=PID_Main.lastpage;
        ArrowFlag=1;
        key_clear_state(KEY_4);
    }
}

void PID_Img_Handler(void)
{
    ips200_show_string(10,90,"P:");
    ips200_show_float(30, 90, (const double)pid_img.kp, 2, 2);
    ips200_show_string(10,110,"I:");
    ips200_show_float(30, 110, (const double)pid_img.ki, 2, 2);
    ips200_show_string(10,130,"D:");
    ips200_show_float(30, 130, (const double)pid_img.kd, 2, 2);
    if(key_get_state(KEY_4)==KEY_SHORT_PRESS)
    {
        PageFlag=PID_Img.lastpage;
        ArrowFlag=1;
        key_clear_state(KEY_4);
    }
}

void PID_Ang_Handler(void)
{
    ips200_show_string(10,90,"P:");
    ips200_show_float(30, 90, (const double)pid_ang.kp, 2, 2);
    ips200_show_string(10,110,"I:");
    ips200_show_float(30, 110, (const double)pid_ang.ki, 2, 2);
    ips200_show_string(10,130,"D:");
    ips200_show_float(30, 130, (const double)pid_ang.kd, 2, 2);
    if(key_get_state(KEY_4)==KEY_SHORT_PRESS)
    {
        PageFlag=PID_Ang.lastpage;
        ArrowFlag=1;
        key_clear_state(KEY_4);
    }
}

void PID_Speed_Handler(void)
{
    ips200_show_string(10,90,"P:");
    ips200_show_float(30, 90, (const double)pid_speed.kp, 2, 2);
    ips200_show_string(10,110,"I:");
    ips200_show_float(30, 110, (const double)pid_speed.ki, 2, 2);
    ips200_show_string(10,130,"D:");
    ips200_show_float(30, 130, (const double)pid_speed.kd, 2, 2);
    if(key_get_state(KEY_4)==KEY_SHORT_PRESS)
    {
        PageFlag=PID_Speed.lastpage;
        ArrowFlag=1;
        key_clear_state(KEY_4);
    }
}

void Img_Main_Handler(void)
{
    ips200_show_string(10,90,"Information");
    ips200_show_string(10,110,"Pianzhi");
    ips200_show_string(10,130,"Width");
    Menu_ShowArrow();
    if(key_get_state(KEY_2)==KEY_SHORT_PRESS)
    {
        arrowadd(Img_Main);
        key_clear_state(KEY_2);
    }
    if(key_get_state(KEY_1)==KEY_SHORT_PRESS)
    {
        arrowsub();
        key_clear_state(KEY_1);
    }
    if(key_get_state(KEY_3)==KEY_SHORT_PRESS)
    {
        PageFlag=Img_Main.nextpage[ArrowFlag];
        ArrowFlag=0;
        key_clear_state(KEY_3);
    }
    if(key_get_state(KEY_4)==KEY_SHORT_PRESS)
    {
        PageFlag=Img_Main.lastpage;
        ArrowFlag=1;
        key_clear_state(KEY_4);
    }
}
extern uint8 Huandao_L, Huandao_R, Cross, Follow_Mode;
extern float Curve;
void Img_Contrast_Handler(void)
{
    ips200_show_string(10,90,"Information:");
        ips200_show_string(10, 110, "Cross:");
        ips200_show_uint(90, 110, (const uint32)Cross, 3);
        ips200_show_string(10, 130, "HD_L:");
        ips200_show_uint(90, 130, (const uint32)Huandao_L, 3);
        ips200_show_string(10, 150, "HD_R:");
        ips200_show_uint(90, 150, (const uint32)Huandao_R, 3);
        ips200_show_string(10, 170, "FM:");
        ips200_show_uint(90, 170, (const uint32)Follow_Mode, 3);
        ips200_show_string(10, 190, "Curve:");
        ips200_show_float(90, 190, (const double)Curve, 4,3);
    if(key_get_state(KEY_4)==KEY_SHORT_PRESS)
    {
        PageFlag=Img_Contrast.lastpage;
        ArrowFlag=1;
        key_clear_state(KEY_4);
    }
}

void Img_Pianzhi_Handler(void)
{
    ips200_show_string(10,90,"Pianzhi:");
    if(key_get_state(KEY_4)==KEY_SHORT_PRESS)
    {
        PageFlag=Img_Pianzhi.lastpage;
        ArrowFlag=1;
        key_clear_state(KEY_4);
    }
}

void Speed_Handler(void)
{
    ips200_show_string(10,90,"Speed:");
    if(key_get_state(KEY_4)==KEY_SHORT_PRESS)
    {
        PageFlag=Speed_Page.lastpage;
        ArrowFlag=1;
        key_clear_state(KEY_4);
    }
}

extern uint16 Width0;
extern float Width_Step;

void Width_Reset_Handler(void)
{
    ips200_show_string(10,90,"Width_Reset: KEY3");
    ips200_show_string(10,110,"Width0=");
    ips200_show_uint(100, 110, (const uint32)Width0, 3);
    ips200_show_string(10,130,"Width_Step=");
    ips200_show_float(100, 130, (const double)Width_Step, 3,2);
    if(key_get_state(KEY_3)==KEY_SHORT_PRESS)
    {
        Width_Reinit();
        key_clear_state(KEY_3);
    }
    if(key_get_state(KEY_4)==KEY_SHORT_PRESS)
    {
        PageFlag=Width_Reset.lastpage;
        ArrowFlag=1;
        key_clear_state(KEY_4);
    }
}

//Page Main={0,3,0,{0,1,5,8}};
//Page PID_Main={1,3,0,{0,2,3,4}};
//Page PID_Img={2,0,1};
//Page PID_Ang={3,0,1};
//Page PID_Speed={4,0,1};
//Page Img_Main={5,3,0,{0,6,7,9}};
//Page Img_Contrast={6,1,5};
//Page Img_Pianzhi={7,1,5};
//Page Speed={8,1,0};
//Page Width_Reset={9,0,5};
uint8 last_pageflag=0;
void Menu_Display(void)
{
    if(last_pageflag!=PageFlag)
        Menu_Clear();
    last_pageflag=PageFlag;
    switch(PageFlag)
    {
    case 0:
        Main_Handler();
        break;
    case 1:
        PID_Main_Handler();
        break;
    case 2:
        PID_Img_Handler();
        break;
    case 3:
        PID_Ang_Handler();
        break;
    case 4:
        PID_Speed_Handler();
        break;
    case 5:
        Img_Main_Handler();
        break;
    case 6:
        Img_Contrast_Handler();
        break;
    case 7:
        Img_Pianzhi_Handler();
        break;
    case 8:
        Speed_Handler();
        break;
    case 9:
        Width_Reset_Handler();
        break;
    }
}
