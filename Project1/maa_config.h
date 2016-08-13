/**
  ******************************************************************************
  * @file    *.h
  * @author  Lxy zlq
  * @version 
  * @date   
  * @brief   This file contains the headers of 
  ******************************************************************************
  * @attention
  *
  *
  * 
  * 
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ALGORITHM_H
#define __ALGORITHM_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct{
	float x;
	float y;
	float z;
}three_axis;

typedef struct{
	float pitch;
	float roll;
	float yaw;
}euler_angles;

typedef struct{
	float q0;
	float q1;
	float q2;
	float q3;
}Quarternion;


typedef enum{FALSE = 0,TRUE = !FALSE}BOOL;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* L3GD20H 寄存器地址-----------------------*/
#define L3GD20H_WHO_AM_I								0x0F

#define L3GD20H_CTRL_REG1								0x20
#define L3GD20H_CTRL_REG2 							0x21
#define L3GD20H_CTRL_REG3 							0x22
#define L3GD20H_CTRL_REG4 							0x23
#define L3GD20H_CTRL_REG5 							0x24

#define L3GD20H_REFERENCE 							0x25
#define L3GD20H_OUT_TEMP 								0x26
#define L3GD20H_STATUS_REG 							0x27

#define L3GD20H_OUT_X_L 								0x28											/* X轴角速率数据，值为2的补码						 */
#define L3GD20H_OUT_X_H 								0x29
#define L3GD20H_OUT_Y_L 								0x2A											/* Y轴角速率数据，值为2的补码						 */
#define L3GD20H_OUT_Y_H 								0x2B
#define L3GD20H_OUT_Z_L 								0x2C											/* Z轴角速率数据，值为2的补码						 */
#define L3GD20H_OUT_Z_H 								0x2D

#define L3GD20H_FIFO_CTRL_REG 					0x2E
#define L3GD20H_FIFO_SRC_REG 						0x2F
#define L3GD20H_INT1_CFG 								0x30
#define L3GD20H_INT1_SRC 								0x31

#define L3GD20H_INT1_TSH_XH 						0x32
#define L3GD20H_INT1_TSH_XL 						0x33
#define L3GD20H_INT1_TSH_YH 						0x34
#define L3GD20H_INT1_TSH_YL 						0x35
#define L3GD20H_INT1_TSH_ZH 						0x36
#define L3GD20H_INT1_TSH_ZL 						0x37
#define L3GD20H_INT1_DURATION 					0x38
#define L3GD20H_LOW_ODR               	0x39


/* ICM20608G 陀螺仪寄存器地址--------------*/
#define ICM20608G_WHO_AM_I							0x75

#define ICM20608G_SELF_TEST_X_GYRO			0x00
#define ICM20608G_SELF_TEST_Y_GYRO			0x01
#define ICM20608G_SELF_TEST_Z_GYRO			0x02
#define ICM20608G_SELF_TEST_X_ACCEL			0x0D
#define ICM20608G_SELF_TEST_Y_ACCEL			0x0E
#define ICM20608G_SELF_TEST_Z_ACCEL			0x0F

#define ICM20608G_XG_OFFS_USRH					0x13
#define ICM20608G_XG_OFFS_USRL					0x14
#define ICM20608G_YG_OFFS_USRH					0x15
#define ICM20608G_YG_OFFS_USRL					0x16
#define ICM20608G_ZG_OFFS_USRH					0x17
#define ICM20608G_ZG_OFFS_USRL					0x18

#define ICM20608G_SMPLRT_DIV						0x19
#define ICM20608G_CONFIG								0x1A
#define ICM20608G_GYRO_CONFIG						0x1B
#define ICM20608G_ACCEL_CONFIG					0x1C
#define ICM20608G_ACCEL_CONFIG2					0x1D
#define ICM20608G_LP_MODE_CFG						0x1E
#define ICM20608G_ACCEL_WOM_THR					0x1F
#define ICM20608G_FIFO_EN								0x23
#define ICM20608G_FSYNC_INT							0x36
#define ICM20608G_INT_PIN_CFG						0x37
#define ICM20608G_INT_ENABLE						0x38
#define ICM20608G_INT_STATUS						0x3A

