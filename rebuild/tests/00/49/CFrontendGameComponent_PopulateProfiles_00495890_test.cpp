#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_PopulateProfiles_00495890.cpp"

unsigned long g_FrontendPopulateCookie_00495890 = 0x13572468;
unsigned long g_EmptyFrontendPointer_00495890;
unsigned long g_FrontendProfileRoot_00495890;

static CFrontendPopulateOps_00495890 g_Manager;
static unsigned long g_AssignCalls;
static unsigned long g_ClearCalls;
static unsigned long g_ConstructRootCalls;
static unsigned long g_CopyRootCalls;
static unsigned long g_DestroyCalls;
static unsigned long g_StartCalls;
static unsigned long g_SortCalls;
static unsigned long g_CheckCalls;
static void* g_AssignThis;
static const void* g_AssignValue;
static void* g_ClearThis;
static void* g_ClearBegin;
static void* g_ClearEnd;
static const void* g_RootValue;
static void* g_SortBegin;
static void* g_SortEnd;
static unsigned long g_CheckedCookie;

void CFrontendPopulateOps_00495890::AssignEmpty(const void* value)
{
    ++g_AssignCalls;
    g_AssignThis = this;
    g_AssignValue = value;
}

void CFrontendPopulateOps_00495890::ClearRange(void* begin, void* end)
{
    ++g_ClearCalls;
    g_ClearThis = this;
    g_ClearBegin = begin;
    g_ClearEnd = end;
}

void* CFrontendPopulateOps_00495890::ConstructRoot(const void* value)
{
    ++g_ConstructRootCalls;
    g_RootValue = value;
    return this;
}

void __fastcall CFrontendPopulateOps_00495890::CopyRoot(void*)
{
    ++g_CopyRootCalls;
}

void CFrontendPopulateOps_00495890::DestroyString()
{
    ++g_DestroyCalls;
}

bool __fastcall CFrontendPopulateOps_00495890::StartIterator(
    void*, void*)
{
    ++g_StartCalls;
    return false;
}

void CFrontendPopulateOps_00495890::CopyCurrent(void*) {}
void CFrontendPopulateOps_00495890::ConstructString() {}

void __fastcall CFrontendPopulateOps_00495890::BuildEntry(
    void*, long, void*, long)
{
}

void CFrontendPopulateOps_00495890::InsertEntry(
    void*, void*, void*, long, long)
{
}

bool __fastcall CFrontendPopulateOps_00495890::NextIterator(
    void*, void*)
{
    return false;
}

CFrontendPopulateOps_00495890* __cdecl
GetFrontendPopulateManager_00495890()
{
    return &g_Manager;
}

void __fastcall SortFrontendProfiles_00495890(
    void* begin, void* end, void*)
{
    ++g_SortCalls;
    g_SortBegin = begin;
    g_SortEnd = end;
}

void __cdecl FrontendPopulateFree_00495890(void*) {}

void __fastcall CheckFrontendPopulateCookie_00495890(unsigned long cookie)
{
    ++g_CheckCalls;
    g_CheckedCookie = cookie;
}

int main()
{
    unsigned char storage[0x1e44];
    CFrontendGameComponent_00495890* component =
        reinterpret_cast<CFrontendGameComponent_00495890*>(storage);
    unsigned long values[2] = { 1, 2 };

    std::memset(storage, 0x5a, sizeof(storage));
    *reinterpret_cast<unsigned long**>(storage + 0x1e30) = values;
    *reinterpret_cast<unsigned long**>(storage + 0x1e34) = values + 2;
    *reinterpret_cast<unsigned long**>(storage + 0x1e38) = values + 2;
    *reinterpret_cast<unsigned long*>(storage + 0x1e3c) = 7;

    component->PopulateProfiles();

    if (g_AssignCalls != 1 || g_AssignThis != storage + 0x1e40 ||
        g_AssignValue != &g_EmptyFrontendPointer_00495890 ||
        g_ClearCalls != 1 || g_ClearThis != storage + 0x1e30 ||
        g_ClearBegin != values || g_ClearEnd != values + 2 ||
        g_ConstructRootCalls != 1 ||
        g_RootValue != &g_FrontendProfileRoot_00495890 ||
        g_CopyRootCalls != 1 || g_DestroyCalls != 2 ||
        g_StartCalls != 1 || g_SortCalls != 1 ||
        g_SortBegin != values || g_SortEnd != values + 2 ||
        *reinterpret_cast<unsigned long*>(storage + 0x1e3c) != 0 ||
        g_CheckCalls != 1 ||
        g_CheckedCookie != g_FrontendPopulateCookie_00495890)
    {
        std::printf(
            "CFrontendGameComponent_PopulateProfiles_00495890_test FAIL\n");
        return 1;
    }

    std::printf(
        "CFrontendGameComponent_PopulateProfiles_00495890_test PASS\n");
    return 0;
}
