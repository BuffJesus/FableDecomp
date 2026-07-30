#include <stdio.h>

struct Category { char pad[0x2c]; };

struct CTCInventoryBase {
    char pad0[0x20];
    Category* m_categories;   // +0x20
};

// Function-under-test: retail __fastcall (this=ecx, index on stack, ret 4).
__declspec(naked) Category* __stdcall GetCategoryWithIndex(int /*index*/)
{
    __asm {
        mov  eax, dword ptr [esp+4]
        imul eax, eax, 0x2c
        add  eax, dword ptr [ecx+0x20]
        ret  4
    }
}

// Shim: load this into ecx, push the stack arg, call.
static Category* call_getcat(CTCInventoryBase* self, int index)
{
    Category* r;
    __asm {
        mov  ecx, self
        push index
        call GetCategoryWithIndex
        mov  r, eax
    }
    return r;
}

int main()
{
    static Category buf[8];
    CTCInventoryBase inv;
    inv.m_categories = buf;

    int fails = 0;
    for (int i = 0; i < 8; ++i) {
        Category* got = call_getcat(&inv, i);
        Category* exp = (Category*)((char*)buf + i * 0x2c);
        if (got != exp) {
            printf("FAIL i=%d got=%p exp=%p\n", i, (void*)got, (void*)exp);
            ++fails;
        }
    }

    Category* c0 = call_getcat(&inv, 0);
    Category* c1 = call_getcat(&inv, 1);
    if ((char*)c1 - (char*)c0 != 0x2c) {
        printf("FAIL stride=%ld\n", (long)((char*)c1 - (char*)c0));
        ++fails;
    }

    if (fails == 0) {
        printf("OK_0x005bce3c\n");
        return 0;
    }
    return 1;
}