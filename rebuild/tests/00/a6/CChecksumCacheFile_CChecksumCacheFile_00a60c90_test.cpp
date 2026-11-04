#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "fable_checksum_cache_file.h"
#include "test_support/bank_file_cleanup_stubs.h"
static int wideCtorCalls;
CWideString::CWideString() : storage_(0) { ++wideCtorCalls; }
CWideString::~CWideString() {}
static unsigned long word(const unsigned char* p,unsigned offset) {return *reinterpret_cast<const unsigned long*>(p+offset);}
int main() {
    const long inputs[4]={0,1,8,0x10000};
    for(unsigned trial=0;trial<4;++trial) {
        unsigned char storage[sizeof(CChecksumCacheFile)];memset(storage,0xA5,sizeof(storage));
        CChecksumCacheFile* cache=new (storage) CChecksumCacheFile(inputs[trial]);
        if(cache!=reinterpret_cast<CChecksumCacheFile*>(storage)||wideCtorCalls!=(int)trial+1)return 1;
        if(word(storage,0)||word(storage,8)||word(storage,0x10)!=((inputs[trial]+8)&~8)||storage[0x14])return 2;
        unsigned long node=word(storage,4);
        if(!node||*(unsigned char*)node||word((const unsigned char*)node,4)||
           word((const unsigned char*)node,8)!=node||word((const unsigned char*)node,12)!=node)return 3;
        free((void*)node);
    }
    puts("CHECKSUM_CACHE_CTOR_00a60c90_PASS");return 0;
}
