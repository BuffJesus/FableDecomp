#include "fable_ui_state_progress.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_ui_state.h"

struct Child : FableUiComponentDrawView { unsigned Id, Kind; };
static FableUiStateProgressView parent;
static unsigned internalDone, childrenDone, mode;
static CUIStateRecoveredLayout state;
static unsigned present;
static CUIStateRecoveredLayout* __fastcall Find(FableUiComponentDrawView*, void*, unsigned requested)
{ printf(" F%u", requested); return present ? &state : 0; }
static bool __fastcall Has(FableUiComponentDrawView*, void*, unsigned requested)
{ printf(" H%u", requested); return present != 0; }
static void __fastcall ZeroSix(FableUiComponentDrawView*, void*) { printf(" Z"); }
static void __fastcall OneFive(FableUiComponentDrawView*, void*) { printf(" O"); }
static FableUiComponentDrawView* __fastcall Parent(FableUiComponentDrawView* view, void*)
{
    printf(" P%u", static_cast<Child*>(view)->Id);
    return view->Parent;
}
static unsigned __fastcall Type(FableUiComponentDrawView* view, void*)
{
    Child* child = static_cast<Child*>(view);
    printf(" T%u", child->Id); return child->Kind;
}
static void __fastcall Change(FableUiComponentDrawView* view, void*, unsigned requested, float time)
{
    unsigned bits; memcpy(&bits, &time, 4);
    printf(" C%u:%u:%u", static_cast<Child*>(view)->Id, requested, bits);
    if (mode == 2) parent.Children.End = parent.Children.Begin;
}
static void __fastcall Process(FableUiComponentDrawView*, void*)
{
    printf(" S");
    if (mode == 1) parent.StatesBeingDone &= ~1U;
}
static bool __fastcall Internal(FableUiComponentDrawView*, void*)
{ printf(" I"); return internalDone != 0; }
static bool __fastcall Children(FableUiComponentDrawView*, void*)
{ printf(" K"); return childrenDone != 0; }

int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input = fopen(argv[1], "r"); if (!input) return 3;
    unsigned pending, done, count, tasks, current, target, requested, childBits;
    FableUiComponentDrawVtable table = {};
    table.GetParent = Parent; table.GetType = Type; table.ChangeState = Change;
    table.ProcessChangeState = Process; table.InternalChanged = Internal; table.ChildrenChanged = Children;
    table.FindState = Find; table.HasState = Has;
    table.RequestedStateZeroOrSix = ZeroSix; table.RequestedStateOneOrFive = OneFive;
    while (fscanf(input, "%u %u %u %u %u %u %u %u %u %u %u", &pending, &done,
        &count, &tasks, &current, &target, &requested, &internalDone, &childrenDone, &childBits, &mode) == 11)
    {
        memset(&parent, 0, sizeof(parent)); parent.Vtable = &table;
        parent.StatesBeingDone = pending; parent.StatesDone = done;
        parent.CurrentState = current; parent.TargetState = target;
        parent.RequestedState = requested; parent.UpdateTime = 2.5f; parent.PreviousState = 99;
#ifdef FABLE_TEST_CHANGE_STATE
        unsigned kind, durationBits, parentBits, oldRequested;
        if (fscanf(input, "%u %u %u %u %u", &present, &kind, &durationBits, &parentBits, &oldRequested) != 5) return 6;
        state.stateChangeType = kind; memcpy(&state.updateTime, &durationBits, 4);
        memcpy(&parent.ParentUpdateTime, &parentBits, 4); parent.RequestedState = oldRequested;
#endif
        FableUiStateTaskNode head = {}; head.Next = head.Previous = &head;
        for (unsigned i = 0; i < count; ++i)
        {
            FableUiStateTaskNode* task = static_cast<FableUiStateTaskNode*>(malloc(sizeof(*task)));
            if (!task) return 4;
            task->Next = &head; task->Previous = head.Previous; task->Tasks = tasks;
            head.Previous->Next = task; head.Previous = task;
        }
        parent.StatesToDo = &head;
        Child children[3]; memset(children, 0, sizeof(children));
        FableUiComponentCountedStorage entries[3] = {};
        for (unsigned i = 0; i < 3; ++i)
        {
            children[i].Vtable = &table; children[i].Id = i;
            children[i].Parent = (childBits & (1U << (i*2))) ? &parent : 0;
            children[i].Kind = (childBits & (2U << (i*2))) ? 8 : 0;
            entries[i].Data = &children[i];
        }
        parent.Children.Begin = entries; parent.Children.End = parent.Children.CapacityEnd = entries+3;
        printf("TRACE");
#ifdef FABLE_TEST_CHANGE_STATE
        FableUiChangeState(&parent, 0, requested);
#else
        FableUiUpdateStateChange(&parent, 0);
#endif
        unsigned remaining = 0;
        for (FableUiStateTaskNode* node = head.Next; node != &head; node = node->Next)
        {
            if (node->Next->Previous != node || node->Previous->Next != node) return 5;
            ++remaining;
        }
        printf(" END %u %u %u %u %u %u %u", parent.StatesBeingDone, parent.StatesDone,
            parent.CurrentState, parent.TargetState, parent.PreviousState, remaining, parent.Children.Size());
#ifdef FABLE_TEST_CHANGE_STATE
        unsigned timeBits; memcpy(&timeBits, &parent.UpdateTime, 4);
        printf(" REQUEST %u %u QUEUE", parent.RequestedState, timeBits);
        for (FableUiStateTaskNode* task = head.Next; task != &head; task = task->Next) printf(" %u", task->Tasks);
#endif
        printf("\n");
        while (head.Next != &head)
        { FableUiStateTaskNode* node = head.Next; head.Next = node->Next; free(node); }
    }
    fclose(input); return 0;
}
