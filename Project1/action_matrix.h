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
	uint32_t  row;
	uint32_t  column;
	double **data;
	uint8_t erase_flag;
public:
	action_matrix(uint32_t len1, uint32_t len2);
	action_matrix(uint32_t len1, uint32_t len2, uint8_t kind);

	void delete_data(void);
	uint32_t get_row() const;
	uint32_t get_column() const;
	double get_data(uint32_t x, uint32_t y) const;
	void set_data(uint32_t x, uint32_t y, double val) const;
	void set_erase(void);
	uint8_t get_erase(void) const;
	void PrintfItself(void);
	void operator = (action_matrix y);
};
/* Exported overload ------------------------------------------------------- */
action_matrix operator + (action_matrix x, action_matrix y);
action_matrix operator - (action_matrix x, action_matrix y);
action_matrix operator * (action_matrix x, action_matrix y);

action_matrix operator * (action_matrix x, double y);
action_matrix operator * (double x, action_matrix y);
action_matrix operator + (action_matrix x, double y);
action_matrix operator + (double x, action_matrix y);
action_matrix operator - (action_matrix x, double y);
action_matrix operator - (double x, action_matrix y);

action_matrix operator / (action_matrix x, double y);

action_matrix operator ! (action_matrix x);
action_matrix operator ~ (action_matrix x);
double operator * (action_matrix x);
#endif

/******************* (C) COPYRIGHT 2015 ACTION *****END OF FILE****/
