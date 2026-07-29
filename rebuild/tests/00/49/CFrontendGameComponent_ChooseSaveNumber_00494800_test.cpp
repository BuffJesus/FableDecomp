#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_ChooseSaveNumber_00494800.cpp"

static int g_Sequence;
static int g_AssignSequence;
static int g_CommitSequence;
static FrontEndWideString_00494800* g_AssignDestination;
static const FrontEndWideString_00494800* g_AssignSource;
static CFrontendGameComponent_00494800* g_CommitThis;

extern "C" FrontEndWideString_00494800* __fastcall
FrontEndWideStringAssign_00494800(
    FrontEndWideString_00494800* destination,
    void*,
    const FrontEndWideString_00494800* source)
{
    g_AssignSequence = ++g_Sequence;
    g_AssignDestination = destination;
    g_AssignSource = source;
    destination->data = source->data;
    return destination;
}

extern "C" void __fastcall
FrontEndSaveCommit_00494800(
    CFrontendGameComponent_00494800* component,
    void*)
{
    g_CommitSequence = ++g_Sequence;
    g_CommitThis = component;
}

static void ResetProbe()
{
    g_Sequence = 0;
    g_AssignSequence = 0;
    g_CommitSequence = 0;
    g_AssignDestination = 0;
    g_AssignSource = 0;
    g_CommitThis = 0;
}

int main()
{
    int failures = 0;
    int unchanged = 0;
    void* vtable[5] = { 0, 0, 0, 0, (void*)&FrontEndSaveCommit_00494800 };
    FrontEndWideString_00494800 saves[5];
    CFrontendGameComponent_00494800 component;

    std::memset(&component, 0, sizeof(component));
    component.vtable = vtable;
    component.saves_begin = saves;
    component.saves_end = saves + 5;
    component.saves_capacity = saves + 5;
    component.first_visible_save = 2;
    saves[3].data = (void*)"save_four";
    ResetProbe();

    component.ChooseSaveNumber(2);
    if (g_AssignSequence != 1 ||
        g_CommitSequence != 2 ||
        g_AssignDestination != &component.selected_save ||
        g_AssignSource != saves + 3 ||
        g_CommitThis != &component ||
        component.selected_save.data != saves[3].data)
    {
        std::printf("valid save selection/order failed\n");
        ++failures;
    }

    component.selected_save.data = &unchanged;
    ResetProbe();
    component.ChooseSaveNumber(4);
    if (g_Sequence != 0 || component.selected_save.data != &unchanged)
    {
        std::printf("out-of-range save selection failed\n");
        ++failures;
    }

    component.first_visible_save = 0;
    ResetProbe();
    component.ChooseSaveNumber(0);
    if (g_Sequence != 0)
    {
        std::printf("save selection unsigned underflow guard failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_00494800_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_00494800_TEST PASS\n");
    return 0;
}
