/********************************************************************************************************
 * @file     aoa.h
 *
 * @brief    This is the header file for BLE SDK
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

#pragma once
#include "analog.h"
#include "gpio.h"
#include "timer.h"

#define RF_REAL_FREQ			2440
#define IQ_NUM                  45
#define SLOT_TIME				2
#define Lamida                  3*10000*1000/(RF_REAL_FREQ*100 + 25)
//#define sign_mark				1		//for rectangle
#define sign_mark				-1
#define Radius					75//129
#define XY_TABLE_NUMBER			45
#define Z_TABLE_NUMBER			31//31
#define AMP_SIZE				32

//#define SRAM_BOTTOM				0x840000
//#define SRAM_TOP				0x84FFFF
#define SRAM_BOTTOM				0x00080000
#define SRAM_TOP				0xc00a0000

/*
 * @brief  Data length type of AOA/AOD sampling.
 * @note   Attention:When the iq data is 20bit, it cannot be used with the 0.25us mode, which will cause the sampling data to overflow.
 */
typedef enum{
	IQ_8_BIT_MODE   		= 0,
	IQ_16_BIT_MODE  		= 1,
	IQ_16_BIT_LOW_MODE		= 2,
	IQ_16_BIT_HIGH_MODE		= 3,
	IQ_20_BIT_MODE			= 4
}aoa_iq_data_mode_e;

/*
 * @brief  Enable AOA or AOD in different packet formats.
 */
typedef enum{
	RF_RX_ACL_AOA_EN  = BIT(0),
	RF_RX_ADV_AOA_EN  = BIT(1),
	RF_TX_ACL_AOA_EN  = BIT(2),
	RF_TX_ADV_AOA_EN  = BIT(3),
	RF_AOA_OFF        = 0
}rf_aoa_aod_mode_e;

/*
 * @brief	Time enumeration of iq sampling slots supported by the chip
 * @note	Attention:In addition to supporting the two sampling slots specified in the protocol, vulture also adds three other modes
 */
typedef enum{
	SAMPLE_NORMAL_SLOT	= 0,
	SAMPLE_1US_SLOT	    = 3,
	SAMPLE_0P5_SLOT	  	= 4,
	SAMPLE_0P25_SLOT  	= 5,
	SAMPLE_0P125_SLOT 	= 6
}sample_slot_time_e;
typedef struct IQ_VALUE
{
	int real_part;
	int imag_part;
}IQ_Value;

typedef struct IQ_CB
{
	IQ_Value IQ[100];
	int number;
}IQ_cb;

typedef struct FUNC_CB
{
	int* 	int_ptr1;
	int* 	int_ptr2;
	char*   char_ptr1;
	char*	char_ptr2;
	int		int1;
	int		int2;
	char	char1;
	char	char2;
	char	char3;
	char	char4;
}Func_CB;

typedef enum{
	BYTE_NUM_2P5	 = 0,
	BYTE_NUM_4		 = 1,
}TypeDefByteNum;

/**
 * @brief	Take 4 antennas as an example to illustrate the antenna switching sequence
 * 			SWITCH_SEQ_MODE0	: antenna index switch sequence 01230123
 * 			SWITCH_SEQ_MODE1	: antenna index switch sequence 0123210
 * 			SWITCH_SEQ_MODE2	: antenna index switch sequence 001000200030
 */
typedef enum{
	SWITCH_SEQ_MODE0		 = 0,//
	SWITCH_SEQ_MODE1		 = 1,
	SWITCH_SEQ_MODE2		 = 2,
}TypeDefAntennaSeq;

/**
 * @brief      This function servers to get the tan_angle.
 * @param[in]  tan  -  The angel which need to be assignment
 * @return     The tan_angle.
 */
char tan_look_table(unsigned int tan);

/**
 * @brief      This function servers to get the sin_angle.
 * @param[in]  sin  -  The angel which need to be assignment
 * @return     The sin_angle.
 */
char sin_look_table(unsigned int sin);

/**
 * @brief      This function servers to get the cos_angle.
 * @param[in]  cos_val  -  The angel which need to be assignment
 * @return     The cos_angle.
 */
int cos_calculate(int cos_val);

