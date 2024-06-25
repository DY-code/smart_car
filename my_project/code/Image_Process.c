#include "zf_common_headfile.h"
#include "math.h"
#include "Image_Process.h"
#include "Image_Elements.h"

// --------->x|W
//|*********
//|*********
//|*********   80
//v y|H     188

//黑：0  白：255
uint8 BMP[MT9V03X_H][MT9V03X_W] = {0};                                      //储存图像

uint16 Points_L[MT9V03X_H][2] = { {0},{0} };                                //左边线
uint16 Points_R[MT9V03X_H][2] = { {0},{0} };                                //右边线
uint16 Count_Left_Data = 79;                                                //左线可用点的最大下标
uint16 Count_Right_Data = 79;                                               //右线可用点的最大下标

uint16 Midline_Points[MT9V03X_H][2] = { {0},{0} };                          //中线坐标
uint16 Midline_Points_Count = 79;                                           //中线点的最大下标

uint16 Points_L_B[MT9V03X_W * 2][2] = { {0},{0} };                          //储存滤波后的左边线 0-x 1-y
uint16 Points_R_B[MT9V03X_W * 2][2] = { {0},{0} };                          //储存滤波后的右边线 0-x 1-y

/**
 * \brief 求绝对值
 * 
 * \param value 要求的值
 * \return 求出的绝对值
 */
float my_abs(float value)
{
    if (value >= 0)
        return value;
    else
        return -value;
}

/**
 * \brief 限幅
 * 
 * \param x 原数据
 * \param a 最小值
 * \param b 最大值
 * \return 限幅后的值
 */
int16 limit_a_b(int16 x, int16 a, int16 b)
{
    if (x < a)
        x = a;
    if (x > b)
        x = b;
    return x;
}

float limit_a_b_f(float x, float a,float b)
{
    if (x < a)
        x = a;
    if (x > b)
        x = b;
    return x;
}

/**
 * \brief 求x,y中的最小值
 * 
 * \param x
 * \param y
 * \return 最小值
 */
int16 my_min(int16 x, int16 y)
{
    if (x > y)
        return y;
    else
        return x;
}

/**
 * \brief 求x,y中的最大值
 * 
 * \param x 
 * \param y
 * \return 最大值
 */
int16 my_max(int16 x, int16 y)
{
    if (x > y)
        return x;
    else
        return y;
}
/**
 * \brief 两点求斜率(1指向2)
 * 
 * \param x1 第一个点的坐标
 * \param y1 第一个点的坐标
 * \param x2 第二个点的坐标
 * \param y2 第二个点的坐标
 * \return 斜率
 */
float my_k_twopoints(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
    if ((x2 - x1) == 0&&y1<y2)
        return 2000;//沿坐标系向y正
    if ((x2 - x1) == 0 && y1 > y2)
        return -2000;//沿坐标系向y负
    return (float)(y2 - y1) / (float)(x2 - x1);
}

/**
 * \brief 线性回归求斜率
 *
 * \param begin 起始下标
 * \param end 结束下标
 * \param border
 * \return 斜率
 */
float my_k(uint16 begin, uint16 end, uint16 border[][2])
{
    int32 xsum = 0, ysum = 0, xysum = 0, x2sum = 0;
    int16 x, y, xy, x2;
    int16 i = 0;
    float result = 0;
    for (i = begin; i <= end; i++)
    {
        x = border[i][0];
        y = border[i][1];
        xy = x * y;
        x2 = x * x;
        xsum += x;
        ysum += y;
        xysum += xy;
        x2sum += x2;
    }
    if ((end - begin + 1) * x2sum - xsum * xsum) //判断除数是否为零
    {
        result = (float)((end - begin + 1) * xysum - xsum * ysum) / (float)((end - begin + 1) * x2sum - xsum * xsum);
    }
    else//除数为零则为直角
    {
        result = 2000;
    }
    return result;
}

/**
 * \brief 斜率转角度（以摄像头视角为基础）
 * 
 * \param x1 第一个点的坐标
 * \param y1 第一个点的坐标
 * \param x2 第二个点的坐标
 * \param y2 第二个点的坐标
 * \return
 */
