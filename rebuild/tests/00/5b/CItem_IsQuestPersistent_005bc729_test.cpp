#include <stdio.h>

struct QData {
    char pad[0x18];
    unsigned char flag;   // +0x18
    int           count;  // +0x1c
};
struct CItem {
    char pad[0x8];
    QData* q;             // +0x8
};

__declspec(naked) int __fastcall IsQuestPersistent(void* self)
{
    __asm {
        mov  eax, dword ptr [ecx+8]
        test eax, eax
        je   null_case
        cmp  byte ptr [eax+0x18], 0
        jne  true_case
        cmp  dword ptr [eax+0x1c], 0
        jg   true_case
        xor  eax, eax
        ret
    true_case:
        xor  eax, eax
        inc  eax
        ret
    null_case:
        mov  al, 1
        ret
    }
}

// Invoke __fastcall (this in ecx, no stack args), capture eax.
static int call_it(CItem* self)
{
    int r;
    void* fn = (void*)&IsQuestPersistent;
    __asm {
        mov  ecx, self
        call fn
        mov  r, eax
    }
    return r;
}

int main()
{
    // Case 1: q == null -> persistent (1)
    CItem a; a.q = 0;
    if (call_it(&a) != 1) { printf("FAIL case1 null\n"); return 1; }

    QData d;
    CItem b; b.q = &d;

    // Case 2: flag=0, count=0 -> not persistent (0)
    d.flag = 0; d.count = 0;
    if (call_it(&b) != 0) { printf("FAIL case2\n"); return 1; }

    // Case 3: flag!=0 -> persistent (1)
    d.flag = 1; d.count = 0;
    if (call_it(&b) != 1) { printf("FAIL case3\n"); return 1; }

    // Case 4: flag=0, count>0 -> persistent (1)
    d.flag = 0; d.count = 42;
    if (call_it(&b) != 1) { printf("FAIL case4\n"); return 1; }

    // Case 5: flag=0, count<0 -> not persistent (signed jg)
    d.flag = 0; d.count = -5;
    if (call_it(&b) != 0) { printf("FAIL case5\n"); return 1; }

    // Case 6: high flag byte value still counts as non-zero
    d.flag = 0xFF; d.count = -1;
    if (call_it(&b) != 1) { printf("FAIL case6\n"); return 1; }

    printf("OK_0x005bc729\n");
    return 0;
}