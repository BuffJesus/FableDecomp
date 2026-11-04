#define FABLE_STLPORT_DECLARATIONS_ONLY
#include "fable_async_bank_find.h"
#include <stdio.h>
class CBankFileAsync;
int main() {
    CBankFileAsync* entries[16];
    for(unsigned i=0;i<16;++i)entries[i]=reinterpret_cast<CBankFileAsync*>(0x1000+i*4);
    for(unsigned count=0;count<=13;++count) {
        for(unsigned target=0;target<16;++target) {
            CBankFileAsync** found=_STL::__find(entries,entries+count,entries[target],_STL::random_access_iterator_tag());
            if(found!=(target<count?entries+target:entries+count))return 1;
        }
    }
    entries[5]=entries[1];
    if(_STL::__find(entries,entries+13,entries[1],_STL::random_access_iterator_tag())!=entries+1)return 2;
    puts("ASYNC_BANK_FIND_009d6e80_PASS");return 0;
}
