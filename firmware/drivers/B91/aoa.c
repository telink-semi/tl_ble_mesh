/********************************************************************************************************
 * @file     aoa.c
 *
 * @brief    This is the source file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         11,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *******************************************************************************************************/

#include "aoa.h"
#include "string.h"
#include "malloc.h"
#include "math.h"
//int phase_diff[4]={0, 918, 462, 207}; 		//1024times
extern int phase_diff[4];//={0, 918, 462, 207}; 		//1024times
extern int phase_rx_angle_cali_angle[8];
//int phase_diff[4]={0, 865, -999, 942}; 		//for xunxi
//int phase_diff[4]={0, 3215, 3010, 5765};		//for rectangle
int temp_theta;

unsigned int theta_range[] =
{
        90,88,86,85,83,81,79,77,76,74,72,70,
        68,66,64,62,60,58,56,54,51,49,47,44,
        41,39,36,32,29,25,20,14,0
};

unsigned int tb_tan[90] =
{
		0   ,4 	  ,9   ,13 	,18  ,22  ,27  ,31 	,
		36 	,41   ,45  ,50 	,54  ,59  ,64  ,69 	,
		73 	,78   ,83  ,88 	,93  ,98  ,103 ,109 ,
		114 ,119  ,125 ,130 ,136 ,142 ,148 ,154 ,
		160 ,166  ,173 ,179 ,186 ,193 ,200 ,207 ,
		215 ,223  ,231 ,239 ,247 ,256 ,265 ,275 ,
		284 ,294  ,305 ,316 ,328 ,340 ,352 ,366 ,
		380 ,394  ,410 ,426 ,443 ,462 ,481 ,502 ,
		525 ,549  ,575 ,603 ,634 ,667 ,703 ,743 ,
		788 ,837  ,893 ,955 ,1027,1109,1204,1317,
		1452,1616 ,1822,2085,2436,2926,3661,4885,
		7331,14666
};

unsigned int tb_sin[90]=
{
		0   ,18  ,36  ,54  ,71  ,89  ,107 ,125 ,
		142 ,160 ,178 ,195 ,213 ,230 ,248 ,265 ,
		282 ,299 ,316 ,333 ,350 ,367 ,383 ,400 ,
		416 ,433 ,449 ,465 ,481 ,496 ,512 ,527 ,
		542 ,557 ,572 ,587 ,602 ,616 ,630 ,644 ,
		658 ,672 ,685 ,698 ,711 ,724 ,736 ,749 ,
		761 ,773 ,784 ,796 ,807 ,818 ,828 ,839 ,
		849 ,859 ,868 ,877 ,887 ,895 ,904 ,912 ,
		920 ,928 ,935 ,942 ,949 ,956 ,962 ,968 ,
		974 ,979 ,984 ,989 ,993 ,998 ,1001,1005,
		1008,1011,1014,1016,1018,1020,1021,1022,
		1023,1024
};

int cos_radian[90] =
{
		1024,1024,1023,1023,1022,1020,1018,1016,
		1014,1011,1008,1005,1002,998 ,994 ,989 ,
		984 ,979 ,974 ,968 ,962 ,956 ,949 ,943 ,
		935 ,928 ,920 ,912 ,904 ,896 ,887 ,878 ,
		868 ,859 ,849 ,839 ,828 ,818 ,807 ,796 ,
		784 ,773 ,761 ,749 ,737 ,724 ,711 ,698 ,
		685 ,672 ,658 ,644 ,630 ,616 ,602 ,587 ,
		573 ,558 ,543 ,527 ,512 ,496 ,481 ,465 ,
		449 ,433 ,416 ,400 ,384 ,367 ,350 ,333 ,
		316 ,299 ,282 ,265 ,248 ,230 ,213 ,195 ,
		178 ,160 ,143 ,125 ,107 ,89  ,71  ,54  ,
		36  ,18
};

int acos_angle[31] =
{
		90,88,86,84,82,80,78,77,
		75,73,71,68,66,64,62,60,
		58,55,53,51,48,46,43,40,
		37,34,30,26,21,15,0
};

unsigned int alpha_range[] =
{
		0  ,1  ,2  ,3  ,4  ,5  ,6  ,7  ,8  ,9  ,10 ,11 ,12 ,13 ,14 ,15 ,
		16 ,17 ,18 ,19 ,20 ,21 ,22 ,23 ,24 ,25 ,26 ,27 ,28 ,29 ,30 ,31 ,
		32 ,33 ,34 ,35 ,36 ,37 ,38 ,39 ,40 ,41 ,42 ,43 ,44 ,45 ,46 ,47 ,
		48 ,49 ,50 ,51 ,52 ,53 ,54 ,55 ,56 ,57 ,58 ,59 ,60 ,61 ,62 ,63 ,
		64 ,65 ,66 ,67 ,68 ,69 ,70 ,71 ,72 ,73 ,74 ,75 ,76 ,77 ,78 ,79 ,
		80 ,81 ,82 ,83 ,84 ,85 ,86 ,87 ,88 ,89 ,90 ,91 ,92 ,93 ,94 ,95 ,
		96 ,97 ,98 ,99 ,100,101,102,103,104,105,106,107,108,109,110,111,
		112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
		128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,
		144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
		160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,
		176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
		192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,
		208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
		224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,
		240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,
		256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,
		272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,
		288,289,290,291,292,293,294,295,296,297,298,299,300,301,302,303,
		304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,
		320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,
		336,337,338,339,340,341,342,343,344,345,346,347,348,349,350,351,
		352,353,354,355,356,357,358,359
};

IQ_Value cal_result;
Func_CB  function;
unsigned int rsquare;
int temp_theta;
int angle_lut_simple[8][XY_TABLE_NUMBER][1];
int mean_val[34] = {0};

/**
 * @brief      This function servers to store data to a specific place .
 * @param[in]  data_table  -  Pointer variable to store data.
 * @param[in]  ptr_packet  -  Pointer variable to support data.
 * @param[in]  number      -  The data numbers.
 * @return     none.
 */
void get_raw_data(unsigned char *data_table,unsigned char *ptr_packet,unsigned int number)
{
	memcpy(data_table,ptr_packet+28,number);
}


/**
 * @brief		This function serve to find the minimum value of a set of values and return its index value.
 * @param[in]	*data - Address requiring minimum number.
 * @param[in]	len - Number of digits.
 * @return		Minimum.
 */
unsigned int min_array(int *data, unsigned char len, unsigned char* index)
{
	int min_value = data[0];
	for(unsigned char i = 1; i < len; ++i)
	{
		if(data[i] < min_value){
			min_value = data[i];
			*index = i;
		}
	}
	return min_value;
}

/**
 * @brief	In order to prevent the data from exceeding the predetermined range, the data beyond the range is rotated.
 * @param[in]	x - the data need to rotated.
 * @param[in]	wrap_range - Pre-set range value.
 */
int angle_prime(int x, int wrap_range)
{
    int y = (x + wrap_range) % wrap_range;
    if(y > (wrap_range/2))
        y -= wrap_range;

    return y;
}


/**
 * @brief      This function servers to get a angle for Rx mode.
 * @param[in]  ptr_packet  -  Pointer variable of the Rx packages.
 * @return     The angle.
 */
