#include <stdio.h>
extern "C" void __cdecl __SEH_epilog();
int main()
{
    const unsigned char expected[17] = {
        0x8B,0x4D,0xF0,0x64,0x89,0x0D,0,0,0,0,0x59,0x5F,0x5E,0x5B,0xC9,0x51,0xC3
    };
    const unsigned char* code = reinterpret_cast<const unsigned char*>(&__SEH_epilog);
    for (int i=0;i<17;++i) if(code[i]!=expected[i]) return 1;
    printf("GLOBAL_SEH_EPILOG_TEST PASS\n"); return 0;
}
