//
// Created by Timothy Herchen on 2021-04-11.
//

#ifndef CANT_MATRIX_HPP
#define CANT_MATRIX_HPP

/**
 * Matrix class, used with a value type (probably float, double, complex or int)
 * @tparam ValueType
 * @tparam width
 * @tparam height
 */
template <int width=2, int height=2, typename ValueType>
class Matrix {
	public:
	Matrix() = default;
	~Matrix();

	private:
	ValueType* _matrix_data;

};

template <int width, int height, typename ValueType>
Matrix::Matrix() {
	_matrix_data = new ValueType[width * height];
}

#endif //CANT_MATRIX_HPP
