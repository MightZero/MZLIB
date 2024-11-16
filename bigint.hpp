// FileName : bigint.hpp
// High-Precision Int Packed Class Header
// Programmed By MightZero
// Copyright 2024 MightZero
#pragma once
#ifndef _MZLIB_BIGINT_HPP
#define _MZLIB_BIGINT_HPP
#endif
#ifndef _BIGINT_MAXBIT
#define _BIGINT_MAXBIT 1000
#endif
namespace MZLIB
{
    class BigInt
    {
        template <class T>
        static void swp(T &_x, T &_y)
        {
            T x = _x, y = _y;
            _x = y;
            _y = x;
        }
        void move(const long long _bit)
        {
            if (_bit > 0)
                for (long long i = _BIGINT_MAXBIT - 1; i >= 0; --i)
                    _d[i] = (i - _bit >= 0) ? _d[i - _bit] : 0;
            else
                for (long long i = 0; i < _BIGINT_MAXBIT; ++i)
                    _d[i] = (i - _bit < _BIGINT_MAXBIT) ? _d[i - _bit] : 0;
        }

    public:
        char _d[_BIGINT_MAXBIT];
        short f;
        // Constructor
        BigInt()
        {
            f = 1;
            for (long long i = 0; i < _BIGINT_MAXBIT; ++i)
                _d[i] = 0;
        }
        BigInt(const char *_x)
        {
            for (long long i = 0; i < _BIGINT_MAXBIT; ++i)
                _d[i] = 0;
            long long i = 0;
            while (_x[i] && i < _BIGINT_MAXBIT)
                ++i;
            --i;
            if (_x[0] == '-')
            {
                f = -1;
                _x++;
                --i;
            }
            else
                f = 1;
            for (long long j = i; i >= 0; --i)
            {
                _d[j - i] = _x[i] - '0';
            }
            if (abs(*this) == BigInt(0ll))
                f = 1;
        }
        BigInt(const long long _x)
        {
            for (long long i = 0; i < _BIGINT_MAXBIT; ++i)
                _d[i] = 0;
            f = (_x >= 0 ? 1 : -1);
            long long x = (_x > 0 ? _x : -_x), pos = 0;
            while (x)
            {
                _d[pos++] = x % 10;
                x /= 10;
            }
        }
        // Compare
        friend bool operator==(const BigInt &_x, const BigInt &_y)
        {
            if (_x.f != _y.f)
                return false;
            for (long long i = _BIGINT_MAXBIT - 1; i >= 0; --i)
            {
                if (_x._d[i] != _y._d[i])
                    return false;
            }
            return true;
        }
        friend bool operator<(const BigInt &_x, const BigInt &_y)
        {
            if (_x.f == 1 && _y.f == -1)
                return false;
            if (_x.f == -1 && _y.f == 1)
                return true;
            if (_x.f == -1 && _y.f == -1)
                return -_x > -_y;
            for (long long i = _BIGINT_MAXBIT - 1; i >= 0; --i)
            {
                if (_x._d[i] > _y._d[i])
                    return false;
                else if (_x._d[i] < _y._d[i])
                    return true;
            }
            if (_x._d[0] == _y._d[0])
                return false;
            return true;
        }
        friend bool operator>(const BigInt &_x, const BigInt &_y)
        {
            if (_x.f == 1 && _y.f == -1)
                return true;
            if (_x.f == -1 && _y.f == 1)
                return false;
            if (_x.f == -1 && _y.f == -1)
                return -_x < -_y;
            for (long long i = _BIGINT_MAXBIT - 1; i >= 0; --i)
            {
                if (_x._d[i] < _y._d[i])
                    return false;
                else if (_x._d[i] > _y._d[i])
                    return true;
            }
            if (_x._d[0] == _y._d[0])
                return false;
            return true;
        }
        friend bool operator<=(const BigInt &_x, const BigInt &_y)
        {
            if (_x.f == 1 && _y.f == -1)
                return false;
            if (_x.f == -1 && _y.f == 1)
                return true;
            if (_x.f == -1 && _y.f == -1)
                return -_x >= -_y;
            for (long long i = _BIGINT_MAXBIT - 1; i >= 0; --i)
            {
                if (_x._d[i] > _y._d[i])
                    return false;
                else if (_x._d[i] < _y._d[i])
                    return true;
            }
            return true;
        }
        friend bool operator>=(const BigInt &_x, const BigInt &_y)
        {
            if (_x.f == 1 && _y.f == -1)
                return true;
            if (_x.f == -1 && _y.f == 1)
                return false;
            if (_x.f == -1 && _y.f == -1)
                return -_x <= -_y;
            for (long long i = _BIGINT_MAXBIT - 1; i >= 0; --i)
            {
                if (_x._d[i] < _y._d[i])
                    return false;
                else if (_x._d[i] > _y._d[i])
                    return true;
            }
            return true;
        }
        // Basic Calculation
        friend BigInt abs(const BigInt &_x)
        {
            BigInt _ans = _x;
            if (_ans.f == -1)
                _ans.f = 1;
            return _ans;
        }
        friend BigInt operator-(const BigInt &_x)
        {
            BigInt _ans = _x;
            _ans.f = -_x.f;
            if (abs(_ans) == BigInt(0ll))
                _ans.f = 1;
            return _ans;
        }
        friend BigInt operator+(const BigInt &_x, const BigInt &_y)
        {
            BigInt x = _x, y = _y, _ans;
            if (x.f == -1 && y.f == 1)
                swp(x, y);
            if (x.f == 1 && y.f == -1)
                return x - (-y);
            if (x.f == -1)
                _ans.f = -1;
            else
                _ans.f = 1;
            for (long long i = 0; i < _BIGINT_MAXBIT; ++i)
            {
                _ans._d[i] += x._d[i] + y._d[i];
                if (_ans._d[i] > 9)
                {
                    _ans._d[i] = _ans._d[i] % 10;
                    ++_ans._d[i + 1];
                }
            }
            return _ans;
        }
        friend BigInt operator-(const BigInt &_x, const BigInt &_y)
        {
            BigInt x = _x, y = _y, _ans;
            if (x.f == -1 && y.f == 1)
                return -((-x) + y);
            if (x.f == 1 && y.f == -1)
                return x + (-y);
            x = abs(x), y = abs(y);
            if (x < y)
                swp(x, y);
            for (long long i = 0; i < _BIGINT_MAXBIT; ++i)
            {
                _ans._d[i] += x._d[i] - y._d[i];
                if (_ans._d[i] < 0)
                {
                    _ans._d[i] = _ans._d[i] + 10;
                    --_ans._d[i + 1];
                }
            }
            if (_x < _y)
                _ans.f = -1;
            else
                _ans.f = 1;
            return _ans;
        }
        friend BigInt operator*(const BigInt &_x, const BigInt &_y)
        {
            BigInt x = _x, y = _y, _ans;
            x = abs(x), y = abs(y);
            if (x < y)
                swp(x, y);
            long long _lx = _BIGINT_MAXBIT - 1, _ly = _BIGINT_MAXBIT - 1;
            while ((!x._d[_lx]) && _lx > 0)
            {
                _lx--;
            }
            while ((!y._d[_ly]) && _ly > 0)
            {
                _ly--;
            }
            if ((!_lx && !x._d[0]) || (!_ly && !y._d[0]))
                return _ans;
            for (long long i = 0; i <= _ly; ++i)
            {
                for (long long j = 0; j <= _lx; ++j)
                {
                    _ans._d[i + j] += x._d[j] * y._d[i];
                    if (_ans._d[i + j] > 9)
                        _ans._d[i + j + 1] += _ans._d[i + j] / 10, _ans._d[i + j] %= 10;
                }
            }
            for (long long i = 0; i < _BIGINT_MAXBIT - 1; ++i)
            {
                if (_ans._d[i] > 9)
                    _ans._d[i + 1] += _ans._d[i] / 10, _ans._d[i] %= 10;
            }
            _ans._d[_BIGINT_MAXBIT - 1] %= 10;
            if (_x.f * _y.f == -1)
                _ans.f = -1;
            else
                _ans.f = 1;
            return _ans;
        }
        friend BigInt operator/(const BigInt &_x, const BigInt &_y)
        {
            BigInt x = _x, y = _y, _tr = 1, _ans;
            long long _t = 0;
            x = abs(x), y = abs(y);
            while (y < x)
            {
                y.move(1);
                _tr.move(1);
            }
            while (x >= _y)
            {
                while (x >= y)
                {
                    x -= y;
                    _ans += _tr;
                }
                while (x < y)
                {
                    y.move(-1);
                    _tr.move(-1);
                }
            }
            if (_x.f * _y.f == -1)
                _ans.f = -1;
            else
                _ans.f = 1;
            return _ans;
        }
        friend BigInt operator%(const BigInt &_x, const BigInt &_y)
        {
            BigInt x = _x, y = _y, _tr = 1, _ans;
            long long _t = 0;
            x = abs(x), y = abs(y);
            while (y < x)
            {
                y.move(1);
                _tr.move(1);
            }
            while (x >= _y)
            {
                while (x >= y)
                {
                    x -= y;
                    _ans += _tr;
                }
                while (x < y)
                {
                    y.move(-1);
                    _tr.move(-1);
                }
            }
            return x;
        }
        inline friend BigInt operator+=(BigInt &_x, const BigInt &_y) { return _x = _x + _y; }
        inline friend BigInt operator-=(BigInt &_x, const BigInt &_y) { return _x = _x - _y; }
        inline friend BigInt operator*=(BigInt &_x, const BigInt &_y) { return _x = _x * _y; }
        inline friend BigInt operator/=(BigInt &_x, const BigInt &_y) { return _x = _x / _y; }
        inline friend BigInt operator%=(BigInt &_x, const BigInt &_y) { return _x = _x % _y; }
        // Std I/O Stream Support
#ifdef _GLIBCXX_IOSTREAM
        friend std::ostream &operator<<(std::ostream &os, BigInt _x)
        {
            if (_x.f == -1)
                os << '-';
            long long i = _BIGINT_MAXBIT - 1;
            while ((!_x._d[i] && (i > 0)))
                --i;
            for (; i >= 0; --i)
            {
                os << (char)(_x._d[i] + '0');
            }
            return os;
        }
        friend std::istream &operator>>(std::istream &is, BigInt &_x)
        {
            char *_s = new char[_BIGINT_MAXBIT + 5];
            _s[_BIGINT_MAXBIT] = 0;
            char *_bp = _s;
            for (long long i = 0; i < _BIGINT_MAXBIT; ++i)
                _x._d[i] = _s[i] = 0;
            auto pt = _s;
            char _ch = is.get();
            while (_ch == '-' || (_ch >= '0' && _ch <= '9'))
            {
                *pt = _ch, pt++;
                _ch = is.get();
            }
            long long i = _BIGINT_MAXBIT;
            while ((!_s[i]) && i >= 0)
                --i;
            if (_s[0] == '-')
            {
                _x.f = -1;
                _s++;
                --i;
            }
            else
                _x.f = 1;
            for (long long j = i; i >= 0; --i)
            {
                _x._d[j - i] = _s[i] - '0';
            }
            if (abs(_x) == BigInt(0ll))
                _x.f = 1;
            delete _bp;
            return is;
        }
#endif
    };
}
