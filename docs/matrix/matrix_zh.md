# MZLIB/Matrix.hpp 使用文档

## 1. 简介
`MZLIB::Matrix` 是一个通用的矩阵类，支持多种类型的元素，默认为 `int` 类型。它提供了丰富的操作接口，包括初始化、加法、乘法（矩阵与矩阵、矩阵与标量）、求逆等。

## 2. 包含文件
在使用 `MZLIB::Matrix` 之前，请确保包含头文件：
```cpp
#include "matrix.hpp"
```

## 3. 类模板定义
```cpp
template <typename _T = int>
class Matrix
```
- `_T`：矩阵元素类型，默认为 `int`。

## 4. 构造函数
### 4.1 默认构造函数
创建一个空矩阵。
```cpp
Matrix() = default;
```

### 4.2 指定大小构造函数
创建一个指定行数和列数的矩阵，并用给定值填充。
```cpp
Matrix(size_t H, size_t W, const element_type &x = element_type()) noexcept
```
- `H`：矩阵的行数。
- `W`：矩阵的列数。
- `x`：用于填充矩阵的初始值，默认为元素类型的默认值。

### 4.3 列表初始化构造函数
使用二维列表初始化矩阵。
```cpp
Matrix(std::initializer_list<std::initializer_list<element_type>> _ls)
```
- `_ls`：二维列表，每一行对应矩阵的一行。

## 5. 成员函数

### 5.1 获取矩阵尺寸
```cpp
inline constexpr size_t getH() const noexcept
inline constexpr size_t getW() const noexcept
inline constexpr size_t size() const noexcept
```
- `getH()`：返回矩阵的行数。
- `getW()`：返回矩阵的列数。
- `size()`：返回矩阵中元素的总数（行数 * 列数）。

### 5.2 矩阵索引操作
```cpp
inline typename std::vector<element_type>::iterator operator[](size_t x)
inline typename std::vector<element_type>::const_iterator operator[](size_t x) const
```
- `operator[]`：通过行索引访问矩阵的某一行，返回该行的迭代器。

### 5.3 矩阵运算
#### 5.3.1 加法
```cpp
Matrix &operator+=(const Matrix &x)
Matrix<_T> operator+(const Matrix<_T> &x, const Matrix<_T> &y)
```
- `operator+=`：将另一个矩阵加到当前矩阵上，并返回引用。
- `operator+`：返回两个矩阵相加的结果。

#### 5.3.2 乘法
##### 矩阵与矩阵相乘
```cpp
Matrix &operator*=(const Matrix &x)
Matrix<_T> operator*(const Matrix<_T> &x, const Matrix<_T> &y)
```
- `operator*=`：将另一个矩阵乘到当前矩阵上，并返回引用。
- `operator*`：返回两个矩阵相乘的结果。

##### 矩阵与标量相乘
```cpp
Matrix &operator*=(element_type x)
Matrix<_T> operator*(const Matrix<_T> &x, _T y)
Matrix<_T> operator*(_T y, const Matrix<_T> &x)
```
- `operator*=`：将标量乘到当前矩阵上，并返回引用。
- `operator*`：返回矩阵与标量相乘的结果。

### 5.4 求逆
```cpp
inline Matrix<element_type> inverse() const
```
- 返回矩阵的逆矩阵。如果矩阵不是方阵或不可逆，则抛出异常。

### 5.5 创建单位矩阵
```cpp
inline static Matrix<element_type> identity(size_t n)
```
- 返回一个 `n x n` 的单位矩阵。

## 6. 迭代器支持
`Matrix` 类提供了标准的迭代器支持，方便遍历矩阵中的元素。
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

## 7. 示例代码

### 7.1 初始化矩阵
```cpp
MZLIB::Matrix<int> mat1(3, 3, 0); // 3x3 全零矩阵
MZLIB::Matrix<int> mat2 = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
```

### 7.2 矩阵运算
```cpp
MZLIB::Matrix<int> mat3 = mat1 + mat2; // 矩阵加法
MZLIB::Matrix<int> mat4 = mat2 * 2;    // 矩阵与标量相乘
MZLIB::Matrix<int> mat5 = mat2 * mat4; // 矩阵与矩阵相乘
```

### 7.3 求逆矩阵
```cpp
try {
    MZLIB::Matrix<double> invMat = mat2.inverse();
} catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

### 7.4 创建单位矩阵
```cpp
MZLIB::Matrix<int> idenMat = MZLIB::Matrix<int>::identity(3);
```

## 8. 注意事项
- 矩阵的乘法要求第一个矩阵的列数等于第二个矩阵的行数。
- 求逆矩阵时，矩阵必须是方阵且可逆。
- 使用列表初始化时，所有子列表的长度必须一致。