#include "fable_ui_state_progress.h"
#include "fable_ui_colour.h"
#include "fable_ui_transform.h"
#include "fable_ui_position_children.h"
#include "fable_ui_deletion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static CUIStateRecoveredLayout states[2];
static bool motion;
unsigned char FableUiCoordinateConversionEnabled;
FableUiStateVector2 FableUiCoordinateSourceExtent, FableUiCoordinateDestinationExtent;
void* FableUiScaleContext;
void* __cdecl FableUiGetManager() { return 0; }
static bool __fastcall False(FableUiComponentDrawView*, void*) { return false; }
void* __cdecl FableUiListAllocate(unsigned bytes) { return malloc(bytes); }
void __cdecl FableUiListFree(void* allocation) { free(allocation); }
void __cdecl FableUiDeleteReference(FableReferenceCount* reference) { free(reference); }
static CUIStateRecoveredLayout* __fastcall Find(FableUiComponentDrawView*, void*, unsigned id)
{ return id < 2 ? &states[id] : 0; }
static bool __fastcall Has(FableUiComponentDrawView*, void*, unsigned id) { return id < 2; }
static void __fastcall NoOp(FableUiComponentDrawView*, void*) {}
static void __fastcall Process(FableUiComponentDrawView* view, void*)
{ FableUiProcessChangeState(static_cast<FableUiStateProgressView*>(view), 0); }
static void __fastcall Advance(FableUiComponentDrawView* view, void*)
{ FableUiUpdateStateChange(static_cast<FableUiStateProgressView*>(view), 0); }
static FableUiColourState ReadColour(FableUiComponentDrawView* component)
{
    FableUiColourState colour;
    colour.Colour = component->Colour; colour.TargetColour = component->TargetColour;
    colour.InitialColour = component->InitialColour; colour.ParentColour = component->ParentColour;
    colour.RenderColour = component->RenderColour;
    colour.ColourTimeElapsed = component->ColourTimeElapsed; colour.ColourTime = component->ColourTime;
    return colour;
}
static void WriteColour(FableUiComponentDrawView* component, const FableUiColourState& colour)
{
    component->Colour = colour.Colour; component->TargetColour = colour.TargetColour;
    component->InitialColour = colour.InitialColour; component->RenderColour = colour.RenderColour;
    component->ColourTimeElapsed = colour.ColourTimeElapsed; component->ColourTime = colour.ColourTime;
}
static void __fastcall ChangeColour(FableUiComponentDrawView* component, void*, const FableUiStateColour* delta, float duration, bool)
{
    FableUiColourState colour = ReadColour(component);
    FableUiStateColour target = {
        static_cast<fable_u8>(component->Colour.red + delta->red),
        static_cast<fable_u8>(component->Colour.green + delta->green),
        static_cast<fable_u8>(component->Colour.blue + delta->blue),
        static_cast<fable_u8>(component->Colour.alpha + delta->alpha)};
    colour.SetTarget(target, duration); WriteColour(component, colour);
}
static void __fastcall UpdateColour(FableUiComponentDrawView* component, void*, float delta)
{
    FableUiColourState colour = ReadColour(component);
    colour.Update(delta, false); WriteColour(component, colour);
}
static unsigned Bits(const void* value) { unsigned bits; memcpy(&bits, value, 4); return bits; }
int main(int argc, char**)
{
    motion = argc > 1;
    FableUiComponentDrawVtable table = {};
    table.FindState = table.FindStateForQuery = Find; table.HasState = Has; table.HasCompletedStateChange = FableUiHasCompletedStateChange;
    table.ProcessChangeState = Process; table.UpdateStateChange = Advance;
    table.InternalChanged = FableUiInternalChanged; table.ChildrenChanged = FableUiChildrenChanged;
    table.RequestedStateZeroOrSix = NoOp; table.RequestedStateOneOrFive = NoOp; table.ChangeColour = ChangeColour;
    table.ChangePosition = FableUiChangePosition; table.ChangePositionDelta = FableUiChangePositionDelta;
    table.ChangeZoom = FableUiChangeZoom; table.ChangeZoomDelta = FableUiChangeZoomDelta;
    table.IsPositionIndependent = FableUiStatePositionIndependent; table.IsZoomIndependent = FableUiStateZoomIndependent;
    table.IsIndependent = table.UseRelativePosition = table.UseRelativeZoom = False;
    table.UpdatePosition=FableUiUpdatePosition; table.UpdateZoom=FableUiUpdateZoom; table.UpdateColour=UpdateColour;
    const float durations[] = {0.0f, 0.5f, 2.0f};
    for (unsigned kind = 0; kind < 5; ++kind) for (unsigned duration = 0; duration < 3; ++duration)
    {
        FableUiStateProgressView component; memset(&component, 0, sizeof(component));
        FableUiStateTaskNode head = {}; head.Next = head.Previous = &head;
        FableUiComponentCountedStorage entries[1] = {};
        FableUiPositionChildNode positionHead = {}; positionHead.Left=&positionHead;
        component.PositionChildren=&positionHead;
        component.Vtable = &table; component.StatesToDo = &head;
        component.Children.Begin = component.Children.End = component.Children.CapacityEnd = entries;
        component.ChildrenToDelete=component.Children;
        component.ParentColour.red = component.ParentColour.green = component.ParentColour.blue = component.ParentColour.alpha = 255;
        component.Colour = component.ParentColour;
        component.TargetColour = component.InitialColour = component.RenderColour = component.Colour;
        if (motion)
        {
            component.Parent = &component;
            component.Zoom.x = component.Zoom.y = 1;
            component.TargetZoom = component.InitialZoom = component.Zoom;
            component.ParentZoom = component.RelativeParentZoom = component.Zoom;
            component.ParentPosition.x = 5; component.ParentPosition.y = -7;
        }
        for (unsigned i = 0; i < 2; ++i)
        {
            memset(&states[i], 0, sizeof(states[i])); states[i].stateChangeFlag = motion ? 7 : 2;
            states[i].stateChangeType = kind; states[i].updateTime = durations[duration];
            states[i].colour = component.Colour; states[i].colour.alpha = i == 0 ? 255 : 0;
            if (motion)
            {
                states[i].position.x = i ? 16 : 0; states[i].position.y = i ? -8 : 0;
                states[i].zoom.x = i ? 2 : 1; states[i].zoom.y = i ? 0.5f : 1;
            }
        }
        for (unsigned frame = 0; frame < 32; ++frame)
        {
            if (frame == 0 || frame == 2 || frame == 8) FableUiChangeState(&component, 0, 1);
            if (frame == 3 || frame == 20) FableUiChangeState(&component, 0, 0);
            const float deltas[] = {0, 0.125f, 0.25f, 2, 0.5f, 0, 0.25f, 0.125f};
            FableUiChangingStateUpdate(&component, 0, motion ? deltas[frame % 8] : 0.25f);
            unsigned edge = FableUiChangedStateLastUpdate(&component, 0);
            printf("%u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u", kind, duration, frame, edge,
                component.PreviousUpdateChanged, component.StatesBeingDone, component.StatesDone,
                component.CurrentState, component.TargetState, component.RequestedState, component.PreviousState,
                Bits(&component.ColourTimeElapsed), Bits(&component.ColourTime), Bits(&component.Colour),
                Bits(&component.TargetColour), Bits(&component.InitialColour), Bits(&component.RenderColour));
            if (motion)
            {
                const unsigned offsets[] = {0x34,0x38,0x3C,0x40,0x44,0x48,0x54,0x58,0xF8,0xFC,
                    0x5C,0x60,0x64,0x68,0x6C,0x70,0x7C,0x80,0x108,0x10C,0x98,0x9C,0xA0,0xA4};
                for (unsigned j = 0; j < sizeof(offsets)/sizeof(offsets[0]); ++j)
                    printf(" %u", Bits(reinterpret_cast<unsigned char*>(&component)+offsets[j]));
            }
            for (FableUiStateTaskNode* node = head.Next; node != &head; node = node->Next) printf(" %u", node->Tasks);
            printf("\n");
        }
        while (head.Next != &head) { FableUiStateTaskNode* node = head.Next; head.Next = node->Next; free(node); }
    }
    return 0;
}
