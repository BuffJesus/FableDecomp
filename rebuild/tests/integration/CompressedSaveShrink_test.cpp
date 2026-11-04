#include "fable_vector_map.h"
#include "fable_stlport_vector_impl.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
typedef _STL::pair<unsigned long,unsigned long> Entry;
typedef CVectorMap<unsigned long,unsigned long,CKeyPairCompareLess<unsigned long,unsigned long> > Map;
static void check(bool ok){if(!ok)abort();}
static void run(unsigned size,unsigned capacity){
    CArrayVector<unsigned char> bytes;Map offsets;
    bytes.reserve(capacity);offsets.reserve(capacity);
    for(unsigned i=0;i<size;++i){bytes.push_back((unsigned char)(i*31+5));offsets.Insert(0x80000000+i,0xffffffff-i);}
    offsets.Dirty=true;
    unsigned char* originalBytes=bytes.begin();Entry* originalOffsets=offsets.begin();
    bytes.DeallocateUnusedMemory();offsets.DeallocateUnusedMemory();
    check(bytes.size()==size&&bytes.capacity()==size&&offsets.size()==size&&offsets.capacity()==size&&offsets.Dirty);
    if(capacity==size)check(bytes.begin()==originalBytes&&offsets.begin()==originalOffsets);
    else check(bytes.begin()!=originalBytes&&offsets.begin()!=originalOffsets);
    for(unsigned i=0;i<size;++i)check(bytes[i]==(unsigned char)(i*31+5)&&offsets[i].first==0x80000000+i&&offsets[i].second==0xffffffff-i);
    unsigned char* compactBytes=bytes.begin();Entry* compactOffsets=offsets.begin();
    bytes.DeallocateUnusedMemory();offsets.DeallocateUnusedMemory();check(bytes.begin()==compactBytes&&offsets.begin()==compactOffsets);
    bytes.DeallocateMemory();offsets.CArrayVector<Entry>::DeallocateMemory();
    check(bytes.size()==0&&bytes.capacity()==0&&bytes.begin()==0&&offsets.size()==0&&offsets.capacity()==0&&offsets.begin()==0&&offsets.Dirty);
}
int main(){run(0,0);run(0,4);run(1,1);run(3,8);run(4,4);run(7,32);puts("COMPRESSED_SAVE_SHRINK_OK cases=12");return 0;}
