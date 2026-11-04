#include "fable_threaded_file.h"
#include <stdio.h>
#include <string.h>

// Controlled scalar-deleting slot verifies ECX, flag 1 and thiscall stack cleanup.
static unsigned char* g_expected;
static unsigned g_calls,g_flags,g_slot;
static void* __fastcall First(void* data,void*,unsigned flags)
{
    if(data!=g_expected)g_flags=0xffffffff;
    else g_flags=flags;
    ++g_calls;g_slot=0;
    static_cast<unsigned char*>(data)[8]=0x51;
    return data;
}
static void* __fastcall Second(void* data,void* unused,unsigned flags)
{
    void* result=First(data,unused,flags);g_slot=1;return result;
}
int main()
{
    unsigned cases=0;
    for(unsigned seed=0;seed<4;++seed)for(unsigned align=0;align<4;++align) {
        g_calls=0;g_flags=0;CCountedPointer<CThreadedFile>::DeleteData(0);
        if(g_calls!=0)return 1;++cases;
        for(unsigned slot=0;slot<2;++slot) {
            unsigned char bytes[96],expected[96];memset(bytes,0xa5+seed,sizeof(bytes));
            typedef void* (__fastcall* Callback)(void*,void*,unsigned);
            Callback table[1]={slot?Second:First};void* vtable=table;
            g_expected=bytes+32+align;memcpy(g_expected,&vtable,4);memcpy(expected,bytes,sizeof(bytes));
            expected[32+align+8]=0x51;g_calls=g_flags=0;g_slot=9;
            CCountedPointer<CThreadedFile>::DeleteData(g_expected);
            if(g_calls!=1 || g_flags!=1 || g_slot!=slot || memcmp(bytes,expected,sizeof(bytes)))return 2;
            ++cases;
        }
    }
    if(cases!=48)return 3;
    printf("THREADED_FILE_COUNTED_DELETE_PASS cases=%u\n",cases);return 0;
}
