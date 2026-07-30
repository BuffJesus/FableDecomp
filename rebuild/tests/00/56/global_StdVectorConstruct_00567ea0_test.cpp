#include <stdio.h>
struct StdVector { void* first; void* last; void* end; };
void __fastcall StdVector_Construct(StdVector* self)
{
    self->first = 0;
    self->last  = 0;
    self->end   = 0;
}
int main()
{
    StdVector v;
    v.first = (void*)0x11111111;
    v.last  = (void*)0x22222222;
    v.end   = (void*)0x33333333;
    StdVector* pv = &v;
    __asm {
        mov ecx, pv
        call StdVector_Construct
    }
    if (v.first != 0) { printf("FAIL first\n"); return 1; }
    if (v.last  != 0) { printf("FAIL last\n");  return 1; }
    if (v.end   != 0) { printf("FAIL end\n");   return 1; }
    printf("OK_0x00567ea0\n");
    return 0;
}