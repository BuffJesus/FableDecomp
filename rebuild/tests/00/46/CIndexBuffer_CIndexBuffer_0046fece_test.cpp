// Behaviour test for CIndexBuffer ctor @ 0x0046fece
#include <stdio.h>
#include <new>

struct CIndexBuffer {
    void* a;
    void* b;
    void* c;
    CIndexBuffer();
};

CIndexBuffer::CIndexBuffer() {
    a = 0;
    b = 0;
    c = 0;
}

int main() {
    // Raw storage pre-filled with garbage so a no-op ctor would fail the test.
    void* raw[3];
    raw[0] = (void*)0xDEADBEEF;
    raw[1] = (void*)0xCAFEBABE;
    raw[2] = (void*)0x12345678;

    // Placement-new invokes the ctor with this=ecx (__fastcall / __fastcall).
    CIndexBuffer* p = new (raw) CIndexBuffer;

    if (p->a == 0 && p->b == 0 && p->c == 0 &&
        raw[0] == 0 && raw[1] == 0 && raw[2] == 0) {
        printf("OK_0x0046fece\n");
        return 0;
    }
    printf("FAIL a=%p b=%p c=%p\n", p->a, p->b, p->c);
    return 1;
}