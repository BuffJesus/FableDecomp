#include <cstdio>

#include "../../../src/compiled/00/49/CFrontendProfileList_Refresh_00495830.cpp"

static CFrontendProfileManager_00495830 g_Manager;
static unsigned long g_ClearCalls;
static unsigned long g_GetCalls;
static unsigned long g_PopulateCalls;
static unsigned long g_FreeCalls;
static CFrontendProfileRange_00495830* g_ClearThis;
static void* g_ClearBegin;
static void* g_ClearEnd;
static CFrontendProfileRange_00495830* g_PopulateList;
static bool g_TemporaryWasZero;
static void* g_Freed;
static bool g_AllocateTemporary;
static unsigned long g_TemporaryStorage;

void CFrontendProfileRange_00495830::Clear(void* begin, void* end)
{
    ++g_ClearCalls;
    g_ClearThis = this;
    g_ClearBegin = begin;
    g_ClearEnd = end;
}

CFrontendProfileManager_00495830* __cdecl
GetFrontendProfileManager_00495830()
{
    ++g_GetCalls;
    return &g_Manager;
}

void CFrontendProfileManager_00495830::PopulateList(
    CFrontendProfileRange_00495830* list,
    CFrontendProfileRange_00495830* temporary)
{
    ++g_PopulateCalls;
    g_PopulateList = list;
    g_TemporaryWasZero =
        temporary->m_Begin == 0 &&
        temporary->m_End == 0 &&
        temporary->m_Capacity == 0;
    if (g_AllocateTemporary)
        temporary->m_Begin = &g_TemporaryStorage;
}

void __cdecl FrontendProfileListFree_00495830(void* allocation)
{
    ++g_FreeCalls;
    g_Freed = allocation;
}

int main()
{
    unsigned long values[3] = { 1, 2, 3 };
    CFrontendProfileRange_00495830 list = {
        values, values + 3, values + 3
    };

    g_AllocateTemporary = true;
    list.Refresh();

    if (g_ClearCalls != 1 || g_ClearThis != &list ||
        g_ClearBegin != values || g_ClearEnd != values + 3 ||
        g_GetCalls != 1 || g_PopulateCalls != 1 ||
        g_PopulateList != &list || !g_TemporaryWasZero ||
        g_FreeCalls != 1 || g_Freed != &g_TemporaryStorage)
    {
        std::printf("CFrontendProfileList_Refresh_00495830_test FAIL\n");
        return 1;
    }

    g_AllocateTemporary = false;
    list.Refresh();
    if (g_ClearCalls != 2 || g_GetCalls != 2 ||
        g_PopulateCalls != 2 || !g_TemporaryWasZero ||
        g_FreeCalls != 1)
    {
        std::printf("CFrontendProfileList_Refresh_00495830_test FAIL\n");
        return 1;
    }

    std::printf("CFrontendProfileList_Refresh_00495830_test PASS\n");
    return 0;
}
