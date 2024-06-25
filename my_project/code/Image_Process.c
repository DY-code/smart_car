#include "zf_common_headfile.h"
#include "math.h"
#include "Image_Process.h"
#include "Image_Elements.h"

// --------->x|W
//|*********
//|*********
//|*********   80
//v y|H     188

//�ڣ�0  �ף�255
uint8 BMP[MT9V03X_H][MT9V03X_W] = {0};                                      //����ͼ��

uint16 Points_L[MT9V03X_H][2] = { {0},{0} };                                //�����
uint16 Points_R[MT9V03X_H][2] = { {0},{0} };                                //�ұ���
uint16 Count_Left_Data = 79;                                                //���߿��õ������±�
uint16 Count_Right_Data = 79;                                               //���߿��õ������±�

uint16 Midline_Points[MT9V03X_H][2] = { {0},{0} };                          //��������
uint16 Midline_Points_Count = 79;                                           //���ߵ������±�

uint16 Points_L_B[MT9V03X_W * 2][2] = { {0},{0} };                          //�����˲��������� 0-x 1-y
uint16 Points_R_B[MT9V03X_W * 2][2] = { {0},{0} };                          //�����˲�����ұ��� 0-x 1-y

/**
 * \brief �����ֵ
 * 
 * \param value Ҫ���ֵ
 * \return ����ľ���ֵ
 */
float my_abs(float value)
{
    if (value >= 0)
        return value;
    else
        return -value;
}

/**
 * \brief �޷�
 * 
 * \param x ԭ����
 * \param a ��Сֵ
 * \param b ���ֵ
 * \return �޷����ֵ
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
 * \brief ��x,y�е���Сֵ
 * 
 * \param x
 * \param y
 * \return ��Сֵ
 */
int16 my_min(int16 x, int16 y)
{
    if (x > y)
        return y;
    else
        return x;
}

/**
 * \brief ��x,y�е����ֵ
 * 
 * \param x 
 * \param y
 * \return ���ֵ
 */
int16 my_max(int16 x, int16 y)
{
    if (x > y)
        return x;
    else
        return y;
}
/**
 * \brief ������б��(1ָ��2)
 * 
 * \param x1 ��һ���������
 * \param y1 ��һ���������
 * \param x2 �ڶ����������
 * \param y2 �ڶ����������
 * \return б��
 */
float my_k_twopoints(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
    if ((x2 - x1) == 0&&y1<y2)
        return 2000;//������ϵ��y��
    if ((x2 - x1) == 0 && y1 > y2)
        return -2000;//������ϵ��y��
    return (float)(y2 - y1) / (float)(x2 - x1);
}

/**
 * \brief ���Իع���б��
 *
 * \param begin ��ʼ�±�
 * \param end �����±�
 * \param border
 * \return б��
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
    if ((end - begin + 1) * x2sum - xsum * xsum) //�жϳ����Ƿ�Ϊ��
    {
        result = (float)((end - begin + 1) * xysum - xsum * ysum) / (float)((end - begin + 1) * x2sum - xsum * xsum);
    }
    else//����Ϊ����Ϊֱ��
    {
        result = 2000;
    }
    return result;
}

/**
 * \brief б��ת�Ƕȣ�������ͷ�ӽ�Ϊ������
 * 
 * \param x1 ��һ���������
 * \param y1 ��һ���������
 * \param x2 �ڶ����������
 * \param y2 �ڶ����������
 * \return
 */
