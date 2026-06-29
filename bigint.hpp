// FileName : bigint.hpp
// High-Precision Int Packed Class Header
// Programmed By MightZero
// Copyright (c) 2025-2026 MightZero
#pragma once
#ifndef _MZLIB_BIGINT_HPP
#define _MZLIB_BIGINT_HPP
#endif
#include <stdexcept>
#include <algorithm>
#include <stddef.h>
#include <vector>
#include <iterator>
#include <string>
#include <stdint.h>

namespace MZLIB
{


    template<uint32_t _Mod,uint32_t _Root,size_t _MaxExp>
    struct NTT
    {
        NTT()=delete;
        using value_type=uint32_t;
        static constexpr value_type mod=_Mod;
        static constexpr value_type root=_Root;
        static constexpr size_t max_exp=_MaxExp;
        // Montgomery inverse: -mod^(-1) mod 2^32
        static constexpr value_type inv=[](){
            value_type inv=1;
            for(int i=0;i<5;++i)inv=inv*(2-mod*inv);
            return -inv;
        }();
        // R^2 mod mod where R=2^32
        static constexpr value_type r2=[](){
            uint64_t r=uint64_t(1)<<32;
            r%=mod;
            return value_type(r*r%mod);
        }();
        static constexpr value_type powmod(value_type a,uint32_t e)
        {
            value_type r=1;
            while(e)
            {
                if(e&1)r=uint64_t(r)*a%mod;
                a=uint64_t(a)*a%mod;
                e>>=1;
            }
            return r;
        }
        static constexpr value_type mod_inv(value_type a){return powmod(a,mod-2);}
        // Montgomery multiplication: a * b * R^(-1) mod mod
        static inline value_type mont_mul(value_type a,value_type b)
        {
            uint64_t prod=uint64_t(a)*b;
            value_type q=value_type(prod)*inv;
            value_type r=value_type((prod+uint64_t(q)*mod)>>32);
            value_type sub=value_type(0)-(r>=mod);
            return r-(sub&mod);
        }
        static inline value_type to_mont(value_type x){return mont_mul(x,r2);}
        static inline value_type from_mont(value_type x){return mont_mul(x,1);}
        static inline void bit_reverse(value_type *data,size_t n)
        {
            for(size_t i=1,j=0;i<n;++i)
            {
                size_t bit=n>>1;
                for(;j&bit;bit>>=1)j^=bit;
                j^=bit;
                if(i<j)std::swap(data[i],data[j]);
            }
        }
        static void fill_roots(value_type *wtable,size_t n,int way)
        {
            for(size_t len=2,pos=0;len<=n;len<<=1)
            {
                size_t half=len>>1;
                value_type wlen=powmod(root,(mod-1)/len);
                if(way==-1)wlen=mod_inv(wlen);
                wlen=to_mont(wlen);
                wtable[pos]=to_mont(1);
                #pragma GCC ivdep
                for(size_t j=1;j<half;++j)
                    wtable[pos+j]=mont_mul(wtable[pos+j-1],wlen);
                pos+=half;
            }
        }
        static void ntt_core(value_type *__restrict__ data,size_t n,const value_type *__restrict__ w)
        {
            for(size_t len=2,pos=0;len<=n;len<<=1)
            {
                size_t half=len>>1;
                const value_type *__restrict__ wptr=w+pos;
                for(size_t i=0;i<n;i+=len)
                {
                    value_type *__restrict__ lo=data+i;
                    value_type *__restrict__ hi=data+i+half;
                    #pragma GCC ivdep
                    for(size_t j=0;j<half;++j)
                    {
                        value_type u=lo[j];
                        value_type v=mont_mul(hi[j],wptr[j]);
                        value_type sum=u+v;
                        value_type diff=u+mod-v;
                        value_type m1=value_type(0)-(sum>=mod);
                        value_type m2=value_type(0)-(diff>=mod);
                        lo[j]=sum-(m1&mod);
                        hi[j]=diff-(m2&mod);
                    }
                }
                pos+=half;
            }
        }
        static void ntt(std::vector<value_type>& poly,int way)
        {
            size_t n=poly.size();
            value_type *data=poly.data();
            for(size_t i=0;i<n;++i)data[i]=to_mont(data[i]);
            bit_reverse(data,n);
            std::vector<value_type> roots(n);
            fill_roots(roots.data(),n,way);
            ntt_core(data,n,roots.data());
            if(way==-1)
            {
                value_type inv_n=to_mont(mod_inv(n));
                for(size_t i=0;i<n;++i)
                {
                    data[i]=mont_mul(data[i],inv_n);
                    data[i]=from_mont(data[i]);
                }
            }
            else
                for(size_t i=0;i<n;++i)data[i]=from_mont(data[i]);
        }
        static std::vector<value_type> convolve(const std::vector<value_type>& a,const std::vector<value_type>& b,size_t n)
        {
            std::vector<value_type> A(n,0),B(n,0);
            for(size_t i=0;i<a.size();++i)A[i]=to_mont(a[i]);
            for(size_t i=0;i<b.size();++i)B[i]=to_mont(b[i]);
            bit_reverse(A.data(),n);
            bit_reverse(B.data(),n);
            std::vector<value_type> roots(n);
            fill_roots(roots.data(),n,1);
            ntt_core(A.data(),n,roots.data());
            ntt_core(B.data(),n,roots.data());
            for(size_t i=0;i<n;++i)
                A[i]=mont_mul(A[i],B[i]);
            bit_reverse(A.data(),n);
            fill_roots(roots.data(),n,-1);
            ntt_core(A.data(),n,roots.data());
            value_type inv_n=to_mont(mod_inv(n));
            for(size_t i=0;i<n;++i)
            {
                A[i]=mont_mul(A[i],inv_n);
                A[i]=from_mont(A[i]);
            }
            return A;
        }
    };
    using NTT1=NTT<998244353,3,23>;
    using NTT2=NTT<469762049,3,26>;
    using NTT3=NTT<1224736769,3,24>;
    
