# `modint.hpp` 使用手册

## 概述

`modint.hpp` 提供了一个类模板，用于在模运算系统中处理整数。这个类 `ModInt` 支持高效的加法、减法、乘法和除法操作，能够在指定的模数下进行运算，非常适用于数论、密码学以及竞技编程等领域。

此外，该类模板还包括了一个计算模逆的工具函数，这是模运算中进行除法运算所必需的。

## 主要组成部分

### 1. `ModInt` 类
`ModInt` 类用于表示一个整数值，并将其按给定的模数进行归约。它支持在该模数下的基本运算，确保结果始终保持在 `[0, MOD-1]` 范围内。

#### 成员类型定义

- `value_type`：表示存储整数值的类型（默认为 `long`）。
- `MOD`：模数，定义了运算的模。

#### 构造函数

- `ModInt()`：默认构造函数，将整数值初始化为 `0`。
- `ModInt(_T _val)`：使用 `_val % MOD` 初始化整数值，确保结果始终在模数范围内。

#### 成员函数

- `const _T getval() const`：返回该整数值模 `MOD` 的结果，始终确保结果位于 `[0, MOD-1]` 范围内。
- `operator _T() const`：隐式转换操作符，将 `ModInt` 对象转换为类型 `_T`。

### 2. `modinv` 函数
`modinv` 函数使用扩展欧几里得算法计算整数 `a` 在模 `m` 下的模逆。模逆 `x` 满足 `a * x ≡ 1 (mod m)`。如果模逆不存在（即 `a` 和 `m` 不是互质的），该函数会抛出 `std::domain_error` 异常。

#### 语法

```cpp
template <typename _T>
inline _T modinv(_T a, size_t m);
```

- `a`：要计算模逆的整数。
- `m`：模数。

#### 返回值

- 返回整数 `a` 在模 `m` 下的模逆。

#### 异常

- `std::domain_error`：如果模逆不存在（即 `a` 和 `m` 不是互质的）。

### 3. 运算符重载

#### 加法

```cpp
template <size_t _MOD, typename _T>
inline ModInt<_MOD, _T> operator+(const ModInt<_MOD, _T> &x, const ModInt<_MOD, _T> &y);
```

- 对两个 `ModInt` 对象 `x` 和 `y` 进行加法运算，并返回其模 `MOD` 下的结果。

#### 减法

```cpp
template <size_t _MOD, typename _T>
inline ModInt<_MOD, _T> operator-(const ModInt<_MOD, _T> &x, const ModInt<_MOD, _T> &y);
```

- 对两个 `ModInt` 对象 `x` 和 `y` 进行减法运算，并返回其模 `MOD` 下的结果。结果始终为非负数（即如果减法结果为负数，结果会自动进行模运算）。

#### 乘法

```cpp
template <size_t _MOD, typename _T>
inline ModInt<_MOD, _T> operator*(const ModInt<_MOD, _T> &x, const ModInt<_MOD, _T> &y);
```

- 对两个 `ModInt` 对象 `x` 和 `y` 进行乘法运算，并返回其模 `MOD` 下的结果。

#### 除法

```cpp
template <size_t _MOD, typename _T>
inline ModInt<_MOD, _T> operator/(const ModInt<_MOD, _T> &x, const ModInt<_MOD, _T> &y);
```

- 对两个 `ModInt` 对象 `x` 和 `y` 进行除法运算。此运算通过计算 `y` 的模逆（使用 `modinv` 函数）并与 `x` 相乘来实现，最终返回其模 `MOD` 下的结果。

## 示例用法

### 1. 创建和初始化 `ModInt` 对象

```cpp
MZLIB::ModInt<7> a(3);  // 模数 7, a = 3
MZLIB::ModInt<7> b(5);  // 模数 7, b = 5
```

### 2. 执行算术运算

```cpp
auto sum = a + b;      // (3 + 5) % 7 = 1
auto diff = a - b;     // (3 - 5 + 7) % 7 = 5
auto prod = a * b;     // (3 * 5) % 7 = 1
auto quot = a / b;     // (3 * modinv(5, 7)) % 7 = 2
```

### 3. 计算模逆

```cpp
int inverse = MZLIB::modinv(5, 7);  // 返回 3，因为 5 * 3 ≡ 1 (mod 7)
```

### 4. 隐式转换

```cpp
MZLIB::ModInt<7> c(6);
int c_value = c;  // 隐式转换为 int 类型，c_value = 6
```

## 错误处理

- 如果尝试进行除法运算时，除数是零或没有模逆（即 `a` 和 `MOD` 不是互质的），程序会抛出 `std::domain_error` 异常，并且错误信息为 `"inverse does not exist"`。
- 如果模数 (`MOD`) 为 1，所有值都会归约为零，因为模 1 的所有整数都相等于 0。

## 模逆失败的示例

```cpp
try
{
    int inv = MZLIB::modinv(2, 4);  // 由于 gcd(2, 4) != 1，模逆不存在
}
catch (const std::domain_error &e)
{
    std::cerr << e.what() << std::endl;  // 输出: "inverse does not exist"
}
```