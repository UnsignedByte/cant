//
// Created by Timothy Herchen on 2021-04-11.
//

#ifndef CANT_MATRIX_HPP
#define CANT_MATRIX_HPP

#include <string>
#include <initializer_list>
#include <sstream>
#include <functional>
#include <array>
#include <algorithm>
#include <type_traits>

// The matrix is just stored as an array of length width * height.



/*class MatrixRow {
	public:
	ValueType operator[](std::size_t i);

	private:
	ValueType* ptr;
};

MatrixRow operator[](std::size_t indx);
MatrixRow get_row(std::size_t indx);*/

void throw_domain_error(int row, int col, int rows, int cols) {
	std::stringstream os;
	os << "Out-of-bounds access at " << row << ", " << col << " on matrix of size " << rows << " x " << cols;

	throw std::domain_error(os.str());
}

enum class Alignment {
	NO_ALIGNMENT,
	LEFT,
	MIDDLE,
	RIGHT,
	BY_CHARACTER,
};

static std::stringstream print_methods_ss;

// We wish to make sure the ValueType function has a definition satisfying operator<<(std::stringstream ss, const ValueType& v)
// or  std::string to_string()
template <typename ValueType>
struct has_print_methods {
	// Test if ValueType has a .to_string() function that is castable to a std::string
	template <typename T>
	static auto constexpr has_to_string_method(T*) -> typename std::is_convertible<decltype(std::declval<T*>().to_string()), std::string>::type;

	// Defualt case (if the substitution fails, this is will be the function definition.
	template <typename T>
	static auto constexpr has_to_string_method(...) -> std::false_type;

	template <typename T>
	static auto constexpr has_operator_method(T*) -> typename std::is_convertible<decltype(operator<<(std::declval<std::stringstream>(), std::declval<T*>())), std::stringstream>::type;

	template <typename T>
	static constexpr std::false_type has_operator_method(...);

	typedef decltype(has_to_string_method<ValueType>(0)) has_to_string_type;
	typedef decltype(has_operator_method<ValueType>(0)) has_operator_type;

	static bool constexpr has_to_string = has_to_string_type::value;
	static bool constexpr has_stream_operator = has_operator_type::value;

	static std::string get_string (ValueType v) {
		auto& ss = print_methods_ss;

		// Using C++17 features!!
		if constexpr (has_to_string) {
			return v.to_string();
		} else if constexpr (has_stream_operator) {
			ss.str("");
			ss << v;

			return ss.str();
		} else {
			static_assert("Given ValueType does not have a valid operator<< or to_string() function.");
		}
	}
};

// The various ways to print a matrix
template <typename ValueType>
std::string base_matrix_to_string(int rows, int cols, ValueType* _matrix_data, Alignment alignment, int horizontal_padding, char alignment_character='.') {
	std::stringstream ss;
	std::vector<std::string> arr;

	arr.resize(rows * cols);

	int max_len = 0, i = -1;
	ValueType* end = _matrix_data + rows * cols;

	int max_characters_left_of_alignment = 0, max_characters_right_of_alignment = 0;

	for (; _matrix_data != end; ++_matrix_data) {
		using methods = has_print_methods<ValueType>;
		static_assert(methods::has_stream_operator || methods::has_to_string, "Given ValueType does not have a valid operator<< or to_string() function.");

		auto str = methods::get_string(*_matrix_data);
		arr[++i] = str;

		max_len = std::max(static_cast<int>(str.length()), max_len);

		if (alignment == Alignment::BY_CHARACTER) {

		}
	}

	// max_len is the length of the longest string. Assuming alignment is not by character, each "cell" will be max_len +
	// horizontal_padding in size. We then
	//

	if (alignment != Alignment::BY_CHARACTER) {
		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < cols; ++col) {
				std::string s = arr[row * cols + col];

				ss << s << std::string(max_len - s.length() + horizontal_padding, ' ');
			}

			ss << '\n';
		}

		return ss.str();
	} else {

	}
}



// Forward delcarations
template <int, int, typename>
class Matrix;

template <int rows, int cols, typename ValueType>
std::ostream& operator<< (std::ostream& out, const Matrix<rows, cols, ValueType>& m);

/**
 * Matrix class, used with a value type (probably float, double, complex or int)
 * @tparam ValueType
 * @tparam width
 * @tparam height
 */
template <int rows=2, int cols=2, typename ValueType=float>
class Matrix {
	public:
	/** Constructors
	 *  Example:
	 *    Matrix<2, 2, float> mtrx1;
	 *    Matrix<2, 2, float> mtrx2({1, 0, 0, 1});
	 */

