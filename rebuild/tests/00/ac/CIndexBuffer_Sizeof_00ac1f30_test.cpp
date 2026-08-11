#include <cstdio>

struct Desc20 {
    int f0;
    int f4;
    int f8;
    int fc;
    int f10;
};

struct IDescObj;
typedef void (__stdcall *GetInfoFn)(IDescObj* self, Desc20* out);

struct IDescVtbl {
    GetInfoFn slot0;
    GetInfoFn slot1;
    GetInfoFn slot2;
    GetInfoFn slot3;
    GetInfoFn slot4;
    GetInfoFn slot5;
    GetInfoFn slot6;
    GetInfoFn slot7;
    GetInfoFn slot8;
    GetInfoFn slot9;
    GetInfoFn slot10;
    GetInfoFn slot11;
    GetInfoFn slot12;
    GetInfoFn slot13;
};

struct IDescObj {
    IDescVtbl* vptr;
};

struct CIndexBuffer {
    int m_pad0;
    IDescObj* m_desc;
};

unsigned long __fastcall CIndexBuffer_Sizeof(CIndexBuffer* self);

static int g_arg = 0;
static void __stdcall fakeGetInfo(IDescObj* self, Desc20* out)
{
    g_arg = (self != 0) ? 1 : 0;
    out->f0 = 111;
    out->f4 = 222;
    out->f8 = 333;
    out->fc = 444;
    out->f10 = 100;
}

int main()
{
    IDescVtbl vt;
    vt.slot13 = fakeGetInfo;
    IDescObj obj;
    obj.vptr = &vt;
    CIndexBuffer ib;
    ib.m_pad0 = 0;
    ib.m_desc = &obj;

    unsigned long r = CIndexBuffer_Sizeof(&ib);
    // expect 100 + 0x18 = 124
    if (r == 124 && g_arg == 1) {
        printf("SIZEOF_OK r=%lu\n", r);
        return 0;
    }
    printf("SIZEOF_FAIL r=%lu arg=%d\n", r, g_arg);
    return 1;
}