//unsigned int raw_data_to_angle_with_z(unsigned char *ptr_packet)
//{
//	unsigned char i,j;
//	IQ_cb src_IQ;			//sourse raw data
//	IQ_cb cal_IQ;			//IQ value that used to calculate
//	IQ_cb conj_IQ;			//conj(IQ)
//	IQ_Value LR_diff_out[5]={{0,0},{0,0},{0,0},{0,0},{0,0}};
//	int angle_lut_temp[8][XY_TABLE_NUMBER];		//1024 times
//	//int diff_angle_antenna[8][4];
//	int diff_angle_antenna[8];  //todo
//	int phase_ref=0;
//	int freq_offset_est=0;
//	int freq_offset_est_step=0;
//	int temp_real=0,temp_imag=0;
//	int phase_rf=0;
//	unsigned char param;
//	int phase_IQ[IQ_NUM+1]={0};
//	int angle_IQ[IQ_NUM+1]={0};
//
//	cal_result.real_part = 0;
//	cal_result.imag_part = 0;
//
//	//1.init IQ value
//	trans_uchar_to_int((ptr_packet+28),(int *)&src_IQ.IQ[1],IQ_NUM*2);
//
//	for(i=1;i<IQ_NUM+1;i++)
//	{
//		cal_IQ.IQ[i].real_part = src_IQ.IQ[i].real_part;
//		cal_IQ.IQ[i].imag_part = src_IQ.IQ[i].imag_part;
//	}
//
//	demodulation(&cal_IQ.IQ[1].real_part,&cal_IQ.IQ[1].imag_part,8);	//demodulation
//
//	for(i=1;i<8+1;i++)
//	{
//		conj_IQ.IQ[i].real_part =  cal_IQ.IQ[i].real_part;
//		conj_IQ.IQ[i].imag_part = -cal_IQ.IQ[i].imag_part;
//	}
//
//
//	for(i=1;i<5;i++)
//	{
//		for(j=i+1;j<9;j++)
//		{
//			param = j-i;
//			dot_product(cal_IQ,conj_IQ,j,param);
//			LR_diff_out[i].real_part += cal_result.real_part;
//			LR_diff_out[i].imag_part += cal_result.imag_part;
//		}
//		LR_diff_out[i].real_part /=  (8-i);
//		LR_diff_out[i].imag_part /=  (8-i);
//	}
//
//
//
//	cal_result.real_part = 0;
//	cal_result.imag_part = 0;
//
//	for(i=1;i<5;i++)
//	{
//		cal_result.real_part += LR_diff_out[i].real_part;
//		cal_result.imag_part += LR_diff_out[i].imag_part;
//	}
//	phase_ref = calc_phase(cal_result.real_part,cal_result.imag_part);
//	freq_offset_est = phase_ref*10240/180/5;		//10240 times, real value
//
//	// init IQ buffer
//	for(i=1;i<IQ_NUM+1;i++)
//	{
//		cal_IQ.IQ[i].real_part = src_IQ.IQ[i].real_part;
//		cal_IQ.IQ[i].imag_part = src_IQ.IQ[i].imag_part;
//		conj_IQ.IQ[i].real_part =  cal_IQ.IQ[i].real_part;
//		conj_IQ.IQ[i].imag_part = -cal_IQ.IQ[i].imag_part;
//	}
//
//	cal_result.real_part = 0;
//	cal_result.imag_part = 0;
//
//	for(i=9;i<IQ_NUM-7;i++)
//	{
//		dot_product(cal_IQ,conj_IQ,i+8,i);
////		dot_product(i+8,i);
//		temp_real += cal_result.real_part;
//		temp_imag += cal_result.imag_part;
//	}
//
//	int temp12 =  angle_prime(freq_offset_est*2*16*SLOT_TIME, 2*10240);
//    phase_rf = calc_phase(temp_real,temp_imag) * 10240 * 314 / 18000;
//
//    temp12 = angle_prime(phase_rf*1000/3141 - temp12, 2*10240);
//    freq_offset_est += (temp12/(2*16*SLOT_TIME));
//
//
//	for(i=1;i<IQ_NUM+1;i++)
//	{
//		phase_IQ[i] = calc_phase(cal_IQ.IQ[i].real_part , cal_IQ.IQ[i].imag_part);
//
//
//		angle_IQ[i] = phase_IQ[i]  *1024 / 180;
//
//	}
//
//    freq_offset_est_step = freq_offset_est * 8 * (-1) / 10;
//
//    int freq_comp = 0;
//    for(i = 10; i < IQ_NUM+1; i++)
//    {
//        freq_comp = freq_comp + freq_offset_est_step;
//        freq_comp = angle_prime(freq_comp, 2048);
//        angle_IQ[i] += freq_comp;
//
//    }
//
//	//7.
//	int antenna_musk[] = {1,2,8,3,7,4,6,5};
//	int angle_antenna[8][4];			//1024 times
//	for(i=0;i<4;i++)
//	{
//		angle_antenna[antenna_musk[0]-1][i] = angle_IQ[16+8*i];
//
//		angle_antenna[antenna_musk[1]-1][i] = angle_IQ[9+8*i];
//
//		angle_antenna[antenna_musk[2]-1][i] = angle_IQ[10+8*i];
//
//		angle_antenna[antenna_musk[3]-1][i] = angle_IQ[11+8*i];
//
//		angle_antenna[antenna_musk[4]-1][i] = angle_IQ[12+8*i];
//
//		angle_antenna[antenna_musk[5]-1][i] = angle_IQ[13+8*i];
//
//		angle_antenna[antenna_musk[6]-1][i] = angle_IQ[14+8*i];
//
//		angle_antenna[antenna_musk[7]-1][i] = angle_IQ[15+8*i];
//	}
//
//	//8.
//	int temp_diff[4] = {0};
//	int temp[9] = {0};
////	int phase_rx_angle_cali_angle[] = {0, -40, 81, 162, 40, -122, 122, 122};//{0,-41,82,163,41,-122,122,122};
//    for(i=0;i<8;i++)
//	{
//		for(j=0;j<4;j++)
//		{
//			angle_antenna[i][j]	-=  phase_rx_angle_cali_angle[i];		//1024 times
//
//		}
//
//
//		for(j=1;j<4;j++)
//		{
//			temp_diff[j] = (angle_antenna[i][j] - angle_antenna[i][0]);
//			temp_diff[j] = angle_prime(temp_diff[j], 2048);
//		}
//
//		for(j=1;j<4;j++)
//		{
//			temp_diff[j] = temp_diff[j] >> 2;
//			temp[i] += temp_diff[j];
//		}
//		temp[i] += angle_antenna[i][0];
//	}
//	temp[8] = temp[0];
//
//	for(i=0;i<8;i++)
//	{
//		diff_angle_antenna[i] = temp[i+1] - temp[i];
//		//printf("temp[%d] = %d, temp[%d] = %d\n", i+1, temp[i+1], i, temp[i]);
//		diff_angle_antenna[i] = angle_prime(diff_angle_antenna[i], 2048);
//	}
//
//	//9.
//
//	unsigned int ii_1,ii;
//	int jj;
//	int temp2[8];
//	int temp6[34];
//	int temp4;
//	int temp_theta_range[2];
////	int minval = 1000*1024;
//    int minval = 10000000;
//	int temp_alpha = 0;
////	int temp_theta;
//
//	for(ii_1=0;ii_1<8;ii_1++)
//	{
//		temp2[ii_1] = abs_value(diff_angle_antenna[ii_1], 1);
//	}
//	temp4 = mean(&temp2[0], 8);
//
//	for(ii_1 = 0;ii_1 < 33; ++ii_1)
//	{
//		temp6[ii_1] = abs_value((temp4 - mean_val[ii_1]), 1);
//		//printf("temp6[%d] = %d\n", ii_1, temp6[ii_1]);
//	}
//	unsigned char temp3 = 0;
//	temp4 = min_array(&temp6[0], 34, &temp3);
//
//	if(temp3 == 1){
//		temp_theta_range[0] = 1;
//		temp_theta_range[1] = 2;
//	}else if(temp3 == (AMP_SIZE+1)){
//		temp_theta_range[0] = AMP_SIZE;
//		temp_theta_range[1] = AMP_SIZE+1;
//	}else{
//		temp_theta_range[0] = temp3 - 1;
//		temp_theta_range[1] = temp3 + 1;
//	}
//	for(ii_1=0;ii_1<8;ii_1++)
//	{
//		function.int_ptr1 = &angle_lut_simple[0][0][0];
//		function.int_ptr2 = &angle_lut_temp[0][0];
//		function.int1     = ii_1;
//		function.char1	  = 0;
//		function.char2	  = 8;
//		function.char3	  = XY_TABLE_NUMBER;
//
//		while (circshift(function) != 0);
//
//		for(ii=0;ii<45;ii++)
//		{
//			for(jj=temp_theta_range[0];jj<temp_theta_range[1] + 1;jj++)
//			{
//				for(i=0;i<8;i++)
//				{
//					temp2[i] = diff_angle_antenna[i] + angle_lut_temp[i][ii]*(jj);
//
//					temp2[i] = angle_prime(temp2[i], 2048);
//                    //printf("temp2[%d] = %d\n", i, temp2[i]);
//					temp2[i] = abs_value((temp2[i] >> 3), 1);//todo
//				}
//				temp4 = sum(&temp2[0], 8);
//
//
//                if(temp4<minval)
//                {
//                	minval = temp4;
//                    temp_alpha = ii+1 + (ii_1) * XY_TABLE_NUMBER;
//
//                    temp_theta = jj+1;
//
//                }
//			}
//		}
//	}
//
//	temp_theta = theta_range[temp_theta-1];
//	return alpha_range[temp_alpha-1];
//}

