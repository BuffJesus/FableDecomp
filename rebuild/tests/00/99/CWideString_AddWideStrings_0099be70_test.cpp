#include <stdio.h>
#include <string.h>

#include "fable_string.h"

fable_i32 g_CWideStringInstanceCount_013BCA20 = 0;

namespace
{
    CWideStringData g_sumStorage;
    wchar_t g_sumText[] = L"left-right";
    unsigned int g_addAssignCalls;
    unsigned int g_unassignCalls;
    unsigned int g_freeCalls;
    unsigned int g_deleteCalls;
    const void* g_observedRight;

    void SetStorage(
        CWideString& value,
        CWideStringData* storage)
    {
        *reinterpret_cast<CWideStringData**>(&value) = storage;
    }

    CWideStringData* GetStorage(const CWideString& value)
    {
        return *reinterpret_cast<CWideStringData* const*>(&value);
    }
}

extern "C" void FABLE_FASTCALL
FableWideAddAssign_0099BE70(
    void* self,
    void*,
    const void* right)
{
    ++g_addAssignCalls;
    g_observedRight = right;
    CWideStringData*& temporary =
        *static_cast<CWideStringData**>(self);
    if (temporary != 0)
    {
        --temporary->owners;
    }
    g_sumStorage.text = g_sumText;
    g_sumStorage.owners = 1;
    temporary = &g_sumStorage;
}

extern "C" void FABLE_FASTCALL
FableWideUnassign_0099BE70(void*, void*)
{
    ++g_unassignCalls;
}

extern "C" void FABLE_CDECL
FableWideAddFree_0099BE70(void*)
{
    ++g_freeCalls;
}

extern "C" void FABLE_CDECL
FableWideAddDelete_0099BE70(void*)
{
    ++g_deleteCalls;
}

CWideString::~CWideString()
{
}

int main()
{
    wchar_t leftText[] = L"left";
    wchar_t rightText[] = L"right";
    CWideStringData leftStorage = {leftText, 0, 0, 1};
    CWideStringData rightStorage = {rightText, 0, 0, 1};
    fable_u8 leftBytes[sizeof(CWideString)] = {0};
    fable_u8 rightBytes[sizeof(CWideString)] = {0};
    CWideString& left =
        *reinterpret_cast<CWideString*>(leftBytes);
    CWideString& right =
        *reinterpret_cast<CWideString*>(rightBytes);
    SetStorage(left, &leftStorage);
    SetStorage(right, &rightStorage);
    g_CWideStringInstanceCount_013BCA20 = 20;

    CWideString result = left + right;

    const bool passed =
        g_addAssignCalls == 1 &&
        g_observedRight == &right &&
        g_unassignCalls == 0 &&
        g_freeCalls == 0 &&
        g_deleteCalls == 0 &&
        leftStorage.owners == 1 &&
        rightStorage.owners == 1 &&
        g_sumStorage.owners == 1 &&
        GetStorage(result) == &g_sumStorage &&
        g_CWideStringInstanceCount_013BCA20 == 21;
    if (!passed)
    {
        printf("FABLETLC_WIDE_STRING_ADD_WIDE FAIL\n");
        return 1;
    }

    printf("FABLETLC_WIDE_STRING_ADD_WIDE PASS\n");
    return 0;
}
