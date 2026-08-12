#include <cstdio>

struct SelfNode {
    void*         vptr;
    int           f4;
    unsigned int* f8;
};
struct SrcNode {
    void*         vptr;
    int           f4;
    unsigned int* f8;
};

void __fastcall Cons_val(SelfNode* self, SrcNode* src);

int main()
{
    unsigned int refc = 5;
    SrcNode src;
    src.vptr = 0;
    src.f4   = 0x1111;
    src.f8   = &refc;

    SelfNode self;
    self.vptr = 0;
    self.f4   = 0;
    self.f8   = 0;

    Cons_val(&self, &src);

    int ok = 1;
    if (self.vptr != (void*)0x1238c8c) ok = 0;
    if (self.f4   != 0x1111)           ok = 0;
    if (self.f8   != &refc)            ok = 0;
    if (refc      != 6)                ok = 0;   // incremented

    // null src->f8 branch: no increment, no crash
    SrcNode src2;
    src2.vptr = 0; src2.f4 = 0x2222; src2.f8 = 0;
    SelfNode self2;
    self2.vptr = 0; self2.f4 = 0; self2.f8 = (unsigned int*)0xdead;
    Cons_val(&self2, &src2);
    if (self2.f4 != 0x2222) ok = 0;
    if (self2.f8 != 0)      ok = 0;

    // null self: early return, no crash
    Cons_val(0, &src);

    if (ok) printf("CONS_VAL_OK\n");
    else    printf("CONS_VAL_FAIL\n");
    return ok ? 0 : 1;
}