volatile unsigned char debug_function;
volatile int frq_num = 0;
unsigned int raw_data_to_angle_with_z(unsigned char *ptr_packet)
{
	unsigned char i,j;
	IQ_cb src_IQ;			//sourse raw data
	IQ_cb cal_IQ;			//IQ value that used to calculate
	IQ_cb conj_IQ;			//conj(IQ)
	IQ_Value LR_diff_out[5]={{0,0},{0,0},{0,0},{0,0},{0,0}};
	int angle_lut_temp[8][XY_TABLE_NUMBER];		//1024 times
	//int diff_angle_antenna[8][4];
	int diff_angle_antenna[8];  //todo
	int phase_ref=0;
	int freq_offset_est=0;
	int freq_offset_est_step=0;
	int temp_real=0,temp_imag=0;
	int phase_rf=0;
	unsigned char param;
	int phase_IQ[IQ_NUM+1]={0};
	int angle_IQ[IQ_NUM+1]={0};

	cal_result.real_part = 0;
	cal_result.imag_part = 0;

	debug_function = 1;

	//1.init IQ value
	trans_uchar_to_int((ptr_packet+28),(int *)&src_IQ.IQ[1],IQ_NUM*2);

	debug_function = 2;

	for(i=1;i<IQ_NUM+1;i++)
	{
		cal_IQ.IQ[i].real_part = src_IQ.IQ[i].real_part;
		cal_IQ.IQ[i].imag_part = src_IQ.IQ[i].imag_part;
	}


	debug_function = 3;

	demodulation(&cal_IQ.IQ[1].real_part,&cal_IQ.IQ[1].imag_part,8);	//demodulation

	debug_function = 4;

	for(i=1;i<8+1;i++)
	{
		conj_IQ.IQ[i].real_part =  cal_IQ.IQ[i].real_part;
		conj_IQ.IQ[i].imag_part = -cal_IQ.IQ[i].imag_part;
	}


	debug_function = 5;

	for(i=1;i<5;i++)
	{
		for(j=i+1;j<9;j++)
		{
			param = j-i;
			dot_product(cal_IQ,conj_IQ,j,param);
			LR_diff_out[i].real_part += cal_result.real_part;
			LR_diff_out[i].imag_part += cal_result.imag_part;
		}
		LR_diff_out[i].real_part /=  (8-i);
		LR_diff_out[i].imag_part /=  (8-i);
	}

	debug_function = 6;

	cal_result.real_part = 0;
	cal_result.imag_part = 0;

	for(i=1;i<5;i++)
	{
		cal_result.real_part += LR_diff_out[i].real_part;
		cal_result.imag_part += LR_diff_out[i].imag_part;
	}

	debug_function = 7;

	phase_ref = calc_phase(cal_result.real_part,cal_result.imag_part);
	freq_offset_est = phase_ref*10240/180/5;		//10240 times, real value

	// init IQ buffer
	for(i=1;i<IQ_NUM+1;i++)
	{
		cal_IQ.IQ[i].real_part = src_IQ.IQ[i].real_part;
		cal_IQ.IQ[i].imag_part = src_IQ.IQ[i].imag_part;
		conj_IQ.IQ[i].real_part =  cal_IQ.IQ[i].real_part;
		conj_IQ.IQ[i].imag_part = -cal_IQ.IQ[i].imag_part;
	}


	debug_function = 8;

	cal_result.real_part = 0;
	cal_result.imag_part = 0;

	for(i=9;i<IQ_NUM-7;i++)
	{
		dot_product(cal_IQ,conj_IQ,i+8,i);
//		dot_product(i+8,i);
		temp_real += cal_result.real_part;
		temp_imag += cal_result.imag_part;
	}
	debug_function = 9;

	int temp12 =  angle_prime(freq_offset_est*2*16*SLOT_TIME, 2*10240);
    phase_rf = calc_phase(temp_real,temp_imag) * 10240 * 314 / 18000;

    temp12 = angle_prime(phase_rf*1000/3141 - temp12, 2*10240);
    freq_offset_est += (temp12/(2*16*SLOT_TIME));


	for(i=1;i<IQ_NUM+1;i++)
	{
		phase_IQ[i] = calc_phase(cal_IQ.IQ[i].real_part , cal_IQ.IQ[i].imag_part);


		angle_IQ[i] = phase_IQ[i]  *1024 / 180;

	}


	debug_function = 10;

    freq_offset_est_step = freq_offset_est * 8 * (-1) / 10;

    debug_function = 11;

    int freq_comp = 0;

    for(i = 10; i < IQ_NUM+1; i++)
    {
    	frq_num++;
        freq_comp = freq_comp + freq_offset_est_step;
        freq_comp = angle_prime(freq_comp, 2048);
        angle_IQ[i] += freq_comp;
    }

    debug_function = 0x22;

	//7.
	int antenna_musk[] = {1,2,8,3,7,4,6,5};
	int angle_antenna[8][4];			//1024 times
	for(i=0;i<4;i++)
	{
		angle_antenna[antenna_musk[0]-1][i] = angle_IQ[16+8*i];

		angle_antenna[antenna_musk[1]-1][i] = angle_IQ[9+8*i];

		angle_antenna[antenna_musk[2]-1][i] = angle_IQ[10+8*i];

		angle_antenna[antenna_musk[3]-1][i] = angle_IQ[11+8*i];

		angle_antenna[antenna_musk[4]-1][i] = angle_IQ[12+8*i];

		angle_antenna[antenna_musk[5]-1][i] = angle_IQ[13+8*i];

		angle_antenna[antenna_musk[6]-1][i] = angle_IQ[14+8*i];

		angle_antenna[antenna_musk[7]-1][i] = angle_IQ[15+8*i];
	}

	debug_function = 12;

	//8.
	int temp_diff[4] = {0};
	int temp[9] = {0};
	int phase_rx_angle_cali_angle_z[] = {0, -40, 81, 162, 40, -122, 122, 122};//{0,-41,82,163,41,-122,122,122};
    for(i=0;i<8;i++)
	{
		for(j=0;j<4;j++)
		{
			angle_antenna[i][j]	-=  phase_rx_angle_cali_angle_z[i];		//1024 times

		}
		debug_function = 13;

		for(j=1;j<4;j++)
		{
			temp_diff[j] = (angle_antenna[i][j] - angle_antenna[i][0]);
			temp_diff[j] = angle_prime(temp_diff[j], 2048);
		}
		debug_function = 14;

		for(j=1;j<4;j++)
		{
			temp_diff[j] = temp_diff[j] >> 2;
			temp[i] += temp_diff[j];
		}
		debug_function = 15;
		temp[i] += angle_antenna[i][0];
	}
	temp[8] = temp[0];
	debug_function = 16;
	for(i=0;i<8;i++)
	{
		diff_angle_antenna[i] = temp[i+1] - temp[i];
		//printf("temp[%d] = %d, temp[%d] = %d\n", i+1, temp[i+1], i, temp[i]);
		diff_angle_antenna[i] = angle_prime(diff_angle_antenna[i], 2048);
	}
	debug_function = 17;
	//9.

	unsigned int ii_1,ii;
	int jj;
	int temp2[8];
	int temp6[34];
	int temp4;
	int temp_theta_range[2];
//	int minval = 1000*1024;
    int minval = 10000000;
	int temp_alpha = 0;
//	int temp_theta;

	for(ii_1=0;ii_1<8;ii_1++)
	{
		temp2[ii_1] = abs_value(diff_angle_antenna[ii_1], 1);
	}
	debug_function = 18;
	temp4 = mean(&temp2[0], 8);

	for(ii_1 = 0;ii_1 < 33; ++ii_1)
	{
		temp6[ii_1] = abs_value((temp4 - mean_val[ii_1]), 1);
		//printf("temp6[%d] = %d\n", ii_1, temp6[ii_1]);
	}
	debug_function = 19;
	unsigned char temp3 = 0;
	temp4 = min_array(&temp6[0], 34, &temp3);

	if(temp3 == 1){
		temp_theta_range[0] = 1;
		temp_theta_range[1] = 2;
	}else if(temp3 == (AMP_SIZE+1)){
		temp_theta_range[0] = AMP_SIZE;
		temp_theta_range[1] = AMP_SIZE+1;
	}else{
		temp_theta_range[0] = temp3 - 1;
		temp_theta_range[1] = temp3 + 1;
	}
	debug_function = 20;
	for(ii_1=0;ii_1<8;ii_1++)
	{
		function.int_ptr1 = &angle_lut_simple[0][0][0];
		function.int_ptr2 = &angle_lut_temp[0][0];
		function.int1     = ii_1;
		function.char1	  = 0;
		function.char2	  = 8;
		function.char3	  = XY_TABLE_NUMBER;

		debug_function = 21;


		while (circshift(function) != 0);

		debug_function = 0xff;

		for(ii=0;ii<45;ii++)
		{
			for(jj=temp_theta_range[0];jj<temp_theta_range[1] + 1;jj++)
			{
				for(i=0;i<8;i++)
				{
					temp2[i] = diff_angle_antenna[i] + angle_lut_temp[i][ii]*(jj);

					temp2[i] = angle_prime(temp2[i], 2048);
                    //printf("temp2[%d] = %d\n", i, temp2[i]);
					temp2[i] = abs_value((temp2[i] >> 3), 1);//todo
				}
				temp4 = sum(&temp2[0], 8);
				debug_function = 22;

                if(temp4<minval)
                {
                	minval = temp4;
                    temp_alpha = ii+1 + (ii_1) * XY_TABLE_NUMBER;

                    temp_theta = jj+1;

                }
                debug_function = 23;
			}
		}
	}

	temp_theta = theta_range[temp_theta-1];
	return alpha_range[temp_alpha-1];
}

/**
 * @brief      This function servers to get a angle for Rx mode.
 * @param[in]  ptr_packet  -  Pointer variable of the Rx packages.
 * @return     The angle.
 */
