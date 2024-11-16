// FileName : matrix.hpp
// Matrix Packed Class Header
// Programmed By MightZero
// Copyright 2024 MightZero
#include <utility>
#include <initializer_list>
#include <stdexcept>
#pragma once
#ifndef _MZLIB_MATRIX_HPP
#define _MZLIB_MATRIX_HPP
#endif

namespace MZLIB
{
    template <typename _T>
    class Matrix;

    template <size_t _H, size_t _W, typename _T>
    class Static_Matrix;

    template <typename _T = int>
    class Matrix
    {
    public:
        using value_type = _T;
        using reference = value_type &;
        using pointer = value_type *;
        using dpointer = pointer *;
        Matrix(size_t H, size_t W) : _H(H), _W(W) { allocate(); }
        ~Matrix() { free(); }
        Matrix(const Matrix &other) : _H(other._H), _W(other._W)
        {
            allocate();
            for (size_t i = 0; i < _H; ++i)
                std::copy(other.mat[i], other.mat[i] + _W, mat[i]);
        }
        Matrix(Matrix &&other) noexcept : mat(other.mat), _H(other._H), _W(other._W)
        {
            other.mat = nullptr, other._H = 0, other._W = 0;
        }
        Matrix(std::initializer_list<std::initializer_list<value_type>> _ls)
        {
            _H = _ls.size(), _W = (*_ls.begin()).size();
            allocate();
            size_t _nH = 0, _nW = 0;
            for (auto &_lsh : _ls)
            {
                if (_lsh.size() != _W)
                {
                    free();
                    throw std::length_error("invalid initializer list size");
                }
                for (auto &x : _lsh)
                {
                    mat[_nH][_nW] = x;
                    ++_nW == _W ? _nW = 0, ++_nH : 0;
                }
            }
        }
        inline Matrix &operator=(const Matrix &other)
        {
            if (this != &other)
            {
                free();
                _H = other._H, _W = other._W;
                allocate();
                for (size_t i = 0; i < _H; ++i)
                    std::copy(other.mat[i], other.mat[i] + _W, mat[i]);
            }
            return *this;
        }

        inline Matrix &operator=(Matrix &&other) noexcept
        {
            if (this != &other)
            {
                free();
                mat = other.mat, _H = other._H, _W = other._W;
                other.mat = nullptr, other._H = 0, other._W = 0;
            }
            return *this;
        }
        inline operator dpointer() noexcept { return mat; }
        inline reference getVal(const size_t &x, const size_t &y)
        {
            if (x > _H || y > _W)
                throw std::out_of_range("invalid access");
            return mat[x][y];
        }
        inline const reference getVal(const size_t &x, const size_t &y) const
        {
            if (x > _H || y > _W)
                throw std::out_of_range("invalid access");
            return mat[x][y];
        }
        inline constexpr size_t getH() const { return _H; }
        inline constexpr size_t getW() const { return _W; }
        inline constexpr size_t size() const { return _H * _W; }
        class iterator
        {
            Matrix &_M;
            size_t _nH, _nW;

        public:
            iterator(Matrix &M, size_t nH, size_t nW) : _M(M), _nH(nH), _nW(nW) {}
            inline reference operator*() { return _M.getVal(_nH, _nW); }
            inline iterator &operator++()
            {
                ++_nW == _M.getW() ? _nW = 0, ++_nH : 0;
                return *this;
            }
            inline bool operator!=(const iterator &x) noexcept { return !(_nH == x._nH && _nW == x._nW); }
            inline pointer operator->() { return &_M.getVal(_nH, _nW); }
        };
        inline iterator begin() noexcept { return iterator(*this, 0, 0); }
        inline iterator end() noexcept { return iterator(*this, _H, 0); }

    private:
        size_t _H, _W;
        dpointer mat;
        inline void free()
        {
            if (mat == nullptr)
                return;
            for (size_t i = 0; i < _H; ++i)
                if (mat[i])
                    delete[] mat[i];
            delete[] mat;
            mat = nullptr;
        }
        inline void allocate()
        {
            mat = new pointer[_H];
            for (size_t i = 0; i < _H; ++i)
                mat[i] = new value_type[_W]{};
        }
    };
    template <typename _T>
    inline Matrix<_T> operator*(const Matrix<_T> &x, const Matrix<_T> &y)
    {
        if (x.getW() != y.getH())
            throw std::invalid_argument("invalid matrix size");
        size_t _Hx = x.getH(), _Wy = y.getW(), _HW = x.getW();
        Matrix<_T> res = Matrix<_T>(_Hx, _Wy);
        for (auto i = 0; i < _Hx; ++i)
            for (auto j = 0; j < _Wy; ++j)
                for (auto k = 0; k < _HW; ++k)
                    res.getVal(i, j) = res.getVal(i, j) + x.getVal(i, k) * y.getVal(k, j);
        return res;
    }
    template <typename _T>
    inline Matrix<_T> operator+(const Matrix<_T> &x, const Matrix<_T> &y)
    {
        if (x.getH() != y.getH() || x.getW() != y.getW())
            throw std::invalid_argument("invalid matrix size");
        size_t _H = x.getH(), _W = x.getW();
        Matrix<_T> res = Matrix<_T>(_H, _W);
        for (auto i = 0; i < _H; ++i)
            for (auto j = 0; j < _W; ++j)
                res.getVal(i, j) = x.getVal(i, j) + y.getVal(i, j);
        return res;
    }

