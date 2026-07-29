#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_InitGraphics_00494840.cpp"

const char g_FrontendGraphicsName_00494840[] = "frontend-resource";

static void* g_FindData;
static CFrontendGraphicsControl_00494840* g_FindControl;
static unsigned long g_NameConstructCalls;
static unsigned long g_NameDestroyCalls;
static unsigned long g_FindCalls;
static unsigned long g_DestroyCalls;
static unsigned long g_DeleteCalls;
static const char* g_NameText;
static long g_NameLength;
static void* g_DestroyContext;
static void* g_DeletedControl;
static const CFrontendGraphicsName_00494840* g_FindName;

CFrontendGraphicsRegistry_00494840 g_RegistryObject;
CFrontendGraphicsRegistry_00494840*
    g_FrontendGraphicsRegistry_00494840 = &g_RegistryObject;

void CFrontendGraphicsName_00494840::Construct(
    const char* text, long length)
{
    ++g_NameConstructCalls;
    g_NameText = text;
    g_NameLength = length;
}

void CFrontendGraphicsName_00494840::Destroy()
{
    ++g_NameDestroyCalls;
}

CFrontendGraphicsPointer_00494840
CFrontendGraphicsRegistry_00494840::Find(
    const CFrontendGraphicsName_00494840& name)
{
    ++g_FindCalls;
    g_FindName = &name;
    CFrontendGraphicsPointer_00494840 result;
    result.m_Data = g_FindData;
    result.m_Control = g_FindControl;
    if (result.m_Control != 0)
        ++result.m_Control->m_References;
    return result;
}

void CFrontendGraphicsContext_00494840::Destroy()
{
    ++g_DestroyCalls;
    g_DestroyContext = this;
}

void __cdecl FrontendGraphicsDelete_00494840(void* control)
{
    ++g_DeleteCalls;
    g_DeletedControl = control;
}

static void ResetCalls()
{
    g_NameConstructCalls = 0;
    g_NameDestroyCalls = 0;
    g_FindCalls = 0;
    g_DestroyCalls = 0;
    g_DeleteCalls = 0;
    g_NameText = 0;
    g_NameLength = 0;
    g_DestroyContext = 0;
    g_DeletedControl = 0;
    g_FindName = 0;
}

static bool CheckReplacement()
{
    unsigned char storage[sizeof(CFrontendGameComponent_00494840)];
    CFrontendGameComponent_00494840* component =
        reinterpret_cast<CFrontendGameComponent_00494840*>(storage);
    unsigned char* bytes = storage;
    CFrontendGraphicsContext_00494840 oldContext;
    CFrontendGraphicsContext_00494840 newContext;
    CFrontendGraphicsControl_00494840 oldControl = {
        1, &CFrontendGraphicsContext_00494840::Destroy, &oldContext
    };
    CFrontendGraphicsControl_00494840 newControl = {
        2, &CFrontendGraphicsContext_00494840::Destroy, &newContext
    };

    std::memset(bytes, 0x5a, sizeof(storage));
    CFrontendGraphicsPointer_00494840* member =
        reinterpret_cast<CFrontendGraphicsPointer_00494840*>(bytes + 0x1e04);
    member->m_Data = reinterpret_cast<void*>(0x33333333);
    member->m_Control = &oldControl;
    g_FindData = reinterpret_cast<void*>(0x44444444);
    g_FindControl = &newControl;
    ResetCalls();

    if (!component->InitGraphics())
        return false;

    bool passed = member->m_Data == g_FindData &&
           member->m_Control == &newControl && oldControl.m_References == 0 &&
           newControl.m_References == 3 && g_DestroyCalls == 1 &&
           g_DestroyContext == oldControl.m_Context && g_DeleteCalls == 1 &&
           g_DeletedControl == &oldControl && g_NameConstructCalls == 1 &&
           g_NameDestroyCalls == 1 && g_FindCalls == 1 &&
           g_NameText == g_FrontendGraphicsName_00494840 &&
           g_NameLength == -1 && g_FindName != 0;
    if (!passed)
        std::printf(
            "replacement data=%p control=%p old=%ld new=%ld destroy=%lu "
            "delete=%lu construct=%lu nameDestroy=%lu find=%lu length=%ld\n",
            member->m_Data, member->m_Control, oldControl.m_References,
            newControl.m_References, g_DestroyCalls, g_DeleteCalls,
            g_NameConstructCalls, g_NameDestroyCalls, g_FindCalls,
            g_NameLength);
    return passed;
}

static bool CheckEqualControl()
{
    unsigned char storage[sizeof(CFrontendGameComponent_00494840)];
    CFrontendGameComponent_00494840* component =
        reinterpret_cast<CFrontendGameComponent_00494840*>(storage);
    unsigned char* bytes = storage;
    CFrontendGraphicsContext_00494840 sharedContext;
    CFrontendGraphicsControl_00494840 sharedControl = {
        4, &CFrontendGraphicsContext_00494840::Destroy, &sharedContext
    };

    std::memset(bytes, 0x5a, sizeof(storage));
    CFrontendGraphicsPointer_00494840* member =
        reinterpret_cast<CFrontendGraphicsPointer_00494840*>(bytes + 0x1e04);
    member->m_Data = reinterpret_cast<void*>(0x66666666);
    member->m_Control = &sharedControl;
    g_FindData = reinterpret_cast<void*>(0x77777777);
    g_FindControl = &sharedControl;
    ResetCalls();

    if (!component->InitGraphics())
        return false;

    bool passed =
           member->m_Data == reinterpret_cast<void*>(0x66666666) &&
           member->m_Control == &sharedControl &&
           sharedControl.m_References == 4 && g_DestroyCalls == 0 &&
           g_DeleteCalls == 0 && g_NameConstructCalls == 1 &&
           g_NameDestroyCalls == 1 && g_FindCalls == 1;
    if (!passed)
        std::printf(
            "equal data=%p control=%p refs=%ld destroy=%lu delete=%lu "
            "construct=%lu nameDestroy=%lu find=%lu\n",
            member->m_Data, member->m_Control, sharedControl.m_References,
            g_DestroyCalls, g_DeleteCalls, g_NameConstructCalls,
            g_NameDestroyCalls, g_FindCalls);
    return passed;
}

int main()
{
    if (!CheckReplacement() || !CheckEqualControl())
    {
        std::printf("CFrontendGameComponent_InitGraphics_00494840_test FAIL\n");
        return 1;
    }

    std::printf("CFrontendGameComponent_InitGraphics_00494840_test PASS\n");
    return 0;
}
