#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/53/CList_DoRecomputeOffsets_0053c332.cpp"

extern "C" const float FableCListDoRecomputeInv255 = 0.003921569f;
extern "C" const float FableCListDoRecomputeZero = 0.0f;
extern "C" const float FableCListDoRecomputeOne = 1.0f;
extern "C" const float FableCListDoRecompute255 = 255.0f;
extern "C" const float FableCListDoRecomputeAlmostOne = 0.999f;

static volatile int g_updateCount;
static volatile int g_replaceCount;
static void* g_replaceOwner;
static void* g_replaceFirst;
static void* g_replaceSecond;

extern "C" __declspec(naked) void __fastcall FableCListDoRecomputeReplaceRange(void*)
{
    __asm {
        inc dword ptr [g_replaceCount]
        mov eax,dword ptr [esp+4]
        mov dword ptr [g_replaceFirst],eax
        mov eax,dword ptr [esp+8]
        mov dword ptr [g_replaceSecond],eax
        ret 8
    }
}

extern "C" void* __fastcall FableCListDoRecomputeTransfer(void* value)
{
    return value;
}

extern "C" void __fastcall FableCListDoRecomputeAddChild(void*)
{
}

extern "C" int __cdecl FableCListDoRecomputeFtol2()
{
    return 0;
}

static void __declspec(naked) UpdateOffsets()
{
    __asm {
        inc dword ptr [g_updateCount]
        mov eax,dword ptr [esp+4]
        mov dword ptr [g_replaceOwner],eax
        ret 4
    }
}

static bool RunEmptyList()
{
    unsigned char list[0x220];
    void* vtable[0x100];
    unsigned char range[8];
    std::memset(list, 0, sizeof(list));
    std::memset(vtable, 0, sizeof(vtable));
    std::memset(range, 0, sizeof(range));
    *(void**)list = vtable;
    *(void**)(list + 0x164) = range;
    *(void**)(list + 0x168) = range;
    *(void**)(list + 0x17c) = range;
    *(void**)(list + 0x1f0) = 0;
    list[0x1f8] = 255;
    vtable[0x270 / 4] = (void*)UpdateOffsets;

    g_updateCount = 0;
    g_replaceCount = 0;
    g_replaceOwner = g_replaceFirst = g_replaceSecond = 0;

    CList_DoRecomputeOffsets_0053c332(list);

    return g_updateCount == 1 &&
        g_replaceCount == 1 &&
        g_replaceOwner == 0 &&
        g_replaceFirst == range &&
        g_replaceSecond == 0 &&
        *reinterpret_cast<unsigned int*>(list + 0x190) == 0x80000000u &&
        *reinterpret_cast<int*>(list + 0x194) == 0 &&
        *reinterpret_cast<float*>(list + 0x198) == 1.0f &&
        *reinterpret_cast<float*>(list + 0x19c) == 1.0f;
}

int main()
{
    const bool ok = RunEmptyList();
    std::puts(ok ? "CList_DoRecomputeOffsets_0053c332_test PASS"
                 : "CList_DoRecomputeOffsets_0053c332_test FAIL");
    return ok ? 0 : 1;
}
