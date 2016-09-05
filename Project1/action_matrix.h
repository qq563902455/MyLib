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
#include <iostream>
#include "config_default.h"
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
	size_t _size;
	double **data;

	int *refcount;					// ���ü�����ǳ���Ƶ�ʱ������1������������������1�������Զ�������Ҫ��д

	void release();
	int refAdd(int * addr, int delta);

	void create(uint32_t len1, uint32_t len2);
public:
	action_matrix();
	action_matrix(uint32_t len1, uint32_t len2);
	action_matrix(uint32_t len1, uint32_t len2, uint8_t kind);
	// ��������
	action_matrix(const action_matrix &m);
	~action_matrix();

	void delete_data(void);
	uint32_t get_row() const;
	uint32_t get_column() const;

	//////////////////////////////////////////////////////////////
	// ���� ���Ը�Ϊ���������
	double get_data(uint32_t x, uint32_t y) const;
	void set_data(uint32_t x, uint32_t y, double val) const;
	void PrintfItself(void);
	// ����
	/////////////////////////////////////////////////////////////

	// Ϊʲô���������أ����ñȸ����ٶȿ�
	action_matrix& operator = (const action_matrix& y);
};

inline uint32_t action_matrix::get_row() const { return row; }
inline uint32_t action_matrix::get_column() const { return column; }

std::ostream &operator<<(std::ostream & os, action_matrix &item);


/* Exported overload ------------------------------------------------------- */
action_matrix operator + (action_matrix& x, action_matrix& y);
action_matrix operator - (action_matrix& x, action_matrix& y);
action_matrix operator * (action_matrix& x, action_matrix& y);

action_matrix operator * (action_matrix& x, double y);
action_matrix operator * (double x, action_matrix& y);
action_matrix operator + (action_matrix& x, double y);
action_matrix operator + (double x, action_matrix& y);
action_matrix operator - (action_matrix& x, double y);
action_matrix operator - (double x, action_matrix& y);

action_matrix operator / (action_matrix& x, double y);

action_matrix operator ! (action_matrix x);
action_matrix operator ~ (action_matrix x);
double operator * (action_matrix x);
#endif

/******************* (C) COPYRIGHT 2015 ACTION *****END OF FILE****/
