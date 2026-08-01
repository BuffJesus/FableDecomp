// NUISystem::CFrontEndManager::Action @ 0x0059A238
// VC7.1, x86, PDB signature:
//   void __thiscall Action(CCountedPointer<CAction>&)
//
// GRADE: behavior-slice (NOT byte-identical). Do NOT count as decomp.
//   This file is an address-isolated behavioral reconstruction: the 1,486-byte
//   retail function's profile deletion/loading, trophy setup, localized
//   invalid-save/delete-profile string construction, and virtual-keyboard
//   payload traversal are all pushed behind narrowly named `extern` ABI
//   helpers rather than decomposed inline, so it CANNOT recompile to the retail
//   .text bytes. It reproduces the observable initial frontend routes (entry
//   clock/installer side effects, used-key lookup before forward navigation,
//   Back, Press Start, quit, invalid-save ordering) only.
//
// Byte-match oracle (captured 2026-07-31, full body): the exact retail bytes
// are in  rebuild/oracles/cfrontendmanager_action.expected.tsv  and the
// complete Ghidra decompilation (every hidden branch) is in
// rebuild/oracles/cfrontendmanager_action.decomp.txt. A byte-identical
// candidate is being authored separately; until it lands and diffs EXACT/RELOC,
// this slice must not be presented as exact decompilation.

struct CFrontEndAction
{
    long id;
};

struct CFrontEndActionPointer
{
    CFrontEndAction* pointer;
};

extern "C" double __cdecl FableFrontEndActionGetTime();
extern "C" int __cdecl FableFrontEndActionInstallerActive();
extern "C" void __cdecl FableFrontEndActionPauseInstaller();
extern "C" void __cdecl FableFrontEndActionUnpauseInstaller();

extern "C" void* __fastcall
FableFrontEndActionResolveUsedKey(void* manager, void*, unsigned long key);
extern "C" void __fastcall
FableFrontEndActionGotoNext(
    void* manager,
    void*,
    void* component,
    int force);
extern "C" void __fastcall
FableFrontEndActionGotoPrevious(void* manager, void*, int force);
extern "C" void __fastcall
FableFrontEndActionDoPressStart(void* manager, void*);
extern "C" void __fastcall
FableFrontEndActionGotoProfileMenu(void* manager, void*);
extern "C" void __fastcall
FableFrontEndActionRequestQuit(void* manager, void*);
extern "C" void __fastcall
FableFrontEndActionShowInvalidSave(
    void* manager,
    void*,
    CFrontEndAction* action);
extern "C" long __fastcall
FableFrontEndActionRefreshSavedGames(void* manager, void*);
extern "C" void __fastcall
FableFrontEndActionNoSavedGameFallback(void* manager, void*);
extern "C" long __fastcall
FableFrontEndActionRefreshProfilesForDelete(void* manager, void*);
extern "C" void __fastcall
FableFrontEndActionLoadProfile(
    void* manager,
    void*,
    CFrontEndAction* action);
extern "C" void __fastcall
FableFrontEndActionBeginNewProfile(void* manager, void*);
extern "C" void __fastcall
FableFrontEndActionKeyboardConfirm(void* keyboard, void*);
extern "C" void __fastcall
FableFrontEndActionKeyboardCancel(void* keyboard, void*);
extern "C" void __fastcall
FableFrontEndActionSetScoreboardOptions(void* manager, void*);

static unsigned char& ByteField(void* object, unsigned long offset)
{
    return *(unsigned char*)((char*)object + offset);
}

static void*& PointerField(void* object, unsigned long offset)
{
    return *(void**)((char*)object + offset);
}

static unsigned long& UnsignedField(void* object, unsigned long offset)
{
    return *(unsigned long*)((char*)object + offset);
}

static void Forward(
    void* manager,
    unsigned long usedKey,
    int force)
{
    void* component =
        FableFrontEndActionResolveUsedKey(manager, 0, usedKey);
    FableFrontEndActionGotoNext(manager, 0, component, force);
}

extern "C" void __fastcall
CFrontEndManager_Action_0059a238(
    void* manager,
    void*,
    CFrontEndActionPointer* countedAction)
{
    *(double*)((char*)manager + 0xd0) =
        FableFrontEndActionGetTime();

    if (ByteField(manager, 0xd8) == 0 &&
        ByteField(manager, 0xd9) == 0 &&
        FableFrontEndActionInstallerActive() != 0)
    {
        FableFrontEndActionPauseInstaller();
        ByteField(manager, 0xd8) = 1;
    }

    CFrontEndAction* action = countedAction->pointer;
    const long id = action->id;
    unsigned long usedKey;

    switch (id)
    {
    case 9:
        usedKey = 0x01;
        break;
    case 11:
        usedKey = 0x03;
        break;
    case 12:
        usedKey = 0x04;
        break;
    case 13:
        usedKey = 0x05;
        break;
    case 14:
        usedKey = 0x06;
        break;

    case 15:
    {
        if (ByteField(manager, 0xd8) != 0 &&
            FableFrontEndActionInstallerActive() != 0)
        {
            FableFrontEndActionUnpauseInstaller();
            ByteField(manager, 0xd8) = 0;
            ByteField(manager, 0xd9) = 1;
        }

        void* component = PointerField(manager, 0x1c);
        void** vtable = *(void***)component;
        typedef void (__fastcall *QuitComponent)(void*, void*);
        ((QuitComponent)vtable[0x10 / 4])(component, 0);
        ByteField(component, 0x29) = 1;
        return;
    }

    case 16:
        FableFrontEndActionGotoProfileMenu(manager, 0);
        return;

    case 66:
        if (FableFrontEndActionRefreshSavedGames(manager, 0) == 0 &&
            UnsignedField(manager, 0xbc) == 0)
        {
            FableFrontEndActionNoSavedGameFallback(manager, 0);
            return;
        }
        usedKey = 0x08;
        break;

    case 67:
        usedKey = 0x09;
        break;

    case 86:
        FableFrontEndActionGotoPrevious(manager, 0, 0);
        return;

    case 219:
        usedKey = 0x0f;
        break;

    case 220:
        FableFrontEndActionShowInvalidSave(manager, 0, action);
        usedKey = 0x10;
        break;

    case 229:
        FableFrontEndActionDoPressStart(manager, 0);
        return;

    case 250:
        usedKey =
            FableFrontEndActionRefreshProfilesForDelete(manager, 0) == 0
                ? 0x0c
                : 0x15;
        break;

    case 283:
        usedKey = 0x16;
        break;

    case 292:
        FableFrontEndActionLoadProfile(manager, 0, action);
        Forward(manager, 0x00, 1);
        return;

    case 293:
        FableFrontEndActionBeginNewProfile(manager, 0);
        return;

    case 294:
    {
        void* keyboard = PointerField(manager, 0x60);
        if (keyboard != 0)
            FableFrontEndActionKeyboardConfirm(keyboard, 0);
        return;
    }

    case 295:
    {
        void* keyboard = PointerField(manager, 0x60);
        if (keyboard != 0)
            FableFrontEndActionKeyboardCancel(keyboard, 0);
        return;
    }

    case 296:
        FableFrontEndActionRequestQuit(manager, 0);
        return;

    case 297:
        usedKey = 0x18;
        break;

    case 312:
        FableFrontEndActionSetScoreboardOptions(manager, 0);
        usedKey = 0x19;
        break;

    case 314:
        usedKey = 0x1a;
        break;

    case 321:
        usedKey = 0x1c;
        break;

    case 3025:
        usedKey = 0x14;
        break;

    default:
        return;
    }

    Forward(manager, usedKey, 0);
}
