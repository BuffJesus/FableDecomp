#include <stdio.h>
#include <stdlib.h>
extern "C" _onexit_t __cdecl __dllonexit(_onexit_t, _onexit_t**, _onexit_t**);
int main()
{
    const unsigned char expected[6] = { 0xFF,0x25,0x74,0x01,0x44,0x01 };
    const unsigned char* code = reinterpret_cast<const unsigned char*>(&__dllonexit);
    for (int i=0;i<6;++i) if(code[i]!=expected[i]) return 1;
    printf("GLOBAL_DLLONEXIT_TEST PASS\n"); return 0;
}