#define ICM20608G_ACCEL_XOUT_H					0x3B
#define ICM20608G_ACCEL_XOUT_L					0x3C
#define ICM20608G_ACCEL_YOUT_H					0x3D
#define ICM20608G_ACCEL_YOUT_L					0x3E
#define ICM20608G_ACCEL_ZOUT_H					0x3F
#define ICM20608G_ACCEL_ZOUT_L					0x40
#define ICM20608G_TEMP_OUT_H						0x41
#define ICM20608G_TEMP_OUT_L						0x42
#define ICM20608G_GYRO_XOUT_H						0x43
#define ICM20608G_GYRO_XOUT_L						0x44
#define ICM20608G_GYRO_YOUT_H						0x45
#define ICM20608G_GYRO_YOUT_L						0x46
#define ICM20608G_GYRO_ZOUT_H						0x47
#define ICM20608G_GYRO_ZOUT_L						0x48

#define ICM20608G_SIGNAL_INTEL_CTRL			0x68							/* Use SIG_COND_RST to clear sensor registers.*/
#define ICM20608G_ACCEL_INTEL_CTRL			0x69
#define ICM20608G_USER_CTRL							0x6A
#define ICM20608G_PWR_MGMT_1						0x6B
#define ICM20608G_PWR_MGMT_2						0x6C

#define ICM20608G_FIFO_COUNTH						0x72
#define ICM20608G_FIFO_COUNTL						0x73
#define ICM20608G_FIFO_R_W							0x74

#define ICM20608G_XA_OFFSET_H						0x77
#define ICM20608G_XA_OFFSET_L						0x78
#define ICM20608G_YA_OFFSET_H						0x7A
#define ICM20608G_YA_OFFSET_L						0x7B
#define ICM20608G_ZA_OFFSET_H						0x7D
#define ICM20608G_ZA_OFFSET_L						0x7E

/* LSM303C 6D电子罗盘寄存器地址------------*/
//加速度计部分
#define LSM303C_WHO_AM_I_A							0x0F

#define LSM303C_ACT_THS_A								0x1E
#define LSM303C_ACT_DUR_A								0x1F

#define LSM303C_CTRL_REG1_A							0x20
#define LSM303C_CTRL_REG2_A							0x21
#define LSM303C_CTRL_REG3_A							0x22
#define LSM303C_CTRL_REG4_A							0x23
#define LSM303C_CTRL_REG5_A							0x24
#define LSM303C_CTRL_REG6_A							0x25
#define LSM303C_STATUS_REG_A						0x27

#define LSM303C_OUT_X_L_A								0x28
#define LSM303C_OUT_X_H_A								0x29
#define LSM303C_OUT_Y_L_A								0x2A
#define LSM303C_OUT_Y_H_A								0x2B
#define LSM303C_OUT_Z_L_A								0x2C
#define LSM303C_OUT_Z_H_A								0x2D

#define LSM303C_FIFO_CTRL								0x2E
#define LSM303C_FIFO_SRC								0x2F

#define LSM303C_IG_CFG1_A								0x30
#define LSM303C_IG_SRC1_A								0x31
#define LSM303C_IG_THS_X1_A							0x32
#define LSM303C_IG_THS_Y1_A							0x33
#define LSM303C_IG_THS_Z1_A							0x34
#define LSM303C_IG_DUR1_A								0x35
#define LSM303C_IG_CFG2_A								0x36
#define LSM303C_IG_SRC2_A								0x37
#define LSM303C_IG_THS2_A								0x38
#define LSM303C_IG_DUR2_A								0x39

#define LSM303C_XL_REFERENCE						0x3A
#define LSM303C_XH_REFERENCE						0x3B
#define LSM303C_YL_REFERENCE						0x3C
#define LSM303C_YH_REFERENCE						0x3D
#define LSM303C_ZL_REFERENCE						0x3E
#define LSM303C_ZH_REFERENCE						0x3F

