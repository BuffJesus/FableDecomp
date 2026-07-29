#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_ChooseProfileNumber_004962c0.cpp"

static CFrontendProfileActionManager_004962c0 g_Manager;
static unsigned long g_GetManagerCalls;
static unsigned long g_DispatchCalls;
static unsigned long g_PopulateCalls;
static void* g_DispatchValue;
static long g_DispatchFlag;

CFrontendProfileValue_004962c0::CFrontendProfileValue_004962c0()
    : m_Value(0)
{
}

CFrontendProfileValue_004962c0::CFrontendProfileValue_004962c0(
    const CFrontendProfileValue_004962c0& other)
    : m_Value(other.m_Value)
{
}

CFrontendProfileValue_004962c0::~CFrontendProfileValue_004962c0()
{
}

void CFrontendProfileValue_004962c0::Destroy()
{
    m_Value = 0;
}

void* CFrontendProfileActionManager_004962c0::GetDefaultProfileAction()
{
    return reinterpret_cast<void*>(0x12345678);
}

void CFrontendProfileActionManager_004962c0::DispatchProfileAction(
    void* value, long flag)
{
    ++g_DispatchCalls;
    g_DispatchValue = value;
    g_DispatchFlag = flag;
}

CFrontendProfileActionManager_004962c0* __cdecl
GetFrontendProfileActionManager_004962c0()
{
    ++g_GetManagerCalls;
    return &g_Manager;
}

CFrontendProfileValue_004962c0
CFrontendGameComponent_004962c0::CreateNewProfile()
{
    CFrontendProfileValue_004962c0 value;
    value.m_Value = 99;
    return value;
}

void CFrontendGameComponent_004962c0::PopulateProfiles()
{
    ++g_PopulateCalls;
}

static void ResetCalls()
{
    g_GetManagerCalls = 0;
    g_DispatchCalls = 0;
    g_PopulateCalls = 0;
    g_DispatchValue = 0;
    g_DispatchFlag = -1;
}

int main()
{
    unsigned char storage[0x1e48];
    CFrontendGameComponent_004962c0* component =
        reinterpret_cast<CFrontendGameComponent_004962c0*>(storage);
    unsigned long profiles[3] = { 10, 20, 30 };

    std::memset(storage, 0x5a, sizeof(storage));
    *reinterpret_cast<unsigned long**>(storage + 0x1e0c) = profiles;
    *reinterpret_cast<unsigned long**>(storage + 0x1e10) = profiles + 3;
    *reinterpret_cast<long*>(storage + 0x1e18) = 0;
    ResetCalls();

    component->ChooseProfileNumber(2);

    if (g_GetManagerCalls != 1 || g_DispatchCalls != 1 ||
        g_DispatchValue != profiles + 1 || g_DispatchFlag != 1 ||
        g_PopulateCalls != 0 ||
        *reinterpret_cast<long*>(storage + 0x1c) != 1 ||
        *reinterpret_cast<long*>(storage + 0x1e44) != 1 ||
        *reinterpret_cast<long*>(storage + 0x1e8) != 0)
        goto fail;

    for (unsigned long index = 0x168; index < 0x1e8; ++index)
    {
        if (storage[index] != 0)
            goto fail;
    }

    std::memset(storage, 0x5a, sizeof(storage));
    *reinterpret_cast<unsigned long**>(storage + 0x1e0c) = profiles;
    *reinterpret_cast<unsigned long**>(storage + 0x1e10) = profiles + 3;
    *reinterpret_cast<long*>(storage + 0x1e18) = 0;
    unsigned char before[sizeof(storage)];
    std::memcpy(before, storage, sizeof(before));
    ResetCalls();

    component->ChooseProfileNumber(-5);

    if (g_GetManagerCalls != 0 || g_DispatchCalls != 0 ||
        g_PopulateCalls != 0 ||
        std::memcmp(storage, before, sizeof(storage)) != 0)
        goto fail;

    std::printf(
        "CFrontendGameComponent_ChooseProfileNumber_004962c0_test PASS\n");
    return 0;

fail:
    std::printf(
        "CFrontendGameComponent_ChooseProfileNumber_004962c0_test FAIL\n");
    return 1;
}
