#ifndef __IMAGE_PROCESS_H__
#define __IMAGE_PROCESS_H__

#define CONTRAST 23                //对比度阈值设置
#define PIANZHI 4                  //局部阈值偏置

void Img_ProcessInit(void);
int16 Img_GetError(void);
void Img_Display(void);
void Img_FinalProcess(void);

//////////////////////数学运算//////////////////////
float my_abs(float value);
int16 limit_a_b(int16 x, int16 a, int16 b);
float limit_a_b_f(float x, float a,float b);
int16 my_min(int16 x, int16 y);
int16 my_max(int16 x, int16 y);
float my_k_twopoints(uint16 x1, uint16 y1, uint16 x2, uint16 y2);
float my_k(uint16 begin, uint16 end, uint16 border[][2]);
int16 my_K_To_Angle(uint16 x1, uint16 y1, uint16 x2, uint16 y2);
int16 my_jiajiao(int16 ang1, int16 ang2);
//////////////////////数学运算//////////////////////

//////////////////////图像操作//////////////////////
void Img_CopyImage(void);
uint8 Img_GetAverage(uint8 image[MT9V03X_H][MT9V03X_W], uint16 x, uint16 y, uint16 width, uint16 height);
uint8 Img_GetPixel(uint8 y, uint8 x);
int8 Img_GetContrast(int16 value, int16 next_value);
void Img_SearchLine(void);
void Img_FindMidline(void);
void Img_BlurPoints(uint16 pts_in[][2], uint16 num, uint16 pts_out[][2], int kernel);
uint8 Img_Judge_StraightLine_Left(uint16 start_y, uint16 end_y);
uint8 Img_Judge_StraightLine_Right(uint16 start_y, uint16 end_y);
uint8 Img_Judge_LostLine(uint16 test_y);
//////////////////////图像操作//////////////////////

/////////////////图像操作（外部调用）////////////////
uint8 Img_FindStartPoint_Single(uint8 find_x, uint8 find_y, uint8 dir, uint8 *start_x, uint8 *start_y);
void Img_SearchLine_Lefthand(uint8 start_x, uint8 start_y, uint16 pts[][2], uint16 *num);
void Img_SearchLine_Righthand(uint8 start_x, uint8 start_y, uint16 pts[][2], uint16 *num);
void Img_CalulateAngle_Single(uint16 pts_in[][2], uint16 num_in, int16 pts_out[], uint8 *num_out);
uint8 Img_TestWidth(uint8 Find_y);
/////////////////图像操作（外部调用）////////////////

#endif