    template<typename _Type=int,typename _Container=std::vector<int>,size_t _BitCnt=9>
    class BigInt
    {
    public:
        using container_type=_Container;
        using element_type=_Type;
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
        inline explicit operator bool() const {return _dat.size();}
        inline operator std::string() const
        {
            size_t cap=_dat.size()*_bitcnt+2;
            std::string _str(cap,'\0');
            char *p=_str.data()+cap;
            for(auto it=_dat.begin();it!=_dat.end();++it)
            {
                auto _val=*it;
                for(size_t i=0;i<_bitcnt;++i)
                {
                    *--p=char('0'+_val%10);
                    _val/=10;
                }
            }
            while(p<_str.data()+cap&&*p=='0')++p;
            if(p==_str.data()+cap)return _flag==-1?"-0":"0";
            if(_flag==-1)*--p='-';
            return std::string(p,_str.data()+cap-p);
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
            uint64_t lst=0;
            for(auto itl=lhs._dat.begin(),itr=rhs._dat.begin();itl!=lhs._dat.end();++itl,++itr)
            {
                uint64_t sum=uint64_t(*itl)+uint64_t(*itr)+lst;
                *it=element_type(sum%_limit);
                lst=sum/_limit;
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
            int64_t lst=0;
            for(auto itl=lhs._dat.begin(),itr=rhs._dat.begin();itl!=lhs._dat.end();++itl,++itr)
            {
                int64_t diff=int64_t(*itl)-int64_t(*itr)+lst;
                *it=element_type(size_t((diff+_limit)%_limit));
                if(diff>=0)lst=0;
                else lst=-((-int64_t(*itl)+int64_t(*itr)-lst)/int64_t(_limit)+!!((-int64_t(*itl)+int64_t(*itr)-lst)%int64_t(_limit)));
            }
            ans.update();
            return ans;
        }
        static BigInt naive_mul(const BigInt& _lhs,const BigInt& _rhs)
        {
            const auto& a=_lhs._dat;
            const auto& b=_rhs._dat;
            BigInt ans=0;
            ans._dat.resize(a.size()+b.size(),0);
            for(size_t i=0;i<a.size();++i)
            {
                uint64_t carry=0;
                for(size_t j=0;j<b.size()||carry;++j)
                {
                    uint64_t cur=ans._dat[i+j]+uint64_t(a[i])*(j<b.size()?b[j]:0)+carry;
                    ans._dat[i+j]=element_type(cur%_limit);
                    carry=cur/_limit;
                }
            }
            ans.flag()=_lhs.flag()*_rhs.flag();
            ans.update();
            return ans;
        }
        inline friend BigInt operator*(const BigInt& _lhs,const BigInt& _rhs)
        {
            if(_lhs._dat.size()<=32||_rhs._dat.size()<=32)
                return naive_mul(_lhs,_rhs);
            size_t n=1;
            while(n<_lhs._dat.size()+_rhs._dat.size())n<<=1;
            static constexpr size_t MOD1=NTT1::mod,MOD2=NTT2::mod,MOD3=NTT3::mod;
            static constexpr size_t INV12=208783132;
            static constexpr size_t INV123=507030951;
            __int128 max_coeff=__int128(n)*(_limit-1)*(_limit-1);
            int num_moduli=1;
            if(max_coeff>=MOD1)num_moduli=2;
            if(num_moduli==2&&max_coeff>=uint64_t(MOD1)*MOD2)num_moduli=3;
            std::vector<uint32_t> a(_lhs._dat.begin(),_lhs._dat.end());
            std::vector<uint32_t> b(_rhs._dat.begin(),_rhs._dat.end());
            std::vector<uint32_t> conv1,conv2,conv3;
            if(num_moduli>=1)conv1=NTT1::convolve(a,b,n);
            if(num_moduli>=2)conv2=NTT2::convolve(a,b,n);
            if(num_moduli>=3)conv3=NTT3::convolve(a,b,n);
            __int128 carry=0;
            BigInt ans=0;
            for(size_t i=0;i<n;++i)
            {
                __int128 coeff;
                if(num_moduli>=3)
                {
                    size_t v1=conv1[i],a2=conv2[i],a3=conv3[i];
                    int64_t t1=(int64_t(a2)-int64_t(v1%MOD2))%int64_t(MOD2);
                    if(t1<0)t1+=MOD2;
                    size_t v2=(size_t(t1)*INV12)%MOD2;
                    int64_t t2=int64_t(a3%MOD3)-int64_t(v1%MOD3)-int64_t((MOD1%MOD3)*(v2%MOD3)%MOD3);
                    t2%=int64_t(MOD3);
                    if(t2<0)t2+=MOD3;
                    size_t v3=(size_t(t2)*INV123)%MOD3;
                    coeff=v1+__int128(MOD1)*v2+__int128(MOD1)*MOD2*v3;
                }
                else if(num_moduli==2)
                {
                    size_t a1=conv1[i],a2=conv2[i];
                    size_t t=(a2+MOD2-a1%MOD2)%MOD2;
                    t=(t*INV12)%MOD2;
                    coeff=a1+__int128(MOD1)*t;
                }
                else coeff=conv1[i];
                coeff+=carry;
                carry=coeff/_limit;
                ans._dat.push_back(element_type(size_t(coeff%_limit)));
            }
            while(carry)
            {
                ans._dat.push_back(element_type(size_t(carry%_limit)));
                carry/=_limit;
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
            size_t sval=_pow10[rhs],lst=0;
            auto it=std::back_inserter(ans._dat);
            for (auto itl=_lhs._dat.begin(); itl!=_lhs._dat.end();++itl)
            {
                uint64_t prod=uint64_t(*itl)*sval+lst;
                *it=element_type(prod%_limit);
                lst=prod/_limit;
            }
            while(lst){*it=element_type(lst%_limit); lst=lst/_limit;}
            ans.flag()=_lhs.flag();
            ans.update();
            return ans;
        }
        inline friend BigInt operator>>(const BigInt &_lhs, const size_t &_rhs)
        {
            size_t limb_shift=_rhs/_bitcnt,bit_shift=_rhs%_bitcnt;
            if(limb_shift>=_lhs._dat.size())return BigInt(0);
            BigInt ans;
            if(bit_shift==0)
            {
                ans._dat.assign(_lhs._dat.begin()+limb_shift,_lhs._dat.end());
            }
            else
            {
                size_t n=_lhs._dat.size()-limb_shift;
                ans._dat.resize(n,0);
                size_t sval=_pow10[bit_shift];
                size_t nsval=_pow10[_bitcnt-bit_shift];
                for(size_t i=0;i+1<n;++i)
                {
                    uint64_t cur=uint64_t(_lhs._dat[limb_shift+i])/sval
                               +(uint64_t(_lhs._dat[limb_shift+i+1])%sval)*nsval;
                    ans._dat[i]=element_type(cur);
                }
                ans._dat.back()=element_type(_lhs._dat.back()/sval);
            }
            ans._flag=_lhs._flag;
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
            static constexpr size_t NEWTON_MIN_LEVEL=4;
            struct{
                BigInt operator()(const BigInt& num, size_t n) const 
                {
                    if(num==0)throw std::invalid_argument("divisor cannot be zero");
                    if(n-num.size()<=NEWTON_MIN_LEVEL)return divmod(BigInt(1)<<n,num).first;
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
            while(r>=rhs)q=q+1,r=r-rhs;
            q.flag()=_lhs.flag()*_rhs.flag(),r.flag()=_lhs.flag();
            q.update(),r.update();
            return {q,r};
        }
        inline friend BigInt operator/(const BigInt& _lhs, const BigInt& _rhs){return fast_divmod(_lhs,_rhs).first;}
        inline friend BigInt operator%(const BigInt& _lhs, const BigInt& _rhs){return fast_divmod(_lhs,_rhs).second;}

        inline BigInt& operator+=(const BigInt& _rhs)
        {
            if(_flag==_rhs._flag)
            {
                size_t sz=std::max(_dat.size(),_rhs._dat.size());
                _dat.resize(sz+1,0);
                uint64_t lst=0;
                for(size_t i=0;i<sz;++i)
                {
                    uint64_t sum=uint64_t(_dat[i])+uint64_t(i<_rhs._dat.size()?_rhs._dat[i]:0)+lst;
                    _dat[i]=element_type(sum%_limit);
                    lst=sum/_limit;
                }
                if(lst)_dat.back()=element_type(lst);
                else _dat.pop_back();
                update();
                return *this;
            }
            BigInt tmp=*this+_rhs;
            _dat.swap(tmp._dat);_flag=tmp._flag;_size=tmp._size;
            return *this;
        }
        inline BigInt& operator-=(const BigInt& _rhs)
        {
            if(_flag==_rhs._flag)
            {
                int c=compare(abs(*this),abs(_rhs));
                if(c==0){_dat.clear();_flag=1;update();return *this;}
                if(c<0)
                {
                    BigInt tmp=abs(_rhs)-abs(*this);
                    _dat.swap(tmp._dat);_flag=-_flag;_size=tmp._size;
                    return *this;
                }
                size_t sz=std::max(_dat.size(),_rhs._dat.size());
                _dat.resize(sz,0);
                int64_t lst=0;
                for(size_t i=0;i<sz;++i)
                {
                    int64_t diff=int64_t(_dat[i])-int64_t(i<_rhs._dat.size()?_rhs._dat[i]:0)+lst;
                    if(diff>=0){_dat[i]=element_type(diff);lst=0;}
                    else{_dat[i]=element_type(diff+int64_t(_limit));lst=-1;}
                }
                update();
                return *this;
            }
            BigInt tmp=*this-_rhs;
            _dat.swap(tmp._dat);_flag=tmp._flag;_size=tmp._size;
            return *this;
        }
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
        static constexpr size_t _bitcnt=_BitCnt;
        static_assert(_bitcnt>=1&&_bitcnt<=9,"_BitCnt must be in [1,9]");
        static constexpr size_t _pow10[10]={1,size_t(1e1),size_t(1e2),size_t(1e3),size_t(1e4),size_t(1e5),size_t(1e6),size_t(1e7),size_t(1e8),size_t(1e9)};
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
                unsigned u=(unsigned)_dat.back();
                int bits=32-__builtin_clz(u);
                size_t nd=1+((uint64_t(bits-1)*0x4D104D42u)>>32);
                static constexpr unsigned p10[9]={10,100,1000,10000,100000,1000000,10000000,100000000,1000000000u};
                if(nd<9&&u>=p10[nd])++nd;
                _size+=nd;
            }
        }
        inline static int compare(const BigInt& _lhs,const BigInt& _rhs)
        {
            if(_lhs.flag()!=_rhs.flag())return _lhs.flag()>_rhs.flag()?1:-1;
            if(_lhs._dat.size()!=_rhs._dat.size())return _lhs.flag()*(_lhs._dat.size()>_rhs._dat.size()?1:-1);
            for(auto itl=_lhs._dat.rbegin(),itr=_rhs._dat.rbegin();itl!=_lhs._dat.rend();++itl,++itr)if(*itl!=*itr)return _lhs.flag()*(*itl>*itr?1:-1);
            return 0;
        }
    };
}