unsigned int raw_data_to_angle_no_z(unsigned char *ptr_packet)
{
	unsigned char i,j;
	IQ_cb src_IQ;			//sourse raw data
	IQ_cb cal_IQ;			//IQ value that used to calculate
	IQ_cb conj_IQ;			//conj(IQ)
	IQ_Value LR_diff_out[5]={{0,0},{0,0},{0,0},{0,0},{0,0}};
//	Func_CB  function;
	int angle_lut_temp[8][XY_TABLE_NUMBER] ;		//1024 times
	int temp_val;
	int diff_angle_antenna[8][4];
	int vecC_angle[IQ_NUM+1]={0};
	int phase_ref=0;
	int freq_offset_est=0;
	int temp_real=0,temp_imag=0;
	int phase_rf=0;
	int angle_rf=0;
	unsigned char param;
	int phase_table[2]={0};
	int phase_IQ[IQ_NUM+1]={0};
	int angle_IQ[IQ_NUM+1]={0};

	cal_result.real_part = 0;
	cal_result.imag_part = 0;

	//1.init IQ value
	trans_uchar_to_int((ptr_packet+28),(int *)&src_IQ.IQ[1],IQ_NUM*2);

	for(i=1;i<IQ_NUM+1;i++)
	{
		cal_IQ.IQ[i].real_part = src_IQ.IQ[i].real_part;
		cal_IQ.IQ[i].imag_part = src_IQ.IQ[i].imag_part;
	}

	//2. first 8 IQ demodulation
	demodulation(&cal_IQ.IQ[1].real_part,&cal_IQ.IQ[1].imag_part,8);	//demodulation

	//3.calculate reference frequency off
	for(i=1;i<8+1;i++)
	{
		conj_IQ.IQ[i].real_part =  cal_IQ.IQ[i].real_part;
		conj_IQ.IQ[i].imag_part = -cal_IQ.IQ[i].imag_part;
	}

	for(i=1;i<5;i++)
	{
		for(j=i+1;j<9;j++)
		{
			param = j-i;
			dot_product(cal_IQ,conj_IQ,j,param);
			//dot_product(j,param);
			LR_diff_out[i].real_part += cal_result.real_part;
			LR_diff_out[i].imag_part += cal_result.imag_part;
		}
		LR_diff_out[i].real_part /=  (8-i);
		LR_diff_out[i].imag_part /=  (8-i);
	}

	cal_result.real_part = 0;
	cal_result.imag_part = 0;

	for(i=1;i<5;i++)
	{
		cal_result.real_part += LR_diff_out[i].real_part;
		cal_result.imag_part += LR_diff_out[i].imag_part;
	}

	//4.calculate other rf frequency off
	phase_ref = calc_phase(cal_result.real_part,cal_result.imag_part);
	freq_offset_est = phase_ref*1024/180/5;		//1024 times, real value

	// init IQ buffer
	for(i=1;i<IQ_NUM+1;i++)
	{
		cal_IQ.IQ[i].real_part = src_IQ.IQ[i].real_part;
		cal_IQ.IQ[i].imag_part = src_IQ.IQ[i].imag_part;
		conj_IQ.IQ[i].real_part =  cal_IQ.IQ[i].real_part;
		conj_IQ.IQ[i].imag_part = -cal_IQ.IQ[i].imag_part;
	}

	cal_result.real_part = 0;
	cal_result.imag_part = 0;

	for(i=9;i<IQ_NUM-7;i++)
	{
		dot_product(cal_IQ,conj_IQ,i+8,i);
//		dot_product(i+8,i);
		temp_real += cal_result.real_part;
		temp_imag += cal_result.imag_part;
	}

	phase_rf = calc_phase(temp_real,temp_imag);
	angle_rf = phase_rf * 1024 * 314 / 18000;   //1024 times

	//5.remove frequency off
	phase_table[0] = (freq_offset_est*2*314/100);
	phase_table[1] = angle_rf;

	freq_offset_est = phase_combine(&phase_table[0],16*SLOT_TIME) *100/314/2;	//1024 times

	for(i=9;i<IQ_NUM+1;i++)
	{
		phase_IQ[i] = calc_phase(cal_IQ.IQ[i].real_part , cal_IQ.IQ[i].imag_part);
		angle_IQ[i] = phase_IQ[i] * 1024 * 314 / 18000 ;		//1024 times
	}

	//6.count vecC_angle
	for(i=0;i<IQ_NUM+1;i++)
	{
		vecC_angle[i] = 0;
	}

	for(i=9;i<IQ_NUM+1;i++)
	{
		vecC_angle[i] = (angle_IQ[i] - 2*314*(i-9)*freq_offset_est*4/100);	//1024 times
	}

	//7.
	int antenna_musk[] = {1,2,8,3,7,4,6,5};
	int angle_antenna[8][4];			//1024 times
	for(i=0;i<4;i++)
	{
		angle_antenna[antenna_musk[0]-1][i] = vecC_angle[16+8*i]*100/314;
		angle_antenna[antenna_musk[1]-1][i] = vecC_angle[9+8*i] *100/314;
		angle_antenna[antenna_musk[2]-1][i] = vecC_angle[10+8*i]*100/314;
		angle_antenna[antenna_musk[3]-1][i] = vecC_angle[11+8*i]*100/314;
		angle_antenna[antenna_musk[4]-1][i] = vecC_angle[12+8*i]*100/314;
		angle_antenna[antenna_musk[5]-1][i] = vecC_angle[13+8*i]*100/314;
		angle_antenna[antenna_musk[6]-1][i] = vecC_angle[14+8*i]*100/314;
		angle_antenna[antenna_musk[7]-1][i] = vecC_angle[15+8*i]*100/314;
	}

	//8.
//	int phase_rx_angle_cali_angle[] = {0,-41,82,163,41,-122,122,122};//{0,-597,-710,-602,-582,-614,-412,-941};//
	for(i=0;i<8;i++)
	{
		for(j=0;j<4;j++)
		{
			angle_antenna[i][j]	-=  phase_rx_angle_cali_angle[i];		//1024 times
		}

	}

	//9.
	int temp[9];
	for(i=0;i<4;i++)
	{
		for(j=0;j<8;j++)
		{
			temp[j] = angle_antenna[j][i];
		}
		temp[8] = angle_antenna[0][i];
		for(j=0;j<8;j++)
		{
			diff_angle_antenna[j][i] = temp[j+1] - temp[j];
		}
	}



	//9.

	unsigned int ii_1,ii,jj,kk;
	int temp2[8];
	int temp4 = 0;
	int minval = 1000*1024;
	int temp_alpha = 0;
//	int temp_theta;

	for(ii_1=0;ii_1<8;ii_1++)
	{
		function.int_ptr1 = &angle_lut_simple[0][0][0];
		function.int_ptr2 = &angle_lut_temp[0][0];
		function.int1     = ii_1;
		function.char1	  = 0;
		function.char2	  = 8;
		function.char3	  = XY_TABLE_NUMBER;

		while (circshift(function) != 0);

		for(ii=0;ii<45;ii++)
		{
			for(jj=0;jj<1;jj++)
			{
				temp_val = 0;

				for(kk=0;kk<4;kk++)
				{
					temp4 = 0;
					for(i=0;i<8;i++)
					{
						temp2[i] = diff_angle_antenna[i][kk] + angle_lut_temp[i][ii];

						function.int_ptr1 = &temp2[i];
						function.int1 = 2048;
						wrap(function);
					}

					for(i=0;i<8;i++)
					{
						if(temp2[i] > 0)
						{
							temp4 += temp2[i];					//1024 times
						}
						else
						{
							temp4 -= temp2[i];
						}
					}
					temp_val += temp4;

					if(temp_val > minval)
					{
						break;
					}
				}

                if(temp_val<minval)
                {
                	minval = temp_val;
                    temp_alpha = ii+1 + (ii_1)*XY_TABLE_NUMBER;
                    temp_theta = jj+1;
                }
			}
		}
	}
	return alpha_range[temp_alpha];
}


/**
 * @brief	 	Demodulate based on real and imaginary parts.
 * @param[in]	real_ptr - Real address.
 * @param[in]	imag_ptr - imag address.
 * @param[in]	num - the number of data.
 * @return		none.
 */
void demodulation(int* real_ptr,int* imag_ptr,unsigned char num)
{
	unsigned char i;
	int temp_real,temp_imag;
	for(i=0;i<num;i++)
	{
		if(i%4 == 0)				//not change
		{
			*real_ptr = *real_ptr;
			*imag_ptr = *imag_ptr;
		}
		else if(i%4 == 1)
		{
			temp_real = *real_ptr;
			temp_imag = *imag_ptr;
			*real_ptr =  temp_imag;	// * -i
			*imag_ptr = -temp_real;
		}
		else if(i%4 == 2)
		{
			*real_ptr = -(*real_ptr);// * -1
			*imag_ptr = -(*imag_ptr);
		}
		else if(i%4 == 3)
		{
			temp_real = *real_ptr;
			temp_imag = *imag_ptr;
			*real_ptr = -temp_imag;	// * i
			*imag_ptr =  temp_real;
		}
		real_ptr+=2;
		imag_ptr+=2;
	}
}

/**
 * @brief	 	Do the dot product operation on the data in the table.
 * @param[in]	Table1 - First form.
 * @param[in]	Table2 - Second form.
 * @param[in]	fst_num - the number of data in the first table.
 * @param[in]	scd_num - the number of data in the second table.
 * @return		none.
 */
void dot_product(IQ_cb Table1,IQ_cb Table2,unsigned char fst_num,unsigned char scd_num)
{
	int temp_real_1,temp_real_2,temp_imag_1,temp_imag_2;
	int temp_real_part,temp_imag_part;
	temp_real_1 = Table1.IQ[fst_num].real_part * Table2.IQ[scd_num].real_part;
	temp_real_2 = Table1.IQ[fst_num].imag_part * Table2.IQ[scd_num].imag_part;
	temp_imag_1 = Table1.IQ[fst_num].real_part * Table2.IQ[scd_num].imag_part;
	temp_imag_2 = Table1.IQ[fst_num].imag_part * Table2.IQ[scd_num].real_part;
	temp_real_part = temp_real_1 - temp_real_2;
	temp_imag_part = temp_imag_1 + temp_imag_2;
	cal_result.real_part = temp_real_part;
	cal_result.imag_part = temp_imag_part;
}


/**
 * @brief	 	Phase calculation based on real and imaginary parts.
 * @param[in]	real - Real value
 * @param[in]	imag - imag value
 * @return		phase value.
 */
int calc_phase(int real, int imag)
{
	unsigned int tan_val;
	int cal_real,cal_imag;
	int ret_val;
	if(real ==0)
	{
		if(imag > 0 )
		{
			return 90;
		}
		else if(imag < 0 )
		{
			return -90;
		}
		else
		{
			return 999;
		}
	}
	else if(imag == 0)
	{
		if(real > 0 )
		{
			return 0;
		}
		else if(real < 0 )
		{
			return 180;
		}
		else
		{
			return 999;
		}
	}

	if(real>0 && imag>0)
	{
		cal_real = real;
		cal_imag = imag;
		tan_val = cal_imag * 256 / cal_real;
		ret_val = tan_look_table(tan_val);
		return ret_val;
	}
	else if(real>0 && imag<0)
	{
		cal_real = real;
		cal_imag = -imag;
		tan_val = cal_imag * 256 / cal_real;
		ret_val = tan_look_table(tan_val);
		return -ret_val;
	}
	else if(real<0 && imag<0)
	{
		cal_real = -real;
		cal_imag = -imag;
		tan_val = cal_imag * 256 / cal_real;
		ret_val = tan_look_table(tan_val);
		return (ret_val-180);
	}
	else
	{
		cal_real = -real;
		cal_imag = imag;
		tan_val = cal_imag * 256 / cal_real;
		ret_val = tan_look_table(tan_val);
		return (180-ret_val);
	}
}

//Find the angle value based on the tan value of an angle
/**
 * @brief	 	Find the angle value based on the tan value of an angle by looking up the table.
 * @param[in]	tan - Tangent of angle.
 * @return		Angle value.
 */
char tan_look_table(unsigned int tan)
{
	unsigned char i;
	for(i=0;i<90;i++)
	{
		if(((tb_tan[i+1]>tan) && (tan > tb_tan[i])) || (tan == tb_tan[i]) )
		{
			return i;
		}
	}
	return 90;
}


/**
 * @brief	 	Find the angle value based on the sin value of an angle by looking up the table.
 * @param[in]	tan - sin of angle.
 * @return		Angle value.
 */
char sin_look_table(unsigned int sin)
{
	unsigned char i;
	for(i=0;i<90;i++)
	{
		if(((tb_sin[i+1]>sin) && (sin > tb_sin[i]))||(sin == tb_sin[i]))
		{
			return i;
		}
	}
	return 90;
}


/**
 * @brief	 	Find the cos value of a digit.
 * @param[in]	cos_val - The digit of value need to calc cos value.
 * @return		cos value.
 */
int cos_calculate(int cos_val)
{
	int temp_cos = cos_val;
	// 1.change cos range from 0 to 360
	while(temp_cos>360 || temp_cos<0)
	{
		if(temp_cos>360)
		{
			temp_cos -= 360;
		}
		else if(temp_cos<0)
		{
			temp_cos += 360;
		}
	}

	// 2.count cos
	if(temp_cos == 0)
	{
		return	1024;
	}
	else if(temp_cos == 90 || temp_cos ==270)
	{
		return 0;
	}
	else if(temp_cos == 180)
	{
		return -1024;
	}
	else if(0<temp_cos && temp_cos<90)
	{
		return cos_radian[temp_cos];
	}
	else if(90<temp_cos && temp_cos<180)
	{
		return -cos_radian[180 - temp_cos];
	}
	else if(180<temp_cos && temp_cos<270)
	{
		return -cos_radian[temp_cos - 180];
	}
	else if(270<temp_cos && temp_cos<360)
	{
		return cos_radian[360 - temp_cos];
	}
	else
	{
		return 0xffffff00;
	}
}