//磁力计部分
#define LSM303C_WHO_AM_I_M							0x0F

#define LSM303C_ACT_THS_M								0x1E
#define LSM303C_ACT_DUR_M								0x1F

#define LSM303C_CTRL_REG1_M							0x20
#define LSM303C_CTRL_REG2_M							0x21
#define LSM303C_CTRL_REG3_M							0x22
#define LSM303C_CTRL_REG4_M							0x23
#define LSM303C_CTRL_REG5_M							0x24
#define LSM303C_STATUS_REG_M						0x27

#define LSM303C_OUT_X_L_M								0x28
#define LSM303C_OUT_X_H_M								0x29
#define LSM303C_OUT_Y_L_M								0x2A
#define LSM303C_OUT_Y_H_M								0x2B
#define LSM303C_OUT_Z_L_M								0x2C
#define LSM303C_OUT_Z_H_M								0x2D

#define LSM303C_TEMP_L_M								0x2E
#define LSM303C_TEMP_H_M								0x2F
#define LSM303C_INT_CFG_M								0x30
#define LSM303C_INT_SRC_M								0x31
#define LSM303C_INT_THS_L_M							0x32
#define LSM303C_INT_THS_H_M							0x33

/* LSM6DS33 3D gyro+3D accel，iNEMO inertial module */

#define LSM6DS33_WHO_AM_I								0x0f

#define LSM6DS33_FUNC_CFG_ACCESS				0x01

#define LSM6DS33_FIFO_CTRL1							0x06
#define LSM6DS33_FIFO_CTRL2							0x07
#define LSM6DS33_FIFO_CTRL3							0x08
#define LSM6DS33_FIFO_CTRL4							0x09
#define LSM6DS33_FIFO_CTRL5							0x0A
#define LSM6DS33_ORIENT_CFG_G						0x0B

#define LSM6DS33_INT1_CTRL							0x0D
#define LSM6DS33_INT2_CTRL							0x0E

#define LSM6DS33_CTRL1_XL								0x10
#define LSM6DS33_CTRL2_G								0x11
#define LSM6DS33_CTRL3_C								0x12
#define LSM6DS33_CTRL4_C								0x13
#define LSM6DS33_CTRL5_C								0x14
#define LSM6DS33_CTRL6_C								0x15
#define LSM6DS33_CTRL7_G								0x16
#define LSM6DS33_CTRL8_XL								0x17
#define LSM6DS33_CTRL9_XL								0x18
#define LSM6DS33_CTRL10_C								0x19

#define LSM6DS33_WAKE_UP_SRC						0x1B
#define LSM6DS33_TAP_SRC								0x1C
#define LSM6DS33_D6D_SRC								0x1D
#define LSM6DS33_STATUS_REG							0x1E

#define LSM6DS33_OUT_TEMP_L							0x20
#define LSM6DS33_OUT_TEMP_H							0x21
#define LSM6DS33_OUTX_L_G								0x22
#define LSM6DS33_OUTX_H_G								0x23
#define LSM6DS33_OUTY_L_G								0x24
#define LSM6DS33_OUTY_H_G								0x25
#define LSM6DS33_OUTZ_L_G								0x26
#define LSM6DS33_OUTZ_H_G								0x27
#define LSM6DS33_OUTX_L_XL							0x28
#define LSM6DS33_OUTX_H_XL							0x29
#define LSM6DS33_OUTY_L_XL							0x2A
#define LSM6DS33_OUTY_H_XL							0x2B
#define LSM6DS33_OUTZ_L_XL							0x2C
#define LSM6DS33_OUTZ_H_XL							0x2D

#define LSM6DS33_FIFO_STATUS1						0x3A
#define LSM6DS33_FIFO_STATUS2						0x3B
#define LSM6DS33_FIFO_STATUS3						0x3C
#define LSM6DS33_FIFO_STATUS4						0x3D
#define LSM6DS33_FIFO_DATA_OUT_L				0x3E
#define LSM6DS33_FIFO_DATA_OUT_H				0x3F