/**
 * @brief      This function servers to demodulation the input angle.
 * @param[in]  real_ptr  -  Pointer variable of real part.
 * @param[in]  imag_ptr  -  Pointer variable of imag part.
 * @param[in]  num       -  The value of the total parts.
 * @return     none.
 */
void demodulation(int* real_ptr,int* imag_ptr,unsigned char num);

//void conj(IQ_cb iq,IQ_cb conj_iq,unsigned char number);

/**
 * @brief      This function servers to calculate the phase.
 * @param[in]  real  -  Stand for the real parts.
 * @param[in]  imag  -  Stand for the image parts.
 * @return     The phase value.
 */
int  calc_phase(int real, int imag);

/**
 * @brief      This function servers to calculate real and imag parts.
 * @param[in]  Table1  -  A struct to store value or stand for numbers.
 * @param[in]  Table2  -  A struct to store value or stand for numbers..
 * @param[in]  fst_num -  The value stand for a start number.
 * @param[in]  scd_num -  The value stand for the next number.
 * @return     none.
 */
void dot_product(IQ_cb Table1,IQ_cb Table2,unsigned char fst_num,unsigned char scd_num);	//,IQ_Value rst_tb

/**
 * @brief      This function servers to get phase.
 * @param[in]  x_ptr  -  Pointer variable of the true phase value.
 * @param[in]  osr    -  The value use for calculation.
 * @return     The phase.
 */
//void dot_product(unsigned char first_num,unsigned char second_num);
int  phase_combine(int* x_ptr,int osr);

/**
 * @brief      This function servers to calculate the angel.
 * @param[in]  x -  The value need to be assignment.
 * @return     The angel.
 */
int  angle_trans(int x);	//input is 1024 times

/**
 * @brief      This function servers sure the angel value is accurate.
 * @param[in]  ptr1 -  Pointer variable of the angel.
 * @param[in]  ptr2 -  Pointer variable of the angel.
 * @return     0.
 */
char unwrap(int* ptr1,int* ptr2);

/**
 * @brief      This function servers to get the angel which already minus the average value.
 * @param[in]  ptr1 -  Pointer variable of the angel.
 * @return     none.
 */
void delate_average(int* ptr);


/**
 * @brief      This function servers to get a angle for Rx mode.
 * @param[in]  ptr_packet  -  Pointer variable of the Rx packages.
 * @return     The angle.
 */
_attribute_ram_code_sec_noinline_ unsigned int raw_data_to_angle_no_z(unsigned char *ptr_packet);

/**
 * @brief      This function servers to get a angle for Rx mode.
 * @param[in]  ptr_packet  -  Pointer variable of the Rx packages.
 * @return     The angle.
 */
_attribute_ram_code_sec_noinline_ unsigned int raw_data_to_angle_with_z(unsigned char *ptr_packet);

/**
 * @brief      This function servers to store data to a specific place .
 * @param[in]  data_table  -  Pointer variable to store data.
 * @param[in]  ptr_packet  -  Pointer variable to support data.
 * @param[in]  number      -  The data numbers.
 * @return     none.
 */
void get_raw_data(unsigned char *data_table,unsigned char *ptr_packet,unsigned int number);


/**
 * @brief      This function servers to initialize the look-up table algorithm without Z-axis information.
 * @param[in]  none.
 * @return     none.
 */
void init_lookup_table_algorithm_no_Z(void);

/**
 * @brief      This function servers to initialize the look-up table algorithm with Z-axis information.
 * @param[in]  none.
 * @return     none.
 */
void init_lookup_table_algorithm_with_Z(void);


unsigned char wrap(Func_CB func);
/**
 * @brief      This function servers to shift the circuit.
 * @param[in]  func -  Structure stand for the parameter.
 * @return     Flag. 1 - shift failure 0 - shift success.
 */
unsigned char circshift(Func_CB func);


unsigned char trans_uchar_to_int(unsigned char* src_tb,int* rst_tb,unsigned int number);

/**
 * @brief		This function is used to convert 20bit to 8bit data
 * @param[in]  	*data_src 				- the ddr of data.
 * @param[in] 	*data_has_amplitude  	- the data with amplitude.
 * @param[in]	*data_no_amplitude		- the data without amplitude.
 * @return     	none.
 */
