// FileName : bigint.hpp
// High-Precision Int Packed Class Header
// Programmed By MightZero
// Copyright 2025 MightZero
#pragma once
#ifndef _MZLIB_BIGINT_HPP
#define _MZLIB_BIGINT_HPP
#endif
#include <stdexcept>
#include <stddef.h>
#include <vector>
#include <cmath>
#include <iterator>
#include <string>
#include <complex>

namespace MZLIB
{
    struct FFT
    {
        FFT()=delete;
        using fft_float=long double;
        using complex=std::complex<fft_float>;
        static std::vector<long> rev;
        static constexpr fft_float pi=fft_float(M_PIl);
        static void fft(std::vector<complex>& _poly,int _way)
        {
            size_t n=_poly.size();
            if(rev.size()!=n)init(n);
            for(size_t i=0;i<n;++i)if(i<rev[i])std::swap(_poly[i],_poly[rev[i]]);
            for(size_t m=2;m<=n;m<<=1)
            {
                auto mid=m>>1;
                complex wn=std::exp(complex(0,_way*2*pi/m));
                for(size_t j=0;j<n;j+=m)
                {
                    complex w=1;
                    for(size_t k=0;k<mid;++k)
                    {
                        complex x=_poly[j+k],y=w*_poly[j+k+mid];
                        _poly[j+k]=x+y,_poly[j+k+mid]=x-y,w=w*wn;
                    }
                }
            }
            if(_way==-1)for(auto &x:_poly)x/=n;
        }
        static void init(size_t n)
        {
            size_t lgn=63-__builtin_clzll(n);
            rev.assign(n,0);
            for(size_t i=0;i<n;++i)rev[i]=(rev[i>>1]>>1)|((i&1)<<lgn-1);
        }
    };
    std::vector<long> FFT::rev=std::vector<long>(0);
    template<typename _T> 
    inline constexpr _T pow10(size_t ex)
    {
        _T res=1,tmp=10;
        while(ex)
        {
            if(ex&1)res=res*tmp;
            tmp=tmp*tmp,ex>>=1;
        }
        return res;
    }

