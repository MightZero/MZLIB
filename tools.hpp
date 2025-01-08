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
    
    
}