// FileName : modint.hpp
// Modulo Int Packed Class Header
// Programmed By MightZero
// Copyright 2024 MightZero
#include <stdexcept>
#include <stddef.h>
#pragma once
#ifndef _MZLIB_MODINT_HPP
#define _MZLIB_MODINT_HPP
#endif

namespace MZLIB
{
    template <size_t _MOD, typename _T = long>
    class ModInt
    {
    public:
        using value_type = _T;
        ModInt() : val(0) {}
        ModInt(value_type _val) : val(_val % _MOD) {}
        inline const value_type getval() const noexcept { return val % _MOD; }
        inline operator value_type() const noexcept { return getval(); }
        inline ModInt& operator+=(const ModInt& x){return (*this)=(*this)+x;}
        inline ModInt& operator-=(const ModInt& x){return (*this)=(*this)-x;}
        inline ModInt& operator*=(const ModInt& x){return (*this)=(*this)*x;}
        inline ModInt& operator/=(const ModInt& x){return (*this)=(*this)/x;}
    private:
        value_type val;
    };
    template <typename _T>
    inline _T modinv(_T a, size_t m)
    {
        if (m == 1)
            return 0;
        _T m0 = m, t, q, x0 = 0, x1 = 1;
        while (a > 1)
        {
            q = a / m;
            t = m;
            m = a % m, a = t;
            t = x0;
            x0 = x1 - q * x0;
            x1 = t;
        }
        x1 = (x1 < 0) ? x1 + m0 : x1;
        if (a != 1)
            throw std::domain_error("inverse does not exist");
        return x1;
    }
    template <size_t _MOD, typename _T>
    inline ModInt<_MOD, _T> operator+(const ModInt<_MOD, _T> &x, const ModInt<_MOD, _T> &y)
    {
        return ModInt<_MOD, _T>((x.getval() + y.getval()) % _MOD);
    }
    template <size_t _MOD, typename _T>
    inline ModInt<_MOD, _T> operator-(const ModInt<_MOD, _T> &x, const ModInt<_MOD, _T> &y)
    {
        return ModInt<_MOD, _T>((x.getval() - y.getval() + _MOD) % _MOD);
    }
    template <size_t _MOD, typename _T>
    inline ModInt<_MOD, _T> operator*(const ModInt<_MOD, _T> &x, const ModInt<_MOD, _T> &y)
    {
        return ModInt<_MOD, _T>((x.getval() * y.getval()) % _MOD);
    }
    template <size_t _MOD, typename _T>
    inline ModInt<_MOD, _T> operator/(const ModInt<_MOD, _T> &x, const ModInt<_MOD, _T> &y)
    {
        return ModInt<_MOD, _T>((x.getval() * modinv(y.getval(), _MOD)) % _MOD);
    }
}