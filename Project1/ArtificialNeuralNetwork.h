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
//����������ڽ������BP�������������
class BP_ANN
{
  private:
	  uint8_t in_port;      //����ڵ���
	  uint8_t hide_layer;   //���ز�ڵ���
	  uint8_t out_port;     //�����ڵ���
	  float **w_L;          //�������ڵ㵽���ز�ڵ����ӵ�Ȩ��
	  float **w_R;          //�����ز�ڵ㵽�����ڵ����ӵ�Ȩ��
	  float *b_L;           //���ز�ڵ����ֵ
	  float *b_R;           //�����ڵ����ֵ
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