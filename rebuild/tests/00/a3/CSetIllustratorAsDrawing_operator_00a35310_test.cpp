#include <stdio.h>

struct Elem { unsigned char v; char pad[19]; };
struct Base { Elem elems[8]; };

static Base* g_base;
static Elem* g_elem;
static unsigned char g_byte;
static int g_called;

void callee(Base* base, Elem* elem, unsigned char b) {
    g_base = base; g_elem = elem; g_byte = b; g_called = 1;
}

struct CSetIllustratorAsDrawing {
    unsigned char flag;
    char pad[3];
    Base* base;
    int index;
    unsigned char done;
};

void op(CSetIllustratorAsDrawing* self) {
    Base* b = self->base;
    unsigned char f = self->flag;
    Elem* elem = &b->elems[self->index];
    callee(b, elem, f);
    self->done = 0;
}

int main() {
    Base base;
    CSetIllustratorAsDrawing s;
    s.flag = 0x7A;
    s.base = &base;
    s.index = 3;
    s.done = 0xFF;
    op(&s);

    int ok = 1;
    if (!g_called) ok = 0;
    if (g_base != &base) ok = 0;
    if (g_elem != &base.elems[3]) ok = 0;
    if (g_byte != 0x7A) ok = 0;
    if (s.done != 0) ok = 0;

    if (ok) printf("PARITY_OK\n");
    else printf("FAIL\n");
    return 0;
}