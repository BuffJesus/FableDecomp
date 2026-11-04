#pragma once
#include "fable_stlport_vector.h"
namespace _STL {
template<class Iterator,class Value> Iterator __fastcall __find(Iterator,Iterator,const Value&,const random_access_iterator_tag&);
#ifndef FABLE_STLPORT_DECLARATIONS_ONLY
template<class Iterator,class Value> Iterator __fastcall __find(Iterator first,Iterator last,const Value& value,const random_access_iterator_tag&) {
    ptrdiff_t trips=(last-first)>>2;
    for(;trips>0;--trips) {
        if(*first==value)return first;++first;
        if(*first==value)return first;++first;
        if(*first==value)return first;++first;
        if(*first==value)return first;++first;
    }
    switch(last-first) {
    case 3: if(*first==value)return first;++first;
    case 2: if(*first==value)return first;++first;
    case 1: if(*first==value)return first;
    }
    return last;
}
#endif
}
