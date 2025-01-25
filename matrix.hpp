// FileName : matrix.hpp
// Matrix Packed Class Header
// Programmed By MightZero
// Copyright 2025 MightZero
#pragma once
#ifndef _MZLIB_MATRIX_HPP
#define _MZLIB_MATRIX_HPP
#endif
#include <utility>
#include <initializer_list>
#include <vector>
#include <stdexcept>
#include <iterator>


namespace MZLIB
{
    template <typename _T = int>
    class Matrix
    {
    public:
        using element_type = _T;
        using reference = element_type &;
        using const_reference = const element_type &;
        using pointer = element_type *;
        using const_pointer = const element_type *;

        Matrix() = default;
        Matrix(size_t H, size_t W, const element_type &x = element_type()) noexcept : _H(H), _W(W) { _dat.assign(size(), x); }
        Matrix(std::initializer_list<std::initializer_list<element_type>> _ls)
        {
            if (_ls.size() == 0)
                throw std::length_error("invalid initializer list size");
            _H = _ls.size(), _W = (*_ls.begin()).size();
            _dat.assign(size(), element_type());
            auto it = this->begin();
            for (auto &_lsh : _ls)
            {
                if (_lsh.size() != _W)
                    throw std::length_error("invalid initializer list size");
                for (auto &x : _lsh)
                    *(it++) = x;
            }
        }
        inline typename std::vector<element_type>::iterator operator[](size_t x) { return _dat.begin() + x * _W; }
        inline typename std::vector<element_type>::const_iterator operator[](size_t x) const { return _dat.begin() + x * _W; }
        inline constexpr size_t getH() const noexcept { return _H; }
        inline constexpr size_t getW() const noexcept { return _W; }
        inline constexpr size_t size() const noexcept { return _H * _W; }

        Matrix &operator+=(const Matrix &x) { return (*this) = (*this) + x; }
        Matrix &operator*=(const Matrix &x) { return (*this) = (*this) * x; }
        Matrix &operator*=(element_type x) { return (*this) = (*this) * x; }

        using iterator = typename std::vector<element_type>::iterator;
        using const_iterator = typename std::vector<element_type>::const_iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        inline iterator begin() noexcept { return _dat.begin(); }
        inline iterator end() noexcept { return _dat.end(); }
        inline const_iterator begin() const noexcept { return _dat.cbegin(); }
        inline const_iterator end() const noexcept { return _dat.cend(); }
        inline const_iterator cbegin() const noexcept { return _dat.cbegin(); }
        inline const_iterator cend() const noexcept { return _dat.cend(); }
        inline reverse_iterator rbegin() noexcept { return _dat.rbegin(); }
        inline reverse_iterator rend() noexcept { return _dat.rend(); }
        inline const_reverse_iterator rbegin() const noexcept { return _dat.crbegin(); }
        inline const_reverse_iterator rend() const noexcept { return _dat.crend(); }
        inline const_reverse_iterator crbegin() const noexcept { return _dat.crbegin(); }
        inline const_reverse_iterator crend() const noexcept { return _dat.crend(); }

        inline Matrix<element_type> inverse() const
        {
            if (_H != _W)
                throw std::invalid_argument("invalid matrix size to compute inverse");
            Matrix<element_type> augmented(_H, _W * 2);
            for (size_t i = 0; i < _H; ++i)
            {
                for (size_t j = 0; j < _W; ++j)
                    augmented[i][j] = _dat[i * _W + j],
                    augmented[i][j + _W] = (i == j) ? 1 : 0;
            }
            for (size_t i = 0; i < _H; ++i)
            {
                size_t pivot = i;
                for (size_t j = i + 1; j < _H; ++j)
                    if (std::abs(augmented[j][i]) > std::abs(augmented[pivot][i]))
                        pivot = j;
                if (pivot != i)
                    std::swap_ranges(augmented[i], augmented[i] + _W * 2, augmented[pivot]);
                element_type scale = augmented[i][i];
                if (std::abs(scale) < 1e-9)
                    throw std::runtime_error("matrix is singular and cannot be inverted");
                for (size_t j = i; j < _W * 2; ++j)
                    augmented[i][j] = augmented[i][j] / scale;
                for (size_t j = 0; j < _H; ++j)
                {
                    if (j == i)
                        continue;
                    element_type factor = augmented[j][i];
                    for (size_t k = i; k < _W * 2; ++k)
                        augmented[j][k] = augmented[j][k] - factor * augmented[i][k];
                }
            }
            Matrix<element_type> inv(_H, _W);
            for (size_t i = 0; i < _H; ++i)
                for (size_t j = 0; j < _W; ++j)
                    inv[i][j] = augmented[i][j + _W];

            return inv;
        }

        inline static Matrix<element_type> identity(size_t n)
        {
            Matrix<element_type> mat(n, n);
            for (size_t i = 0; i < n; ++i)
                mat[i][i] = element_type(1);
            return mat;
        }

    private:
        size_t _H, _W;
        std::vector<element_type> _dat;
    };

    template <typename _T>
    inline Matrix<_T> operator*(const Matrix<_T> &x, const Matrix<_T> &y)
    {
        if (!x.size())
            return x;
        if (!y.size())
            return y;
        if (x.getW() != y.getH())
            throw std::invalid_argument("invalid matrix size");
        size_t _Hx = x.getH(), _Wy = y.getW(), _HW = x.getW();
        Matrix<_T> res(_Hx, _Wy);
        for (size_t i = 0; i < _Hx; ++i)
            for (size_t j = 0; j < _Wy; ++j)
                for (size_t k = 0; k < _HW; ++k)
                    res[i][j] = res[i][j] + x[i][k] * y[k][j];
        return res;
    }
    template <typename _T>
    inline Matrix<_T> operator*(const Matrix<_T> &x, _T y)
    {
        Matrix<_T> res = x;
        for (auto &val : res)
            val = val * y;
        return res;
    }
    template <typename _T>
    inline Matrix<_T> operator*(_T y, const Matrix<_T> &x) { return x * y; }
    template <typename _T>
    inline Matrix<_T> operator+(const Matrix<_T> &x, const Matrix<_T> &y)
    {
        if (!x.size())
            return y;
        if (!y.size())
            return x;
        if (x.getH() != y.getH() || x.getW() != y.getW())
            throw std::invalid_argument("invalid matrix size");
        size_t _H = x.getH(), _W = x.getW();
        Matrix<_T> res(_H, _W);
        for (size_t i = 0; i < _H; ++i)
            for (size_t j = 0; j < _W; ++j)
                res[i][j] = x[i][j] + y[i][j];
        return res;
    }
}