    template<typename _T=long>
    class BigInt
    {
    public:
        static_assert(std::is_convertible<_T,long>::value,"Invalid element type");
        using element_type=_T;
        BigInt(){_dat.resize(0),_flag=1;}
        BigInt(long _val)
        {
            *this=BigInt();
            auto it=std::back_inserter(_dat);
            if(_val<0)_flag=-1,_val=-_val;
            *it=0;
            element_type npow=1;
            size_t nexp=0;
            while(_val)
            {
                if(nexp==_bitcnt)*(++it)=0,npow=1,nexp=0;
                _dat.back()=_dat.back()+npow*(_val%10);
                _val/=10,npow*=10,++nexp;
            }
            this->simplify();
        }
        BigInt(const std::string _val)
        {
            *this=BigInt();
            auto it=std::back_inserter(_dat);
            if(_val.front()=='-')_flag=-1;
            *it=0;
            element_type npow=1;
            size_t nexp=0;
            for(auto itv=_val.rbegin();itv!=_val.rend();++itv)
            {
                if(*itv=='-'&&itv+1==_val.rend())continue;
                if(!isdigit(*itv))throw std::invalid_argument("invalid BigInt initial string");
                if(nexp==_bitcnt)*(++it)=0,npow=1,nexp=0;
                _dat.back()=_dat.back()+npow*(*itv-'0');
                npow*=10,++nexp;
            }
            this->simplify();
        }
        inline static int bitcnt() {return _bitcnt;}
        inline int& flag() {return _flag;}
        inline const int& flag() const {return _flag;}
        inline std::vector<element_type>& vec(){return _dat;} 
        inline const std::vector<element_type>& vec()const{return _dat;} 
        inline operator std::string() const
        {
            std::string _str="";
            for(auto it=_dat.begin();it!=_dat.end();++it)
            {
                auto _val=*it;
                for(auto i=0;i<_bitcnt;++i)
                {
                    _str.push_back(_val%10+'0');
                    _val/=10;
                }
            }
            while(_str.size()&&_str.back()=='0')_str.pop_back();
            if(!_str.size())_str.push_back('0');
            if(_flag==-1)_str.push_back('-');
            reverse(_str.begin(),_str.end());
            return _str;
        }
        inline void simplify()
        {
            while(_dat.size()&&_dat.back()==0)_dat.pop_back();
            if(!_dat.size())_flag=1;
        }
        inline friend BigInt abs(const BigInt& _val)
        {
            BigInt _tmp=_val;
            _tmp.flag()=1;
            return _tmp;
        }
        inline size_t digit_count() const 
        {
            this->simplify();
            if (_dat.empty()) return 0;
            size_t sz=_dat.size()*_bitcnt;
            element_type tmp = _dat.back();
            while(tmp)++sz,tmp/=10;
            return sz;
        }
        inline static int compare(const BigInt& _lsh,const BigInt& _rsh)
        {
            if(_lsh.flag()!=_rsh.flag())return _lsh.flag()>_rsh.flag()?1:-1;
            if(_lsh.vec().size()!=_rsh.vec().size())return _lsh.flag()*(_lsh.vec().size()>_rsh.vec().size()?1:-1);
            int p=0;
            for(auto itl=_lsh.vec().rbegin(),itr=_rsh.vec().rbegin();itl!=_lsh.vec().rend();++itl,++itr)if(*itl!=*itr)return _lsh.flag()*(*itl>*itr?1:-1);
            return 0;
        }
        inline friend bool operator==(const BigInt& _lsh,const BigInt& _rsh){return compare(_lsh,_rsh)==0;}
#if __cplusplus>=202002L
        inline friend int operator<=>(const BigInt& _lsh,const BigInt& _rsh){return compare(_lsh,_rsh);}
#else
        inline friend bool operator!=(const BigInt& _lsh,const BigInt& _rsh){return compare(_lsh,_rsh)!=0;}
        inline friend bool operator<(const BigInt& _lsh,const BigInt& _rsh){return compare(_lsh,_rsh)<0;}
        inline friend bool operator>(const BigInt& _lsh,const BigInt& _rsh){return compare(_lsh,_rsh)>0;}
        inline friend bool operator<=(const BigInt& _lsh,const BigInt& _rsh){return compare(_lsh,_rsh)<=0;}
        inline friend bool operator>=(const BigInt& _lsh,const BigInt& _rsh){return compare(_lsh,_rsh)>=0;}
#endif
        inline BigInt operator-() const
        {
            BigInt _tmp=*this;
            _tmp.flag()=-_tmp.flag();
            return _tmp;
        }
        inline friend BigInt operator+(const BigInt& _lsh,const BigInt& _rsh)
        {
            if(_lsh.flag()==-1&&_rsh.flag()==1){return _rsh-(-_lsh);};
            if(_lsh.flag()==1&&_rsh.flag()==-1){return _lsh-(-_rsh);};
            BigInt ans=BigInt(),lsh=_lsh,rsh=_rsh;
            ans.flag()=lsh.flag();
            auto it=back_inserter(ans.vec());
            size_t sz=std::max(lsh.vec().size(),rsh.vec().size());
            lsh.vec().resize(sz+1,0),rsh.vec().resize(sz+1,0);
            element_type lst=0;
            for(auto itl=lsh.vec().begin(),itr=rsh.vec().begin();itl!=lsh.vec().end();++itl,++itr)
            {
                *it=(*itl+*itr+lst)%_limit;
                lst=(*itl+*itr+lst)/_limit;
            }
            ans.simplify();
            return ans;
        }
        inline friend BigInt operator-(const BigInt& _lsh,const BigInt& _rsh)
        {
            if(_lsh.flag()==1&&_rsh.flag()==-1)return _lsh+(-_rsh);
            if(_lsh.flag()==-1&&_rsh.flag()==1)return (-_lsh)+_rsh;
            BigInt ans=BigInt(),lsh=abs(_lsh),rsh=abs(_rsh);
            ans.flag()=_lsh.flag();
            if(lsh<rsh)std::swap(lsh,rsh),ans.flag()=-ans.flag();
            auto it=back_inserter(ans.vec());
            size_t sz=std::max(lsh.vec().size(),rsh.vec().size());
            lsh.vec().resize(sz,0),rsh.vec().resize(sz,0);
            element_type lst=0;
            for(auto itl=lsh.vec().begin(),itr=rsh.vec().begin();itl!=lsh.vec().end();++itl,++itr)
            {
                *it=(*itl-*itr+lst+_limit)%_limit;
                if(*itl-*itr+lst>=0)lst=0;
                else lst=-((-*itl+*itr-lst)/_limit+!!((-*itl+*itr-lst)%_limit));
            }
            ans.simplify();
            return ans;
        }
        inline friend BigInt operator*(const BigInt& _lsh,const BigInt& _rsh)
        {
            size_t n=1;
            while(n<_lsh.vec().size()+_rsh.vec().size())n<<=1;
            std::vector<FFT::complex> vc(_lsh.vec().begin(),_lsh.vec().end());
            vc.resize(n);
            for(size_t i=0;i<_rsh.vec().size();++i)vc[i].imag(_rsh.vec()[i]);
            FFT::fft(vc,1);
            for(auto &x:vc)x=x*x;
            FFT::fft(vc,-1);
            BigInt ans=0;
            ans.vec().assign(n,0);
            for(size_t i=0;i<n;++i)
            {
                ans.vec()[i]+=(element_type(std::round(vc[i].imag()/2)));
                if(i<n-1)ans.vec()[i+1]+=ans.vec()[i]/_limit,ans.vec()[i]%=_limit;
            }
            while(ans.vec().back()>=_limit)
            {
                element_type res=ans.vec().back()/_limit;
                ans.vec().back()%=_limit,ans.vec().push_back(res);
            }
            ans.flag()=_lsh.flag()*_rsh.flag();
            ans.simplify();
            return ans;
        }
        inline friend BigInt operator<<(const BigInt &_lsh, const size_t &_rsh)
        {
            BigInt ans=BigInt();
            size_t rsh=_rsh;
            for (; _lsh.bitcnt()<=rsh; rsh-=_lsh.bitcnt())
                ans.vec().push_back(0);
            element_type sval=pow10<element_type>(_rsh),lst=0;
            auto it=std::back_inserter(ans.vec());
            for (auto itl=_lsh.vec().begin(); itl!=_lsh.vec().end();++itl)
            {
                *it=((*itl)*sval + lst)%_limit;
                lst =((*itl)*sval + lst)/_limit;
            }
            while(lst)*it=lst%_limit, lst=lst/_limit;
            ans.flag()=_lsh.flag();
            ans.simplify();
            return ans;
        }
        inline friend BigInt operator>>(const BigInt &_lsh, const size_t &_rsh)
        {
            size_t shr=_rsh/_bitcnt+(_rsh%_bitcnt>0),shp=_bitcnt-_rsh%_bitcnt;
            BigInt ans=_lsh<<shp;
            reverse(ans.vec().begin(),ans.vec().end());
            while(ans.vec().size()&&shr--)ans.vec().pop_back();
            reverse(ans.vec().begin(),ans.vec().end());
            ans.simplify();
            return ans;
        }