/**
 * @brief	 	Find the cos value of a digit.
 * @param[in]	cos_val - The digit of value need to calc cos value.
 * @return		cos value.
 */
int phase_combine(int* x_ptr,int osr)
{
	int temp,delta;
	int ret_val;
	temp = angle_trans( (*x_ptr) * osr);
	delta = angle_trans(*(x_ptr+1)-temp );
	ret_val= *x_ptr + delta/osr;
	return ret_val;
}

/**
 * @brief	 	To prevent mutation, adjust the value to between -pi and pi.
 * @param[in]	x - The digit  need to translate.
 * @return		the value after translate.
 */
int angle_trans(int x)	//input is 1024 times
{
	int temp_x;
	int pi;
	int temp;
	int ret_val;
	temp_x = x *256;	//256 times
	pi = 804;	//256 times
	temp_x = temp_x/pi ;
	temp = temp_x % 2048;
	if(temp>1024)
	{
		temp = temp - 2048;
	}
	ret_val = temp * 314/100;
	return ret_val;
}

/**
 * @brief	 	Adjust the difference between the two to -pi to pi.
 * @param[in]	ptr1 - The address of first value.
 * @param[in]	ptr1 - The address of second value.
 * @return		0.
 */
char unwrap(int* ptr1,int* ptr2)
{
	if(*(ptr1)-*(ptr2)>3215)
	{
		*(ptr1) -= 6430;
	}
	else if(*(ptr2)-*(ptr1)>3215)
	{
		*(ptr1) += 6430;
	}
	return 0;
}

/**
 * @brief	 	Find the difference between each value in a set of values and their average.
 * @param[in]	ptr - The address of value.
 * @return		none.
 */
void delate_average(int* ptr)
{
	int temp_sum;
	int temp_average;
	temp_sum = *ptr + *(ptr+1) + *(ptr+2) + *(ptr+3);
	temp_average = temp_sum / 4;
//	printf("temp_average = %x\n",temp_average);
	*ptr = *ptr - temp_average;
	*(ptr+1) = *(ptr+1) - temp_average;
	*(ptr+2) = *(ptr+2) - temp_average;
	*(ptr+3) = *(ptr+3) - temp_average;
}

/**
 *
 */
void delta_numerator(int* ptr,int b2,int* ptr2)
{
	int temp_sum;
	int temp1,temp2,temp3,temp4;
	temp1 = *ptr + b2/2;
	temp2 = *(ptr+1) - b2/2;
	temp3 = *(ptr+2) - b2*3/2;
	temp4 = *(ptr+3) + b2*3/2;
	temp_sum = temp1*temp1 + temp2*temp2 + temp3*temp3 + temp4*temp4;
	*ptr2 = temp_sum;
}

/**
 * @brief	 	Get the sum of squares of a set of numbers.
 * @param[in]	ptr1 - The address of value.
 * @param[in]	ptr2 - The address to store the sum of squares.
 * @return		none.
 */
void  delta_denominator(int* ptr,int* ptr2)
{
	int temp_sum;
	temp_sum = (*ptr)*(*ptr) + *(ptr+1) * *(ptr+1) + *(ptr+2) * *(ptr+2) + *(ptr+3) * *(ptr+3);
//	printf("delta_denominator is %d\n",temp_sum);
	*ptr2 = temp_sum;
}


/**
 * @brief      This function servers to initialize the look-up table algorithm without Z-axis information.
 * @param[in]  none.
 * @return     none.
 */
void init_lookup_table_algorithm_no_Z(void)
{
	unsigned int ii,jj,kk;
	int alpha_range_temp[45];
	int theta_range_no_z[1] = {};
	int antenna_pos[8];
	int cos_temp[8];
	int temp_1024[9];
	int temp2_1024[9];


	//1.init alpha_range_temp

	for(ii=0;ii<XY_TABLE_NUMBER;ii++)
	{
		alpha_range_temp[ii] = ii ;
	}

	for(ii=0;ii<8;ii++)
	{
		antenna_pos[ii] = ii * 45 ;	//256 times
	}

	for(ii=1;ii<XY_TABLE_NUMBER+1;ii++)
	{
		for(jj=1;jj<2;jj++)
		{
			for(kk=1;kk<9;kk++)
			{
				cos_temp[kk-1] = cos_calculate(alpha_range_temp[ii-1] - antenna_pos[kk-1]);		//1024 times
#if(1)
				temp_1024[kk-1]= (2*Radius*cos_temp[kk-1]*cos_calculate(theta_range_no_z[0]) / 1024 )* RF_REAL_FREQ / 3 /100000 	;	//1024 times
#else
				temp_1024[kk-1]= (2*Radius*cos_temp[kk-1]*cos_calculate(theta_range[0]) / 1024 )* RF_REAL_FREQ / 3 /100000 /30 	;	//1024 times
#endif
//				printf("temp[%d] is %x\n",kk-1,temp_1024[kk-1]);
			}
			temp_1024[8] = temp_1024[0];

			for(kk=1;kk<9;kk++)
			{
				temp2_1024[kk-1] = temp_1024[kk-1] - temp_1024[kk];		//1024 times

				function.int_ptr1 = &temp2_1024[kk-1];
				function.int1 = 2048;
				wrap(function);

				angle_lut_simple[kk-1][ii-1][jj-1] = temp2_1024[kk-1];	//1024 times
			}

		}
	}
}


/**
 * @brief      This function servers to initialize the look-up table algorithm with Z-axis information.
 * @param[in]  none.
 * @return     none.
 */
void init_lookup_table_algorithm_with_Z(void)
{
	unsigned int ii,jj,kk;
	int alpha_range_temp[45];
	int theta_range_z[1] = {0};
	int antenna_pos[8];
	int cos_temp[8];
	int temp_1024[9];
	int temp2_1024[9];
	int sum_temp;



	//1.init alpha_range_temp

	for(ii=0;ii<XY_TABLE_NUMBER;ii++)
	{
		alpha_range_temp[ii] = ii ;
	}

	for(ii=0;ii<8;ii++)
	{
		antenna_pos[ii] = ii * 45 ;	//256 times
	}

	for(ii=1;ii<XY_TABLE_NUMBER+1;ii++)
	{
		for(jj=1;jj<2;jj++)
		{
			for(kk=1;kk<9;kk++)
			{
				cos_temp[kk-1] = cos_calculate(alpha_range_temp[ii-1] - antenna_pos[kk-1]);		//1024 times
#if(0)
				temp_1024[kk-1]= (2*Radius*cos_temp[kk-1]*cos_calculate(theta_range[0]) / 1024 )* RF_REAL_FREQ / 3 /100000 	;	//1024 times
#else
				temp_1024[kk-1]= (2*Radius*cos_temp[kk-1]*cos_calculate(theta_range_z[0]) / 1024 )* RF_REAL_FREQ / 3 /100000 /AMP_SIZE 	;	//1024 times
#endif
			}
			temp_1024[8] = temp_1024[0];

			for(kk=1;kk<9;kk++)
			{
				temp2_1024[kk-1] = temp_1024[kk-1] - temp_1024[kk];		//1024 times

				function.int_ptr1 = &temp2_1024[kk-1];
				function.int1 = 2048;
				wrap(function);

				angle_lut_simple[kk-1][ii-1][jj-1] = temp2_1024[kk-1];	//1024 times
			}
		}
	}

	for(ii=1;ii<XY_TABLE_NUMBER+1;ii++)
	{

		for(jj = 1;jj<AMP_SIZE+2;jj++)
//		for(jj = 1;jj<3;jj++)
		{
			sum_temp = 0;

			for(kk=1;kk<9;kk++)
			{
				temp_1024[kk-1] = angle_lut_simple[kk-1][ii-1][0] * (jj - 1) ;	//1048 times

				if(temp_1024[kk-1] <0)
				{
					temp_1024[kk-1] = -temp_1024[kk-1];					//1024 times
				}

				sum_temp += temp_1024[kk-1];

			}
			mean_val[jj-1] += sum_temp;
		}
	}

	for(jj = 1;jj<AMP_SIZE+2;jj++)
	{
		mean_val[jj-1] = mean_val[jj-1]/XY_TABLE_NUMBER/8;
	}
}

/***********************************************
*Input:
*		func.int_ptr1:source data table pointer
*		func.int_ptr2:result data table pointer
*		func.int1:shift value.can be posetive or negtive
*		func.char1:Kim choose  0 : array shift   1 : row_shift
*		func.char2:array number
*		func.char3:row number
*************************************************/
volatile unsigned char debug_shift;

unsigned char circshift(Func_CB func)
{
	//1.set value
	int* src_table = func.int_ptr1;
	int* rlt_table = func.int_ptr2;
	int  shift_val = func.int1;
	char Kim	   = func.char1;
	char x_num	   = func.char2;
	char y_num	   = func.char3;

	unsigned int cyc1,cyc2;
	unsigned int old_x,old_y,new_x,new_y;

	//2.check parameter
	if(src_table < (int *)SRAM_BOTTOM || src_table> (int *)SRAM_TOP)
	{
		debug_shift = 1;
		return 1;
	}

	if(rlt_table < (int *)SRAM_BOTTOM || rlt_table> (int *)SRAM_TOP)
	{
		debug_shift = 2;
		return 1;
	}

	if(Kim != 0 && Kim != 1)
	{
		debug_shift = 3;
		return 1;
	}

	if(x_num < 1 )
	{
		debug_shift = 4;
		return 1;
	}

	if(y_num < 1 )
	{
		debug_shift = 5;
		return 1;
	}

	while(shift_val<0)
	{
		debug_shift = 6;
		if(Kim == 0)
		{
			shift_val += x_num;
			debug_shift = 7;
		}
		else if(Kim == 1)
		{
			shift_val += y_num;
			debug_shift = 8;
		}
	}

	debug_shift = 9;

	//3.realize function
	for(cyc1=0;cyc1<x_num;cyc1++)
	{
		debug_shift = 0x0a;
		for(cyc2=0;cyc2<y_num;cyc2++)
		{
			debug_shift = 0x0b;
			if(Kim == 0)
			{
				debug_shift = 0x0d;
				old_x = cyc1;
				old_y = cyc2;
				new_x = (old_x + shift_val)%x_num;
				new_y = old_y;
				rlt_table[new_x*y_num+new_y] = src_table[old_x*y_num+old_y];
			}
			else if(Kim == 1)
			{
				debug_shift = 0x0e;
				old_x = cyc1;
				old_y = cyc2;
				new_x = old_x;
				new_y = (old_y + shift_val)%y_num;
				rlt_table[new_x*y_num+new_y] = src_table[old_x*y_num+old_y];
			}
		}
		debug_shift = 0x0f;
	}
	debug_shift = 0x10;
	return 0;
}

