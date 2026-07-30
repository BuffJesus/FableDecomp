#include <stdio.h>

// ---- function under test (same source as source_cpp) ----
__declspec(naked) int __fastcall SortTreeRecursively(void* /*this ecx*/)
{
    __asm {
        mov edx, dword ptr [ecx]
        mov ecx, dword ptr [edx]
        xor eax, eax
        cmp ecx, edx
        je  done
        _emit 0x8d
        _emit 0x9b
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
    loop_top:
        mov ecx, dword ptr [ecx]
        inc eax
        cmp ecx, edx
        jne loop_top
    done:
        ret
    }
}

struct Node { Node* next; };
struct Obj  { Node* header; };

int main()
{
    // Build a circular list with a header sentinel and 3 real elements.
    // header.next -> a -> b -> c -> header
    Node header, a, b, c;
    header.next = &a;
    a.next = &b;
    b.next = &c;
    c.next = &header;
    Obj o; o.header = &header;

    // __fastcall: pass 'this' in ecx via asm shim.
    Obj* pobj = &o;
    int count = 0;
    __asm {
        mov ecx, pobj
        call SortTreeRecursively
        mov count, eax
    }
    if (count != 3) { printf("FAIL count=%d expected 3\n", count); return 1; }

    // Empty list: header.next -> header  => count 0
    Node h2; h2.next = &h2;
    Obj o2; o2.header = &h2;
    Obj* p2 = &o2;
    int c2 = 0;
    __asm {
        mov ecx, p2
        call SortTreeRecursively
        mov c2, eax
    }
    if (c2 != 0) { printf("FAIL empty=%d expected 0\n", c2); return 1; }

    printf("OK_0x005143e0 count=%d empty=%d\n", count, c2);
    return 0;
}