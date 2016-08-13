/**
******************************************************************************
* @file    *.h
* @author  Lxy Action
* @version
* @date
* @brief   This file contains the headers of C++ FILE
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
#ifndef __ACTION_MATRIX_H
#define __ACTION_MATRIX_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported enum -------------------------------------------------------------*/
enum {
	ERR_EQUAL = 0,
	ERR_PLUS,
	ERR_SUB,
	ERR_MUL,
	ERR_INVERSE
};
enum {
	MATRIX_I = 0,
	MATRIX_ZERO,
	MATRIX_ALL_I
};
/* Exported class ------------------------------------------------------------*/
class action_matrix
{
private:
	int16_t  row;
	int16_t  column;
	float **data;
	uint8_t erase_flag;
public:
	action_matrix(int16_t len1, int16_t len2);
	action_matrix(int16_t len1, int16_t len2, uint8_t kind);

	void delete_data(void);
	int16_t get_row() const;
	int16_t get_column() const;
	float get_data(int16_t x, int16_t y) const;
	void set_data(int16_t x, int16_t y, float val) const;
	void set_erase(void);
	uint8_t get_erase(void) const;
	void PrintfItself(void);
	void operator = (action_matrix y);
};
/* Exported overload ------------------------------------------------------- */
action_matrix operator + (action_matrix x, action_matrix y);
action_matrix operator - (action_matrix x, action_matrix y);
action_matrix operator * (action_matrix x, action_matrix y);

action_matrix operator * (action_matrix x, float y);
action_matrix operator * (float x, action_matrix y);
action_matrix operator + (action_matrix x, float y);
action_matrix operator + (float x, action_matrix y);
action_matrix operator - (action_matrix x, float y);
action_matrix operator - (float x, action_matrix y);

action_matrix operator / (action_matrix x, float y);

action_matrix operator ! (action_matrix x);
action_matrix operator ~ (action_matrix x);
float operator * (action_matrix x);
#endif

/******************* (C) COPYRIGHT 2015 ACTION *****END OF FILE****/