int16 my_K_To_Angle(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
    float k = my_k_twopoints(x1, y1, x2, y2);
    if (k == 2000)//沿坐标系向y正，以摄像头为视角向下
        return 270;
    if (k == -2000)//沿坐标系向y负，以摄像头为视角向上
        return 90;
    int16 a = 0;
    a = (int16)(atan(-k) * 180 / 3.1415);
    if (a == 0)
    {
        if (x1 > x2)
            a = 180;
    }
    if (a < 0)
        a += 180;
    if (y1 < y2)
        a += 180;
    return a;
}

/**
 * \brief 计算夹角
 *
 * \param ang1
 * \param ang2
 * \return 夹角
 */
int16 my_jiajiao(int16 ang1, int16 ang2)
{
    int16 tmp = my_abs(ang1 - ang2);
    if (tmp <= 180)
        return tmp;
    if (tmp > 180)
        return 360-tmp;
    return 0;
}

/**
 * \brief 复制图像
 * 
 */
void Img_CopyImage(void)
{
    DMA_Cmd(DMA1_Channel1, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel1, MT9V03X_H*MT9V03X_W);
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

/**
 * \brief 计算平均阈值
 * 
 * \param x
 * \param y
 * \param width
 * \param height
 */
uint8 Img_GetAverage(uint8 image[MT9V03X_H][MT9V03X_W], uint16 x, uint16 y, uint16 width, uint16 height)
{
    int pixelSum = width * height;
    int i,j,sum = 0;
    for (i = y; i < y + height; i ++)
    {
        for (j = x; j < x + width; j ++)
        {
            sum += image[i][j];
        }
    }
    return (uint8)(sum / pixelSum);
}

/**
 * \brief 获取一个像素的二值化值
 *
 * \param y
 * \param x
 * \return 值
 */
uint8 Img_GetPixel(uint8 y, uint8 x)
{
    limit_a_b(x, 3, MT9V03X_W - 4);
    limit_a_b(y, 3, MT9V03X_H - 4);
    if (BMP[y][x] == 0 || BMP[y][x] == 255)
        return BMP[y][x];
    uint8 threshold = Img_GetAverage(BMP, x - 3, y - 3, 7, 7);
    if (BMP[y][x] > threshold - PIANZHI)
    {
        //BMP[y][x] = 255;
        return 255;
    }
    else
    {
        //BMP[y][x] = 0;
        return 0;
    }
}

/**
 * \brief 差比和求对比度
 *
 * \param value 灰度值
 * \param next_value 下一个灰度值
 * \return 对比度值
 */
int8 Img_GetContrast(int16 value, int16 next_value)
{
    return (value - next_value) * 100 / (value + next_value);
}

//                          前     左    后     右
int8 seeds[4][4][2] = { { {0,-1},{-1,0},{0,1},{1,0} },
                      { {-1,0},{0,1},{1,0},{0,-1} },
                      { {0,1},{1,0},{0,-1},{-1,0} },
                      { {1,0},{0,-1},{-1,0},{0,1} } };//向前0向左1向后2向右3

/**
 * \brief 寻找迷宫法扫任意边线的起始点
 *
 * \param find_x 从此点开始找
 * \param find_y 从此点开始找
 * \param dir 方向 0:向左 1: 向右  2:向上  3:向下
 * \param start_x 找到的起点
 * \param start_y 找到的起点
 * \return 0；未找到 1：找到
 */
uint8 Img_FindStartPoint_Single(uint8 find_x, uint8 find_y, uint8 dir, uint8 *start_x, uint8 *start_y)
{
    switch (dir)
    {
    case 0:
        for (uint8 i = find_x; i >= 7; i--)
        {
            if (Img_GetContrast(BMP[find_y][i], BMP[find_y][i - 4]) > CONTRAST)
            {
                for (uint8 j = i; j >= i - 4; j--)
                {
                    if ((Img_GetPixel(find_y, j) == 0) || (j == i - 4))
                    {
                        *start_x = j+1;
                        *start_y = find_y;
                        return 1;
                    }
                }
            }
        }
        break;
    case 1:
        for (uint8 i = find_x; i <= MT9V03X_W - 8; i++)
        {
            if (Img_GetContrast(BMP[find_y][i], BMP[find_y][i + 4]) > CONTRAST)
            {
                for (uint8 j = i; j <= i + 4; j++)
                {
                    if ((Img_GetPixel(find_y, j) == 0) || (j == i + 4))
                    {
                        *start_x = j-1;
                        *start_y = find_y;
                        return 1;
                    }
                }
            }
        }
        break;
    case 2:
        for (uint8 i = find_y; i >= 7; i--)
        {
            if (Img_GetContrast(BMP[i][find_x], BMP[i-4][find_x]) > CONTRAST)
            {
                for (uint8 j = i; j >= i - 4; j--)
                {
                    if ((Img_GetPixel(j, find_x) == 0) || (j == i - 4))
                    {
                        *start_x = find_x;
                        *start_y = j+1;
                        return 1;
                    }
                }
            }
        }
        break;
    case 3:
        for (uint8 i = find_y; i <= MT9V03X_H - 8; i++)
        {
            if (Img_GetContrast(BMP[i][find_x], BMP[i + 4][find_x]) > CONTRAST)
            {
                for (uint8 j = i; j <= i + 4; j++)
                {
                    if ((Img_GetPixel(j, find_x) == 0) || (j == i + 4))
                    {
                        *start_x = find_x;
                        *start_y = j-1;
                        return 1;
                    }
                }
            }
        }
        break;
    }
    return 0;
}

/**
 * \brief 迷宫法左手扫任意边线
 *
 * \param start_x 开始的点
 * \param start_y 开始的点
 * \param pts 存边线的数组
 * \param num 下标形式的点的个数
 */
void Img_SearchLine_Lefthand(uint8 start_x, uint8 start_y, uint16 pts[][2], uint16 *num)//遇到边界就停止
{
    int8 count = 0;
    //左边变量
    uint16 dir_l = 0;
    uint16 center_point_l[2] = { 0 };
    uint16 l_data_statics = 0;
    uint16 break_flag = 130;
    //填入开始点的数据
    center_point_l[0] = start_x;
    center_point_l[1] = start_y;
    pts[l_data_statics][0] = center_point_l[0];
    pts[l_data_statics][1] = center_point_l[1];
    while (break_flag--)
    {
        if (center_point_l[0] < 6 || center_point_l[0] > 180)
            break;
        if (center_point_l[1] < 6 || center_point_l[1] > 75)
            break;
        l_data_statics++;
        //左边判断
        if (Img_GetPixel(center_point_l[1] + seeds[dir_l][1][1], center_point_l[0] + seeds[dir_l][1][0]) == 0)
        {
            count = 4;
            while (Img_GetPixel(center_point_l[1] + seeds[dir_l][0][1], center_point_l[0] + seeds[dir_l][0][0]) == 0 && count--)
            {
                if (dir_l == 1 || dir_l == 2 || dir_l == 3)
                {
                    dir_l--;
                }
                else if (dir_l == 0)
                {
                    dir_l = 3;
                }
            }
            if (count < 0)
                break;
            center_point_l[1] += seeds[dir_l][0][1];
            center_point_l[0] += seeds[dir_l][0][0];
            pts[l_data_statics][0] = center_point_l[0];//x
            pts[l_data_statics][1] = center_point_l[1];//y
        }
        else
        {
            l_data_statics--;
            if (dir_l == 1 || dir_l == 2 || dir_l == 0)
            {
                dir_l++;
            }
            else if (dir_l == 3)
            {
                dir_l = 0;
            }
            if (l_data_statics > 0)
            {
                center_point_l[1] += seeds[dir_l][0][1];
                center_point_l[0] += seeds[dir_l][0][0];
                pts[l_data_statics][0] = center_point_l[0];//x
                pts[l_data_statics][1] = center_point_l[1];//y
            }
        }
    }
    *num = l_data_statics;
}

/**
 * \brief 迷宫法右手找任意边线
 *
 * \param start_x 开始的点
 * \param start_y 开始的点
 * \param pts 存边线的数组
 * \param num 下标形式的点的个数
 */
void Img_SearchLine_Righthand(uint8 start_x, uint8 start_y, uint16 pts[][2], uint16 *num)//遇到边界就停止
{
    int8 count = 0;
    //右边变量
    uint16 dir_r = 0;
    uint16 center_point_r[2] = { 0 };
    uint16 r_data_statics = 0;
    uint16 break_flag = 130;
    //填入开始点的数据
    center_point_r[0] = start_x;
    center_point_r[1] = start_y;
    pts[r_data_statics][0] = center_point_r[0];
    pts[r_data_statics][1] = center_point_r[1];
    while (break_flag--)
    {
        if (center_point_r[0] < 6 || center_point_r[0] > 180)
            break;
        if (center_point_r[1] < 6 || center_point_r[1] > 75)
            break;
        r_data_statics++;
        //右边判断
        if (Img_GetPixel(center_point_r[1] + seeds[dir_r][3][1], center_point_r[0] + seeds[dir_r][3][0]) == 0)
        {
            count = 4;
            while (Img_GetPixel(center_point_r[1] + seeds[dir_r][0][1], center_point_r[0] + seeds[dir_r][0][0]) == 0 && count--)
            {
                if (dir_r == 1 || dir_r == 2 || dir_r == 0)
                {
                    dir_r++;
                }
                else if (dir_r == 3)
                {
                    dir_r = 0;
                }
            }
            if (count < 0)
                break;
            center_point_r[1] += seeds[dir_r][0][1];
            center_point_r[0] += seeds[dir_r][0][0];
            pts[r_data_statics][0] = center_point_r[0];//x
            pts[r_data_statics][1] = center_point_r[1];//y
        }
        else
        {
            r_data_statics--;
            if (dir_r == 1 || dir_r == 2 || dir_r == 3)
            {
                dir_r--;
            }
            else if (dir_r == 0)
            {
                dir_r = 3;
            }
            if (r_data_statics > 0)
            {
                center_point_r[1] += seeds[dir_r][0][1];
                center_point_r[0] += seeds[dir_r][0][0];
                pts[r_data_statics][0] = center_point_r[0];//x
                pts[r_data_statics][1] = center_point_r[1];//y
            }
        }
    }
    *num = r_data_statics;
}

/**
 * \brief 计算任意边线的局部斜率（六个点为一组）
 *
 * \param pts_int 传入的边线
 * \param num_in 传入边线下标形式的点的个数
 * \param pts_out 传出的存角度的数组
 * \param num_out 下标形式的角度个数
 */
void Img_CalulateAngle_Single(uint16 pts_in[][2], uint16 num_in, int16 pts_out[], uint8 *num_out)//计算任意边线的局部斜率（六个点为一组）
{
    if (num_in >= 17&&num_in<400)
    {
        *num_out = (num_in + 1) / 6 - 1;
        for (int i = 0; i <= *num_out; i++)
        {
            pts_out[i] = my_K_To_Angle(pts_in[6 * i][0], pts_in[6 * i][1], pts_in[6 * i + 5][0], pts_in[6 * i + 5][1]);
        }
    }
    else
        *num_out = 0;
}

/**
 * \brief 灰度扫线
 *
 */
void Img_SearchLine(void)
{
    static uint8 Last_StartMidPoint = MT9V03X_W / 2;
    uint8 lff = 0, rff = 0;
    for (uint8 j = Last_StartMidPoint; j >= 7; j--)
    {
        if (Img_GetContrast(BMP[79][j], BMP[79][j - 4]) > CONTRAST)
        {
            Points_L[79][0] = j-4;
            Points_L[79][1] = 79;
            lff = 1;
            break;
        }
    }
    for (uint8 j = Last_StartMidPoint; j <= MT9V03X_W - 8; j++)
    {
        if (Img_GetContrast(BMP[79][j], BMP[79][j + 4]) > CONTRAST)
        {
            Points_R[79][0] = j + 4;
            Points_R[79][1] = 79;
            rff = 1;
            break;
        }
    }
    if (lff == 0)
    {
        Points_L[79][0] = 3;
        Points_L[79][1] = 79;
    }
    if (rff == 0)
    {
        Points_R[79][0] = MT9V03X_W - 4;
        Points_R[79][1] = 79;
    }
    Last_StartMidPoint = (Points_L[79][0] + Points_R[79][0]) / 2;
    for (int8 i = 78; i >=0; i--)
    {
        lff = 0;
        rff = 0;
        for (uint8 j = (Points_L[i+1][0]+ Points_R[i+1][0])/2; j >= 7; j--)
        {
            if (Img_GetContrast(BMP[i][j], BMP[i][j - 4]) > CONTRAST)
            {
                Points_L[i][0] = j-4;
                Points_L[i][1] = i;
                lff = 1;
                break;
            }
        }
        for (uint8 j = (Points_L[i + 1][0] + Points_R[i + 1][0]) / 2; j <= MT9V03X_W-8; j++)
        {
            if (Img_GetContrast(BMP[i][j], BMP[i][j + 4]) > CONTRAST)
            {
                Points_R[i][0] = j + 4;
                Points_R[i][1] = i;
                rff = 1;
                break;
            }
        }
        if (lff == 0)
        {
            Points_L[i][0] = 3;
            Points_L[i][1] = i;
        }
        if (rff == 0)
        {
            Points_R[i][0] = MT9V03X_W - 4;
            Points_R[i][1] = i;
        }
    }
}

/**
 * \brief 找到中线坐标
 * 
 */
void Img_FindMidline(void)
{
    for (uint8 i = 0; i < 80; i++)
    {
        Midline_Points[i][0] = (Points_L[i][0] + Points_R[i][0]) / 2;
        Midline_Points[i][1] = i;
    }
}

/**
 * \brief 边线滤波
 *
 * \param pts_in 输入的边线
 * \param num 点的个数（下标形式）
 * \param pts_in 输出的边线
 */
void Img_BlurPoints(uint16 pts_in[][2], uint16 num, uint16 pts_out[][2], int kernel)
{
    int half = kernel / 2;
    for (int i = 0; i < num; i++) {
        pts_out[i][0] = pts_out[i][1] = 0;
        for (int j = -half; j <= half; j++) {
            pts_out[i][0] += pts_in[limit_a_b(i + j, 0, num)][0] * (half + 1 - (int)my_abs(j));
            pts_out[i][1] += pts_in[limit_a_b(i + j, 0, num)][1] * (half + 1 - (int)my_abs(j));
        }
        pts_out[i][0] /= (2 * half + 2) * (half + 1) / 2;
        pts_out[i][1] /= (2 * half + 2) * (half + 1) / 2;
    }
}

/**
 * \brief 判断左边线是否为直线
 *
 * \param start_y 起始行数
 * \param end_y 结束行数
 * \return 1:是直线；0:不是直线
 */
uint8 Img_Judge_StraightLine_Left(uint16 start_y, uint16 end_y)
{
    Img_BlurPoints(Points_L, Count_Left_Data, Points_L_B, 7);
    if (end_y < start_y)
    {
        uint8 tmp = end_y;
        end_y = start_y;
        start_y = tmp;
    }
    uint16 step = (end_y - start_y+1) / 5;
    float angles[5] = {0};
    uint8 count = 0;
    for (uint16 i = start_y; i + step <= end_y; i += step)
    {
        angles[count] = my_K_To_Angle(Points_L_B[i + step - 1][0], Points_L_B[i + step - 1][1],Points_L_B[i][0], Points_L_B[i][1]);
        count++;
        if (count > 4)
            break;
    }
    for (uint16 i = 1; i <= 4; i++)//外层循环是比较的轮数，数组内有5个数，那么就应该比较5-1=4轮
    {
        for (uint16 j = 0; j <= 4 - i; j++)//内层循环比较的是当前一轮的比较次数，例如：第一轮比较4-1=3次，第二轮比较4-2=2次
        {
            if (angles[j] > angles[j + 1])//相邻两个数如果逆序，则交换位置
            {
                int16 temp = 0;
                temp = angles[j];
                angles[j] = angles[j + 1];
                angles[j + 1] = temp;
            }
        }
    }
    if (my_abs((float)angles[4] - (float)angles[0]) > 15)
        return 0;
    return 1;
}

/**
 * \brief 判断右边是否为直线
 *
 * \param start_y 起始行数
 * \param end_y 结束行数
 * \return 1:是直线；0:不是直线
 */
uint8 Img_Judge_StraightLine_Right(uint16 start_y, uint16 end_y)
{
    Img_BlurPoints(Points_R, Count_Right_Data, Points_R_B, 7);
    if (end_y < start_y)
    {
        uint8 tmp = end_y;
        end_y = start_y;
        start_y = tmp;
    }
    uint16 step = (end_y - start_y + 1) / 5;
    float angles[5] = {0};
    uint8 count = 0;
    for (uint16 i = start_y; i + step <= end_y; i += step)
    {
        angles[count] = my_K_To_Angle(Points_R_B[i + step - 1][0], Points_R_B[i + step - 1][1],Points_R_B[i][0], Points_R_B[i][1]);
        if (count == 4)
            break;
        count++;
    }
    for (uint16 i = 1; i <= 4; i++)//外层循环是比较的轮数，数组内有5个数，那么就应该比较5-1=4轮
    {
        for (uint16 j = 0; j <= 4 - i; j++)//内层循环比较的是当前一轮的比较次数，例如：第一轮比较4-1=3次，第二轮比较4-2=2次
        {
            if (angles[j] > angles[j + 1])//相邻两个数如果逆序，则交换位置
            {
                int16 temp = 0;
                temp = angles[j];
                angles[j] = angles[j + 1];
                angles[j + 1] = temp;
            }
        }
    }
    if (my_abs((float)angles[4] - (float)angles[0]) > 15)
        return 0;
    return 1;
}

/**
 * \brief 判断任意一行是否丢线
 *
 * \param test_y 判断的行数
 * \return 返回：0：未丢线   1：左边丢线    2：右边丢线    3：左右均丢线
 */
uint8 Img_Judge_LostLine(uint16 test_y)
{
    if (Points_L[test_y][0] >= 4 && Points_R[test_y][0] <= 183)
        return 0;
    else if (Points_L[test_y][0] < 4 && Points_R[test_y][0] <= 183)
        return 1;
    else if (Points_L[test_y][0] >= 4 && Points_R[test_y][0] > 183)
        return 2;
    return 3;
}

/**
 * \brief 测试赛道宽度
 *
 */
uint8 Img_TestWidth(uint8 Find_y)
{
    uint8 i = 0, leftx=0, rightx=0;
    uint8 left_foundflag = 0, right_foundflag = 0;
    for (i = 94; i >= 7;i--)
    {
        if (Img_GetContrast(BMP[Find_y][i], BMP[Find_y][i - 4]) > 17)
        {
            for(uint8 j=i;j>=i-4;j--)
            {
                if ((Img_GetPixel(Find_y, j) == 0)||(j==i-4))
                {
                    leftx = j;
                    left_foundflag = 1;
                    break;
                }
            }
        }
        if (left_foundflag == 1)
            break;
    }
    for (i = 94; i <= MT9V03X_W-8; i++)
    {
        if (Img_GetContrast(BMP[Find_y][i], BMP[Find_y][i + 4]) > 17)
        {
            for (uint8 j = i; j <= i + 4; j++)
            {
                if ((Img_GetPixel(Find_y, j) == 0))
                {
                    rightx = j;
                    right_foundflag = 1;
                    break;
                }
            }
        }
        if (right_foundflag == 1)
            break;
    }
    return rightx-leftx;
}

/**
 * \brief 初始化图像处理
 *
 */
void Img_ProcessInit(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BMP;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)mt9v03x_image;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, DISABLE);
}

