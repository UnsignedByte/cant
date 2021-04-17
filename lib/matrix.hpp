//
// Created by Timothy Herchen on 2021-04-11.
//

#ifndef CANT_MATRIX_HPP
#define CANT_MATRIX_HPP

#include <string>
#include <initializer_list>
#include <sstream>
#include <functional>

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

	template <std::size_t Count = rows * cols>
	explicit Matrix(const ValueType(&values)[Count]);

	Matrix(const Matrix&); // copy constructor

	// move constructor
	Matrix(Matrix&& matrix) noexcept : _matrix_data(std::move(matrix._matrix_data)) {
		matrix._matrix_data = nullptr;
	};

	/*class MatrixRow {
		public:
		ValueType operator[](std::size_t i);

		private:
		ValueType* ptr;
	};

	MatrixRow operator[](std::size_t indx);
	MatrixRow get_row(std::size_t indx);*/

	// copy assignment operator
	Matrix& operator=(const Matrix&);
	~Matrix();

	[[nodiscard]] bool in_bounds(int, int) const;

	ValueType value_at(int row, int col) const;
	inline ValueType unsafe_value_at(int row, int col) const;

	ValueType sum() const;

	static constexpr int element_count = rows * cols;
	static constexpr bool is_square = rows == cols;
	static constexpr int width = cols;
	static constexpr int height = rows;

	friend std::ostream& operator<< (std::ostream& out, const Matrix& m);

	template <std::size_t Count = rows * cols>
	void set_values(const ValueType(&values)[Count]);

	std::string to_string() const;

	ValueType* begin() noexcept;
	ValueType* end() noexcept;

	private:
	ValueType* _matrix_data;
};

void throw_domain_error(int row, int col, int rows, int cols) {
	std::stringstream os;
	os << "Out-of-bounds access at " << row << ", " << col << " on matrix of size " << rows << " x " << cols;

	throw std::domain_error(os.str());
}

template <int rows, int cols, class ValueType>
bool Matrix<rows, cols, ValueType>::in_bounds(int row, int col) const {
	return 0 <= row && 0 <= col && col < cols && row < rows;
}

template <int rows, int cols, class ValueType>
std::ostream& operator<< (std::ostream& out, const Matrix<rows, cols, ValueType>& m) {
	ValueType* t = m._matrix_data;

	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col, ++t) {
			out << *t << ' ';
		}
		out << '\n';
	}

	return out;
}

template <int rows, int cols, class ValueType>
ValueType Matrix<rows, cols, ValueType>::value_at(int row, int col) const {
	if (in_bounds(row, col)) return unsafe_value_at(row, col);
	throw_domain_error(row, col, rows, cols);
}

template <int rows, int cols, class ValueType>
inline ValueType Matrix<rows, cols, ValueType>::unsafe_value_at(int row, int col) const {
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

template<int rows, int cols, typename ValueType>
template<std::size_t Count>
void Matrix<rows, cols, ValueType>::set_values(const ValueType (&values)[Count]) {
	std::copy_n(values, Count, _matrix_data);
}

template<int rows, int cols, typename ValueType>
template<std::size_t Count>
Matrix<rows, cols, ValueType>::Matrix(const ValueType (&values)[Count]) {
	_matrix_data = new ValueType[Count];

	std::copy_n(values, Count, _matrix_data);
}

template<int rows, int cols, typename ValueType>
Matrix<rows, cols, ValueType>::Matrix(const Matrix<rows, cols, ValueType> & matrix) {
	_matrix_data = new ValueType[rows * cols];

	std::copy_n(matrix._matrix_data, rows * cols, _matrix_data);
}

template<int rows, int cols, typename ValueType>
Matrix<rows, cols, ValueType>& Matrix<rows, cols, ValueType>::operator= (const Matrix & matrix) {
	auto new_data = new ValueType[rows * cols];
	std::copy_n(matrix._matrix_data, rows * cols, new_data);

	delete[] _matrix_data;

	_matrix_data = new_data;
}

template<int rows, int cols, typename ValueType>
ValueType *Matrix<rows, cols, ValueType>::end() noexcept {
	return &_matrix_data[element_count - 1];
}

template<int rows, int cols, typename ValueType>
ValueType *Matrix<rows, cols, ValueType>::begin() noexcept {
	return _matrix_data;
}

template<int rows, int cols, typename ValueType>
std::string Matrix<rows, cols, ValueType>::to_string() const {
	std::ostringstream ss;
	return (ss << *this).str();
}

template<int rows, int cols, typename ValueType>
Matrix<rows, cols, ValueType> Matrix<rows, cols, ValueType>::to_string() const {
	std::ostringstream ss;
	return (ss << *this).str();
}


#endif //CANT_MATRIX_HPP
