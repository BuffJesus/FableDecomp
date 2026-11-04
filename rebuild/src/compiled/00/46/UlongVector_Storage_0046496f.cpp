// This retail unit value-initializes POD tags and forwards copy tags by value.
// Other vector units retain the default empty-constructor tag profile.
#define FABLE_STLP_ZERO_TRUE_TAG
#pragma optimize("s",on)
#include <new>
#include "fable_stlport_vector_impl.h"
namespace _STL {
    template<> unsigned long* FABLE_FASTCALL __uninitialized_copy(const unsigned long*,const unsigned long*,unsigned long*,const __true_type&);
    template<> void vector<unsigned long>::_M_insert_overflow(unsigned long*,const unsigned long&,const __true_type&,size_t,bool);
    static __forceinline unsigned long* CopyTagged(const unsigned long* first,const unsigned long* last,unsigned long* out,__true_type tag) { return __uninitialized_copy(first,last,out,tag); }
    template<> __forceinline void fill(unsigned long* first,unsigned long* last,const unsigned long& value) {
        for(;first!=last;++first)*first=value;
    }
    template<> __forceinline unsigned long* fill_n(unsigned long* first,size_t count,const unsigned long& value) {
        for(;count>0;--count,++first)*first=value;
        return first;
    }
    template<> __forceinline unsigned long* __copy_backward_ptrs(unsigned long* first,unsigned long* last,unsigned long* result,const __true_type&) {
        ptrdiff_t bytes=reinterpret_cast<char*>(last)-reinterpret_cast<char*>(first);
        if(bytes>0)memmove(reinterpret_cast<char*>(result)-bytes,first,bytes);
        return reinterpret_cast<unsigned long*>(reinterpret_cast<char*>(result)-bytes);
    }
    template<> inline unsigned long* vector<unsigned long>::erase(unsigned long* first,unsigned long* last) {
        char* end=reinterpret_cast<char*>(_M_finish);
        char* tail=reinterpret_cast<char*>(last);
        _M_finish=end==tail ? first : reinterpret_cast<unsigned long*>(
            static_cast<char*>(memmove(first,last,end-tail))+(end-tail));
        return first;
    }
    template unsigned long* vector<unsigned long>::erase(unsigned long*,unsigned long*);
    template<> void vector<unsigned long>::_M_fill_insert(unsigned long* position,size_t count,const unsigned long& value) {
        if(count!=0) {
            if(size_t(_M_end_of_storage._M_data-_M_finish)>=count) {
                unsigned long copy=value;
                const size_t after=_M_finish-position;
                unsigned long* oldFinish=_M_finish;
                if(after>count) {
                    CopyTagged(_M_finish-count,_M_finish,_M_finish,__true_type());
                    _M_finish+=count;
                    __copy_backward_ptrs(position,oldFinish-count,oldFinish,__true_type());
                    fill(position,position+count,copy);
                } else {
                    __uninitialized_fill_n(_M_finish,count-after,copy,__true_type());
                    _M_finish+=count-after;
                    CopyTagged(position,oldFinish,_M_finish,__true_type());
                    _M_finish+=after;
                    fill(position,oldFinish,copy);
                }
            } else {
                _M_insert_overflow(position,value,__true_type(),count,false);
            }
        }
    }
}
