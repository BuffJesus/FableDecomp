#include <cstdio>

struct CDefClassBase;

struct CAICreatureWillPowerIndicatorDefBase {
    void CloneFields(const CDefClassBase* other);
};

struct CAICreatureWillPowerIndicatorDef : public CAICreatureWillPowerIndicatorDefBase {
    char pad[0x28];
    unsigned long field28;
};

static int g_baseCalled = 0;
static const void* g_baseSelf = 0;
static const void* g_baseOther = 0;

void CAICreatureWillPowerIndicatorDefBase::CloneFields(const CDefClassBase* other)
{
    g_baseCalled = 1;
    g_baseSelf = this;
    g_baseOther = other;
}

void __fastcall CAICreatureWillPowerIndicatorDef_Copy(
    CAICreatureWillPowerIndicatorDef* self, int /*edx*/, const CDefClassBase* other);

int main()
{
    CAICreatureWillPowerIndicatorDef src, dst;
    src.field28 = 0xDEADBEEF;
    dst.field28 = 0x11111111;

    CAICreatureWillPowerIndicatorDef_Copy(
        &dst, 0, reinterpret_cast<const CDefClassBase*>(&src));

    if (g_baseCalled == 1 &&
        g_baseSelf == static_cast<CAICreatureWillPowerIndicatorDefBase*>(&dst) &&
        g_baseOther == reinterpret_cast<const CDefClassBase*>(&src) &&
        dst.field28 == 0xDEADBEEF)
    {
        std::printf("004e14fd_TEST PASS\n");
    }
    else
    {
        std::printf("FAIL called=%d f28=%08lx\n", g_baseCalled, dst.field28);
    }
    return 0;
}