/***********************************************
*Input:
*		func.int_ptr1:source data table pointer
*		func.int1:wrap level.
*************************************************/
unsigned char wrap(Func_CB func)
{
	//1.set value
	int* src_table = func.int_ptr1;
	int warp_level = func.int1;

	*src_table = (*src_table+warp_level) % warp_level;			//1024 times
	if(*src_table > (warp_level/2) )
	{
		*src_table -= warp_level;					//1024 times
	}
	return 0;
}

unsigned char trans_uchar_to_int(unsigned char* src_tb,int* rst_tb,unsigned int number)
{
	unsigned int i;
	for(i=0;i<number;i++)
	{
		if( ((*(src_tb+i))&0x80) == 0x80)
		{
			*(rst_tb+i) = ((int)*(src_tb+i)) | 0xffffff00;
		}
		else{
			*(rst_tb+i) = (int)*(src_tb+i);
		}
	}
	return 0;
}


/**
 * @brief		This function servers to find the sum of a set of numbers
 * @param[in]  	*data - the ddr of data.
 * @param[in] 	len   - the number of data.
 * @return     	sum.
 */
unsigned int sum(int *data, unsigned char len)
{
	int sum = 0;
	for(unsigned char i =0; i < len; ++i)
	{
		sum += (data[i]);
	}
	return sum;
}


/**
 * @brief		This function servers to find the average of a set of numbers
 * @param[in]  	*data - the ddr of data.
 * @param[in] 	len   - the number of data.
 * @return     	average.
 */
unsigned int mean(int *data, unsigned char len)
{
	int average = sum(data, len);
	return average/len;
}


/**
 * @brief		This function servers to find the average of a set of numbers
 * @param[in]  	data - the  data.
 * @param[in] 	bytenum   - the type of data.
 * @return     	abs.
 */
/*************************************************************************************************************/
	//update by zhiwei

unsigned int abs_value(int data, TypeDefByteNum bytenum)
{
	int max_int = 0x80000000;
//	int max_int_40b = 0x80000;
	if(BYTE_NUM_2P5 == bytenum){
		if(data>=0x80000)
			return (0xfffff-data+1);
		else if(data < 0x80000)
			return(data);
	}
	else if(BYTE_NUM_4 == bytenum){
		if(data>=max_int)
			return(0xffffffff-data+1);
		else if(data < max_int)
			return(data);
	}
	return 0;
}

/*************************************************************************************************************/

/**
 * @brief		This function is used to convert 20bit to 8bit data
 * @param[in]  	*data_src 				- the ddr of data.
 * @param[in] 	*data_has_amplitude  	- the data with amplitude.
 * @param[in]	*data_no_amplitude		- the data without amplitude.
 * @return     	none.
 */
void frond_end(unsigned char *data_src, unsigned char *data_has_amplitude, unsigned char *data_no_amplitude)
{
	int temp[90] = {0};
//	int max = 0;
	unsigned int max = 0;
	unsigned char ii = 0;
	for(unsigned char i = 0; i < 45; i++)
	{
        temp[i*2] = ((data_src[i*5+2]&0x0f)<<16) + (data_src[i*5+1]<<8) + data_src[i*5];
		temp[i*2+1] = (data_src[i*5+4]<<12) + (data_src[i*5+3]<<4) + ((data_src[i*5+2]>>4)&0x0f);

		if(abs_value(temp[i*2],BYTE_NUM_2P5) > abs_value(temp[i*2+1],BYTE_NUM_2P5))
            max = abs_value(temp[i*2],BYTE_NUM_2P5);
        else
            max = abs_value(temp[i*2+1],BYTE_NUM_2P5);

        if(max>128)
        {
            for (ii=0; ii < 12; ++ii)
            {
                max = max >> 1;
                if(max<=128)
                    break;
            }
            data_no_amplitude[i*2] = (abs_value(temp[i*2], BYTE_NUM_2P5) >> (ii+1));
            data_no_amplitude[i*2+1] = (abs_value(temp[i*2+1], BYTE_NUM_2P5) >> (ii+1));
        }else{
            data_no_amplitude[i*2] = (abs_value(temp[i*2], BYTE_NUM_2P5));
            data_no_amplitude[i*2+1] = (abs_value(temp[i*2+1], BYTE_NUM_2P5));
        }

        if(temp[i*2] > 0x7ffff)
            data_no_amplitude[i*2] = 0x100 - data_no_amplitude[i*2];
        if(temp[i*2+1] > 0x7ffff)
            data_no_amplitude[i*2+1] = 0x100 - data_no_amplitude[i*2+1];
	}

	max = 0;
	for(unsigned char i = 0; i < 90; i++)
	{
		if(abs_value(temp[i],BYTE_NUM_2P5) > max){
			max = abs_value(temp[i],BYTE_NUM_2P5);
		}
	}

	if(max>128)
	{
		for (ii=0; ii < 12; ++ii)
		{
			max = max >> 1;
			if(max<=128)
				break;
		}
		ii = ii + 1;
	}else{
	    ii = 0;
	}

	for (int i = 0; i < 90; ++i)
	{
        data_has_amplitude[i] = (abs_value(temp[i],BYTE_NUM_2P5) >> (ii));
		if(temp[i] > 0x7ffff)
            data_has_amplitude[i] = 0x100 - data_has_amplitude[i];

	}
}

/**
 * @brief		this function to set the table of antenna switch and select the switch mode.
 * @param[in]	addr - the addr of switch tale.
 * @param[in] 	seq_mode - switch mode.
 * @return	  	none.
 */
void supp_ant_lut(unsigned char * addr,TypeDefAntennaSeq seq_mode)
{
	set_antenna_switch_sequence(seq_mode);

	write_reg8(0x140868,((read_reg8(0x140868)&0xf8)|addr[0]));
	write_reg8(0x140868,((read_reg8(0x140868)&0x8f)|(addr[1]<<4)));
	write_reg8(0x140869,((read_reg8(0x140869)&0xf8)|addr[2]));
	write_reg8(0x140869,((read_reg8(0x140869)&0x8f)|(addr[3]<<4)));
	write_reg8(0x14086a,((read_reg8(0x14086a)&0xf8)|addr[4]));
	write_reg8(0x14086a,((read_reg8(0x14086a)&0x8f)|(addr[5]<<4)));
	write_reg8(0x14086b,((read_reg8(0x14086b)&0xf8)|addr[6]));
	write_reg8(0x14086b,((read_reg8(0x14086b)&0x8f)|(addr[7]<<4)));
	write_reg8(0x14086c,((read_reg8(0x14086c)&0xf8)|addr[8]));
	write_reg8(0x14086c,((read_reg8(0x14086c)&0x8f)|(addr[9]<<4)));

	write_reg8(0x14086d,((read_reg8(0x14086d)&0xf8)|addr[10]));
	write_reg8(0x14086d,((read_reg8(0x14086d)&0x8f)|(addr[11]<<4)));
	write_reg8(0x14086e,((read_reg8(0x14086e)&0xf8)|addr[12]));
	write_reg8(0x14086e,((read_reg8(0x14086e)&0x8f)|(addr[13]<<4)));
	write_reg8(0x14086f,((read_reg8(0x14086f)&0xf8)|addr[14]));
	write_reg8(0x14086f,((read_reg8(0x14086f)&0x8f)|(addr[15]<<4)));


//	sub_wr(0x468, addr[0], 2, 0);
//	sub_wr(0x468, addr[1], 6, 4);
//	sub_wr(0x469, addr[2], 2, 0);
//	sub_wr(0x469, addr[3], 6, 4);
//	sub_wr(0x46a, addr[4], 2, 0);
//	sub_wr(0x46a, addr[5], 6, 4);
//	sub_wr(0x46b, addr[6], 2, 0);
//	sub_wr(0x46b, addr[7], 6, 4);
//	sub_wr(0x46c, addr[8], 2, 0);
//	sub_wr(0x46c, addr[9], 6, 4);
//	sub_wr(0x46d, addr[10], 2, 0);
//	sub_wr(0x46d, addr[11], 6, 4);
//	sub_wr(0x46e, addr[12], 2, 0);
//	sub_wr(0x46e, addr[13], 6, 4);
//	sub_wr(0x46f, addr[14], 2, 0);
//	sub_wr(0x46f, addr[15], 6, 4);
}


void triangle_all_open(void)
{
	/*
	 * RF ALL OPEN
	 */
	write_reg8(0x140316,(read_reg8(0x140316)&0xf1));

	write_reg8(0x140334,(read_reg8(0x140334)&0xf3)|0x54);

//	sub_wr(0x59e, 0, 6, 6); //d[6], act as gpio
//	sub_wr(0x58e, 0, 1, 0); //b[1:0], act as gpio
//	sub_wr(0x5af, 2, 5, 4); //d[6], antsel_0
//	sub_wr(0x5aa, 2, 1, 0) ;//b[0], antsel_1
//	sub_wr(0x5aa, 2, 3, 2) ;//b[1], antsel_2
//	sub_wr(0x438, 7, 7, 4); //ant_num logical num
}

/**
 * @brief		This function serve to set the antenna switch sequence.
 * @param[in]	TypeDefAntennaSeq - the switch sequence mode num.
 * @return		none.
 */
void set_antenna_switch_sequence(TypeDefAntennaSeq seq_mode)
{
	if(seq_mode == SWITCH_SEQ_MODE0)
	{
		write_reg8(0x14083c,(read_reg8(0x14083c)&0x3f));
	}
	else if(seq_mode == SWITCH_SEQ_MODE1)
	{
		write_reg8(0x14083c,(read_reg8(0x14083c)&0x3f)|0x40);
	}
	else if(seq_mode == SWITCH_SEQ_MODE2)
	{
		write_reg8(0x14083c,(read_reg8(0x14083c)&0x3f)|0x80);
	}
}


