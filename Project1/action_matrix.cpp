/**
******************************************************************************
* @file    action_matrix.cpp
* @author  lxy zlq
* @version V1.0
* @date    2016.9.6
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
* @brief        为矩阵分配内存
* @attention    分配内存分为初次分配和非初次分配，非初次分配内存也就是重新分配内存
*               这需要检查是否要释放掉之前已经分配好的内存
* @param  len1: 矩阵的行数
* @param  len2: 矩阵的列数
* @retval None
*/
void action_matrix::create(uint32_t len1, uint32_t len2)
{
	row = len1;
	column = len2;
	refcount = 0;
	data = 0;

	delete_data();

	// 分配内存
	data = new double *[len1];
	for (uint32_t i = 0; i < len1; i++){
		data[i] = new double[len2];
	}
	
	refcount = new int(1);
}
/**
* @brief        矩阵构造函数
* @param  len1: 矩阵的行数
* @param  len2: 矩阵的列数
* @retval None
*/
action_matrix::action_matrix(uint32_t len1, uint32_t len2)
{
	create(len1, len2);
}
/**
* @brief        矩阵构造函数
* @param  len1: 矩阵的行数
* @param  len2: 矩阵的列数
* @param  kind: MATRIX_I       单位矩阵
	            MATRIX_ZERO    全0矩阵
	            MATRIX_ALL_I   全1矩阵
* @retval None
*/
action_matrix::action_matrix(uint32_t len1, uint32_t len2, uint8_t kind)
{
	create(len1, len2);
	switch (kind)
	{
	case MATRIX_I:
		for (uint32_t i = 0; i < len1; i++)
			for (uint32_t j = 0; j < len2; j++)
			{
				if (i != j)
					data[i][j] = 0;
				else
					data[i][j] = 1;
			}
		break;
	case MATRIX_ZERO:
		for (uint32_t i = 0; i < len1; i++)
			for (uint32_t j = 0; j < len2; j++)
			{
				data[i][j] = 0;
			}
		break;
	case MATRIX_ALL_I:
		for (uint32_t i = 0; i < len1; i++)
			for (uint32_t j = 0; j < len2; j++)
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
* @brief  复制函数
* @attention 这是一个浅复制
*/
action_matrix::action_matrix(const action_matrix &m)
	:data(m.data), row(m.row), column(m.column), refcount(m.refcount)
{
	if (refcount)
		refAdd(refcount, 1);
}

/**
* @brief  释放矩阵里的动态空间
* @attention 1,这个函数应该只有在析构函数和内存分配函数中使用，不应该在其它地方使用,
*            因为既然是通过引用计数实现的GC，那么应该通过C++自身的机构，作用域和析构来实现释放资源，
*            再就是重新分配对旧资源的处理。
             2,如果矩阵本身就是通过动态分配内存得到，则需要手动调用该方法来释放空间
* @param  none
* @retval None
*/
void action_matrix::delete_data(void)
{
	if (refcount && refAdd(refcount, -1) == 1) {
		for (uint32_t i = 0; i < row; i++)
		{
			delete[] data[i];
		}
		delete[] data;
		data = 0;

		delete refcount;
		refcount = 0;
	}
}


/**
* @berif 引用增加
* @param addr:   指向引用计数的指针
* @param delta:  引用计数减掉的值
* @retval temp:  减数前的引用计数的值
*/
int action_matrix::refAdd(int * addr, int delta)
{
	int temp = *addr;
	*addr += delta;
	return temp;
}
/**
* @berif 析构函数，当引用计数为0时，释放资源
*/
action_matrix::~action_matrix()
{
	delete_data();
}
/**
* @brief  获得矩阵的某一行某一列的值
* @attention 越界检查
* @param  x: 行数
* @param  y: 列数
* @retval data[x][y]
*/
double action_matrix::get_data(uint32_t x, uint32_t y) const
{
	if (x < row && y < column)
		return data[x][y];
	else
		return 0.0;
}
/**
* @brief  改变矩阵某一行某一列的值
* @attention 越界检查
* @param  x: 行数
* @param  y: 列数
* @retval None
*/
void action_matrix::set_data(uint32_t x, uint32_t y, double val) const
{
	if (x < row && y < column)
		data[x][y] = val;
}

/**
* @brief  矩阵取值
* @retval none
*/
double* const action_matrix::operator [] (size_t i)
{
	return data[i];
}
const double * action_matrix::operator[](size_t i) const
{
	return data[i];
}
/**
 * @brief  矩阵赋值
 * @attention 这是一个深度复制
 * @param  this[hiden] : x=y运算中的x
 * @param  y: x=y中的y
 * @retval none
 */
action_matrix& action_matrix::operator = (const action_matrix& y)
{
	for (uint32_t i = 0; i <row; i++)
	{
		for (uint32_t j = 0; j < column; j++)
		{
			data[i][j] = y.get_data(i, j);
		}
	}
	return *this;
}
/**
* @brief  矩阵加法
* @attention 如果相乘之后不赋值也不会内存泄漏
* @param  none
* @retval none
*/
action_matrix operator + (action_matrix& x, action_matrix& y)
{
	if (x.get_column() != y.get_column() || x.get_row() != y.get_row())
	{
		throw ERR_PLUS;
	}
	else
	{
		action_matrix result(x.get_row(), x.get_column());
		for (uint32_t i = 0; i < x.get_row(); i++)
		{
			for (uint32_t j = 0; j < x.get_column(); j++)
			{
				result.set_data(i, j, x.get_data(i, j) + y.get_data(i, j));
			}
		}
		return result;
	}
}
action_matrix operator+(action_matrix& x, double y)
{

	if (x.get_row() != x.get_column())
	{
		throw ERR_PLUS;
	}
	else
	{
		action_matrix result(x.get_row(), x.get_column());
		for (uint32_t i = 0; i < x.get_row(); i++)
		{
			for (uint32_t j = 0; j < x.get_column(); j++)
			{
				if(i==j)
				  result.set_data(i, j, x.get_data(i, j) + y);
				else
					result.set_data(i, j, x.get_data(i, j));
			}
		}

		return result;
	}
}
action_matrix operator+(double x, action_matrix& y)
{
	if (y.get_row() != y.get_column())
	{
		throw ERR_PLUS;
	}
	else
	{
		action_matrix result(y.get_row(), y.get_column());
		for (uint32_t i = 0; i < y.get_row(); i++)
		{
			for (uint32_t j = 0; j < y.get_column(); j++)
			{
				if (i == j)
					result.set_data(i, j, y.get_data(i, j) + x);
				else
					result.set_data(i, j, y.get_data(i, j));
			}
		}
		return result;
	}
}


/**
* @brief  矩阵减法
* @param  none
* @retval none
*/
action_matrix operator - (action_matrix& x, action_matrix& y)
{
	if (x.get_column() != y.get_column() || x.get_row() != y.get_row())
	{
		throw ERR_SUB;
	}
	else
	{
		action_matrix result(x.get_row(), x.get_column());
		for (uint32_t i = 0; i < x.get_row(); i++)
		{
			for (uint32_t j = 0; j < x.get_column(); j++)
			{
				result.set_data(i, j, x.get_data(i, j) - y.get_data(i, j));
			}
		}
		return result;
	}
}
action_matrix operator - (action_matrix& x, double y)
{
	return x + (-y);
}
action_matrix operator - (double x, action_matrix& y)
{
	if (y.get_row() != y.get_column())
	{
		throw ERR_PLUS;
	}
	else
	{
		action_matrix result(y.get_row(), y.get_column());
		for (uint32_t i = 0; i < y.get_row(); i++)
		{
			for (uint32_t j = 0; j < y.get_column(); j++)
			{
				if (i == j)
					result.set_data(i, j, x - y.get_data(i, j));
				else
					result.set_data(i, j, y.get_data(i, j));
			}
		}
		return result;
	}
}


/**
* @brief  矩阵乘法
* @param  none
* @retval none
*/
action_matrix operator * (action_matrix& x, action_matrix& y)
{
	if (x.get_column() != y.get_row())
	{
		throw ERR_MUL;
	}
	else
	{
		action_matrix result(x.get_row(), y.get_column());
		double temp = 0;
		for (uint32_t i = 0; i < x.get_row(); i++) {
			for (uint32_t j = 0; j < y.get_column(); j++)
			{
				temp = 0;
				for (uint32_t w = 0; w < x.get_column(); w++)
				{
					temp = temp + x.get_data(i, w)*y.get_data(w, j);
				}
				result.set_data(i, j, temp);
			}
		}
		return result;
	}
}
action_matrix operator * (action_matrix& x, double y)
{

	action_matrix result(x.get_row(), x.get_column());
	for (uint32_t i = 0; i < x.get_row(); i++) {
		for (uint32_t j = 0; j < x.get_column(); j++){
			result.set_data(i, j, x.get_data(i, j)*y);
		}
	}
	return result;
}
action_matrix operator * (double x, action_matrix& y)
{
	action_matrix result(y.get_row(), y.get_column());
	for (uint32_t i = 0; i < y.get_row(); i++) {
		for (uint32_t j = 0; j < y.get_column(); j++) {
			result.set_data(i, j, y.get_data(i, j)*x);
		}
	}
	return result;
}

action_matrix operator / (action_matrix& x, double y)
{
	action_matrix result(x.get_row(), x.get_column());
	for (uint32_t i = 0; i < x.get_row(); i++) {
		for (uint32_t j = 0; j < x.get_column(); j++) {
			result.set_data(i, j, x.get_data(i, j) / y);
		}
	}
	return result;
}
/**
* @brief  矩阵转置
* @param  none
* @retval none
*/
action_matrix operator !(action_matrix &x)
{
	action_matrix result(x.get_column(), x.get_row());
	for (uint32_t i = 0; i < x.get_row(); i++)
	{
		for (uint32_t j = 0; j < x.get_column(); j++)
		{
			result.set_data(j, i, x.get_data(i, j));
		}
	}
	return result;
}
/**
* @brief  矩阵求逆
* @attention 使用了delete_data，应该需要修改，太复杂了，不是我写的不好改
* @param  none
* @retval none
*/
action_matrix operator ~(action_matrix &x) //矩阵求逆
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
	double temp_val = 0;
	double temp_val2 = 0;
	for (uint32_t i = 0; i < x.get_row(); i++)
	{
		for (uint32_t j = i; j < x.get_row(); j++)
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
	for (uint32_t i = 0; i < x.get_row(); i++)
	{
		for (uint32_t j = 0; j < x.get_row(); j++)
		{
			matrix_L_Inverse.set_data(i, j, 0);
			matrix_U_Inverse.set_data(i, j, 0);
		}
	}

	/* LU分解 */
	for (uint32_t i = 0; i < x.get_row(); i++)
	{
		matrix_U.set_data(0, i, x.get_data(0, i));
		matrix_L.set_data(i, 0, x.get_data(i, 0) / matrix_U.get_data(0, 0));
	}
	for (uint32_t i = 1; i < x.get_row(); i++)
	{
		for (uint32_t j = i; j < x.get_row(); j++)
		{
			temp_val = 0;
			for (uint32_t w = 0; w <= i - 1; w++)
			{
				temp_val = temp_val + matrix_L.get_data(i, w)*matrix_U.get_data(w, j);
			}
			matrix_U.set_data(i, j, x.get_data(i, j) - temp_val);
		}
		for (uint32_t j = i + 1; j < x.get_row(); j++)
		{
			temp_val = 0;
			for (uint32_t w = 0; w <= i - 1; w++)
			{
				temp_val = temp_val + matrix_L.get_data(j, w)*matrix_U.get_data(w, i);
			}
			matrix_L.set_data(j, i, (x.get_data(j, i) - temp_val) / matrix_U.get_data(i, i));
		}
	}
	matrix_L = !matrix_L;
	temp_val = 0;
	temp_val2 = 0;
	for (uint32_t j = x.get_column() - 1; (j+2) >= (2); j--)
	{
		for (uint32_t i = x.get_column() - 1; (i+2) >= (2); i--)
		{
			temp_val = 0;
			temp_val2 = 0;
			for (uint32_t w = x.get_column() - 1; (w+2) > (i+2); w--)
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

	//matrix_U_Inverse.delete_data();
	//matrix_L_Inverse.delete_data();
	//matrix_U.delete_data();
	//matrix_L.delete_data();

	return result;
}
/**
* @brief  矩阵求行列式
* @attention 使用了delete_data，应该需要修改，这个..
* @param  none
* @retval none
*/
double operator *(action_matrix &x)
{
	double out=0;
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
			delete data;
		}
	}
	else
		out = x.get_data(0, 0)*x.get_data(1, 1) - x.get_data(0, 1)*x.get_data(1, 0);
	return out;
}

/**
* @brief  重载<<输出运算符，矩阵打印自身
* @param  none
* @retval none
*/
std::ostream &operator<<(std::ostream & os, action_matrix &item)
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


/************************ (C) COPYRIGHT 2016 ACTION *****END OF FILE****/