#define LSM6DS33_TIMESTAMP0_REG					0x40
#define LSM6DS33_TIMESTAMP1_REG					0x41
#define LSM6DS33_TIMESTAMP2_REG					0x42

#define LSM6DS33_STEP_TIMESTAMP_L				0x49
#define LSM6DS33_STEP_TIMESTAMP_H				0x4A
#define LSM6DS33_STEP_COUNTER_L					0x4B
#define LSM6DS33_STEP_COUNTER_H					0x4C

#define LSM6DS33_FUNC_SRC								0x53
#define LSM6DS33_TAP_CFG								0x58
#define LSM6DS33_TAP_THS_6D							0x59
#define LSM6DS33_INT_DUR2								0x5A
#define LSM6DS33_WAKE_UP_THS						0x5B
#define LSM6DS33_WAKE_UP_DUR						0x5C
#define LSM6DS33_FREE_FALL							0x5D
#define LSM6DS33_MD1_CFG								0x5E
#define LSM6DS33_MD2_CFG								0x5F



/* LIS3MDL,3D磁力计------------------------*/
#define LIS3MDL_WHO_AM_I								0x0F

#define LIS3MDL_CTRL_REG1								0x20
#define LIS3MDL_CTRL_REG2								0x21
#define LIS3MDL_CTRL_REG3								0x22
#define LIS3MDL_CTRL_REG4								0x23
#define LIS3MDL_CTRL_REG5								0x24
#define LIS3MDL_STATUS_REG							0x27

#define LIS3MDL_OUT_X_L									0x28
#define LIS3MDL_OUT_X_H									0x29
#define LIS3MDL_OUT_Y_L									0X2A
#define LIS3MDL_OUT_Y_H									0x2B
#define LIS3MDL_OUT_Z_L									0x2C
#define LIS3MDL_OUT_Z_H									0x2D
#define LIS3MDL_TEMP_OUT_L							0x2E
#define LIS3MDL_TEMP_OUT_H							0x2F

#define LIS3MDL_INT_CFG									0x30
#define LIS3MDL_INT_SRC									0x31
#define LIS3MDL_INT_THS_L								0x32
#define LIS3MDL_INT_THS_H								0x33



#define L3GD20H    0X00
#define LSM6DS33   0X01
#define ICM20608G  0X02
#define LIS3MDL    0X03
#define LSM303C    0X04

/* FIFO  enable */
//#define LSM6DS33_FIFO_ENABLE
/* DEBUG enable*/
#define DEBUG_ENABLE
/* Hardware Test enable*/
//#define HD_TEST


#define	MAG_SENSOR_ENABLE
#define	ACC_SENSOR_ENABLE

#define R_wheel        25.4f///3088.0*3130.0

#define R0_DE         0.0389631    // 0.0387907 //0.0387681 //0.0388072
#define R0_IN         0.0389631          //0.0388095  //0.0389072 //0.0388368

#define R1_DE         0.0389631 //0.03886230599208180515411333219985//0.0388627779599470494650295721451//0.038841550748913650377491321341
#define R1_IN         0.0389631 //0.03882364359895176162282830243618//0.0388373080890830754293343042660//0.03883966500788930695472751547518

#define ECD_RANGE      4096
#define dT 			   0.005f           //积分的步长
#define PI             3.1415926f       //圆周率的值
#define Temp_ctr       47             //控温的值    //大车上控制的温度 37 45 30
#define TempTable_max  55
#define TempTable_min  10
#define TIME_HEAT      5              //初始化时加热的时间值

#define BF_TH          2               //最下二乘法拟合的阶数

/* Exported functions ------------------------------------------------------- */

#endif	// !__ALGORITHM_H

/******************* (C) COPYRIGHT 2015 ACTION *****END OF FILE****/
