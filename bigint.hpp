// FileName : bigint.hpp
// High-Precision Int Packed Class Header
// Programmed By MightZero
// Copyright 2025 MightZero
#pragma once
#ifndef _MZLIB_BIGINT_HPP
#define _MZLIB_BIGINT_HPP
#endif
#include <stdexcept>
#include <algorithm>
#include <stddef.h>
#include <vector>
#include <cmath>
#include <iterator>
#include <string>
#include <complex>
#include <stdint.h>

namespace MZLIB
{
    template<typename _Type>
    struct FFT
    {
        FFT()=delete;
        using fft_float=_Type;
        using complex=std::complex<fft_float>;
        static constexpr fft_float pi=fft_float(M_PI);
        inline static uint64_t bitreverse(uint64_t x)
        {
            x=((x&0xaaaaaaaaaaaaaaaa)>>1)|((x&0x5555555555555555)<<1);
            x=((x&0xcccccccccccccccc)>>2)|((x&0x3333333333333333)<<2);
            x=((x&0xf0f0f0f0f0f0f0f0)>>4)|((x&0x0f0f0f0f0f0f0f0f)<<4);
            x=((x&0xff00ff00ff00ff00)>>8)|((x&0x00ff00ff00ff00ff)<<8);
            x=((x&0xffff0000ffff0000)>>16)|((x&0x0000ffff0000ffff)<<16);
            return (x<<32)|(x>>32);
        }
        inline static uint64_t bitreverse(uint64_t x,size_t k)
        {
            uint64_t mask=(1ull<<k)-1;
            return (bitreverse(x&mask)>>(64-k))|(x&~mask);
        }
        static void fft(std::vector<complex>& _poly,int _way)
        {
            size_t n=1,pw=0;
            while(_poly.size()>n)n<<=1,++pw;
            _poly.resize(n);
            for(size_t i=0,j;j=bitreverse(i,pw),i<n;++i)if(i<j)std::swap(_poly[i],_poly[j]);
            for(size_t m=2;m<=n;m<<=1)
            {
                complex wn(cos(_way*2*pi/m),sin(_way*2*pi/m));
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
    };
    
    template<typename _Type=int,typename _Container=std::vector<int>,typename _FFT_Type=double>
    class BigInt
    {
    public:
        using container_type=_Container;
        using element_type=_Type;
        using fft_float=_FFT_Type;
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
        inline friend bool operator==(const BigInt& _lhs,const BigInt& _rhs){return compare(_lhs,_rhs)==0;}
#if __cplusplus>=202002L
        inline friend int operator<=>(const BigInt& _lhs,const BigInt& _rhs){return compare(_lhs,_rhs);}
#else
        inline friend bool operator!=(const BigInt& _lhs,const BigInt& _rhs){return compare(_lhs,_rhs)!=0;}
        inline friend bool operator<(const BigInt& _lhs,const BigInt& _rhs){return compare(_lhs,_rhs)<0;}
        inline friend bool operator>(const BigInt& _lhs,const BigInt& _rhs){return compare(_lhs,_rhs)>0;}
        inline friend bool operator<=(const BigInt& _lhs,const BigInt& _rhs){return compare(_lhs,_rhs)<=0;}
        inline friend bool operator>=(const BigInt& _lhs,const BigInt& _rhs){return compare(_lhs,_rhs)>=0;}
#endif
        inline BigInt operator-() const
        {
            BigInt _tmp=*this;
            _tmp.flag()=-_tmp.flag();
            return _tmp;
        }
        inline friend BigInt operator+(const BigInt& _lhs,const BigInt& _rhs)
        {
            if(_lhs.flag()==-1&&_rhs.flag()==1){return _rhs-(-_lhs);};
            if(_lhs.flag()==1&&_rhs.flag()==-1){return _lhs-(-_rhs);};
            BigInt ans=BigInt(),lhs=_lhs,rhs=_rhs;
            ans.flag()=lhs.flag();
            auto it=back_inserter(ans._dat);
            size_t sz=std::max(lhs._dat.size(),rhs._dat.size());
            lhs._dat.resize(sz+1,0),rhs._dat.resize(sz+1,0);
            element_type lst=0;
            for(auto itl=lhs._dat.begin(),itr=rhs._dat.begin();itl!=lhs._dat.end();++itl,++itr)
            {
                *it=(*itl+*itr+lst)%_limit;
                lst=(*itl+*itr+lst)/_limit;
            }
            ans.update();
            return ans;
        }
        inline friend BigInt operator-(const BigInt& _lhs,const BigInt& _rhs)
        {
            if(_lhs.flag()==1&&_rhs.flag()==-1)return _lhs+(-_rhs);
            if(_lhs.flag()==-1&&_rhs.flag()==1)return (-_lhs)+_rhs;
            BigInt ans=BigInt(),lhs=abs(_lhs),rhs=abs(_rhs);
            ans.flag()=_lhs.flag();
            if(lhs<rhs)std::swap(lhs,rhs),ans.flag()=-ans.flag();
            auto it=back_inserter(ans._dat);
            size_t sz=std::max(lhs._dat.size(),rhs._dat.size());
            lhs._dat.resize(sz,0),rhs._dat.resize(sz,0);
            element_type lst=0;
            for(auto itl=lhs._dat.begin(),itr=rhs._dat.begin();itl!=lhs._dat.end();++itl,++itr)
            {
                *it=(*itl-*itr+lst+_limit)%_limit;
                if(*itl-*itr+lst>=0)lst=0;
                else lst=-((-*itl+*itr-lst)/_limit+!!((-*itl+*itr-lst)%_limit));
            }
            ans.update();
            return ans;
        }
        inline friend BigInt operator*(const BigInt& _lhs,const BigInt& _rhs)
        {
            size_t n=1;
            while(n<_lhs._dat.size()+_rhs._dat.size())n<<=1;
            std::vector<typename FFT<fft_float>::complex> vc(_lhs._dat.begin(),_lhs._dat.end());
            vc.resize(n);
            for(size_t i=0;i<_rhs._dat.size();++i)vc[i].imag(_rhs._dat[i]);
            FFT<fft_float>::fft(vc,1);
            for(auto &x:vc)x=x*x;
            FFT<fft_float>::fft(vc,-1);
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
            ans.flag()=_lhs.flag()*_rhs.flag();
            ans.update();
            return ans;
        }
        inline friend BigInt operator<<(const BigInt &_lhs, const size_t &_rhs)
        {
            BigInt ans=BigInt();
            size_t rhs=_rhs;
            for (; _bitcnt<=rhs; rhs-=_bitcnt)
                ans._dat.push_back(0);
            element_type sval=_pow10[rhs],lst=0;
            auto it=std::back_inserter(ans._dat);
            for (auto itl=_lhs._dat.begin(); itl!=_lhs._dat.end();++itl)
            {
                *it=((*itl)*sval+lst)%_limit;
                lst =((*itl)*sval+lst)/_limit;
            }
            while(lst)*it=lst%_limit, lst=lst/_limit;
            ans.flag()=_lhs.flag();
            ans.update();
            return ans;
        }
        inline friend BigInt operator>>(const BigInt &_lhs, const size_t &_rhs)
        {
            size_t shr=_rhs/_bitcnt+(_rhs%_bitcnt>0),shp=(_bitcnt-_rhs%_bitcnt)%_bitcnt;
            BigInt ans=_lhs<<shp;
            reverse(ans._dat.begin(),ans._dat.end());
            while(ans._dat.size()&&shr--)ans._dat.pop_back();
            reverse(ans._dat.begin(),ans._dat.end());
            ans.update();
            return ans;
        }
        inline friend std::pair<BigInt,BigInt> divmod(const BigInt& _lhs, const BigInt& _rhs)
        {
            if(_rhs==0)throw std::invalid_argument("divisor cannot be zero");
            BigInt ans=0,pw=1,lhs=abs(_lhs),rhs=abs(_rhs);
            while(lhs>=rhs)rhs=rhs<<1,pw=pw<<1;
            while(pw>=1)
            {
                while(abs(lhs)>=abs(rhs))lhs=lhs-rhs,ans=ans+pw;
                rhs=rhs>>1,pw=pw>>1;
            }
            ans.flag()=_lhs.flag()*_rhs.flag();
            ans.update(),lhs.update();
            return {ans,lhs};
        }
        inline friend std::pair<BigInt, BigInt> fast_divmod(const BigInt& _lhs, const BigInt& _rhs)
        {
            if(_rhs==0)throw std::invalid_argument("divisor cannot be zero");
            BigInt lhs=abs(_lhs), rhs=abs(_rhs);
            if(lhs<rhs)return {BigInt(0),_lhs};
            if(rhs==1)return {_lhs,BigInt(0)};
            static constexpr size_t NEWTON_MIN_LEVEL=16;
            struct{
                BigInt operator()(const BigInt& num, size_t n) const 
                {
                    if(num==0)throw std::invalid_argument("divisor cannot be zero");
                    if(std::min(num.size(),n-num.size())<=NEWTON_MIN_LEVEL)return divmod(BigInt(1)<<n,num).first;
                    size_t k=(n-num.size()+2)>>1,k2=k>num.size()?0:num.size()-k;
                    BigInt x=num>>k2;
                    size_t n2=k+x.size();
                    BigInt y=(*this)(x,n2),a=y+y,b=num*y*y;
                    return (a<<(n-n2-k2))-(b>>(2*(n2+k2)-n))-1;
                }
            }newton_inv;
            size_t k=lhs.size()-rhs.size()+2,k2=(k>rhs.size()?0:rhs.size()-k);
            BigInt adjusted_rhs=rhs>>k2;
            if(k2!=0)adjusted_rhs=adjusted_rhs+1;
            size_t n2=k+adjusted_rhs.size();
            BigInt inv=newton_inv(adjusted_rhs,n2);
            BigInt q=(lhs*inv)>>(n2+k2),r=lhs-q*rhs;
            while(r>=rhs)q=q+1;r=r-rhs;
            q.flag()=_lhs.flag()*_rhs.flag(),r.flag()=_lhs.flag();
            q.update(),r.update();
            return {q, r};
        }
        inline friend BigInt operator/(const BigInt& _lhs, const BigInt& _rhs){return fast_divmod(_lhs,_rhs).first;}
        inline friend BigInt operator%(const BigInt& _lhs, const BigInt& _rhs){return fast_divmod(_lhs,_rhs).second;}

        inline BigInt& operator+=(const BigInt& _rhs){return (*this)=(*this)+_rhs;}
        inline BigInt& operator-=(const BigInt& _rhs){return (*this)=(*this)-_rhs;}
        inline BigInt& operator*=(const BigInt& _rhs){return (*this)=(*this)*_rhs;}
        inline BigInt& operator/=(const BigInt& _rhs){return (*this)=(*this)/_rhs;}
        inline BigInt& operator%=(const BigInt& _rhs){return (*this)=(*this)%_rhs;}
        inline BigInt& operator<<=(const size_t& _rhs){return (*this)=(*this)<<_rhs;}
        inline BigInt& operator>>=(const size_t& _rhs){return (*this)=(*this)>>_rhs;}
        inline BigInt& operator++(){return (*this)=(*this)+1;}
        inline BigInt operator++(int){BigInt tmp=*this;return (*this)=(*this)+1,tmp;}
        inline BigInt& operator--(){return (*this)=(*this)-1;}
        inline BigInt operator--(int){BigInt tmp=*this;return (*this)=(*this)-1,tmp;}

    protected:
        static constexpr size_t _bitcnt=sizeof(element_type)/2;
        static constexpr size_t _pow10[5]={1,size_t(1e1),size_t(1e2),size_t(1e3),size_t(1e4)};
        static constexpr size_t _limit=_pow10[_bitcnt];
        size_t _size;
        container_type _dat;
        int _flag;
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
        inline static int compare(const BigInt& _lhs,const BigInt& _rhs)
        {
            if(_lhs.flag()!=_rhs.flag())return _lhs.flag()>_rhs.flag()?1:-1;
            if(_lhs._dat.size()!=_rhs._dat.size())return _lhs.flag()*(_lhs._dat.size()>_rhs._dat.size()?1:-1);
            int p=0;
            for(auto itl=_lhs._dat.rbegin(),itr=_rhs._dat.rbegin();itl!=_lhs._dat.rend();++itl,++itr)if(*itl!=*itr)return _lhs.flag()*(*itl>*itr?1:-1);
            return 0;
        }
    };
}