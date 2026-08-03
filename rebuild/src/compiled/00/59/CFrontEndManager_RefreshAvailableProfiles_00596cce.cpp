// NUISystem::CFrontEndManager::RefreshAvailableProfiles @ 0x00596CCE
//
// GRADE: behavior-slice (NOT byte-identical). Status NO_ORACLE (never diffed).
//   Retail is an 824-byte inlined construction pipeline (vector,
//   CCountedPointer<CComponent>, CCountedPointer<CAction>, CActionParamString,
//   CCharString temporary lifetimes). This source keeps the complete OBSERVABLE
//   profile-list contract but pushes all of that inlined object
//   construction/ownership behind 11 named `extern` boundaries
//   (FableRefreshProfiles*), so it CANNOT recompile to the retail .text. It is a
//   faithful behavioral reconstruction, not exact decomp -- do not count it as
//   byte-identical.
//   To reach EXACT/RELOC: capture the retail oracle for 0x00596CCE (Ghidra
//   DumpFuncBytesAndC), then decompose the 11 construction boundaries inline
//   (model the real vector/CCountedPointer/CCharString ctors/dtors) so the
//   inlined bytes are present. Same TU as CFrontEndManager::Action, so the
//   per-TU flag is /O2 /Oy- (/G7); frame-kept. Large effort; not attempted here.

struct FableRefreshProfilesWideString
{
    void* data;
};

struct FableRefreshProfilesNameVector
{
    FableRefreshProfilesWideString* begin;
    FableRefreshProfilesWideString* end;
    FableRefreshProfilesWideString* capacity;
};

extern "C" void __fastcall
FableRefreshProfilesGetNames(
    void* manager,
    void*,
    FableRefreshProfilesNameVector* names);
extern "C" void* __fastcall
FableRefreshProfilesBeginList(
    void* manager,
    void*,
    unsigned long usedKey);
extern "C" void* __fastcall
FableRefreshProfilesCreateButton(
    void* manager,
    void*,
    const FableRefreshProfilesWideString* name,
    float y,
    unsigned char newProfile);
extern "C" unsigned char __fastcall
FableRefreshProfilesVerifyName(
    void* manager,
    void*,
    const FableRefreshProfilesWideString* name);
extern "C" void* __fastcall
FableRefreshProfilesCreateAction(
    void* manager,
    void*,
    unsigned long actionId,
    const FableRefreshProfilesWideString* name);
extern "C" void __fastcall
FableRefreshProfilesAttachAction(
    void* button,
    void*,
    void* action);
extern "C" void __fastcall
FableRefreshProfilesAppendButton(
    void* list,
    void*,
    void* button);
extern "C" void __fastcall
FableRefreshProfilesReleaseAction(void* action, void*);
extern "C" void __fastcall
FableRefreshProfilesReleaseButton(void* button, void*);
extern "C" void __fastcall
FableRefreshProfilesFinishList(void* list, void*);
extern "C" void __fastcall
FableRefreshProfilesDestroyNames(
    FableRefreshProfilesNameVector* names,
    void*);

extern "C" long __fastcall
CFrontEndManager_RefreshAvailableProfiles_00596cce(
    void* manager,
    void*)
{
    FableRefreshProfilesNameVector names;
    names.begin = 0;
    names.end = 0;
    names.capacity = 0;
    FableRefreshProfilesGetNames(manager, 0, &names);

    void* list = FableRefreshProfilesBeginList(manager, 0, 7);

    void* button =
        FableRefreshProfilesCreateButton(manager, 0, 0, 0.0f, 1);
    void* action =
        FableRefreshProfilesCreateAction(manager, 0, 0x125, 0);
    FableRefreshProfilesAttachAction(button, 0, action);
    FableRefreshProfilesAppendButton(list, 0, button);
    void* newProfileButton = button;
    void* newProfileAction = action;

    const long count = (long)(names.end - names.begin);
    float y = 50.0f;
    for (long index = 0; index != count; ++index)
    {
        const FableRefreshProfilesWideString* name =
            names.begin + index;
        button =
            FableRefreshProfilesCreateButton(
                manager,
                0,
                name,
                y,
                0);
        const unsigned long actionId =
            FableRefreshProfilesVerifyName(manager, 0, name)
                ? 0x124
                : 0xdb;
        action =
            FableRefreshProfilesCreateAction(
                manager,
                0,
                actionId,
                name);
        FableRefreshProfilesAttachAction(button, 0, action);
        FableRefreshProfilesAppendButton(list, 0, button);
        FableRefreshProfilesReleaseAction(action, 0);
        FableRefreshProfilesReleaseButton(button, 0);
        y += 30.0f;
    }

    FableRefreshProfilesFinishList(list, 0);
    FableRefreshProfilesReleaseAction(newProfileAction, 0);
    FableRefreshProfilesReleaseButton(newProfileButton, 0);
    FableRefreshProfilesDestroyNames(&names, 0);
    return count;
}
