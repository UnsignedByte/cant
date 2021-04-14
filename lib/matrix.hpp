//
// Created by Timothy Herchen on 2021-04-11.
//

#ifndef CANT_MATRIX_HPP
#define CANT_MATRIX_HPP

#include <string>

// The matrix is just stored as an array of length width * height.

/**
 * Matrix class, used with a value type (probably float, double, complex or int)
 * @tparam ValueType
 * @tparam width
 * @tparam height
 */
template <int rows=2, int cols=2, typename ValueType=double>
class Matrix {
	public:
	Matrix();
	~Matrix();

	bool in_bounds(int, int) const;

	ValueType value_at(int row, int col) const;
	ValueType unsafe_value_at(int row, int col) const;

	ValueType sum() const;

	static constexpr int element_count = rows * cols;
	static constexpr bool is_square = rows == cols;
	static constexpr int width = cols;
	static constexpr int height = rows;

	/*template <typename... Values,
					typename = typename std::enable_if<sizeof...(Args) == width * height>::type>
	void set_values();*/

	private:
	ValueType* _matrix_data;

};

void throw_domain_error(int row, int col, int rows, int cols) {
	throw std::domain_error(std::format("Out-of-bounds access at {}, {} on matrix of size {} x {}.", row, col, rows, cols));
}

template <int rows, int cols, class ValueType>
bool Matrix<rows, cols, ValueType>::in_bounds(int row, int col) const {
	return 0 <= row && 0 <= col && col < cols && row < rows;
}

/*template <int rows, int cols, class ValueType>
friend std::ostringstream operator<< (std::ostringstream& out, Matrix& m) {
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < width; ++col) {
			out << get_value_at(row, col)
		}
	}
}*/

template <int rows, int cols, class ValueType>
ValueType Matrix<rows, cols, ValueType>::value_at(int row, int col) const {
	if (in_bounds(row, col)) return unsafe_value_at(row, col);
	throw_domain_error(row, col, rows, cols);
}

template <int rows, int cols, class ValueType>
ValueType Matrix<rows, cols, ValueType>::unsafe_value_at(int row, int col) const {
	return _matrix_data[cols * row + col];
}

template <int rows, int cols, class ValueType>
Matrix<rows, cols, ValueType>::Matrix() {
	static_assert(rows > 0 && cols > 0, "The matrix's dimensions must be positive.");

	_matrix_data = new ValueType[rows * cols];
}

template <int rows, int cols, class ValueType>
ValueType Matrix<rows, cols, ValueType>::sum() const {
	ValueType sum;

	for (int i = 0; i < element_count; ++i) {
		sum += _matrix_data[i];
	}

	return sum;
}

template <int rows, int cols, typename ValueType>
Matrix<rows, cols, ValueType>::~Matrix() {
	delete[] _matrix_data;
}

#endif //CANT_MATRIX_HPP
