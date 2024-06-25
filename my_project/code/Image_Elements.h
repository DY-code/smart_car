/*
 * Image_Elements.h
 *
 *  Created on: 2024年4月2日
 *      Author: lilinyang
 */

#ifndef __IMAGE_ELEMENTS_H__
#define __IMAGE_ELEMENTS_H__

typedef struct{         //拐点
    uint8 find_flag;        //拐点找到后置1，未找到置0
    uint8 x;                //横坐标
    uint8 y;                //纵坐标
    float k;                //补线需要的斜率
}Turn_Point;

//////////////////////外部调用//////////////////////
void Img_ElementsProcess(void);
void Width_Reinit(void);
//////////////////////外部调用//////////////////////

uint16 Width(uint8 y);
Turn_Point Find_Cross_TP(uint8 form);
Turn_Point Find_Huandao_L_TP(uint8 form);
Turn_Point Find_Huandao_R_TP(uint8 form);
void Img_ChangePoints_Left(uint16 x, uint16 y, float k, uint16 start_y, uint16 stop_y);
void Img_ChangePoints_Right(uint16 x, uint16 y, float k, uint16 start_y, uint16 stop_y);
void Img_Adapt_Right_To_Left(uint16 start_y, uint16 stop_y);
void Img_Adapt_Left_To_Right(uint16 start_y, uint16 stop_y);
void Cross_Handler(void);
void Huandao_L_Handler(void);
void Huandao_R_Handler(void);
void Running_Handler(void);


#endif