void frond_end(unsigned char *data_src, unsigned char *data_has_amplitude, unsigned char *data_no_amplitude);

/**
 * @brief		This function servers to find the sum of a set of numbers
 * @param[in]  	*data - the ddr of data.
 * @param[in] 	len   - the number of data.
 * @return     	sum.
 */
unsigned int sum(int *data, unsigned char len);

/**
 * @brief		This function servers to find the average of a set of numbers
 * @param[in]  	*data - the ddr of data.
 * @param[in] 	len   - the number of data.
 * @return     	average.
 */
unsigned int mean(int *data, unsigned char len);

/**
 * @brief		This function servers to find the average of a set of numbers
 * @param[in]  	data - the  data.
 * @param[in] 	bytenum   - the type of data.
 * @return     	abs.
 */
unsigned int abs_value(int data, TypeDefByteNum bytenum);


/**
 * @brief		This function serve to find the minimum value of a set of values and return its index value.
 * @param[in]	*data - Address requiring minimum number.
 * @param[in]	len - Number of digits.
 * @return		Minimum.
 */
unsigned int min_array(int *data, unsigned char len, unsigned char* index);

/**
 * @brief		This function serve to set the antenna switch sequence.
 * @param[in]	TypeDefAntennaSeq - the switch sequence mode num.
 * @return		none.
 */
void set_antenna_switch_sequence(TypeDefAntennaSeq seq_mode);

/**
 * @brief		This function serve to set the antenna switch sequence.
 * @param[in]	ant_num - the number of antenna.
 * @return		none.
 */
void set_antenna_num(unsigned char ant_num);

/**
 * @brief		this function to set the table of antenna switch and select the switch mode.
 * @param[in]	addr - the addr of switch tale.
 * @param[in] 	seq_mode - switch mode.
 * @return	  	none.
 */
void supp_ant_lut(unsigned char * addr,TypeDefAntennaSeq seq_mode);


void triangle_all_open(void);

/**
 * @brief		This function is used to set the AOA or AOD IQ data length.
 * @param[in]	mode	- The length of each I or Q data.
 * @return		none.
 * @note		Attention :When the iq data is 20bit, it cannot be used with the 0.25us mode, which will cause the sampling data to overflow.
 */
void aoa_set_iq_data_mode(aoa_iq_data_mode_e mode);

/**
 * @brief		This function enables the sending and receiving functions of AOA/AOD in ordinary format packets or ADV format packets.
 * @param[in]	mode - AOA/AOD broadcast package or normal package trx mode.
 * @return		none.
 */
void rf_set_aoa_aod_trx_mode(rf_aoa_aod_mode_e mode);

/**
 * @brief		This function is used to calibrate AOA, AOD sampling frequency offset.
 * @param[in]	sample_locate - The default value is 0x2d, add 1 means sample point delay 0.125us.
 * @return		none.
 */
void rf_adjust_ant_sample_offset(unsigned char sample_locate);

/**
 * @brief		This function is used to set the AOD/AOA sampling time.
 * @param[in]	sample_time - The default value is 0x2d, add 1 means sample point delay 0.125us.
 * @return		none.
 * @note		Attention:It should be noted that in normal mode, AOD can set 1us or 2us slot time according to CTE information,
 * 				while AOA is 2us slot in normal mode; at the same time, AOD can be manually set to 1us slot through this function.
 */
void aoa_set_sample_slot_time(sample_slot_time_e sample_time);





#define		Eagle_AOA		1

#if Eagle_AOA
#define SPEEDOFLIGHT 299792458
#define PI 3.141592653589793
#define MIN_32 -2147483648
#define MAX_32  2147483647
#define MIN_16 -32768
#define MAX_16  32767

//algo related
//#define ALGOMODE 1// 1 as default, for UCA, 2 is brute force version
//D:\git telink\telink_aoa\matlab code\ULA 8258\aoa_data_for_yeyang_0728.m
//D:\git telink\telink_aoa\matlab code\UCA 8278\aoa_uca_for_yibao_2020_PCB.m

#define ANTMODE 1//1:ULA 2:external UCA, 3:external PCA
//antenna array related
#if (ANTMODE == 1)
#define ANTNUM 4
#define ALGOMODE 1
#else
#define ANTNUM 8
#define ALGOMODE 2
#endif

