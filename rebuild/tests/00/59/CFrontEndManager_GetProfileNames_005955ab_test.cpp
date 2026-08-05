#include <cstdio>
#include <cstring>

struct VectorProbe
{
    unsigned long begin;
    unsigned long end;
    unsigned long capacity;
};

static VectorProbe* g_names;
static int g_vectorInit;
static int g_findFirst;
static int g_cleanup;

extern "C" unsigned long FableProfileNamesGlobal_005955ab = 0;
extern "C" const unsigned char FableProfileNamesDefaultPattern_005955ab[] = { 0 };
extern "C" const unsigned char FableProfileNamesWildcard_005955ab[] = { 0 };
extern "C" const unsigned char FableProfileNamesEmpty_005955ab[] = { 0 };

extern "C" void FableProfileNamesCall_00412450()
{
    ++g_vectorInit;
    g_names->begin = 0;
    g_names->end = 0;
    g_names->capacity = 0;
}
extern "C" void FableProfileNamesCall_0041a540() {}
extern "C" void FableProfileNamesCall_0099bf30() {}
extern "C" void FableProfileNamesCall_0099b510() {}
extern "C" unsigned char FableProfileNamesCall_00999760()
{
    ++g_findFirst;
    return 0;
}
extern "C" void FableProfileNamesCall_00999850() {}
extern "C" void FableProfileNamesCall_0099b6b0() {}
extern "C" void FableProfileNamesCall_0099ebf0() {}
extern "C" void FableProfileNamesCall_0099b2c0() {}
extern "C" void FableProfileNamesCall_0099eae0() {}
extern "C" unsigned char FableProfileNamesCall_00999a30() { return 0; }
extern "C" void FableProfileNamesCall_0099b940() {}
extern "C" void FableProfileNamesCall_0099aed0() {}
extern "C" void FableProfileNamesCall_00997620() {}
extern "C" void FableProfileNamesCall_0099b720() {}
extern "C" void FableProfileNamesCall_004128a0() {}
extern "C" void FableProfileNamesCall_00bfe9f9()
{
    ++g_cleanup;
}

extern "C" void __fastcall
CFrontEndManager_GetProfileNames_005955ab(void*);

int main()
{
    unsigned char manager[0x80];
    VectorProbe names;
    std::memset(manager, 0, sizeof(manager));
    names.begin = 1;
    names.end = 2;
    names.capacity = 3;
    g_names = &names;

    __asm {
        lea ecx, manager
        lea eax, names
        push eax
        call CFrontEndManager_GetProfileNames_005955ab
    }

    if (g_vectorInit != 1 || g_findFirst != 1 || g_cleanup != 1 ||
        names.begin != 0 || names.end != 0 || names.capacity != 0)
    {
        std::printf("GET_PROFILE_NAMES_005955AB_TEST FAIL\n");
        return 1;
    }
    std::printf("GET_PROFILE_NAMES_005955AB_TEST PASS\n");
    return 0;
}