/**
 * \brief 获取误差
 *
 */
int16 Img_GetError(void)
{
    int16 sum=0;
    for(uint16 i = 30;i<60;i++)
    {
        sum+=94 - (int16)Midline_Points[i][0];
    }
    return sum/30;
}

//////////////////测试数据//////////////////
//uint8 left_straight=0,right_straight=0,lost_line=0;
//extern uint8 Huandao_L, Huandao_R, Cross, Follow_Mode;
//extern uint16 Width0;                                                     //0行赛道的宽度
//extern float Width_Step;                                                  //下一行宽-上一行宽
//////////////////测试数据//////////////////
extern uint8 Follow_Mode;
/**
 * \brief 显示图像
 *
 */
void Img_Display(void)
{
    ips200_show_gray_image(0, 0, (const uint8 *)BMP, 188, 80, 188, 80, 0);
    if(Follow_Mode==0)
    {
        for(int i = 0; i <= Count_Left_Data; i++)
        {
            ips200_draw_point(limit_a_b(Points_L[i][0]+2,1,186), limit_a_b(Points_L[i][1]+2,1,78), RGB565_BLUE);//显示起点
        }
        for(int i = 0; i <= Count_Right_Data; i++)
        {
            ips200_draw_point(limit_a_b(Points_R[i][0]-2,1,186), limit_a_b(Points_R[i][1]+2,1,78), RGB565_RED);//显示起点
        }
        for(int i = 0; i <= Midline_Points_Count; i++)
        {
            ips200_draw_point(limit_a_b(Midline_Points[i][0],1,186), limit_a_b(Midline_Points[i][1],1,78), RGB565_GREEN);//显示起点
        }
    }
}

/**
 * \brief 最终处理函数
 *
 */
void Img_FinalProcess(void)
{
    Img_CopyImage();                                          //复制图像
    Img_SearchLine();                                         //灰度扫线
    Img_ElementsProcess();                                    //元素处理
    mt9v03x_finish_flag = 0;
}
