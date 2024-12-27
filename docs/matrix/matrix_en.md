# MZLIB/Matrix.hpp Usage Documentation

## 1. Introduction
`MZLIB::Matrix` is a generic matrix class that supports various element types, defaulting to `int`. It provides a rich set of operations including initialization, addition, multiplication (matrix-matrix and matrix-scalar), inversion, and more.

## 2. Include File
Before using `MZLIB::Matrix`, ensure you include the header file:
```cpp
#include "matrix.hpp"
```

## 3. Class Template Definition
```cpp
template <typename _T = int>
class Matrix
```
- `_T`: The type of elements in the matrix, defaults to `int`.

## 4. Constructors
### 4.1 Default Constructor
Creates an empty matrix.
```cpp
Matrix() = default;
```

### 4.2 Size-Specified Constructor
Creates a matrix with specified rows and columns, initialized with a given value.
```cpp
Matrix(size_t H, size_t W, const element_type &x = element_type()) noexcept
```
- `H`: Number of rows.
- `W`: Number of columns.
- `x`: Initial value for filling the matrix, defaults to the default value of the element type.

### 4.3 List Initialization Constructor
Initializes the matrix using a two-dimensional list.
```cpp
Matrix(std::initializer_list<std::initializer_list<element_type>> _ls)
```
- `_ls`: A two-dimensional list where each sublist corresponds to a row in the matrix.

## 5. Member Functions

### 5.1 Get Matrix Dimensions
```cpp
inline constexpr size_t getH() const noexcept
inline constexpr size_t getW() const noexcept
inline constexpr size_t size() const noexcept
```
- `getH()`: Returns the number of rows.
- `getW()`: Returns the number of columns.
- `size()`: Returns the total number of elements (rows * columns).

### 5.2 Matrix Indexing
```cpp
inline typename std::vector<element_type>::iterator operator[](size_t x)
inline typename std::vector<element_type>::const_iterator operator[](size_t x) const
```
- `operator[]`: Accesses a specific row of the matrix by index, returning an iterator to that row.

### 5.3 Matrix Operations
#### 5.3.1 Addition
```cpp
Matrix &operator+=(const Matrix &x)
Matrix<_T> operator+(const Matrix<_T> &x, const Matrix<_T> &y)
```
- `operator+=`: Adds another matrix to the current matrix and returns a reference.
- `operator+`: Returns the result of adding two matrices.

#### 5.3.2 Multiplication
##### Matrix-Matrix Multiplication
```cpp
Matrix &operator*=(const Matrix &x)
Matrix<_T> operator*(const Matrix<_T> &x, const Matrix<_T> &y)
```
- `operator*=`: Multiplies another matrix to the current matrix and returns a reference.
- `operator*`: Returns the result of multiplying two matrices.

##### Matrix-Scalar Multiplication
```cpp
Matrix &operator*=(element_type x)
Matrix<_T> operator*(const Matrix<_T> &x, _T y)
Matrix<_T> operator*(_T y, const Matrix<_T> &x)
```
- `operator*=`: Multiplies a scalar to the current matrix and returns a reference.
- `operator*`: Returns the result of multiplying a matrix by a scalar.

### 5.4 Inversion
```cpp
inline Matrix<element_type> inverse() const
```
- Returns the inverse of the matrix. Throws an exception if the matrix is not square or is singular.

### 5.5 Identity Matrix Creation
```cpp
inline static Matrix<element_type> identity(size_t n)
```
- Returns an `n x n` identity matrix.

## 6. Iterator Support
The `Matrix` class provides standard iterator support for iterating over its elements.
```cpp
using iterator = typename std::vector<element_type>::iterator;
using const_iterator = typename std::vector<element_type>::const_iterator;
using reverse_iterator = std::reverse_iterator<iterator>;
using const_reverse_iterator = std::reverse_iterator<const_iterator>;

inline iterator begin() noexcept
inline iterator end() noexcept
inline const_iterator begin() const noexcept
inline const_iterator end() const noexcept
inline const_iterator cbegin() const noexcept
inline const_iterator cend() const noexcept
inline reverse_iterator rbegin() noexcept
inline reverse_iterator rend() noexcept
inline const_reverse_iterator rbegin() const noexcept
inline const_reverse_iterator rend() const noexcept
inline const_reverse_iterator crbegin() const noexcept
inline const_reverse_iterator crend() const noexcept
```

## 7. Example Code

### 7.1 Initializing Matrices
```cpp
MZLIB::Matrix<int> mat1(3, 3, 0); // 3x3 zero matrix
MZLIB::Matrix<int> mat2 = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
```

### 7.2 Matrix Operations
```cpp
MZLIB::Matrix<int> mat3 = mat1 + mat2; // Matrix addition
MZLIB::Matrix<int> mat4 = mat2 * 2;    // Matrix-scalar multiplication
MZLIB::Matrix<int> mat5 = mat2 * mat4; // Matrix-matrix multiplication
```

### 7.3 Matrix Inversion
```cpp
try {
    MZLIB::Matrix<double> invMat = mat2.inverse();
} catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

### 7.4 Creating an Identity Matrix
```cpp
MZLIB::Matrix<int> idenMat = MZLIB::Matrix<int>::identity(3);
```

## 8. Important Notes
- Matrix multiplication requires that the number of columns in the first matrix equals the number of rows in the second matrix.
- Inverting a matrix requires it to be square and non-singular.
- When using list initialization, all sublists must have the same length.