#define ADVANCEDFREQEST 1

//data source related
#define FILEIDX 1
#define IQLEN 45
#if (FILEIDX == 1)
#if (ANTMODE == 1)
#define TOTALLEN 2500
#else
#define TOTALLEN 3600
#endif

#elif (FILEIDX == 2)
#if (ANTMODE == 1)
#define TOTALLEN 25
#else
#define TOTALLEN 36
#endif

#endif


//precsion related
#define FLOATPOINT 0
#define DEBUG 0
#define ANDES 1


#if ANDES
#include <nds_math_types.h>
#include <nds_intrinsic.h>
#include <nds_utils_math.h>
#include <nds_basic_math.h>
#include <nds_statistics_math.h>
#include <nds_complex_math.h>
#endif


#if FLOATPOINT
#define PRECBITS 0
#define PRECBITSNORM 1
#define WRAPRANGE 2.0

#define Word16_CTE double
#define Word32_CTE double
#define Word64_CTE double
#define UWord16_CTE double
#define UWord32_CTE double
#define UWord64_CTE double
#define WRAPRANGE 2.0

#else
#define PRECBITS 15
#define PRECBITSNORM  32768//2^PRECBITS
#define WRAPRANGE (PRECBITSNORM*2)

#if(ANDES == 1)
#define Word16_CTE q15_t
#define Word32_CTE q31_t
#define Word64_CTE q63_t
#define UWord16_CTE unsigned q15_t
#define UWord32_CTE unsigned q31_t
#define UWord64_CTE unsigned q63_t
#else
#define Word16_CTE __int16
#define Word32_CTE __int32
#define Word64_CTE __int64
#define UWord16_CTE unsigned __int16
#define UWord32_CTE unsigned __int32
#define UWord64_CTE unsigned __int64
#endif

#endif

#define GETIDX(a,b) ((a)*ANTNUM+(b))



struct complex16 {
	Word16_CTE real;
	Word16_CTE imag;
};


struct complex32 {
	Word32_CTE real;
	Word32_CTE imag;
};


struct testInfo {
	float idealAzimuth;
	float idealElevation;
	float stat;
	float estAzimuth;
	float estElevation;
	struct complex32 iqSample[IQLEN];
};

struct paramterConst {
	int symbolRate;
	Word64_CTE carrierFreq;
	int slotDuration;
	float moduationIdx;
	float lambda;
	int azimuthRange;
	int elevationRange;
	int iqLen;

	Word32_CTE phaseCali[ANTNUM];
	int antEmitSeq[ANTNUM];
#if (ANTMODE == 1)
	float antD;
#else
	float antPos[ANTNUM];
	float antRadius;
#endif

};


inline Word32_CTE anglePrime(Word32_CTE x) {
#if FLOATPOINT
	x = fmod(x, WRAPRANGE);
#else
	x = x%WRAPRANGE;
#endif
	if (x > WRAPRANGE/2.0) {
		x -= WRAPRANGE;
	}
	if (x < -WRAPRANGE/2.0) {
		x += WRAPRANGE;
	}

	return x;
};


inline Word32_CTE convert_float_qPhase(float src) {
	Word32_CTE dst = src * PRECBITSNORM;
	return dst;
};

inline struct complex32 complexFlip(struct complex32 c1) {
	struct complex32 c3;
	c3.real = -c1.real;
	c3.imag = -c1.imag;
	return c3;
}

inline struct complex32 complexAdd(struct complex32 c1, struct complex32 c2) {
	struct complex32 c3;
	c3.real = c1.real + c2.real;
	c3.imag = c1.imag + c2.imag;
	return c3;
};

inline struct complex32 complexMultConjHighPrec(struct complex32 c1, struct complex32 c2) {
	struct complex32 c3;
	c3.real = c1.real * c2.real + c1.imag * c2.imag;
	c3.imag = -c1.real * c2.imag + c1.imag * c2.real;
	return c3;
};

inline struct complex32 complexInit(Word32_CTE real, Word32_CTE imag) {
	struct complex32 c;
	c.real = real;
	c.imag = imag;
	return c;
};


struct paramterConst setParamterConst();

//int aoaCalc();

int aoaCalc(unsigned char ant_num,unsigned char *addr);






#endif