        // Todo: faster divmod algorithm
        inline friend std::pair<BigInt,BigInt> divmod(const BigInt& _lsh, const BigInt& _rsh)
        {
            if(_rsh==0)throw std::invalid_argument("divisor cannot be zero");
            BigInt ans=0,pw=1,lsh=abs(_lsh),rsh=abs(_rsh);
            while(lsh>=rsh)rsh=rsh<<1,pw=pw<<1;
            while(pw>=1)
            {
                while(abs(lsh)>=abs(rsh))lsh=lsh-rsh,ans=ans+pw;
                rsh=rsh>>1,pw=pw>>1;
            }
            ans.flag()=_lsh.flag()*_rsh.flag();
            ans.simplify(),lsh.simplify();
            return {ans,lsh};
        }
        inline friend BigInt operator/(const BigInt& _lsh, const BigInt& _rsh){return divmod(_lsh,_rsh).first;}
        inline friend BigInt operator%(const BigInt& _lsh, const BigInt& _rsh){return divmod(_lsh,_rsh).second;}


        inline BigInt& operator+=(const BigInt& _rsh){return (*this)=(*this)+_rsh;}
        inline BigInt& operator-=(const BigInt& _rsh){return (*this)=(*this)-_rsh;}
        inline BigInt& operator*=(const BigInt& _rsh){return (*this)=(*this)*_rsh;}
        inline BigInt& operator/=(const BigInt& _rsh){return (*this)=(*this)/_rsh;}
        inline BigInt& operator%=(const BigInt& _rsh){return (*this)=(*this)%_rsh;}
        inline BigInt& operator<<=(const size_t& _rsh){return (*this)=(*this)<<_rsh;}
        inline BigInt& operator>>=(const size_t& _rsh){return (*this)=(*this)>>_rsh;}
        inline BigInt& operator++(){return (*this)=(*this)+1;}
        inline BigInt operator++(int){BigInt tmp=*this;return (*this)=(*this)+1,tmp;}
        inline BigInt& operator--(){return (*this)=(*this)-1;}
        inline BigInt operator--(int){BigInt tmp=*this;return (*this)=(*this)-1,tmp;}

    private:
        static constexpr size_t _bitcnt=sizeof(element_type)*__CHAR_BIT__/16;
        static constexpr element_type _limit=pow10<element_type>(_bitcnt);
        std::vector<element_type> _dat;
        int _flag;
    };
}