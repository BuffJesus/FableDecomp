#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/59/CNewFrontendGameComponent_LoadGame_00594f36.cpp"

static int g_Sequence;
static int g_PrepareSequence;
static int g_AssignSequence;
static CNewFrontendGameComponent_00594f36* g_PrepareThis;
static FrontEndWideString_00594f36* g_AssignDestination;
static const FrontEndWideString_00594f36* g_AssignSource;
static unsigned char g_FlagSeenAtPrepare;
static unsigned char g_FlagSeenAtAssign;

extern "C" void __fastcall
FrontEndLoadPrepare_00594f36(
    CNewFrontendGameComponent_00594f36* self,
    void*)
{
    g_PrepareSequence = ++g_Sequence;
    g_PrepareThis = self;
    g_FlagSeenAtPrepare = self->load_pending;
}

extern "C" FrontEndWideString_00594f36* __fastcall
FrontEndWideStringAssign_00594f36(
    FrontEndWideString_00594f36* destination,
    void*,
    const FrontEndWideString_00594f36* source)
{
    g_AssignSequence = ++g_Sequence;
    g_AssignDestination = destination;
    g_AssignSource = source;
    g_FlagSeenAtAssign =
        ((CNewFrontendGameComponent_00594f36*)
            ((unsigned char*)destination - 0x84))->load_pending;
    destination->data = source->data;
    return destination;
}

static void ResetProbe()
{
    g_Sequence = 0;
    g_PrepareSequence = 0;
    g_AssignSequence = 0;
    g_PrepareThis = 0;
    g_AssignDestination = 0;
    g_AssignSource = 0;
    g_FlagSeenAtPrepare = 0xff;
    g_FlagSeenAtAssign = 0xff;
}

int main()
{
    int failures = 0;
    void* vtable[5] = { 0, 0, 0, 0, (void*)&FrontEndLoadPrepare_00594f36 };
    CNewFrontendGameComponent_00594f36 component;
    FrontEndWideString_00594f36 requested;

    std::memset(&component, 0, sizeof(component));
    component.vtable = vtable;
    component.load_name.data = (void*)"old";
    requested.data = (void*)"save_slot_1";
    ResetProbe();

    component.LoadGame(requested);
    if (g_PrepareSequence != 1 ||
        g_AssignSequence != 2 ||
        g_PrepareThis != &component ||
        g_AssignDestination != &component.load_name ||
        g_AssignSource != &requested ||
        g_FlagSeenAtPrepare != 0 ||
        g_FlagSeenAtAssign != 1 ||
        component.load_pending != 1 ||
        component.load_name.data != requested.data)
    {
        std::printf("prepare/flag/assignment ordering failed\n");
        ++failures;
    }

    component.load_pending = 0;
    requested.data = (void*)"save_slot_2";
    ResetProbe();
    component.LoadGame(requested);
    if (g_PrepareSequence != 1 ||
        g_AssignSequence != 2 ||
        component.load_pending != 1 ||
        component.load_name.data != requested.data)
    {
        std::printf("repeat load forwarding failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_00594f36_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_00594f36_TEST PASS\n");
    return 0;
}
