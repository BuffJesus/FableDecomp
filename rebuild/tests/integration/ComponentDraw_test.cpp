#include "fable_ui_component_draw.h"
#include <stdio.h>
#include <string.h>

struct Probe : FableUiComponentDrawView
{
    unsigned Id;
    unsigned SuppressionCalls;
};

static Probe parent, nodes[7];
static FableUiComponentCountedStorage live[3], retiring[3];
static unsigned mode;
static void Event(unsigned kind, Probe* node) { printf(" %u:%u", kind, node->Id); }

static FableUiComponentDrawView* __fastcall GetParent(FableUiComponentDrawView* view, void*)
{
    Probe* node = static_cast<Probe*>(view);
    Event(0, node);
    if (mode == 2 && node->Id == 1) live[0].Data = &nodes[6];
    return node->Parent;
}

static bool __fastcall Foreign(FableUiComponentDrawView* view, void*)
{
    Probe* node = static_cast<Probe*>(view); Event(1, node);
    return (node->Flags & 0x80) != 0;
}

static bool __fastcall Independent(FableUiComponentDrawView* view, void*)
{
    Probe* node = static_cast<Probe*>(view); Event(2, node);
    return (node->Flags & 0x40) != 0;
}

static bool __fastcall LayerIndependent(FableUiComponentDrawView* view, void*)
{
    Probe* node = static_cast<Probe*>(view); Event(3, node);
    return (node->Flags & 0x20) != 0;
}

static bool __fastcall Suppressed(FableUiComponentDrawView* view, void*)
{
    Probe* node = static_cast<Probe*>(view); Event(4, node);
    ++node->SuppressionCalls;
    if (mode == 3 && node->Id == 1 && node->SuppressionCalls == 2)
        node->FlagsSetTwo |= 1;
    return (node->FlagsSetTwo & 1) != 0;
}

static void __fastcall Draw(FableUiComponentDrawView* view, void*,
    void* engine, void* handle, long layer, long* index, FableUiComponentDrawView* owner)
{
    Probe* node = static_cast<Probe*>(view);
    printf(" 5:%u:%ld:%u:%u:%ld:%u", node->Id, layer,
        engine == reinterpret_cast<void*>(0x11111111),
        handle == reinterpret_cast<void*>(0x22222222), *index, owner == &parent);
    ++*index;
    if (mode == 1 && node->Id == 1) parent.Children.End = parent.Children.Begin;
}

int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input = fopen(argv[1], "r");
    if (!input) return 3;
    FableUiComponentDrawVtable vtable = {};
    vtable.GetParent = GetParent; vtable.AcceptForeignParent = Foreign;
    vtable.IsIndependent = Independent; vtable.IsLayerIndependent = LayerIndependent;
    vtable.IsDrawSuppressed = Suppressed; vtable.Draw = Draw;
    unsigned flags, liveCount, retiringCount;
    long layer, parentLayer;
    while (fscanf(input, "%u %ld %ld %u %u %u", &flags, &layer, &parentLayer,
                  &liveCount, &retiringCount, &mode) == 6)
    {
        if (liveCount > 3 || retiringCount > 3) return 4;
        memset(&parent, 0, sizeof(parent)); memset(nodes, 0, sizeof(nodes));
        parent.Vtable = &vtable; parent.Flags = static_cast<unsigned char>(flags);
        parent.Layer = static_cast<signed char>(layer);
        parent.Children.Begin = live; parent.Children.End = live+liveCount;
        parent.ChildrenToDelete.Begin = retiring; parent.ChildrenToDelete.End = retiring+retiringCount;
        for (unsigned i = 0; i < 7; ++i)
        {
            unsigned childFlags, hidden, owns;
            if (fscanf(input, "%u %u %u", &childFlags, &hidden, &owns) != 3) return 5;
            nodes[i].Vtable = &vtable; nodes[i].Id = i+1;
            nodes[i].Flags = static_cast<unsigned char>(childFlags);
            nodes[i].FlagsSetTwo = static_cast<unsigned char>(hidden);
            nodes[i].Parent = owns ? &parent : 0;
        }
        for (unsigned i = 0; i < 3; ++i)
        {
            live[i].Data = &nodes[i]; live[i].Info = 0;
            retiring[i].Data = &nodes[i+3]; retiring[i].Info = 0;
        }
        long index = 9;
        printf("TRACE");
        FableUiComponentDraw(&parent, 0, reinterpret_cast<void*>(0x11111111),
            reinterpret_cast<void*>(0x22222222), parentLayer, &index, 0);
        printf(" END:%ld\n", index);
    }
    fclose(input);
    return 0;
}
