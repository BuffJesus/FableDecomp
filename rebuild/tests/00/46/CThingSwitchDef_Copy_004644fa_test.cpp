#include <cstdio>

struct CDefClassBase;

struct CThingSwitchDef {
    char pad[0x70];
    int f70;
    void base_clone(const CDefClassBase* src);
};

static void* g_self;
static const void* g_src;

void CThingSwitchDef::base_clone(const CDefClassBase* src)
{
    g_self = this;
    g_src = src;
}

void __fastcall CThingSwitchDef_Copy(CThingSwitchDef* self, int /*edx*/, const CDefClassBase* src);

int main()
{
    CThingSwitchDef dst; dst.f70 = 0;
    CThingSwitchDef srcObj; srcObj.f70 = 0x12345678;
    CThingSwitchDef_Copy(&dst, 0, (const CDefClassBase*)&srcObj);
    if (dst.f70 == 0x12345678 && g_self == &dst && g_src == (const void*)&srcObj)
        printf("004644fa_TEST PASS\n");
    else
        printf("FAIL %x\n", dst.f70);
    return 0;
}