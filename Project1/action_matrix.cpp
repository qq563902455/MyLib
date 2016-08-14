/**
******************************************************************************
* @file    action_matrix.cpp
* @author  lxy
* @version V1.0
* @date    2016.4.16
* @brief   this is a tool for calculation of matrix
******************************************************************************
* @attention
*
*
*
*
******************************************************************************
*/
/* Includes -------------------------------------------------------------------*/
#include <cmath>
#include <cstdint>
#include "action_matrix.h"
#include <iostream>

using namespace std;
//#include "usart.h"
/* Private  typedef -----------------------------------------------------------*/
/* Private  define ------------------------------------------------------------*/
/* Private  macro -------------------------------------------------------------*/
/* Private  variables ---------------------------------------------------------*/
/* Extern   variables ---------------------------------------------------------*/
/* Extern   function prototypes -----------------------------------------------*/
/* Private  function prototypes -----------------------------------------------*/
/* Private  functions ---------------------------------------------------------*/
/* Exported function prototypes -----------------------------------------------*/
/* Exported class functions ---------------------------------------------------*/
/**
* @brief        矩阵构造函数
* @param  len1: 矩阵的行数
* @param  len2: 矩阵的列数
* @retval None
*/
action_matrix::action_matrix(int16_t len1, int16_t len2)
{
	data = new float *[len1];
	for (int i = 0; i < len1; i++)
	{
		data[i] = new float[len2];
	}
	row = len1;
	column = len2;
	erase_flag = 0;
}
action_matrix::action_matrix(int16_t len1, int16_t len2, uint8_t kind)
{
	data = new float *[len1];
	for (int i = 0; i < len1; i++)
	{
		data[i] = new float[len2];
	}
	row = len1;
	column = len2;
	erase_flag = 0;

	switch (kind)
	{
	case MATRIX_I:
		for (uint8_t i = 0; i < len1; i++)
			for (uint8_t j = 0; j < len2; j++)
			{
				if (i != j)
					data[i][j] = 0;
				else
					data[i][j] = 1;
			}
		break;
	case MATRIX_ZERO:
		for (uint8_t i = 0; i < len1; i++)
			for (uint8_t j = 0; j < len2; j++)
			{
				data[i][j] = 0;
			}
		break;
	case MATRIX_ALL_I:
		for (uint8_t i = 0; i < len1; i++)
			for (uint8_t j = 0; j < len2; j++)
			{
				data[i][j] = 1;
			}
		break;
	default:
		while (1);
		//break;
	}
}
/**
* @brief  释放矩阵里的动态空间
* @param  none
* @retval None
*/
void action_matrix::delete_data(void)
{
	for (int i = 0; i < row; i++)
	{
		delete[] data[i];
	}
	delete[] data;
}
void action_matrix::PrintfItself(void)
{
	for (int i = 0; i<row; i++)
	{
		//USART_OUT(USART3, "\r\n");
		cout << endl;
		for (int j = 0; j<column; j++)
		{
			//USART_OUT(USART3, "%d ", (int)(data[i][j] * 1000));
			cout << data[i][j]<<' ';
		}
	}
	cout << endl;
	//USART_OUT(USART3, "\r\n");
}
/**
* @brief  获得矩阵的行数
* @param  none
* @retval row
*/
int16_t action_matrix::get_row() const
{
	return row;
}
/**
* @brief  获得矩阵的列数
* @param  none
* @retval column
*/
int16_t action_matrix::get_column() const
{
	return column;
}
/**
* @brief  获得矩阵的某一行某一列的值
* @param  x: 行数
* @param  y: 列数
* @retval data[x][y]
*/
float action_matrix::get_data(int16_t x, int16_t y) const
{
	return data[x][y];
}
/**
* @brief  改变矩阵某一行某一列的值
* @param  x: 行数
* @param  y: 列数
* @retval None
*/
void action_matrix::set_data(int16_t x, int16_t y, float val) const
{
	data[x][y] = val;
}
void action_matrix::set_erase(void)
{
	erase_flag = 1;
}
uint8_t action_matrix::get_erase(void) const
{
	return erase_flag;
}
/**
* @brief  矩阵赋值
* @param  this[hiden] : x=y运算中的x
* @param  y: x=y中的y
* @retval none
*/
void action_matrix::operator = (action_matrix y)
{
	if (this->row != y.get_row() || this->column != y.get_column())
	{
		throw ERR_EQUAL;
	}
	this->row = y.get_row();
	this->column = y.get_column();
	for (int i = 0; i < this->row; i++)
	{
		for (int j = 0; j < this->column; j++)
		{
			this->data[i][j] = y.get_data(i, j);
		}
	}
	if (y.get_erase())
		y.delete_data();
	if (this->erase_flag)
		this->delete_data();
}
/**
* @brief  矩阵加法
* @param  none
* @retval none
*/
action_matrix operator + (action_matrix x, action_matrix y)
{
	if (x.get_column() != y.get_column() || x.get_row() != y.get_row())
	{
		throw ERR_PLUS;
	}
	else
	{
		action_matrix result(x.get_row(), x.get_column());
		for (int i = 0; i < x.get_row(); i++)
		{
			for (int j = 0; j < x.get_column(); j++)
			{
				result.set_data(i, j, x.get_data(i, j) + y.get_data(i, j));
			}
		}
		if (y.get_erase())
			y.delete_data();
		if (x.get_erase())
			x.delete_data();
		result.set_erase();
		return result;
	}
}
action_matrix operator+(action_matrix x, float y)
{

	if (x.get_row() != x.get_column())
	{
		throw ERR_PLUS;
	}
	else
	{
		action_matrix result(x.get_row(), x.get_column());
		for (int i = 0; i < x.get_row(); i++)
		{
			for (int j = 0; j < x.get_column(); j++)
			{
				if(i==j)
				  result.set_data(i, j, x.get_data(i, j) + y);
				else
					result.set_data(i, j, x.get_data(i, j));
			}
		}
		if (x.get_erase())
			x.delete_data();
		result.set_erase();
		return result;
	}
}
action_matrix operator+(float x, action_matrix y)
{
	if (y.get_row() != y.get_column())
	{
		throw ERR_PLUS;
	}
	else
	{
		action_matrix result(y.get_row(), y.get_column());
		for (int i = 0; i < y.get_row(); i++)
		{
			for (int j = 0; j < y.get_column(); j++)
			{
				if (i == j)
					result.set_data(i, j, y.get_data(i, j) + x);
				else
					result.set_data(i, j, y.get_data(i, j));
			}
		}
		if (y.get_erase())
			y.delete_data();
		result.set_erase();
		return result;
	}
}
/**
* @brief  矩阵减法
* @param  none
* @retval none
*/
action_matrix operator - (action_matrix x, action_matrix y)
{
	if (x.get_column() != y.get_column() || x.get_row() != y.get_row())
	{
		throw ERR_SUB;
	}
	else
	{
		action_matrix result(x.get_row(), x.get_column());
		for (int i = 0; i < x.get_row(); i++)
		{
			for (int j = 0; j < x.get_column(); j++)
			{
				result.set_data(i, j, x.get_data(i, j) - y.get_data(i, j));
			}
		}
		if (y.get_erase())
			y.delete_data();
		if (x.get_erase())
			x.delete_data();
		result.set_erase();
		return result;
	}
}
action_matrix operator - (action_matrix x, float y)
{
	return x + (-y);
}
action_matrix operator - (float x, action_matrix y)
{
	if (y.get_row() != y.get_column())
	{
		throw ERR_PLUS;
	}
	else
	{
		action_matrix result(y.get_row(), y.get_column());
		for (int i = 0; i < y.get_row(); i++)
		{
			for (int j = 0; j < y.get_column(); j++)
			{
				if (i == j)
					result.set_data(i, j, x - y.get_data(i, j));
				else
					result.set_data(i, j, y.get_data(i, j));
			}
		}
		if (y.get_erase())
			y.delete_data();
		result.set_erase();
		return result;
	}
}
/**
* @brief  矩阵乘法
* @param  none
* @retval none
*/
action_matrix operator * (action_matrix x, action_matrix y)
{
	if (x.get_column() != y.get_row())
	{
		throw ERR_MUL;
	}
	else
	{
		action_matrix result(x.get_row(), y.get_column());
		float temp = 0;
		for (int i = 0; i < x.get_row(); i++)
			for (int j = 0; j < y.get_column(); j++)
			{
				temp = 0;
				for (int w = 0; w < x.get_column(); w++)
				{
					temp = temp + x.get_data(i, w)*y.get_data(w, j);
				}
				result.set_data(i, j, temp);
			}
		if (y.get_erase())
			y.delete_data();
		if (x.get_erase())
			x.delete_data();
		result.set_erase();
		return result;
	}
}
action_matrix operator * (action_matrix x, float y)
{

	action_matrix result(x.get_row(), x.get_column());
	for (int i = 0; i < x.get_row(); i++)
		for (int j = 0; j < x.get_column(); j++)
		{
			result.set_data(i, j, x.get_data(i, j)*y);
		}
	if (x.get_erase())
		x.delete_data();
	result.set_erase();
	return result;
}
action_matrix operator * (float x, action_matrix y)
{

	action_matrix result(y.get_row(), y.get_column());
	for (int i = 0; i < y.get_row(); i++)
		for (int j = 0; j < y.get_column(); j++)
		{
			result.set_data(i, j, y.get_data(i, j)*x);
		}
	if (y.get_erase())
		y.delete_data();
	result.set_erase();
	return result;
}