/**
 * @brief		This function serve to set the antenna switch sequence.
 * @param[in]	ant_num - the number of antenna.
 * @return		none.
 */
void set_antenna_num(unsigned char ant_num)
{
	ant_num = ((ant_num & 0x07) << 4);
	write_reg8(0x140838,(read_reg8(0x140838)&0x0f)|ant_num);
}

/**
 * @brief		This function is used to set the AOA or AOD IQ data length.
 * @param[in]	mode	- The length of each I or Q data.
 * @return		none.
 * @note		Attention :When the iq data is 20bit, it cannot be used with the 0.25us mode, which will cause the sampling data to overflow.
 */
void aoa_set_iq_data_mode(aoa_iq_data_mode_e mode)
{
	write_reg8(0x14083d,((read_reg8(0x14083d)&0x8f)|(mode << 4)));
}

/**
 * @brief		This function enables the sending and receiving functions of AOA/AOD in ordinary format packets or ADV format packets.
 * @param[in]	mode - AOA/AOD broadcast package or normal package trx mode.
 * @return		none.
 */
void rf_set_aoa_aod_trx_mode(rf_aoa_aod_mode_e mode)
{
	write_reg8(0x140838,((read_reg8(0x140838) & 0xf0) | mode));
}


/**
 * @brief		This function is used to calibrate AOA, AOD sampling frequency offset.
 * @param[in]	sample_locate - This time is the time of a single switch or a single sample slot.
 * @return		none.
 */
void rf_adjust_ant_sample_offset(unsigned char sample_locate)
{
	write_reg8(0x14083b,sample_locate);
}

/**
 * @brief		This function is used to set the AOD/AOA sampling time.
 * @param[in]	sample_time - The default value is 0x2d, add 1 means sample point delay 0.125us.
 * @return		none.
 * @note		Attention:It should be noted that in normal mode, AOD can set 1us or 2us slot time according to CTE information,
 * 				while AOA is 2us slot in normal mode; at the same time, AOD can be manually set to 1us slot through this function.
 */
void aoa_set_sample_slot_time(sample_slot_time_e sample_time)
{
	if(sample_time <= 3)
	{
		write_reg8 (0x14083c, (( (read_reg8(0x14083c)&0xcf) | (sample_time << 4) )));
		write_reg8(0x14083e,(read_reg8(0x14083e)&0xfc));
	}
	else
	{
		write_reg8 (0x14083c, ( read_reg8(0x14083c)&0xcf));
		write_reg8(0x14083e,(read_reg8(0x14083e)&0xfc) | (sample_time - 3));
	}
}






#if 1
struct paramterConst setParamterConst() {
	struct paramterConst para;
	para.symbolRate = 1;//M symbol/s
	para.iqLen = 45;

#if (ANTMODE == 1)
	para.carrierFreq =  2420e6;
#else
	para.carrierFreq =  2440e6;
#endif

	para.slotDuration = 2;//us
	if (para.symbolRate == 1) {
		para.moduationIdx = 0.5;
		para.carrierFreq += 250e3;
	}else {
		para.moduationIdx = 1.0;
		para.carrierFreq += 500e3;
	}

	para.lambda = SPEEDOFLIGHT / ((float) para.carrierFreq) * 1000.0;//mm

#if (ANTMODE == 1)
	para.azimuthRange = 90;
	para.elevationRange = 0;//0 means same plane only;
	para.antD = 35;
	float phaseCaliInit[ANTNUM] = {0, 0.1195,    0.2639,    0.1633};
	int emitSeqInit[ANTNUM] = { 2, 3, 4, 1};
#else
	int emitSeqInit[ANTNUM] = { 2, 8, 3, 7, 4, 6, 5, 1 };
	for (int i = 0; i < ANTNUM; i++) {
		para.antPos[i] = i * 360.0 / ANTNUM;
	}

#if (ANTMODE == 2)
	para.antRadius = 75; //mm
	para.azimuthRange = 360; // >= 360 means full span
	para.elevationRange = 90; // >= 90 means full span
	float phaseCaliInit[ANTNUM] = {0, 0.0184, 0.0234, -0.0538, -0.1844, -0.3435, -0.0759, -0.0984};
#elif (ANTMODE == 3)
	para.antRadius = 129; //mm
	para.azimuthRange = 360; // >= 360 means full span
	para.elevationRange = 90; // >= 90 means full span
	float phaseCaliInit[ANTNUM] = { 0, -0.7235, -0.6501, -0.8608,    0.5241,    0.1871,    0.1246, -0.1877 };
#endif

#endif

	for (int i = 0; i < ANTNUM; i++) {
		para.antEmitSeq[i] = emitSeqInit[i];
		para.phaseCali[i] = phaseCaliInit[i] * PRECBITSNORM;
	}



	return para;
};



