#include <cstdio>
struct Category { char pad[0x2c]; };
struct CTCInventoryBase {
    char pad0[0x20];
    Category* m_begin;   // +0x20
    Category* m_end;     // +0x24
    char pad2[0x70 - 0x28];
    int m_selected;      // +0x70
};

// Behavioural reference: identical asm body, callable as __fastcall (this=ecx).
__declspec(naked) Category* __fastcall GetSelectedCategory(CTCInventoryBase*)
{
    __asm {
        mov  eax, dword ptr [ecx+0x20]
        cmp  eax, dword ptr [ecx+0x24]
        je   L_empty
        mov  eax, dword ptr [ecx+0x70]
        imul eax, eax, 0x2c
        add  eax, dword ptr [ecx+0x20]
        ret
    L_empty:
        xor  eax, eax
        ret
    }
}

int main()
{
    if (((char*)&((CTCInventoryBase*)0)->m_begin)    - (char*)0 != 0x20) { printf("BADOFF begin\n"); return 1; }
    if (((char*)&((CTCInventoryBase*)0)->m_end)      - (char*)0 != 0x24) { printf("BADOFF end\n");   return 1; }
    if (((char*)&((CTCInventoryBase*)0)->m_selected) - (char*)0 != 0x70) { printf("BADOFF sel\n");   return 1; }
    if (sizeof(Category) != 0x2c) { printf("BADSIZE\n"); return 1; }

    Category cats[4];
    CTCInventoryBase obj;
    obj.m_begin = cats;

    obj.m_end = cats;          // empty
    obj.m_selected = 0;
    if (GetSelectedCategory(&obj) != 0) { printf("FAIL empty\n"); return 1; }

    obj.m_end = cats + 4;      // non-empty
    obj.m_selected = 2;
    if (GetSelectedCategory(&obj) != &cats[2]) { printf("FAIL sel2\n"); return 1; }

    obj.m_selected = 0;
    if (GetSelectedCategory(&obj) != &cats[0]) { printf("FAIL sel0\n"); return 1; }

    obj.m_selected = 3;
    if (GetSelectedCategory(&obj) != &cats[3]) { printf("FAIL sel3\n"); return 1; }

    printf("OK_0x005bcace\n");
    return 0;
}