#include <cstdio>

struct CDefClassBase;

struct CTavernDef {
    char pad[0x28];
    int f28;
    void base_clone(const CDefClassBase* src);
};

static void* g_self;
static const void* g_src;

void CTavernDef::base_clone(const CDefClassBase* src)
{
    g_self = this;
    g_src = src;
}

void __fastcall CTavernDef_Copy(CTavernDef* self, int /*edx*/, const CDefClassBase* src);

int main()
{
    CTavernDef dst; dst.f28 = 0;
    CTavernDef srcObj; srcObj.f28 = 0x12345678;
    CTavernDef_Copy(&dst, 0, (const CDefClassBase*)&srcObj);
    if (dst.f28 == 0x12345678 && g_self == &dst && g_src == (const void*)&srcObj)
        printf("004e0f02_TEST PASS\n");
    else
        printf("FAIL %x\n", dst.f28);
    return 0;
}