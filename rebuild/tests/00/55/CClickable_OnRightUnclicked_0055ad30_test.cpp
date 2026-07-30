#include <cstdio>

static int  g_self_slot48_arg  = -999;
static void* g_self_this       = 0;
static int  g_sub_v4_arg       = -999;
static void* g_sub_this        = 0;

struct Sub {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4(int);   // slot 4 (offset 0x10)
};
struct CClickable {
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03();
    virtual void s04(); virtual void s05(); virtual void s06(); virtual void s07();
    virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15();
    virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19();
    virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27();
    virtual void s28(); virtual void s29(); virtual void s30(); virtual void s31();
    virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35();
    virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39();
    virtual void s40(); virtual void s41(); virtual void s42(); virtual void s43();
    virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47();
    virtual void s48(int);
    Sub  sub;
    char pad[0x180 - 0x8];
    int  arg;
    void OnRightUnclicked();
};
void CClickable::OnRightUnclicked() {
    this->s48(this->arg);
    this->sub.v4(0x1d);
}

static void __fastcall self48_impl(void* thisptr, void* /*edx*/, int a) {
    g_self_this = thisptr;
    g_self_slot48_arg = a;
}
static void __fastcall subv4_impl(void* thisptr, void* /*edx*/, int a) {
    g_sub_this = thisptr;
    g_sub_v4_arg = a;
}

int main() {
    // self vtable: 49 slots, slot 48 = self48_impl
    void* selfvt[49];
    for (int i = 0; i < 49; ++i) selfvt[i] = 0;
    selfvt[48] = (void*)&self48_impl;

    // sub vtable (embedded subobject): 5 slots, slot 4 (offset 0x10) = subv4_impl
    void* subvt[5] = { 0,0,0,0, (void*)&subv4_impl };

    // raw layout: +0 self vtable, +4 sub-vtable ptr (embedded sub), +0x180 arg
    char buf[0x188];
    for (int i = 0; i < (int)sizeof(buf); ++i) buf[i] = 0;
    *(void**)(buf + 0x0)   = selfvt;
    *(void**)(buf + 0x4)   = subvt;     // embedded Sub's vtable pointer
    *(int*)  (buf + 0x180) = 0x1234;

    CClickable* obj = (CClickable*)buf;
    obj->OnRightUnclicked();

    if (g_self_this != buf) { printf("FAIL self_this %p != %p\n", g_self_this, (void*)buf); return 1; }
    if (g_self_slot48_arg != 0x1234) { printf("FAIL self arg %d\n", g_self_slot48_arg); return 1; }
    if (g_sub_this != (buf + 0x4)) { printf("FAIL sub_this %p != %p\n", g_sub_this, (void*)(buf+0x4)); return 1; }
    if (g_sub_v4_arg != 0x1d) { printf("FAIL sub arg %d\n", g_sub_v4_arg); return 1; }

    printf("OK_0x0055ad30 self.arg=0x%x sub.arg=%d\n", g_self_slot48_arg, g_sub_v4_arg);
    return 0;
}