#pragma optimize("s",on)
#include <new>
#include "fable_stlport_vector.h"
namespace {
__forceinline unsigned long* FillWords(unsigned long* output,size_t count,const unsigned long& value) {
    for(;count>0;--count,++output)*output=value;
    return output;
}
__forceinline unsigned long* CopyWords(const unsigned long* first,const unsigned long* last,unsigned long* output) {
    return last==first?output:reinterpret_cast<unsigned long*>(
        static_cast<char*>(memmove(output,first,reinterpret_cast<const char*>(last)-reinterpret_cast<const char*>(first)))
        +(reinterpret_cast<const char*>(last)-reinterpret_cast<const char*>(first)));
}
}
namespace _STL {
    template<> unsigned long* FABLE_FASTCALL __uninitialized_copy(const unsigned long* first,const unsigned long* last,unsigned long* output,const __true_type&) {
        return CopyWords(first,last,output);
    }
    template<> void vector<unsigned long>::_M_insert_overflow(unsigned long* position,const unsigned long& value,const __true_type&,size_t count,bool atend) {
        const size_t oldSize=size();
        const size_t capacity=oldSize+(max)(oldSize,count);
        unsigned long* newStart=capacity?static_cast<unsigned long*>(malloc(capacity*sizeof(unsigned long))):0;
        unsigned long* newFinish=CopyWords(_M_start,position,newStart);
        newFinish=FillWords(newFinish,count,value);
        if(!atend)newFinish=CopyWords(position,_M_finish,newFinish);
        if(_M_start)free(_M_start);
        _M_start=newStart;_M_finish=newFinish;_M_end_of_storage._M_data=newStart+capacity;
    }
}
