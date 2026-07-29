#include <cstdio>
#include <cstring>

#include "../../../src/compiled/00/49/CFrontendGameComponent_ChooseWorldNumber_00494710.cpp"

extern "C" unsigned short FrontEndWorldSuffix_00494710[] =
{
    '.', 'w', 'l', 'd', 0
};

static int g_Sequence;
static int g_ConcatSequence;
static int g_AssignSequence;
static int g_DestroySequence;
static int g_CommitSequence;
static const FrontEndWideString_00494710* g_ConcatSource;
static const unsigned short* g_ConcatSuffix;
static FrontEndWideString_00494710* g_Temporary;
static FrontEndWideString_00494710* g_AssignDestination;
static const FrontEndWideString_00494710* g_AssignSource;
static CFrontendGameComponent_00494710* g_CommitThis;

extern "C" FrontEndWideString_00494710* __fastcall
FrontEndWideStringConcat_00494710(
    FrontEndWideString_00494710* result,
    const FrontEndWideString_00494710* source,
    const unsigned short* suffix)
{
    g_ConcatSequence = ++g_Sequence;
    g_ConcatSource = source;
    g_ConcatSuffix = suffix;
    g_Temporary = result;
    result->data = source->data;
    return result;
}

extern "C" FrontEndWideString_00494710* __fastcall
FrontEndWideStringAssign_00494710(
    FrontEndWideString_00494710* destination,
    void*,
    const FrontEndWideString_00494710* source)
{
    g_AssignSequence = ++g_Sequence;
    g_AssignDestination = destination;
    g_AssignSource = source;
    destination->data = source->data;
    return destination;
}

extern "C" void __fastcall
FrontEndWideStringDestroy_00494710(
    FrontEndWideString_00494710* value,
    void*)
{
    g_DestroySequence = ++g_Sequence;
    if (value != g_Temporary)
    {
        g_DestroySequence = -1;
    }
}

extern "C" void __fastcall
FrontEndWorldCommit_00494710(
    CFrontendGameComponent_00494710* component,
    void*)
{
    g_CommitSequence = ++g_Sequence;
    g_CommitThis = component;
}

static void ResetProbe()
{
    g_Sequence = 0;
    g_ConcatSequence = 0;
    g_AssignSequence = 0;
    g_DestroySequence = 0;
    g_CommitSequence = 0;
    g_ConcatSource = 0;
    g_ConcatSuffix = 0;
    g_Temporary = 0;
    g_AssignDestination = 0;
    g_AssignSource = 0;
    g_CommitThis = 0;
}

int main()
{
    int failures = 0;
    void* vtable[5] = { 0, 0, 0, 0, (void*)&FrontEndWorldCommit_00494710 };
    FrontEndWideString_00494710 entries[5];
    CFrontendGameComponent_00494710 component;
    int unchanged_value = 0;

    std::memset(&component, 0, sizeof(component));
    component.vtable = vtable;
    component.entries_begin = entries;
    component.entries_end = entries + 5;
    component.entries_capacity = entries + 5;
    component.first_visible_entry = 2;
    entries[2].data = (void*)"world_three";
    component.selected_world.data = (void*)"old";
    ResetProbe();

    component.ChooseWorldNumber(1);
    if (g_ConcatSequence != 1 ||
        g_AssignSequence != 2 ||
        g_DestroySequence != 3 ||
        g_CommitSequence != 4 ||
        g_ConcatSource != entries + 2 ||
        g_ConcatSuffix != FrontEndWorldSuffix_00494710 ||
        g_AssignDestination != &component.selected_world ||
        g_AssignSource != g_Temporary ||
        g_CommitThis != &component ||
        component.selected_world.data != entries[2].data)
    {
        std::printf("valid world selection/order failed\n");
        ++failures;
    }

    ResetProbe();
    component.selected_world.data = &unchanged_value;
    component.ChooseWorldNumber(4);
    if (g_Sequence != 0 ||
        component.selected_world.data != &unchanged_value)
    {
        std::printf(
            "out-of-range selection failed sequence=%d first=%lu\n",
            g_Sequence,
            component.first_visible_entry);
        ++failures;
    }

    component.first_visible_entry = 0;
    ResetProbe();
    component.ChooseWorldNumber(0);
    if (g_Sequence != 0)
    {
        std::printf("unsigned one-based underflow guard failed\n");
        ++failures;
    }

    if (failures != 0)
    {
        std::printf(
            "FRONTEND_00494710_TEST FAIL count=%d\n",
            failures);
        return 1;
    }

    std::printf("FRONTEND_00494710_TEST PASS\n");
    return 0;
}
