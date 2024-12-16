# Matrix.hpp 使用手册

## 概述

`matrix.hpp` 提供了一个矩阵类库，用于处理二维矩阵的基本操作，包括矩阵的动态和静态类型实现。支持矩阵的构造、赋值、加法、乘法、迭代器遍历、初始化列表等功能。该库适用于不同数据类型的矩阵，默认情况下使用整数类型 (`int`)。

## 主要组件

### 1. `Matrix` 类
`Matrix` 是一个动态大小的矩阵类，支持动态分配和释放内存。用户可以通过指定矩阵的行数和列数来创建一个矩阵，并且支持通过初始化列表来初始化矩阵的元素。

#### 成员类型定义

- `value_type`: 矩阵元素的类型。
- `reference`: 对矩阵元素的引用类型。
- `pointer`: 矩阵元素指针类型。
- `dpointer`: 指向指针的指针类型，用于表示二维矩阵。

#### 构造函数与析构函数

- `Matrix(size_t H, size_t W)`: 使用指定的行数 (`H`) 和列数 (`W`) 创建一个矩阵。
- `~Matrix()`: 析构函数，释放矩阵的内存。
- `Matrix(const Matrix &other)`: 拷贝构造函数，创建一个与另一个矩阵相同的矩阵。
- `Matrix(Matrix &&other) noexcept`: 移动构造函数，转移另一个矩阵的所有权。
- `Matrix(std::initializer_list<std::initializer_list<value_type>> _ls)`: 通过嵌套的初始化列表创建矩阵。

#### 运算符重载

- `Matrix& operator=(const Matrix& other)`: 赋值运算符，进行深拷贝。
- `Matrix& operator=(Matrix&& other) noexcept`: 移动赋值运算符，将一个矩阵的资源转移到另一个矩阵。
- `operator dpointer() noexcept`: 返回矩阵的指针，便于其他操作。

#### 访问方法

- `reference getVal(const size_t &x, const size_t &y)`: 返回矩阵元素的引用，给定行 (`x`) 和列 (`y`)。
- `const reference getVal(const size_t &x, const size_t &y) const`: 返回常量引用，给定行 (`x`) 和列 (`y`)。

#### 辅助方法

- `size_t getH() const`: 返回矩阵的行数。
- `size_t getW() const`: 返回矩阵的列数。
- `size_t size() const`: 返回矩阵元素的总数量（行数 * 列数）。

#### 迭代器

- `iterator begin() noexcept`: 返回矩阵的开始迭代器。
- `iterator end() noexcept`: 返回矩阵的结束迭代器。

### 2. `Static_Matrix` 类
`Static_Matrix` 是一个固定大小的矩阵类，矩阵的行数和列数在编译时就已经确定。与 `Matrix` 类类似，`Static_Matrix` 提供了类似的操作，但不支持动态调整矩阵大小。

#### 构造函数与析构函数

- `Static_Matrix()`: 默认构造函数，创建一个固定大小的矩阵。
- `~Static_Matrix()`: 析构函数，释放矩阵的内存。
- `Static_Matrix(const Static_Matrix &other)`: 拷贝构造函数。
- `Static_Matrix(Static_Matrix &&other) noexcept`: 移动构造函数。
- `Static_Matrix(std::initializer_list<value_type> _ls)`: 通过初始化列表创建矩阵。

#### 运算符重载

- `Static_Matrix& operator=(const Static_Matrix& other)`: 赋值运算符，进行深拷贝。
- `Static_Matrix& operator=(Static_Matrix&& other) noexcept`: 移动赋值运算符，将一个矩阵的资源转移到另一个矩阵。
- `operator dpointer() noexcept`: 返回矩阵的指针。

#### 访问方法

- `reference getVal(const size_t &x, const size_t &y)`: 返回矩阵元素的引用。
- `const reference getVal(const size_t &x, const size_t &y) const`: 返回常量引用。

#### 辅助方法

- `size_t getH() const`: 返回矩阵的行数。
- `size_t getW() const`: 返回矩阵的列数。
- `size_t size() const`: 返回矩阵元素的总数量。

#### 迭代器

- `iterator begin() noexcept`: 返回矩阵的开始迭代器。
- `iterator end() noexcept`: 返回矩阵的结束迭代器。

### 3. 操作符重载

#### 矩阵乘法

- `operator*`: 矩阵乘法运算符，返回两个矩阵的乘积。

```cpp
template <typename _T>
inline Matrix<_T> operator*(const Matrix<_T> &x, const Matrix<_T> &y)
```

#### 矩阵加法

- `operator+`: 矩阵加法运算符，返回两个矩阵的和。

```cpp
template <typename _T>
inline Matrix<_T> operator+(const Matrix<_T> &x, const Matrix<_T> &y)
```

#### 静态矩阵乘法

- `operator*`: 固定大小矩阵的乘法。

```cpp
template <size_t _Hx, size_t _HW, size_t _Wy, typename _T>
inline Static_Matrix<_Hx, _Wy, _T> operator*(const Static_Matrix<_Hx, _HW, _T> &x, const Static_Matrix<_HW, _Wy, _T> &y)
```

#### 静态矩阵加法

- `operator+`: 固定大小矩阵的加法。

```cpp
template <size_t _H, size_t _W, typename _T>
inline Static_Matrix<_H, _W, _T> operator+(const Static_Matrix<_H, _W, _T> &x, const Static_Matrix<_H, _W, _T> &y)
```

## 使用示例

### 1. 创建和初始化矩阵

```cpp
// 动态矩阵
MZLIB::Matrix<int> mat1(3, 3);  // 创建一个 3x3 的矩阵
mat1.getVal(0, 0) = 1;
mat1.getVal(0, 1) = 2;
mat1.getVal(0, 2) = 3;

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

### 4. 迭代器遍历矩阵

```cpp
for (auto it = mat1.begin(); it != mat1.end(); ++it)
{
    std::cout << *it << " ";
}
```

### 5. 静态矩阵

```cpp
MZLIB::Static_Matrix<2, 2, int> staticMat = {{1, 2}, {3, 4}};
```

## 注意事项

1. 使用 `getVal` 方法时，请确保传入的行列索引在矩阵范围内，否则将抛出 `std::out_of_range` 异常。
2. 矩阵的加法和乘法操作要求参与运算的矩阵具有兼容的尺寸，否则会抛出 `std::invalid_argument` 异常。