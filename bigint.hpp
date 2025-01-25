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
    class BigInt
    {
    public:
        using element_type=int;
        BigInt(){_dat.resize(0),_flag=1,update();}
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
            update();
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
            update();
        }
        inline int& flag() {return _flag;}
        inline const int& flag() const {return _flag;}
        inline size_t size() const {return _size;}
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
        inline friend BigInt abs(const BigInt& _val)
        {
            BigInt _tmp=_val;
            _tmp.flag()=1;
            return _tmp;
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
            auto it=back_inserter(ans._dat);
            size_t sz=std::max(lsh._dat.size(),rsh._dat.size());
            lsh._dat.resize(sz+1,0),rsh._dat.resize(sz+1,0);
            element_type lst=0;
            for(auto itl=lsh._dat.begin(),itr=rsh._dat.begin();itl!=lsh._dat.end();++itl,++itr)
            {
                *it=(*itl+*itr+lst)%_limit;
                lst=(*itl+*itr+lst)/_limit;
            }
            ans.update();
            return ans;
        }
        inline friend BigInt operator-(const BigInt& _lsh,const BigInt& _rsh)
        {
            if(_lsh.flag()==1&&_rsh.flag()==-1)return _lsh+(-_rsh);
            if(_lsh.flag()==-1&&_rsh.flag()==1)return (-_lsh)+_rsh;
            BigInt ans=BigInt(),lsh=abs(_lsh),rsh=abs(_rsh);
            ans.flag()=_lsh.flag();
            if(lsh<rsh)std::swap(lsh,rsh),ans.flag()=-ans.flag();
            auto it=back_inserter(ans._dat);
            size_t sz=std::max(lsh._dat.size(),rsh._dat.size());
            lsh._dat.resize(sz,0),rsh._dat.resize(sz,0);
            element_type lst=0;
            for(auto itl=lsh._dat.begin(),itr=rsh._dat.begin();itl!=lsh._dat.end();++itl,++itr)
            {
                *it=(*itl-*itr+lst+_limit)%_limit;
                if(*itl-*itr+lst>=0)lst=0;
                else lst=-((-*itl+*itr-lst)/_limit+!!((-*itl+*itr-lst)%_limit));
            }
            ans.update();
            return ans;
        }
        inline friend BigInt operator*(const BigInt& _lsh,const BigInt& _rsh)
        {
            size_t n=1;
            while(n<_lsh._dat.size()+_rsh._dat.size())n<<=1;
            std::vector<FFT::complex> vc(_lsh._dat.begin(),_lsh._dat.end());
            vc.resize(n);
            for(size_t i=0;i<_rsh._dat.size();++i)vc[i].imag(_rsh._dat[i]);
            FFT::fft(vc,1);
            for(auto &x:vc)x=x*x;
            FFT::fft(vc,-1);
            BigInt ans=0;
            ans._dat.assign(n,0);
            for(size_t i=0;i<n;++i)
            {
                ans._dat[i]+=(element_type(std::round(vc[i].imag()/2)));
                if(i<n-1)ans._dat[i+1]+=ans._dat[i]/_limit,ans._dat[i]%=_limit;
            }
            while(ans._dat.back()>=_limit)
            {
                element_type res=ans._dat.back()/_limit;
                ans._dat.back()%=_limit,ans._dat.push_back(res);
            }
            ans.flag()=_lsh.flag()*_rsh.flag();
            ans.update();
            return ans;
        }
        inline friend BigInt operator<<(const BigInt &_lsh, const size_t &_rsh)
        {
            BigInt ans=BigInt();
            size_t rsh=_rsh;
            for (; _bitcnt<=rsh; rsh-=_bitcnt)
                ans._dat.push_back(0);
            element_type sval=_pow10[rsh],lst=0;
            auto it=std::back_inserter(ans._dat);
            for (auto itl=_lsh._dat.begin(); itl!=_lsh._dat.end();++itl)
            {
                *it=((*itl)*sval+lst)%_limit;
                lst =((*itl)*sval+lst)/_limit;
            }
            while(lst)*it=lst%_limit, lst=lst/_limit;
            ans.flag()=_lsh.flag();
            ans.update();
            return ans;
        }
        inline friend BigInt operator>>(const BigInt &_lsh, const size_t &_rsh)
        {
            size_t shr=_rsh/_bitcnt+(_rsh%_bitcnt>0),shp=_bitcnt-_rsh%_bitcnt;
            BigInt ans=_lsh<<shp;
            reverse(ans._dat.begin(),ans._dat.end());
            while(ans._dat.size()&&shr--)ans._dat.pop_back();
            reverse(ans._dat.begin(),ans._dat.end());
            ans.update();
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
            ans.update(),lsh.update();
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
        static constexpr size_t _bitcnt=2;
        static constexpr element_type _limit=100;
        static constexpr element_type _pow10[3]={1,10,100};
        size_t _size;
        std::vector<element_type> _dat;
        int _flag;
        struct FFT
        {
            FFT()=delete;
            using fft_float=double;
            using complex=std::complex<fft_float>;
            static std::vector<size_t> rev;
            static constexpr fft_float pi=fft_float(M_PI);
            static void fft(std::vector<complex>& _poly,int _way)
            {
                size_t n=_poly.size();
                if(rev.size()!=n)init(n);
                for(size_t i=0;i<n;++i)if(i<rev[i])std::swap(_poly[i],_poly[rev[i]]);
                for(size_t m=2;m<=n;m<<=1)
                {
                    complex wn=std::exp(complex(0,_way*2*pi/m));
                    auto mid=m>>1;
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
        inline void update()
        {
            while(_dat.size()&&_dat.back()==0)_dat.pop_back();
            if(!_dat.size())_flag=1,_size=1;
            else
            {
                _size=(_dat.size()-1)*_bitcnt;
                element_type tmp = _dat.back();
                while(tmp)++_size,tmp/=10;
            }
        }
        inline static int compare(const BigInt& _lsh,const BigInt& _rsh)
        {
            if(_lsh.flag()!=_rsh.flag())return _lsh.flag()>_rsh.flag()?1:-1;
            if(_lsh._dat.size()!=_rsh._dat.size())return _lsh.flag()*(_lsh._dat.size()>_rsh._dat.size()?1:-1);
            int p=0;
            for(auto itl=_lsh._dat.rbegin(),itr=_rsh._dat.rbegin();itl!=_lsh._dat.rend();++itl,++itr)if(*itl!=*itr)return _lsh.flag()*(*itl>*itr?1:-1);
            return 0;
        }
    };
    std::vector<size_t> BigInt::FFT::rev=std::vector<size_t>(0);
}