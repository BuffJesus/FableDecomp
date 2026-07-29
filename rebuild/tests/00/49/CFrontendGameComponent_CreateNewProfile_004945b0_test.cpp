#include <cstdarg>
#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_CreateNewProfile_004945b0.cpp"

const char g_FrontendProfileFormat_004945b0[] = "Profile %ld";
const unsigned short g_FrontendProfilePrefix_004945b0[] = {
    'N', 'e', 'w', ' ', 0
};

static CFrontendProfileManager_004945b0 g_Manager;
static unsigned long g_ConstructCalls;
static unsigned long g_CopyCalls;
static unsigned long g_DestroyCalls;
static unsigned long g_FormatCalls;
static unsigned long g_BuildCalls;
static unsigned long g_GetManagerCalls;
static unsigned long g_CreateCalls;
static unsigned long g_SelectCalls;
static unsigned long g_FinaliseCalls;
static long g_FormattedNumber;
static long g_CreateInputValue;
static long g_CreateFlag;
static long g_SelectInputValue;
static long g_SelectFlag;

CFrontendProfileString_004945b0::CFrontendProfileString_004945b0()
{
    Construct();
}

CFrontendProfileString_004945b0::CFrontendProfileString_004945b0(
    const CFrontendProfileString_004945b0& other)
{
    Copy(&other);
}

CFrontendProfileString_004945b0::~CFrontendProfileString_004945b0()
{
    Destroy();
}

void CFrontendProfileString_004945b0::Construct()
{
    ++g_ConstructCalls;
    m_Value = 0;
}

void CFrontendProfileString_004945b0::Copy(
    const CFrontendProfileString_004945b0* other)
{
    ++g_CopyCalls;
    m_Value = other->m_Value;
}

void CFrontendProfileString_004945b0::Destroy()
{
    ++g_DestroyCalls;
    m_Value = 0;
}

void __cdecl CFrontendProfileString_004945b0::Format(
    const char* format, ...)
{
    ++g_FormatCalls;
    va_list arguments;
    va_start(arguments, format);
    g_FormattedNumber = va_arg(arguments, long);
    va_end(arguments);
    m_Value = g_FormattedNumber;
}

void __fastcall BuildFrontendProfileName_004945b0(
    CFrontendProfileString_004945b0* output,
    const unsigned short* prefix,
    const CFrontendProfileString_004945b0* source)
{
    ++g_BuildCalls;
    if (prefix != g_FrontendProfilePrefix_004945b0)
        output->m_Value = -1;
    else
        output->m_Value = source->m_Value + 100;
}

CFrontendProfileManager_004945b0* __cdecl
GetFrontendProfileManager_004945b0()
{
    ++g_GetManagerCalls;
    return &g_Manager;
}

void CFrontendProfileManager_004945b0::CreateProfileResult(
    CFrontendProfileString_004945b0* output,
    const CFrontendProfileString_004945b0* input,
    long flag)
{
    ++g_CreateCalls;
    g_CreateInputValue = input->m_Value;
    g_CreateFlag = flag;
    output->m_Value = input->m_Value + 1000;
}

void CFrontendProfileManager_004945b0::SelectProfile(
    const CFrontendProfileString_004945b0* input, long flag)
{
    ++g_SelectCalls;
    g_SelectInputValue = input->m_Value;
    g_SelectFlag = flag;
}

void CFrontendProfileManager_004945b0::FinaliseProfiles()
{
    ++g_FinaliseCalls;
}

int main()
{
    unsigned char storage[sizeof(CFrontendGameComponent_004945b0)];
    CFrontendGameComponent_004945b0* component =
        reinterpret_cast<CFrontendGameComponent_004945b0*>(storage);
    CFrontendProfileString_004945b0 profiles[3];
    std::memset(storage, 0x5a, sizeof(storage));
    *reinterpret_cast<CFrontendProfileString_004945b0**>(
        storage + 0x1e0c) = profiles;
    *reinterpret_cast<CFrontendProfileString_004945b0**>(
        storage + 0x1e10) = profiles + 3;

    CFrontendProfileString_004945b0 result =
        component->CreateNewProfile();

    if (g_FormattedNumber != 4 || result.m_Value != 104 ||
        g_FormatCalls != 1 || g_BuildCalls != 1 ||
        g_GetManagerCalls != 3 || g_CreateCalls != 1 ||
        g_CreateInputValue != 104 || g_CreateFlag != 1 ||
        g_SelectCalls != 1 || g_SelectInputValue != 104 ||
        g_SelectFlag != 0 || g_FinaliseCalls != 1)
    {
        std::printf(
            "CFrontendGameComponent_CreateNewProfile_004945b0_test FAIL\n");
        return 1;
    }

    std::printf(
        "CFrontendGameComponent_CreateNewProfile_004945b0_test PASS\n");
    return 0;
}