	// Default constructor
	Matrix() {
		static_assert(rows > 0 && cols > 0, "The matrix's dimensions must be positive.");

		_matrix_data = new ValueType[rows * cols];
	}

	// Initialize from list of values
	explicit Matrix(const ValueType (&values)[rows * cols]) {
		_matrix_data = new ValueType[rows * cols];

		std::copy_n(values, rows * cols, _matrix_data);
	}

	// copy constructor (called when passed by value, to copy a matrix)
	Matrix(const Matrix& matrix) {
		_matrix_data = new ValueType[rows * cols];

		std::copy_n(matrix._matrix_data, rows * cols, _matrix_data);
	}

	// copy assignment operator
	Matrix& operator= (const Matrix& matrix) {
		auto new_data = new ValueType[rows * cols];
		std::copy_n(matrix._matrix_data, rows * cols, new_data);

		delete[] _matrix_data;

		_matrix_data = new_data;
	}

	~Matrix() {
		delete[] _matrix_data;
	}

	// move constructor (called when passing as an intermediate value)
	Matrix(Matrix&& matrix) noexcept : _matrix_data(std::move(matrix._matrix_data)) {
		matrix._matrix_data = nullptr;
	};

	// Static variables for each matrix of size <rows, cols>
	static constexpr int element_count = rows * cols;
	static constexpr bool is_square = rows == cols;

	/** Member functions: Value access */

	// Return the value at [row][col] with bounds checking
	ValueType value_at(int row, int col) const {
		if (in_bounds(row, col)) return unsafe_value_at(row, col);
		throw_domain_error(row, col, rows, cols);
	}

	// Return the value at [row][col] without bounds checking
	inline ValueType unsafe_value_at(int row, int col) const {
		return _matrix_data[cols * row + col];
	}

	// Set the matrix's values. Ex: Matrix<2, 2> m; m.set_values({2, 4, 1, 2});
	void set_values(const ValueType (&values)[rows * cols]) {
		std::copy_n(values, rows * cols, _matrix_data);
	}

	// Set value of mtrx[row][col] to value
	inline void set_value_at(int row, int col, ValueType value) {
		if (in_bounds(row, col)) return unsafe_set_value_at(row, col, value);
		throw_domain_error(row, col, rows, cols);
	}

	inline void unsafe_set_value_at(int row, int col, ValueType value) {
		_matrix_data[cols * row + col] = value;
	}

	// Sum of all members
	ValueType sum() const {
		ValueType sum;

		for (int i = 0; i < element_count; ++i) sum += _matrix_data[i];

		return sum;
	}

	// Iterator functions
	ValueType* begin() const noexcept { return _matrix_data; }
	ValueType* end() const noexcept { return _matrix_data + element_count; };

  // Whether a point [row][col] is in bounds
  bool in_bounds(int row, int col) const {
	  return 0 <= row && 0 <= col && col < cols && row < rows;
  }

	/* Pretty output */
	friend std::ostream& operator<< <rows, cols, ValueType>(std::ostream& out, const Matrix<rows, cols, ValueType>& m);

  // Convert matrix to a string. Not intended to be efficient


	// Map function turning this matrix into another one via some lambda function
	template <typename ReturnedType>
	Matrix<rows, cols, ReturnedType> map(std::function<ReturnedType(ValueType)> lambda) {
		Matrix<rows, cols, ReturnedType> mtrx;

		ValueType* this_ptr = _matrix_data;
		ValueType* other_ptr = mtrx._matrix_data;

		ValueType* this_ptr_end = end();

		for (; this_ptr != this_ptr_end; this_ptr++, other_ptr++) {
			*other_ptr = lambda(*this_ptr);
		}

		return mtrx;
	}

	/*std::string to_string () {
		return base_matrix_to_string(rows, cols, _matrix_data, Alignment::LEFT, 1);
	}*/

#define FLOATING_POINT_SPECIALIZATION template <typename FloatingPointType = ValueType, \
	typename std::enable_if_t<std::is_floating_point<FloatingPointType>::value, bool> = 0>

	// Special version of to_string() for floating-point numbers, with an optional specifier
	FLOATING_POINT_SPECIALIZATION
	std::string to_string(const char* fmt_string="%f") {
		return base_matrix_to_string(rows, cols, _matrix_data, Alignment::LEFT, 4);
	}

	private:
	ValueType* _matrix_data;
};

template <int rows, int cols, class ValueType>
std::ostream& operator<< (std::ostream& out, const Matrix<rows, cols, ValueType>& m) {
	out << m.to_string();
	return out;
}



#endif //CANT_MATRIX_HPP
