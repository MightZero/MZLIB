# Matrix.hpp 使用手册

## 概述

`matrix.hpp` 提供了一个矩阵类库，用于处理二维矩阵的基本操作。支持矩阵的构造、赋值、加法、乘法、迭代器遍历、初始化列表等功能。该库适用于不同数据类型的矩阵，默认情况下使用整数类型 (`int`)。

## 主要组件

### 1. `Matrix` 类
`Matrix` 是一个动态大小的矩阵类，支持动态分配和释放内存。用户可以通过指定矩阵的行数和列数来创建一个矩阵，并且支持通过初始化列表来初始化矩阵的元素。

#### 成员类型定义

- `element_type`: 矩阵元素的类型。
- `reference`: 对矩阵元素的引用类型。
- `const_reference`: 对矩阵元素的常量引用类型。
- `pointer`: 矩阵元素指针类型。
- `const_pointer`: 矩阵元素的常量指针类型。

#### 构造函数

- `Matrix()`: 默认构造函数，创建一个空矩阵（0x0）。
- `Matrix(size_t H, size_t W, const element_type &x = element_type()) noexcept`: 使用指定的行数 (`H`) 和列数 (`W`) 创建一个矩阵，并用给定值 (`x`) 初始化所有元素。
- `Matrix(std::initializer_list<std::initializer_list<element_type>> _ls)`: 通过嵌套的初始化列表创建矩阵。

#### 访问方法

- `inline typename std::vector<element_type>::iterator operator[](size_t x)`: 返回第 `x` 行的开始迭代器。
- `inline typename std::vector<element_type>::const_iterator operator[](size_t x) const`: 返回第 `x` 行的常量开始迭代器。
- `inline constexpr size_t getH() const noexcept`: 返回矩阵的行数。
- `inline constexpr size_t getW() const noexcept`: 返回矩阵的列数。
- `inline constexpr size_t size() const noexcept`: 返回矩阵元素的总数量（行数 * 列数）。

#### 辅助方法

- `Matrix& operator+=(const Matrix& x)`: 矩阵加法赋值运算符。
- `Matrix& operator*=(const Matrix& x)`: 矩阵乘法赋值运算符。
- `Matrix& operator*=(_T x)`: 矩阵与标量乘法赋值运算符。

#### 迭代器

- `iterator begin() noexcept`: 返回矩阵的开始迭代器。
- `iterator end() noexcept`: 返回矩阵的结束迭代器。
- `const_iterator begin() const noexcept`: 返回矩阵的常量开始迭代器。
- `const_iterator end() const noexcept`: 返回矩阵的常量结束迭代器。
- `reverse_iterator rbegin() noexcept`: 返回矩阵的反向开始迭代器。
- `reverse_iterator rend() noexcept`: 返回矩阵的反向结束迭代器。
- `const_reverse_iterator rbegin() const noexcept`: 返回矩阵的常量反向开始迭代器。
- `const_reverse_iterator rend() const noexcept`: 返回矩阵的常量反向结束迭代器。
- `const_reverse_iterator crbegin() const noexcept`: 返回矩阵的常量反向开始迭代器。
- `const_reverse_iterator crend() const noexcept`: 返回矩阵的常量反向结束迭代器。

### 2. 操作符重载

#### 矩阵乘法

- `operator*`: 矩阵乘法运算符，返回两个矩阵的乘积。

```cpp
template <typename _T>
inline Matrix<_T> operator*(const Matrix<_T> &x, const Matrix<_T> &y)
```

- `operator*`: 矩阵与标量乘法运算符，返回矩阵与标量的乘积。

```cpp
template <typename _T>
inline Matrix<_T> operator*(const Matrix<_T> &x, _T y)

template <typename _T>
inline Matrix<_T> operator*(_T y, const Matrix<_T> &x)
```

#### 矩阵加法

- `operator+`: 矩阵加法运算符，返回两个矩阵的和。

```cpp
template <typename _T>
inline Matrix<_T> operator+(const Matrix<_T> &x, const Matrix<_T> &y)
```

## 使用示例

### 1. 创建和初始化矩阵

```cpp
// 动态矩阵
MZLIB::Matrix<int> mat1(3, 3);  // 创建一个 3x3 的矩阵
mat1[0][0] = 1;
mat1[0][1] = 2;
mat1[0][2] = 3;

// 使用初始化列表创建矩阵
MZLIB::Matrix<int> mat2 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
```

### 2. 矩阵加法

```cpp
MZLIB::Matrix<int> mat3 = mat1 + mat2;
```

### 3. 矩阵乘法

```cpp
MZLIB::Matrix<int> mat4 = mat1 * mat2;
```

### 4. 矩阵与标量乘法

```cpp
MZLIB::Matrix<int> mat5 = mat1 * 2;
MZLIB::Matrix<int> mat6 = 2 * mat1;
```

### 5. 迭代器遍历矩阵

```cpp
for (auto it = mat1.begin(); it != mat1.end(); ++it)
{
    std::cout << *it << " ";
}
```

## 注意事项

1. 使用 `operator[]` 方法时，请确保传入的行列索引在矩阵范围内，否则将导致未定义行为。
2. 矩阵的加法和乘法操作要求参与运算的矩阵具有兼容的尺寸，否则会抛出 `std::invalid_argument` 异常。
3. 在进行矩阵乘法时，左矩阵的列数必须等于右矩阵的行数，否则会抛出 `std::invalid_argument` 异常。