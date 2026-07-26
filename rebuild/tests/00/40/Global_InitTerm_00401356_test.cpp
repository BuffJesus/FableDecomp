#include <stdio.h>
typedef void (__cdecl* InitTermFunction)();
extern "C" void __cdecl initterm(InitTermFunction*, InitTermFunction*);
int main()
{
    const unsigned char expected[6] = { 0xFF,0x25,0x90,0x01,0x44,0x01 };
    const unsigned char* code = reinterpret_cast<const unsigned char*>(&initterm);
    for (int i=0;i<6;++i) if(code[i]!=expected[i]) return 1;
    printf("GLOBAL_INITTERM_TEST PASS\n"); return 0;
}