int aoaCalc(unsigned char ant_num,unsigned char *addr)
{
	(void) addr;
	(void) ant_num;
	struct testInfo *tInfo = NULL;
//	tInfo = (struct testInfo *)malloc(TOTALLEN * sizeof(struct testInfo));
	tInfo = (struct testInfo *)malloc(sizeof(struct testInfo));

	struct paramterConst para = setParamterConst();


	///////////////LUT Generation azimuthLut,elevationLut,meanVar

#if (ANTMODE == 1)//LUT for ULA
	Word32_CTE azimuthLut[91];//[-azimuthRange:azimuthRange],181 is the max
#if FLOATPOINT
	Word32_CTE tmpD = para.antD / para.lambda * 2;
#else
	Word32_CTE tmpD = para.antD / para.lambda * 2 * MAX_16;
#endif


	for (int i = 0; i <= para.azimuthRange; i++) {
#if ANDES
		azimuthLut[i] = tmpD * nds_sin_f32(i * (PI / 180.0));
#else
		azimuthLut[i] = tmpD * sin(i * (PI / 180.0));
#endif
	}



#else//LUT for UCA
	Word32_CTE azimuthLut[360];
	Word32_CTE tmpRL = convert_float_qPhase(para.antRadius / para.lambda);
#if !FLOATPOINT
	tmpRL = tmpRL >> 2;
#endif

	int zSearchSize = 33;
	float zSearchStep = 1 / (float)(zSearchSize - 1);

#if (ANTNUM % 2 == 0)
	int antAcc = ANTNUM / 2;
#endif

	for (int i = 0; i < 360 / ANTNUM; i++) {
		Word32_CTE tmp[ANTNUM + 1];
		for (int j = 0; j < antAcc; j++) {
#if ANDES
			tmp[j] = convert_float_qPhase(2 * nds_cos_f32((i - para.antPos[j]) * (PI / 180.0)) * zSearchStep);
#else
			tmp[j] = convert_float_qPhase(2 * cos((i - para.antPos[j]) * (PI / 180.0)) * zSearchStep);
#endif
		}

#if (ANTNUM % 2 == 0)
		tmp[0 + ANTNUM / 2] = -tmp[0];
#else
		tmp[ANTNUM] = tmp[0];
#endif

		for (int j = 0; j < antAcc; j++) {
			azimuthLut[GETIDX(i, j)] = tmpRL * (tmp[j] - tmp[j + 1]);
#if (ANTNUM % 2 == 0)
			azimuthLut[GETIDX(i, j + ANTNUM / 2)] = -azimuthLut[GETIDX(i, j)];
#endif
		}

	}

#if (!FLOATPOINT)
#if ANDES
	nds_shift_q31(azimuthLut, -13, azimuthLut, 360);
#else
	for (int i = 0; i < 360; i++) {
		azimuthLut[i] = azimuthLut[i] >> 13;
	}
#endif
#endif


	float elevationLut[90];
	for (int i = 0; i < zSearchSize; i++) {
		elevationLut[i] = acos(i * zSearchStep) * (180.0 / PI);
	}

	int validZSize = zSearchSize;
	//the range define direct below as 0
	if (para.elevationRange < 90) {
		int tmp = 90 - para.elevationRange;
		for (int i = 0; i < zSearchSize; i++) {
			if (elevationLut[i] < tmp) {
				validZSize = i + 1;
				break;
			}
		}
	}
#endif

#if ((ALGOMODE == 1)&&(ANTMODE != 1))
	Word32_CTE meanVar[90];
	memset(meanVar, 0, sizeof(meanVar));
	for (int j = 1; j < validZSize; j++) {
		for (int i = 0; i < 360 / ANTNUM; i++) {
			for (int k = 0; k < ANTNUM; k++) {
				Word32_CTE tmpLut = azimuthLut[GETIDX(i, k)] * j;
				tmpLut = anglePrime(tmpLut);
				tmpLut = (tmpLut > 0) ? tmpLut : (-tmpLut);
				meanVar[j] += tmpLut;
			}
		}
		meanVar[j] *= ANTNUM / 360.0;
	}
#endif



#if (DEBUG)
	for (int tInfoIdx = 0; tInfoIdx < TOTALLEN; tInfoIdx++) {
#else
		int tInfoIdx = DEBUG;
//		printf("start to process\n");
#endif

		struct testInfo tInfoTmp = tInfo[tInfoIdx];

		//reference signal demod
		int rotNum = 0;
		if (para.moduationIdx == 0.5){
			for (int i = 0; i < 8; i++) {
				if ((rotNum % 4) == 1) {
					//tmp = -i;
					Word32_CTE tmpImag = tInfoTmp.iqSample[i].imag;
					tInfoTmp.iqSample[i].imag = -tInfoTmp.iqSample[i].real;
					tInfoTmp.iqSample[i].real = tmpImag;
				}else if((rotNum % 4) == 2) {
					//tmp = -1;
					tInfoTmp.iqSample[i] = complexFlip(tInfoTmp.iqSample[i]);
				}
				else if((rotNum % 4) == 3){
					//tmp = i;
					Word32_CTE tmpImag = tInfoTmp.iqSample[i].imag;
					tInfoTmp.iqSample[i].imag = tInfoTmp.iqSample[i].real;
					tInfoTmp.iqSample[i].real = -tmpImag;
				}
				rotNum++;
			}
		}
		else {
			for (int i = 0; i < 8; i++) {
				if ((rotNum%2) == 1) {
					tInfoTmp.iqSample[i] = complexFlip(tInfoTmp.iqSample[i]);
				}
				rotNum++;
			}
		}


		//////freqEst based on reference signal
		struct complex32 LRDiffOut[4];
		struct complex32 LRSum = complexInit(0, 0);
		memset(LRDiffOut, 0, sizeof(LRDiffOut));
		for (int i = 0; i < 4; i++) {
			for (int j = (i + 1); j < 8; j++) {
				struct complex32 tmp = complexMultConjHighPrec(tInfoTmp.iqSample[j], tInfoTmp.iqSample[j - i - 1]);
				LRDiffOut[i] = complexAdd(LRDiffOut[i], tmp);
			}

			float tmpFactor = 1024 / (8 - i - 1);
			LRSum.real += LRDiffOut[i].real * tmpFactor;
			LRSum.imag += LRDiffOut[i].imag * tmpFactor;
		}

#if ANDES
		Word32_CTE freqEst = convert_float_qPhase(nds_atan2_f32(LRSum.imag, LRSum.real) / PI / 5.0);
#else
		Word32_CTE freqEst = convert_float_qPhase(atan2(LRSum.imag, LRSum.real) / PI / 5.0);
#endif




		float tmpDemodRot = fmod(para.moduationIdx  * 2.0 * para.slotDuration, 2.0); //tmpDemodRot can be 0 or 1 only.
		if (tmpDemodRot != 0) {
			//int rotNum = 0;
			rotNum = 0;
			for (int i = 8; i < (para.iqLen - ANTNUM); i++) {
				if ((rotNum % 2) == 1) {
					tInfoTmp.iqSample[i] = complexFlip(tInfoTmp.iqSample[i]);
				}
				rotNum++;
			}
		}

#if ADVANCEDFREQEST
		///////freqEst base on samples in switch slot
		LRSum = complexInit(0, 0);
		for (int i = 8; i < (para.iqLen - ANTNUM); i++) {
			LRSum = complexAdd(LRSum,complexMultConjHighPrec(tInfoTmp.iqSample[i + ANTNUM],tInfoTmp.iqSample[i]));
		}

#if ANDES
		Word32_CTE freqEst2 = nds_atan2_q31(LRSum.imag, LRSum.real) >> 16;
#else
		Word32_CTE freqEst2 = convert_float_qPhase(atan2(LRSum.imag, LRSum.real) / PI);
#endif

		//////combine freqEst and freqEst2
		Word32_CTE tmpFreqEst = anglePrime(freqEst * 2 * ANTNUM * 2 * para.slotDuration);
		tmpFreqEst = anglePrime(freqEst2 - tmpFreqEst);
		freqEst += tmpFreqEst / 2.0 / 2.0 / ANTNUM / para.slotDuration;
#endif
		//foff compensation
		Word32_CTE freqEstStep = - freqEst * 2.0 * para.slotDuration * 2.0;
		freqEstStep = anglePrime(freqEstStep);
		Word32_CTE freqEstComp = 0;

		Word32_CTE angleIq[IQLEN - 8];
		for (int i = 0; i < (para.iqLen - 8); i++) {
#if ANDES
			angleIq[i] = (Word32_CTE) nds_atan2_q15(tInfoTmp.iqSample[i + 8].imag,tInfoTmp.iqSample[i + 8].real) ;
#else
			angleIq[i] = convert_float_qPhase(atan2(tInfoTmp.iqSample[i + 8].imag,tInfoTmp.iqSample[i + 8].real) / PI);
#endif
			angleIq[i] += freqEstComp;
			freqEstComp += freqEstStep;
		}

		//allocate angle information to antennas
		//average angle information of different sample slot
		//compensate phaseCali
		//	Word32_CTE angleAnt[ANTNUM + 1];
		Word32_CTE angleAnt[ANTNUM * 2];
		memset(angleAnt, 0, sizeof(angleAnt));

		for (int i = 0; i < ANTNUM; i++) {
			Word32_CTE tmpAngleAnt[IQLEN - 8];
			int sampleIdx = 0;
			for (int j = 0; j < (para.iqLen - 8); j++) {
				if (i == (para.antEmitSeq[j%ANTNUM] - 1)) {
					tmpAngleAnt[sampleIdx] = angleIq[j];
					sampleIdx++;
				}
			}
			Word32_CTE tmpAngleAntDiff = 0.0;
			for (int j = 1; j < sampleIdx; j++) {
				tmpAngleAntDiff += anglePrime(tmpAngleAnt[j] - tmpAngleAnt[0]);
			}
			tmpAngleAntDiff = tmpAngleAntDiff / sampleIdx;
			angleAnt[i] = tmpAngleAnt[0] +  tmpAngleAntDiff -  para.phaseCali[i];
		}

#if (ANTMODE == 1)
		///average angle information of different antenna differential
		Word32_CTE angleDiffMean = 0;

		for (int i = 0; i < ANTNUM - 1; i++) {
			angleAnt[i] = anglePrime(angleAnt[i + 1] - angleAnt[i]);
		}
		for (int i = 1; i < ANTNUM - 1; i++) {
			angleDiffMean += anglePrime(angleAnt[i] - angleAnt[0]);
		}


		angleDiffMean /= (ANTNUM - 1.0);
		angleDiffMean += angleAnt[0];

		//search the azimuthRange
		Word32_CTE minVal = MAX_32;
		int azimuthIdx = 0;
		int signMark = 1;
		if (angleDiffMean < 0) {
			angleDiffMean = -angleDiffMean;
			signMark = -1;
		}

		for (int i = 0; i <= para.azimuthRange; i++) {
			Word32_CTE tmp = angleDiffMean - azimuthLut[i];
			tmp = (tmp > 0) ? tmp : (-tmp);
			if (tmp < minVal) {
				minVal = tmp;
				azimuthIdx = i;
			} else {
				break;
			}
		}

		minVal = 0;
		Word32_CTE tmp = (signMark > 0) ? azimuthLut[azimuthIdx] : (-azimuthLut[azimuthIdx]);
		for (int i = 0; i < ANTNUM - 1; i++) {
			minVal += (angleAnt[i] > tmp) ? (angleAnt[i] - tmp):(tmp - angleAnt[i]);
		}
		minVal = minVal / (ANTNUM - 1.0);

#else
		//differential different antennas
		angleAnt[ANTNUM] = angleAnt[0];
		for (int i = 0; i < ANTNUM; i++) {
			angleAnt[i] = anglePrime(angleAnt[i + 1] - angleAnt[i]);
		}

		int startZidx = 0;
		int endZidx = validZSize;

#if (ALGOMODE == 1)
		//// trim the search space of elevationRange based on the mean variation
		Word32_CTE sumDiff = 0.0;
		for (int i = 0; i < ANTNUM; i++) {
			sumDiff += (angleAnt[i] > 0)?(angleAnt[i]):(-angleAnt[i]);
		}

		Word32_CTE minDiff = sumDiff;
		int tmpIdx = 0;
		for (int i = 1; i < validZSize; i++) {
			Word32_CTE tmpDiff = (sumDiff > meanVar[i]) ? (sumDiff - meanVar[i]) : (meanVar[i] - sumDiff);
			if (minDiff > tmpDiff) {
				minDiff = tmpDiff;
				tmpIdx = i;
			}
		}

		if (tmpIdx == 0) {
			endZidx = 2;
		}else if(tmpIdx == (validZSize - 1)) {
			startZidx = (validZSize - 2);
		}else {
			startZidx = tmpIdx - 1;
			endZidx = tmpIdx + 2;
		}
#endif
		//search the azimuthRange and elevationRange
		Word32_CTE minVal = MAX_32;
		int azimuthIdx = 0;
		int elevationIdx = 0;
		int iAzimuth = 0;

		if (startZidx == 0) {
			minVal = 0;
			startZidx++;
			for (int k = 0; k < ANTNUM; k++) {
				minVal += (angleAnt[k] > 0) ? angleAnt[k] : (-angleAnt[k]);
			}
		}

		for (int i = 0; i < ANTNUM; i++) {
			angleAnt[i + ANTNUM] = angleAnt[i];
		}



		for (int i1 = 0; i1 < ANTNUM; i1++) {
			for (int i = 0; i < 360 / ANTNUM; i++) {
				int idx = GETIDX(i, 0);
				for (int j = startZidx; j < endZidx; j++) {
					//if ((i + 1 + i1 * 45 == 88)&(j + 1 == 14)) {
					//	int debuga = 1;
					//}

					Word32_CTE tmpSum = 0;
					for (int k = 0; k < ANTNUM; k++) {
						Word32_CTE tmp = angleAnt[k + i1] + azimuthLut[idx + k] * j;
						tmp = anglePrime(tmp);
						tmpSum += (tmp > 0) ? tmp : (-tmp);
						if (tmpSum > minVal) {
							break;
						}
					}
					if (tmpSum < minVal) {
						minVal = tmpSum;
						azimuthIdx = iAzimuth;
						elevationIdx = j;
					}
				}
				iAzimuth++;
			}
		}




#endif

		//Word32_CTE stat = minVal;
#if (ANTMODE == 1)
//		Word32_CTE estAzimuth = (signMark == 1) ? (azimuthIdx):(-azimuthIdx);
//		float estElevation = 0;
#else
		///unwrap the output
		Word32_CTE tmpAziDiff = azimuthIdx - tInfoTmp.idealAzimuth;
#if FLOATPOINT
		tmpAziDiff = fmod(tmpAziDiff, 360);
#else
		tmpAziDiff = tmpAziDiff%360;
#endif
		if (tmpAziDiff > 180) {
			tmpAziDiff -= 360;
		}
		if (tmpAziDiff < -180) {
			tmpAziDiff += 360;
		}

		Word32_CTE estAzimuth = tmpAziDiff + tInfoTmp.idealAzimuth;
		float estElevation = elevationLut[elevationIdx];
#endif


//#if (DEBUG == 0)
#if(DEBUG)
	}
#endif

	free(tInfo);

#if (ANDES == 0)
	getchar();
#endif

	return 0;
}

#endif

