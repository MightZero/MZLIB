// FileName : modint.hpp
// MZLIB Tools Header
// Programmed By MightZero
// Copyright 2024 MightZero
#pragma once
#ifndef _MZLIB_TOOLS_HPP
#define _MZLIB_TOOLS_HPP
#endif
#include<type_traits>
#include<chrono>
#include<utility>

namespace MZLIB
{
    template <typename _Func, typename... _Arg>
    inline auto chrono_run(_Func&& func, _Arg&&... args)
    {
        auto beginT = std::chrono::high_resolution_clock::now();
        if constexpr (std::is_void_v<std::invoke_result_t<_Func, _Arg...>>)
        {
            func(std::forward<_Arg>(args)...);
            auto endT = std::chrono::high_resolution_clock::now();
            return std::make_pair(std::monostate(),std::chrono::duration_cast<std::chrono::nanoseconds>(endT - beginT));
        } 
        else 
        {
            auto result = func(std::forward<_Arg>(args)...);
            auto endT = std::chrono::high_resolution_clock::now();
            return std::make_pair(result, std::chrono::duration_cast<std::chrono::nanoseconds>(endT - beginT));
        }
    }

    template<typename _T>
    class range
    {
        _T _L, _R;
        class iterator
        {
            _T _pos;
            iterator(_T pos) : _pos(pos) {}

        public:
            auto operator==(const iterator &_rhs) const { return _pos == _rhs._pos; }
#if __cplusplus >= 202002
            auto operator<=>(const iterator &_rhs) const { return _pos <=> _rhs._pos; }
#else
            auto operator!=(const iterator &_rhs) const { return _pos != _rhs._pos; }
            auto operator<(const iterator &_rhs) const { return _pos < _rhs._pos; }
            auto operator>(const iterator &_rhs) const { return _pos > _rhs._pos; }
            auto operator<=(const iterator &_rhs) const { return _pos <= _rhs._pos; }
            auto operator>=(const iterator &_rhs) const { return _pos >= _rhs._pos; }
#endif
            auto operator++() { return ++_pos, *this; }
            auto operator++(int) { return iterator(_pos++); }
            auto operator*() const { return _pos; }
            auto operator[](_T _rhs) const { return _pos + _rhs; }
            friend class range;
        };

    public:
        range(_T L, _T R) : _L(L), _R(R) {}
        range(_T R) : _L(0), _R(R) {}
        iterator begin() { return iterator(_L); }
        iterator end() { return iterator(_R); }
    };
    
}