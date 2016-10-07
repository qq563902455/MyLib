/**
******************************************************************************
* @file    ArtificialNeuralNetwork.h
* @author  Lxy ACTION
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
#ifndef __ANN_H
#define __ANN_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
enum
{
	SIGMOID = 0,
    TANH,
	SPECIAL
};
/* Exported macro ------------------------------------------------------------*/
/* Exported class ------------------------------------------------------------*/
//下面的类用于解决三层BP神经网络相关问题
class BP_ANN
{
  private:
	  uint8_t in_port;      //输入节点数
	  uint8_t hide_layer;   //隐藏层节点数
	  uint8_t out_port;     //输出层节点数
	  float **w_L;          //从输入层节点到隐藏层节点链接的权重
	  float **w_R;          //从隐藏层节点到输出层节点链接的权重
	  float *b_L;           //隐藏层节点的阈值
	  float *b_R;           //输出层节点的阈值
  public:
	  BP_ANN(uint8_t in, uint8_t hide, uint8_t out, float **data_l, float **data_r, float *bL, float *bR);
	  BP_ANN(uint8_t in, uint8_t hide, uint8_t out);
	  ~BP_ANN();
	  void out(float *indata,float *outdata);
	  void study(float *indata, const float *outdata);

};

/* Exported functions ------------------------------------------------------- */
#endif
/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/