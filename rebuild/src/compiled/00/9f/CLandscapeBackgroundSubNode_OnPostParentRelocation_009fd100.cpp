#include "rebuild_abi.h"

// CLandscapeBackgroundSubNode::OnPostParentRelocation @ 0x009fd100
// __fastcall: stores the arg into node fields +0x26c and +0x19c. One stack arg, ret 4.
struct FableNode_009fd100
{
    void OnPostParentRelocation(void* value);
};

void FableNode_009fd100::OnPostParentRelocation(void* value)
{
    *(void**)((char*)this + 0x26c) = value;
    *(void**)((char*)this + 0x19c) = value;
}