int16 my_K_To_Angle(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
    float k = my_k_twopoints(x1, y1, x2, y2);
    if (k == 2000)//������ϵ��y����������ͷΪ�ӽ�����
        return 270;
    if (k == -2000)//������ϵ��y����������ͷΪ�ӽ�����
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
 * \brief ����н�
 *
 * \param ang1
 * \param ang2
 * \return �н�
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
 * \brief ����ͼ��
 * 
 */
void Img_CopyImage(void)
{
    DMA_Cmd(DMA1_Channel1, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel1, MT9V03X_H*MT9V03X_W);
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

/**
 * \brief ����ƽ����ֵ
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
 * \brief ��ȡһ�����صĶ�ֵ��ֵ
 *
 * \param y
 * \param x
 * \return ֵ
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
 * \brief ��Ⱥ���Աȶ�
 *
 * \param value �Ҷ�ֵ
 * \param next_value ��һ���Ҷ�ֵ
 * \return �Աȶ�ֵ
 */
int8 Img_GetContrast(int16 value, int16 next_value)
{
    return (value - next_value) * 100 / (value + next_value);
}

//                          ǰ     ��    ��     ��
int8 seeds[4][4][2] = { { {0,-1},{-1,0},{0,1},{1,0} },
                      { {-1,0},{0,1},{1,0},{0,-1} },
                      { {0,1},{1,0},{0,-1},{-1,0} },
                      { {1,0},{0,-1},{-1,0},{0,1} } };//��ǰ0����1���2����3

/**
 * \brief Ѱ���Թ���ɨ������ߵ���ʼ��
 *
 * \param find_x �Ӵ˵㿪ʼ��
 * \param find_y �Ӵ˵㿪ʼ��
 * \param dir ���� 0:���� 1: ����  2:����  3:����
 * \param start_x �ҵ������
 * \param start_y �ҵ������
 * \return 0��δ�ҵ� 1���ҵ�
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
 * \brief �Թ�������ɨ�������
 *
 * \param start_x ��ʼ�ĵ�
 * \param start_y ��ʼ�ĵ�
 * \param pts ����ߵ�����
 * \param num �±���ʽ�ĵ�ĸ���
 */
void Img_SearchLine_Lefthand(uint8 start_x, uint8 start_y, uint16 pts[][2], uint16 *num)//�����߽��ֹͣ
{
    int8 count = 0;
    //��߱���
    uint16 dir_l = 0;
    uint16 center_point_l[2] = { 0 };
    uint16 l_data_statics = 0;
    uint16 break_flag = 130;
    //���뿪ʼ�������
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
        //����ж�
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
 * \brief �Թ����������������
 *
 * \param start_x ��ʼ�ĵ�
 * \param start_y ��ʼ�ĵ�
 * \param pts ����ߵ�����
 * \param num �±���ʽ�ĵ�ĸ���
 */
void Img_SearchLine_Righthand(uint8 start_x, uint8 start_y, uint16 pts[][2], uint16 *num)//�����߽��ֹͣ
{
    int8 count = 0;
    //�ұ߱���
    uint16 dir_r = 0;
    uint16 center_point_r[2] = { 0 };
    uint16 r_data_statics = 0;
    uint16 break_flag = 130;
    //���뿪ʼ�������
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
        //�ұ��ж�
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
 * \brief ����������ߵľֲ�б�ʣ�������Ϊһ�飩
 *
 * \param pts_int ����ı���
 * \param num_in ��������±���ʽ�ĵ�ĸ���
 * \param pts_out �����Ĵ�Ƕȵ�����
 * \param num_out �±���ʽ�ĽǶȸ���
 */
void Img_CalulateAngle_Single(uint16 pts_in[][2], uint16 num_in, int16 pts_out[], uint8 *num_out)//����������ߵľֲ�б�ʣ�������Ϊһ�飩
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
 * \brief �Ҷ�ɨ��
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
 * \brief �ҵ���������
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
 * \brief �����˲�
 *
 * \param pts_in ����ı���
 * \param num ��ĸ������±���ʽ��
 * \param pts_in ����ı���
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
 * \brief �ж�������Ƿ�Ϊֱ��
 *
 * \param start_y ��ʼ����
 * \param end_y ��������
 * \return 1:��ֱ�ߣ�0:����ֱ��
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
    for (uint16 i = 1; i <= 4; i++)//���ѭ���ǱȽϵ���������������5��������ô��Ӧ�ñȽ�5-1=4��
    {
        for (uint16 j = 0; j <= 4 - i; j++)//�ڲ�ѭ���Ƚϵ��ǵ�ǰһ�ֵıȽϴ��������磺��һ�ֱȽ�4-1=3�Σ��ڶ��ֱȽ�4-2=2��
        {
            if (angles[j] > angles[j + 1])//������������������򽻻�λ��
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
 * \brief �ж��ұ��Ƿ�Ϊֱ��
 *
 * \param start_y ��ʼ����
 * \param end_y ��������
 * \return 1:��ֱ�ߣ�0:����ֱ��
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
    for (uint16 i = 1; i <= 4; i++)//���ѭ���ǱȽϵ���������������5��������ô��Ӧ�ñȽ�5-1=4��
    {
        for (uint16 j = 0; j <= 4 - i; j++)//�ڲ�ѭ���Ƚϵ��ǵ�ǰһ�ֵıȽϴ��������磺��һ�ֱȽ�4-1=3�Σ��ڶ��ֱȽ�4-2=2��
        {
            if (angles[j] > angles[j + 1])//������������������򽻻�λ��
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
 * \brief �ж�����һ���Ƿ���
 *
 * \param test_y �жϵ�����
 * \return ���أ�0��δ����   1����߶���    2���ұ߶���    3�����Ҿ�����
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
 * \brief �����������
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
 * \brief ��ʼ��ͼ����
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
 * \brief ��ȡ���
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

//////////////////��������//////////////////
//uint8 left_straight=0,right_straight=0,lost_line=0;
//extern uint8 Huandao_L, Huandao_R, Cross, Follow_Mode;
//extern uint16 Width0;                                                     //0�������Ŀ��
//extern float Width_Step;                                                  //��һ�п�-��һ�п�
//////////////////��������//////////////////
extern uint8 Follow_Mode;
/**
 * \brief ��ʾͼ��
 *
 */
void Img_Display(void)
{
    ips200_show_gray_image(0, 0, (const uint8 *)BMP, 188, 80, 188, 80, 0);
    if(Follow_Mode==0)
    {
        for(int i = 0; i <= Count_Left_Data; i++)
        {
            ips200_draw_point(limit_a_b(Points_L[i][0]+2,1,186), limit_a_b(Points_L[i][1]+2,1,78), RGB565_BLUE);//��ʾ���
        }
        for(int i = 0; i <= Count_Right_Data; i++)
        {
            ips200_draw_point(limit_a_b(Points_R[i][0]-2,1,186), limit_a_b(Points_R[i][1]+2,1,78), RGB565_RED);//��ʾ���
        }
        for(int i = 0; i <= Midline_Points_Count; i++)
        {
            ips200_draw_point(limit_a_b(Midline_Points[i][0],1,186), limit_a_b(Midline_Points[i][1],1,78), RGB565_GREEN);//��ʾ���
        }
    }
}

/**
 * \brief ���մ�����
 *
 */
void Img_FinalProcess(void)
{
    Img_CopyImage();                                          //����ͼ��
    Img_SearchLine();                                         //�Ҷ�ɨ��
    Img_ElementsProcess();                                    //Ԫ�ش���
    mt9v03x_finish_flag = 0;
}
