# Matrix.hpp User Manual

## Overview

`matrix.hpp` provides a matrix class library for handling basic operations on two-dimensional matrices. It supports matrix construction, assignment, addition, multiplication, iterator traversal, and initialization lists. This library is suitable for matrices of various data types, with `int` being the default type.

## Main Components

### 1. `Matrix` Class
The `Matrix` class is a dynamically sized matrix that supports dynamic memory allocation and deallocation. Users can create a matrix by specifying the number of rows and columns, and it also supports initializing matrix elements using an initialization list.

#### Member Type Definitions

- `element_type`: The type of matrix elements.
- `reference`: Reference type to matrix elements.
- `const_reference`: Constant reference type to matrix elements.
- `pointer`: Pointer type to matrix elements.
- `const_pointer`: Constant pointer type to matrix elements.

#### Constructors

- `Matrix()`: Default constructor, creates an empty matrix (0x0).
- `Matrix(size_t H, size_t W, const element_type &x = element_type()) noexcept`: Creates a matrix with specified number of rows (`H`) and columns (`W`), initializing all elements with the given value (`x`).
- `Matrix(std::initializer_list<std::initializer_list<element_type>> _ls)`: Creates a matrix using nested initialization lists.

#### Access Methods

- `inline typename std::vector<element_type>::iterator operator[](size_t x)`: Returns an iterator to the beginning of row `x`.
- `inline typename std::vector<element_type>::const_iterator operator[](size_t x) const`: Returns a constant iterator to the beginning of row `x`.
- `inline constexpr size_t getH() const noexcept`: Returns the number of rows in the matrix.
- `inline constexpr size_t getW() const noexcept`: Returns the number of columns in the matrix.
- `inline constexpr size_t size() const noexcept`: Returns the total number of elements in the matrix (rows * columns).

#### Helper Methods

- `Matrix& operator+=(const Matrix& x)`: Addition assignment operator for matrices.
- `Matrix& operator*=(const Matrix& x)`: Multiplication assignment operator for matrices.
- `Matrix& operator*=(_T x)`: Scalar multiplication assignment operator for matrices.

#### Iterators

- `iterator begin() noexcept`: Returns an iterator to the beginning of the matrix.
- `iterator end() noexcept`: Returns an iterator to the end of the matrix.
- `const_iterator begin() const noexcept`: Returns a constant iterator to the beginning of the matrix.
- `const_iterator end() const noexcept`: Returns a constant iterator to the end of the matrix.
- `reverse_iterator rbegin() noexcept`: Returns a reverse iterator to the beginning of the matrix.
- `reverse_iterator rend() noexcept`: Returns a reverse iterator to the end of the matrix.
- `const_reverse_iterator rbegin() const noexcept`: Returns a constant reverse iterator to the beginning of the matrix.
- `const_reverse_iterator rend() const noexcept`: Returns a constant reverse iterator to the end of the matrix.
- `const_reverse_iterator crbegin() const noexcept`: Returns a constant reverse iterator to the beginning of the matrix.
- `const_reverse_iterator crend() const noexcept`: Returns a constant reverse iterator to the end of the matrix.

### 2. Operator Overloading

#### Matrix Multiplication

- `operator*`: Matrix multiplication operator, returns the product of two matrices.

```cpp
template <typename _T>
inline Matrix<_T> operator*(const Matrix<_T> &x, const Matrix<_T> &y)
```

- `operator*`: Scalar multiplication operator, returns the product of a matrix and a scalar.

```cpp
template <typename _T>
inline Matrix<_T> operator*(const Matrix<_T> &x, _T y)

template <typename _T>
inline Matrix<_T> operator*(_T y, const Matrix<_T> &x)
```

#### Matrix Addition

- `operator+`: Matrix addition operator, returns the sum of two matrices.

```cpp
template <typename _T>
inline Matrix<_T> operator+(const Matrix<_T> &x, const Matrix<_T> &y)
```

## Usage Examples

### 1. Creating and Initializing Matrices

```cpp
// Dynamic matrix
MZLIB::Matrix<int> mat1(3, 3);  // Create a 3x3 matrix
mat1[0][0] = 1;
mat1[0][1] = 2;
mat1[0][2] = 3;

// Using initialization list to create a matrix
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

### 4. Scalar Multiplication

```cpp
MZLIB::Matrix<int> mat5 = mat1 * 2;
MZLIB::Matrix<int> mat6 = 2 * mat1;
```

### 5. Iterator Traversal

```cpp
for (auto it = mat1.begin(); it != mat1.end(); ++it)
{
    std::cout << *it << " ";
}
```

## Precautions

1. When using the `operator[]` method, ensure that the provided row and column indices are within the matrix bounds; otherwise, undefined behavior may occur.
2. Matrix addition and multiplication require compatible dimensions between the matrices involved; otherwise, a `std::invalid_argument` exception will be thrown.
3. For matrix multiplication, the number of columns in the left matrix must equal the number of rows in the right matrix; otherwise, a `std::invalid_argument` exception will be thrown.