#include <cstdio>

/* Stubs for the externs referenced by the naked factory tail. Only the
 * GetSizeofClass entry path (mov eax,0xc8; ret) is exercised here. */
extern "C" void* opnew(unsigned int n) { (void)n; return 0; }
void* __fastcall bctor(void* p) { return p; }

__declspec(naked) unsigned long __fastcall CHeroMarriageDef_GetSizeofClass(void* self)
{
    __asm {
        mov eax, 0c8h
        ret
        push 32ch
        call opnew
        test eax, eax
        pop ecx
        je short L1
        mov ecx, eax
        jmp bctor
    L1:
        xor eax, eax
        ret
    }
}

int main()
{
    unsigned long r = CHeroMarriageDef_GetSizeofClass(0);
    if (r == 0xc8) {
        std::printf("CHeroMarriageDef_0044c2e2_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL got %lu\n", (unsigned long)r);
    return 1;
}