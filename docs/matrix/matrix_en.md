# Matrix.hpp User Manual

## Overview

`matrix.hpp` provides a matrix class library designed to handle basic operations on two-dimensional matrices, including both dynamic and static matrix types. It supports operations like matrix construction, assignment, addition, multiplication, iterator traversal, and initialization with lists. The library works with different data types for matrices, defaulting to `int`.

## Key Components

### 1. `Matrix` Class
`Matrix` is a dynamic-sized matrix class that supports dynamic memory allocation and deallocation. A matrix is created by specifying the number of rows and columns, and it can also be initialized using an initializer list.

#### Member Type Definitions

- `value_type`: The type of matrix elements.
- `reference`: The reference type for matrix elements.
- `pointer`: The pointer type for matrix elements.
- `dpointer`: A pointer to a pointer type, used to represent a 2D matrix.

#### Constructors and Destructor

- `Matrix(size_t H, size_t W)`: Creates a matrix with the specified number of rows (`H`) and columns (`W`).
- `~Matrix()`: Destructor that frees the matrix memory.
- `Matrix(const Matrix &other)`: Copy constructor that creates a matrix identical to another.
- `Matrix(Matrix &&other) noexcept`: Move constructor that transfers ownership of the matrix.
- `Matrix(std::initializer_list<std::initializer_list<value_type>> _ls)`: Constructs a matrix using a nested initializer list.

#### Operator Overloads

- `Matrix& operator=(const Matrix& other)`: Assignment operator that performs a deep copy.
- `Matrix& operator=(Matrix&& other) noexcept`: Move assignment operator that transfers resources from one matrix to another.
- `operator dpointer() noexcept`: Returns the matrix pointer for other operations.

#### Access Methods

- `reference getVal(const size_t &x, const size_t &y)`: Returns a reference to the matrix element at position `(x, y)`.
- `const reference getVal(const size_t &x, const size_t &y) const`: Returns a constant reference to the matrix element at position `(x, y)`.

#### Helper Methods

- `size_t getH() const`: Returns the number of rows in the matrix.
- `size_t getW() const`: Returns the number of columns in the matrix.
- `size_t size() const`: Returns the total number of elements in the matrix (rows * columns).

#### Iterator

- `iterator begin() noexcept`: Returns the beginning iterator for the matrix.
- `iterator end() noexcept`: Returns the end iterator for the matrix.

### 2. `Static_Matrix` Class
`Static_Matrix` is a fixed-size matrix class where the number of rows and columns is determined at compile-time. It provides similar functionality as the `Matrix` class but does not support dynamic resizing.

#### Constructors and Destructor

- `Static_Matrix()`: Default constructor that creates a fixed-size matrix.
- `~Static_Matrix()`: Destructor that frees the matrix memory.
- `Static_Matrix(const Static_Matrix &other)`: Copy constructor.
- `Static_Matrix(Static_Matrix &&other) noexcept`: Move constructor.
- `Static_Matrix(std::initializer_list<value_type> _ls)`: Initializes the matrix from a flat initializer list.

#### Operator Overloads

- `Static_Matrix& operator=(const Static_Matrix& other)`: Assignment operator that performs a deep copy.
- `Static_Matrix& operator=(Static_Matrix&& other) noexcept`: Move assignment operator that transfers resources from one matrix to another.
- `operator dpointer() noexcept`: Returns the matrix pointer for other operations.

#### Access Methods

- `reference getVal(const size_t &x, const size_t &y)`: Returns a reference to the matrix element at position `(x, y)`.
- `const reference getVal(const size_t &x, const size_t &y) const`: Returns a constant reference to the matrix element at position `(x, y)`.

#### Helper Methods

- `size_t getH() const`: Returns the number of rows in the matrix.
- `size_t getW() const`: Returns the number of columns in the matrix.
- `size_t size() const`: Returns the total number of elements in the matrix.

#### Iterator

- `iterator begin() noexcept`: Returns the beginning iterator for the matrix.
- `iterator end() noexcept`: Returns the end iterator for the matrix.

### 3. Operator Overloads

#### Matrix Multiplication

- `operator*`: Matrix multiplication operator that returns the product of two matrices.

```cpp
template <typename _T>
inline Matrix<_T> operator*(const Matrix<_T> &x, const Matrix<_T> &y)
```

#### Matrix Addition

- `operator+`: Matrix addition operator that returns the sum of two matrices.

```cpp
template <typename _T>
inline Matrix<_T> operator+(const Matrix<_T> &x, const Matrix<_T> &y)
```

#### Static Matrix Multiplication

- `operator*`: Operator for multiplying fixed-size matrices.

```cpp
template <size_t _Hx, size_t _HW, size_t _Wy, typename _T>
inline Static_Matrix<_Hx, _Wy, _T> operator*(const Static_Matrix<_Hx, _HW, _T> &x, const Static_Matrix<_HW, _Wy, _T> &y)
```

#### Static Matrix Addition

- `operator+`: Operator for adding fixed-size matrices.

```cpp
template <size_t _H, size_t _W, typename _T>
inline Static_Matrix<_H, _W, _T> operator+(const Static_Matrix<_H, _W, _T> &x, const Static_Matrix<_H, _W, _T> &y)
```

## Example Usage

### 1. Creating and Initializing Matrices

```cpp
// Dynamic Matrix
MZLIB::Matrix<int> mat1(3, 3);  // Create a 3x3 matrix
mat1.getVal(0, 0) = 1;
mat1.getVal(0, 1) = 2;
mat1.getVal(0, 2) = 3;

// Using initializer list to create a matrix
MZLIB::Matrix<int> mat2 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
```

### 2. Matrix Addition

```cpp
MZLIB::Matrix<int> mat3 = mat1 + mat2;
```

### 3. Matrix Multiplication

```cpp
MZLIB::Matrix<int> mat4 = mat1 * mat2;
```

### 4. Iterating Through a Matrix

```cpp
for (auto it = mat1.begin(); it != mat1.end(); ++it)
{
    std::cout << *it << " ";
}
```

### 5. Static Matrix

```cpp
MZLIB::Static_Matrix<2, 2, int> staticMat = {{1, 2}, {3, 4}};
```

## Notes

1. When using `getVal`, ensure the provided row and column indices are within the matrix bounds; otherwise, an `std::out_of_range` exception will be thrown.
2. Matrix addition and multiplication require compatible matrix dimensions; otherwise, an `std::invalid_argument` exception will be thrown.