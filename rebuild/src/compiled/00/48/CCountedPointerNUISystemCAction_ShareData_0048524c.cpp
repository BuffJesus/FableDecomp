#pragma optimize("s",on)
// CCountedPointer<NUISystem::CAction>::ShareData @ 0x0048524C  (38 bytes)
//
// Genuine C++ decompilation of the leading function only. The retail oracle
// slice over-captures: after ShareData's `ret 8` (offset 0x23..0x26) there is a
// separate, unrelated __fastcall forwarding thunk (push [esp+4]; call; ret 8)
// starting at 0x26. Only ShareData is reconstructed here; real_len = 0x26 = 38.
//
// A counted-pointer holder stores the referenced object plus a pointer to the
// shared reference-count control block ("pointer info"). ShareData rebinds the
// holder to a new object/control pair:
//   * If the control block is already the one we hold, nothing changes.
//   * Otherwise release the current ownership, install BOTH new holder fields,
//     and only then acquire a reference on the new control block (if non-null).
//
// Retail idiom (this in ecx = esi, edi = new_pointer_info):
//   if (this->pointer_info == new_pointer_info) return;
//   this->ReleaseData();                 // __fastcall, no stack args
//   this->action       = new_action;     // stores happen BEFORE the null test
//   this->pointer_info = new_pointer_info;// is consumed (test edi,edi is hoisted
//   if (new_pointer_info)                 //  above the two stores by the sched.)
//       ++new_pointer_info->reference_count;

// Shared reference-count control block for a counted CAction pointer.
struct FrontEndActionPointerInfo_0048524c
{
    long  reference_count;   // +0x0  ++'d on acquire
    void* destroy_callback;  // +0x4
    void* action;            // +0x8
};

class CCountedPointerNUISystemCAction_0048524c
{
public:
    void*                              action;        // +0x0
    FrontEndActionPointerInfo_0048524c* pointer_info;  // +0x4

    // Masked callee: releases the currently-held ownership. __fastcall, no
    // stack arguments (call rel32 with no post-call esp cleanup).
    void ReleaseData();

    void ShareData(
        void*                               new_action,
        FrontEndActionPointerInfo_0048524c* new_pointer_info);
};

void CCountedPointerNUISystemCAction_0048524c::ShareData(
    void*                               new_action,
    FrontEndActionPointerInfo_0048524c* new_pointer_info)
{
    if (this->pointer_info == new_pointer_info)
        return;

    this->ReleaseData();

    this->action       = new_action;
    this->pointer_info = new_pointer_info;

    if (new_pointer_info)
        ++new_pointer_info->reference_count;
}