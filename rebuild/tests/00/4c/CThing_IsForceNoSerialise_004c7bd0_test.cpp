#include <stdio.h>

struct Inner {
    char pad[0x41];
    unsigned char flags; // +0x41
};

struct CThing {
    char pad[0x70];
    Inner* m_inner; // +0x70
    bool IsForceNoSerialise();
};

bool CThing::IsForceNoSerialise() {
    Inner* p = m_inner;
    if (p != 0 && (p->flags & 0x40) != 0)
        return true;
    return false;
}

static bool call_it(CThing* t) {
    unsigned char rv = 0;
    bool (CThing::*mp)() = &CThing::IsForceNoSerialise;
    __asm {
        mov ecx, t
        mov eax, dword ptr [mp]
        call eax
        mov rv, al
    }
    return rv != 0;
}

int main() {
    CThing t;
    Inner node;

    t.m_inner = 0;
    if (call_it(&t) != false) { printf("FAIL case1\n"); return 1; }

    node.flags = 0x00;
    t.m_inner = &node;
    if (call_it(&t) != false) { printf("FAIL case2\n"); return 1; }

    node.flags = 0x40;
    if (call_it(&t) != true) { printf("FAIL case3\n"); return 1; }

    node.flags = 0xBF;
    if (call_it(&t) != false) { printf("FAIL case4\n"); return 1; }

    node.flags = 0xC1;
    if (call_it(&t) != true) { printf("FAIL case5\n"); return 1; }

    printf("OK_0x004c7bd0\n");
    return 0;
}