action_matrix operator / (action_matrix x, float y)
{
	action_matrix result(x.get_row(), x.get_column());
	for (int i = 0; i < x.get_row(); i++)
		for (int j = 0; j < x.get_column(); j++)
		{
			result.set_data(i, j, x.get_data(i, j) / y);
		}
	if (x.get_erase())
		x.delete_data();
	result.set_erase();
	return result;
}
/**
* @brief  矩阵转置
* @param  none
* @retval none
*/
action_matrix operator !(action_matrix x) //矩阵的转置
{
	action_matrix result(x.get_column(), x.get_row());
	for (int i = 0; i < x.get_row(); i++)
	{
		for (int j = 0; j < x.get_column(); j++)
		{
			result.set_data(j, i, x.get_data(i, j));
		}
	}
	if (x.get_erase())
		x.delete_data();
	result.set_erase();
	return result;
}
/**
* @brief  矩阵求逆
* @param  none
* @retval none
*/
action_matrix operator ~(action_matrix x) //矩阵求逆
{
	if (x.get_column() != x.get_row())
	{
		throw ERR_INVERSE;
	}

	action_matrix matrix_L(x.get_column(), x.get_column());
	action_matrix matrix_U(x.get_column(), x.get_column());
	action_matrix matrix_L_Inverse(x.get_column(), x.get_column());
	action_matrix matrix_U_Inverse(x.get_column(), x.get_column());
	action_matrix result(x.get_column(), x.get_column());
	float temp_val = 0;
	float temp_val2 = 0;
	for (int i = 0; i < x.get_row(); i++)
	{
		for (int j = i; j < x.get_row(); j++)
		{
			if (j == i)
			{
				matrix_L.set_data(i, j, 1);
			}
			else
			{
				matrix_L.set_data(i, j, 0);
				matrix_U.set_data(j, i, 0);
			}
		}
	}
	for (int i = 0; i < x.get_row(); i++)
	{
		for (int j = 0; j < x.get_row(); j++)
		{
			matrix_L_Inverse.set_data(i, j, 0);
			matrix_U_Inverse.set_data(i, j, 0);
		}
	}

	/* LU分解 */
	for (int i = 0; i < x.get_row(); i++)
	{
		matrix_U.set_data(0, i, x.get_data(0, i));
		matrix_L.set_data(i, 0, x.get_data(i, 0) / matrix_U.get_data(0, 0));
	}
	for (int i = 1; i < x.get_row(); i++)
	{
		for (int j = i; j < x.get_row(); j++)
		{
			temp_val = 0;
			for (int w = 0; w <= i - 1; w++)
			{
				temp_val = temp_val + matrix_L.get_data(i, w)*matrix_U.get_data(w, j);
			}
			matrix_U.set_data(i, j, x.get_data(i, j) - temp_val);
		}
		for (int j = i + 1; j < x.get_row(); j++)
		{
			temp_val = 0;
			for (int w = 0; w <= i - 1; w++)
			{
				temp_val = temp_val + matrix_L.get_data(j, w)*matrix_U.get_data(w, i);
			}
			matrix_L.set_data(j, i, (x.get_data(j, i) - temp_val) / matrix_U.get_data(i, i));
		}
	}
	matrix_L = !matrix_L;
	temp_val = 0;
	temp_val2 = 0;
	for (int j = x.get_column() - 1; j >= 0; j--)
	{
		for (int i = x.get_column() - 1; i >= 0; i--)
		{
			temp_val = 0;
			temp_val2 = 0;
			for (int w = x.get_column() - 1; w > i; w--)
			{
				temp_val = temp_val + matrix_U.get_data(i, w)*matrix_U_Inverse.get_data(w, j);
				temp_val2 = temp_val2 + matrix_L.get_data(i, w)*matrix_L_Inverse.get_data(w, j);
			}
			if (i == j)
			{
				matrix_U_Inverse.set_data(i, j, (1 - temp_val) / matrix_U.get_data(i, i));
				matrix_L_Inverse.set_data(i, j, (1 - temp_val2) / matrix_L.get_data(i, i));
			}
			else
			{
				matrix_U_Inverse.set_data(i, j, (0 - temp_val) / matrix_U.get_data(i, i));
				matrix_L_Inverse.set_data(i, j, (0 - temp_val2) / matrix_L.get_data(i, i));
			}
		}
	}
	result = matrix_U_Inverse*(!matrix_L_Inverse);

	matrix_U_Inverse.delete_data();
	matrix_L_Inverse.delete_data();
	matrix_U.delete_data();
	matrix_L.delete_data();

	if (x.get_erase())
		x.delete_data();
	result.set_erase();
	return result;
}
float operator *(action_matrix x)
{
	float out=0;
	if (x.get_row()>2)
	{
		action_matrix *data;
		for (uint8_t i = 0; i<x.get_row(); i++)
		{
			data = new action_matrix(x.get_row() - 1, x.get_row() - 1);
			for (uint8_t j = 0; j<data->get_row(); j++)
				for (uint8_t w = 0; w<data->get_row(); w++)
				{
					if (w<i)
						data->set_data(j, w, x.get_data(j + 1, w));
					else
						data->set_data(j, w, x.get_data(j + 1, w + 1));
				}
			if (i % 2 == 0)
				out += (*(*data))*x.get_data(0, i);
			else
				out -= (*(*data))*x.get_data(0, i);
			data->delete_data();
			delete data;
		}
	}
	else
		out = x.get_data(0, 0)*x.get_data(1, 1) - x.get_data(0, 1)*x.get_data(1, 0);
	return out;
}
/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