    template <size_t _H, size_t _W, typename _T = int>
    class Static_Matrix
    {
    public:
        using value_type = _T;
        using reference = value_type &;
        using pointer = value_type *;
        using dpointer = pointer *;
        Static_Matrix() { allocate(); }
        ~Static_Matrix() { free(); }
        Static_Matrix(const Static_Matrix &other)
        {
            allocate();
            for (size_t i = 0; i < _H; ++i)
                std::copy(other.mat[i], other.mat[i] + _W, mat[i]);
        }
        Static_Matrix(Static_Matrix &&other) noexcept : mat(other.mat)
        {
            other.mat = nullptr;
        }
        Static_Matrix(std::initializer_list<value_type> _ls)
        {
            if (_ls.size() != size())
                throw std::length_error("invalid initializer list size");
            allocate();
            size_t _nH = 0, _nW = 0;
            for (auto &x : _ls)
            {
                mat[_nH][_nW] = x;
                ++_nW == _W ? _nW = 0, ++_nH : 0;
            }
        }
        Static_Matrix(std::initializer_list<std::initializer_list<value_type>> _ls)
        {
            if (_ls.size() != _H)
                throw std::length_error("invalid initializer list size");
            allocate();
            size_t _nH = 0, _nW = 0;
            for (auto &_lsh : _ls)
            {
                if (_lsh.size() != _W)
                {
                    free();
                    throw std::length_error("invalid initializer list size");
                }
                for (auto &x : _lsh)
                {
                    mat[_nH][_nW] = x;
                    ++_nW == _W ? _nW = 0, ++_nH : 0;
                }
            }
        }
        inline Static_Matrix &operator=(const Static_Matrix &other)
        {
            if (this != &other)
            {
                free();
                allocate();
                for (size_t i = 0; i < _H; ++i)
                    std::copy(other.mat[i], other.mat[i] + _W, mat[i]);
            }
            return *this;
        }

        inline Static_Matrix &operator=(Static_Matrix &&other) noexcept
        {
            if (this != &other)
            {
                free();
                mat = other.mat;
                other.mat = nullptr;
            }
            return *this;
        }
        inline operator dpointer() noexcept { return mat; }
        inline reference getVal(const size_t &x, const size_t &y)
        {
            if (x > _H || y > _W)
                throw std::out_of_range("invalid access");
            return mat[x][y];
        }
        inline const reference getVal(const size_t &x, const size_t &y) const
        {
            if (x > _H || y > _W)
                throw std::out_of_range("invalid access");
            return mat[x][y];
        }
        inline constexpr size_t getH() const { return _H; }
        inline constexpr size_t getW() const { return _W; }
        inline constexpr size_t size() const { return _H * _W; }
        class iterator
        {
            Static_Matrix &_M;
            size_t _nH, _nW;

        public:
            iterator(Static_Matrix &M, size_t nH, size_t nW) : _M(M), _nH(nH), _nW(nW) {}
            inline reference operator*() { return _M.getVal(_nH, _nW); }
            inline iterator &operator++()
            {
                ++_nW == _M.getW() ? _nW = 0, ++_nH : 0;
                return *this;
            }
            inline bool operator!=(const iterator &x) noexcept { return !(_nH == x._nH && _nW == x._nW); }
            inline pointer operator->() { return &_M.getVal(_nH, _nW); }
        };
        inline iterator begin() noexcept { return iterator(*this, 0, 0); }
        inline iterator end() noexcept { return iterator(*this, _H, 0); }

    private:
        dpointer mat;
        inline void free()
        {
            if (mat == nullptr)
                return;
            for (size_t i = 0; i < _H; ++i)
                if (mat[i])
                    delete[] mat[i];
            delete[] mat;
            mat = nullptr;
        }
        inline void allocate()
        {
            mat = new pointer[_H];
            for (size_t i = 0; i < _H; ++i)
                mat[i] = new value_type[_W]{};
        }
    };
    template <size_t _Hx, size_t _HW, size_t _Wy, typename _T>
    inline Static_Matrix<_Hx, _Wy, _T> operator*(const Static_Matrix<_Hx, _HW, _T> &x, const Static_Matrix<_HW, _Wy, _T> &y)
    {
        Static_Matrix<_Hx, _Wy, _T> res = Static_Matrix<_Hx, _Wy, _T>();
        for (auto i = 0; i < _Hx; ++i)
            for (auto j = 0; j < _Wy; ++j)
                for (auto k = 0; k < _HW; ++k)
                    res.getVal(i, j) = res.getVal(i, j) + x.getVal(i, k) * y.getVal(k, j);
        return res;
    }
    template <size_t _H, size_t _W, typename _T>
    inline Static_Matrix<_H, _W, _T> operator+(const Static_Matrix<_H, _W, _T> &x, const Static_Matrix<_H, _W, _T> &y)
    {
        Static_Matrix<_H, _W, _T> res = Static_Matrix<_H, _W, _T>();
        for (auto i = 0; i < _H; ++i)
            for (auto j = 0; j < _W; ++j)
                res.getVal(i, j) = x.getVal(i, j) + y.getVal(i, j);
        return res;
    }
}