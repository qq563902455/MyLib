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
using namespace std;
/* Exported enum -------------------------------------------------------------*/
enum  matrixUseErr {
	ERR_EQUAL = 0,
	ERR_PLUS,
	ERR_SUB,
	ERR_MUL,
	ERR_INVERSE,
	ERR_TRACE
};
enum {
	MATRIX_I = 0,
	MATRIX_ZERO,
	MATRIX_ALL_I,
	MATRIX_VOID
};
typedef struct arm_matrix_instance_f32
{
	uint32_t numRows;     /**< number of rows of the matrix.     */
	uint32_t numCols;     /**< number of columns of the matrix.  */
	double	*pData;     /**< points to the data of the matrix. */
}arm_matrix_instance_f32;
/* Exported class ------------------------------------------------------------*/
class action_matrix
{
private:
	arm_matrix_instance_f32 matrix;
	int*	referCount;
public:
	explicit action_matrix(uint32_t len1 = 0u, uint32_t len2 = 0u, uint8_t kind = MATRIX_VOID);
	action_matrix(const action_matrix &m);
	~action_matrix();
	uint32_t get_row() const;
	uint32_t get_column() const;
	double get_data(uint32_t x, uint32_t y) const;
	void set_data(uint32_t x, uint32_t y, double val) const;
	void operator = (action_matrix& y);
	void operator = (double y);
	double* operator [] (size_t i) const;

	friend  action_matrix operator + ( action_matrix& x,  action_matrix& y);
	friend	action_matrix operator - ( action_matrix& x,  action_matrix& y);
	friend	action_matrix operator * ( action_matrix& x,  action_matrix& y);

	friend	action_matrix operator * ( action_matrix& x, double y);
	friend	action_matrix operator * (double x,  action_matrix& y);
	friend	action_matrix operator + ( action_matrix& x, double y);
	friend	action_matrix operator + (double x,  action_matrix& y);
	friend	action_matrix operator - ( action_matrix& x, double y);
	friend	action_matrix operator - (double x,  action_matrix& y);

	friend	action_matrix operator / ( action_matrix& x, double y);

	friend	action_matrix operator ! ( action_matrix &x);
	friend	action_matrix operator ~ ( action_matrix &x);
	friend	double operator * (const action_matrix &x);
	friend	double tr(const action_matrix& x);
	friend	double abs(const action_matrix& x);
	friend void rowTrans(action_matrix& x, uint32_t rowid, double scale);
	friend void rowTrans(action_matrix& x, uint32_t rowid, uint32_t rowid2);
	friend void rowTrans(action_matrix& x, uint32_t rowid, uint32_t rowid2, double scale);
	friend void columnTrans(action_matrix& x, uint32_t columnid, double scale);
	friend void columnTrans(action_matrix& x, uint32_t columnid, uint32_t columnid2);
	friend void columnTrans(action_matrix& x, uint32_t columnid, uint32_t columnid2, double scale);
};

inline uint32_t action_matrix::get_row() const { return matrix.numRows; }
inline uint32_t action_matrix::get_column() const { return matrix.numCols; }
template<typename stream>
stream &operator<<(stream& os, action_matrix& item)
{
	os << '[';
	for (uint32_t i = 0; i < item.get_row(); ++i) {
		for (uint32_t j = 0; j < item.get_column(); ++j) {
			os << item.get_data(i, j);
			if (item.get_column() != j + 1)
				os << ',';
		}
		if (item.get_row() != i + 1)
			os << ';' << endl << ' ';
		else
			os << ']' << endl;
	}
	return os;
}
#endif

/******************* (C) COPYRIGHT 2015 ACTION *****END OF FILE****/
