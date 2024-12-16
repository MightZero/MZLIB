# Modint.hpp User Manual

## Overview

`modint.hpp` provides a class template for working with integers under a modular arithmetic system. This class, `ModInt`, allows efficient arithmetic operations such as addition, subtraction, multiplication, and division with modulus, making it particularly useful for problems in number theory, cryptography, and competitive programming. 

The class template also includes a utility function for computing modular inverses, which is essential for division in modular arithmetic.

## Key Components

### 1. `ModInt` Class
The `ModInt` class represents an integer value modulo a given modulus. It supports various arithmetic operations that work within the modulus, ensuring results are always confined to the range `[0, MOD-1]`.

#### Member Type Definitions

- `value_type`: The type used to store the integer value (`_T`), which defaults to `long`.
- `MOD`: The modulus used for the operations.

#### Constructors

- `ModInt()`: Default constructor that initializes the value to 0.
- `ModInt(_T _val)`: Constructor that initializes the value with `_val % MOD`, ensuring the result is always within the valid range for the modulus.

#### Member Functions

- `const _T getval() const`: Returns the value of the integer modulo `MOD`. This function ensures the value is always in the range `[0, MOD-1]`.
- `operator _T() const`: Implicit conversion operator to return the value as the type `_T`.

### 2. `modinv` Function
The `modinv` function computes the modular inverse of an integer `a` modulo `m` using the Extended Euclidean Algorithm. The modular inverse `x` satisfies the equation `a * x ≡ 1 (mod m)`. If the inverse does not exist (i.e., if `a` and `m` are not coprime), the function throws a `std::domain_error`.

#### Syntax

```cpp
template <typename _T>
inline _T modinv(_T a, size_t m);
```

- `a`: The integer whose modular inverse is to be computed.
- `m`: The modulus.

#### Returns

- The modular inverse of `a` modulo `m`.

#### Throws

- `std::domain_error`: If the modular inverse does not exist (i.e., `a` and `m` are not coprime).

### 3. Operator Overloads

#### Addition

```cpp
template <size_t _MOD, typename _T>
inline ModInt<_MOD, _T> operator+(const ModInt<_MOD, _T> &x, const ModInt<_MOD, _T> &y);
```

- Adds two `ModInt` objects `x` and `y` and returns the result as a new `ModInt` object modulo `MOD`.

#### Subtraction

```cpp
template <size_t _MOD, typename _T>
inline ModInt<_MOD, _T> operator-(const ModInt<_MOD, _T> &x, const ModInt<_MOD, _T> &y);
```

- Subtracts `y` from `x` and returns the result as a new `ModInt` object modulo `MOD`. The result is always positive (i.e., it wraps around if the subtraction results in a negative number).

#### Multiplication

```cpp
template <size_t _MOD, typename _T>
inline ModInt<_MOD, _T> operator*(const ModInt<_MOD, _T> &x, const ModInt<_MOD, _T> &y);
```

- Multiplies two `ModInt` objects `x` and `y` and returns the result as a new `ModInt` object modulo `MOD`.

#### Division

```cpp
template <size_t _MOD, typename _T>
inline ModInt<_MOD, _T> operator/(const ModInt<_MOD, _T> &x, const ModInt<_MOD, _T> &y);
```

- Divides `x` by `y` by multiplying `x` with the modular inverse of `y` (calculated using the `modinv` function) and returns the result as a new `ModInt` object modulo `MOD`.

## Example Usage

### 1. Creating and Initializing ModInt Objects

```cpp
MZLIB::ModInt<7> a(3);  // Modulo 7, a = 3
MZLIB::ModInt<7> b(5);  // Modulo 7, b = 5
```

### 2. Performing Arithmetic Operations

```cpp
auto sum = a + b;      // (3 + 5) % 7 = 1
auto diff = a - b;     // (3 - 5 + 7) % 7 = 5
auto prod = a * b;     // (3 * 5) % 7 = 1
auto quot = a / b;     // (3 * modinv(5, 7)) % 7 = 2
```

### 3. Modular Inverse

```cpp
int inverse = MZLIB::modinv(5, 7);  // Returns 3, since 5 * 3 ≡ 1 (mod 7)
```

### 4. Implicit Conversion

```cpp
MZLIB::ModInt<7> c(6);
int c_value = c;  // Implicitly converts ModInt to int, c_value = 6
```

## Error Handling

- If an attempt is made to divide by a `ModInt` object that is zero or has no inverse (i.e., the greatest common divisor between `a` and `MOD` is not 1), the program will throw a `std::domain_error` with the message `"inverse does not exist"`.
- If the modulus (`_MOD`) is 1, all values will be equivalent to zero because of the properties of modular arithmetic (i.e., everything modulo 1 is 0).

## Example of Modular Inverse Failure

```cpp
try
{
    int inv = MZLIB::modinv(2, 4);  // No inverse exists since gcd(2, 4) != 1
}
catch (const std::domain_error &e)
{
    std::cerr << e.what() << std::endl;  // Output: "inverse does not exist"
}
```

## Performance Considerations

The `modinv` function uses the Extended Euclidean Algorithm, which runs in `O(log m)` time complexity, making it efficient for large values of `m`. Arithmetic operations on `ModInt` objects are constant time (`O(1)`), making them suitable for use in performance-critical applications like competitive programming.