#include <cstdint>

struct CEnginePrimitive;
struct CSystemManager;

struct CCharString
{
    std::uint32_t opaque;
};
static_assert(sizeof(CCharString) == 4);

struct BranchCleanupSlot
{
    std::uint32_t opaque;
};
static_assert(sizeof(BranchCleanupSlot) == 4);

extern "C" void __thiscall CCharString_ctor(CCharString* self, char const* text, int length);
extern "C" CCharString* __fastcall CCharString__AppendCString(
    void* destination,
    CCharString const* base,
    char const* suffix);
extern "C" char const* __thiscall CCharString_operator_char_const(CCharString const* self);

extern "C" CSystemManager* __stdcall GFHandleSystemInitError_AddChildPrimitiveThunk(CEnginePrimitive* message);
extern "C" void __thiscall CSystemManager__DisplayCriticalMessage(CSystemManager* self, char const* message);
extern "C" void __thiscall GFHandleSystemInitError_CleanupThunk(void* slot);

extern "C" void __stdcall GFHandleSystemInitError(std::uint32_t errorCode)
{
    CCharString fable;
    BranchCleanupSlot local_28;
    BranchCleanupSlot local_24;
    BranchCleanupSlot local_20;
    BranchCleanupSlot local_1c;
    BranchCleanupSlot local_18;
    BranchCleanupSlot local_14;
    BranchCleanupSlot local_10;
    BranchCleanupSlot local_c;
    BranchCleanupSlot local_8;
    BranchCleanupSlot local_4;

    CCharString_ctor(&fable, "Fable", -1);

    switch (errorCode)
    {
    case 2:
    {
        CCharString* const appended = CCharString__AppendCString(
            &errorCode,
            &fable,
            " was unable to initialise the display. If you are running in a window, make sure the pixel depth is correct, and that your 3D card can run in a window. Use the -fullscreen option to force fullscreen, and/or -forceprimary if you have a secondary 3D accelerator you wish to ignore. Otherwise, make sure you have closed down all other applications and try again.");
        char const* const message = CCharString_operator_char_const(appended);
        CSystemManager* const systemManager =
            GFHandleSystemInitError_AddChildPrimitiveThunk(reinterpret_cast<CEnginePrimitive*>(const_cast<char*>(message)));
        CSystemManager__DisplayCriticalMessage(systemManager, message);
        GFHandleSystemInitError_CleanupThunk(&errorCode);
        GFHandleSystemInitError_CleanupThunk(&fable);
        return;
    }

    case 4:
    {
        CCharString* const appended = CCharString__AppendCString(
            &local_28,
            &fable,
            "  was unable to initialise a window. You may need to reboot your machine.");
        char const* const message = CCharString_operator_char_const(appended);
        CSystemManager* const systemManager =
            GFHandleSystemInitError_AddChildPrimitiveThunk(reinterpret_cast<CEnginePrimitive*>(const_cast<char*>(message)));
        CSystemManager__DisplayCriticalMessage(systemManager, message);
        GFHandleSystemInitError_CleanupThunk(&local_28);
        GFHandleSystemInitError_CleanupThunk(&fable);
        return;
    }

    case 5:
    {
        CCharString* const appended = CCharString__AppendCString(
            &local_20,
            &fable,
            "  was unable to access the hard disc. Lord knows why.");
        char const* const message = CCharString_operator_char_const(appended);
        CSystemManager* const systemManager =
            GFHandleSystemInitError_AddChildPrimitiveThunk(reinterpret_cast<CEnginePrimitive*>(const_cast<char*>(message)));
        CSystemManager__DisplayCriticalMessage(systemManager, message);
        GFHandleSystemInitError_CleanupThunk(&local_20);
        GFHandleSystemInitError_CleanupThunk(&fable);
        return;
    }

    case 6:
    {
        CCharString* const appended = CCharString__AppendCString(
            &local_14,
            &fable,
            "  was unable to initialise a joystick. Please make sure you have one connected.");
        char const* const message = CCharString_operator_char_const(appended);
        CSystemManager* const systemManager =
            GFHandleSystemInitError_AddChildPrimitiveThunk(reinterpret_cast<CEnginePrimitive*>(const_cast<char*>(message)));
        CSystemManager__DisplayCriticalMessage(systemManager, message);
        GFHandleSystemInitError_CleanupThunk(&local_14);
        GFHandleSystemInitError_CleanupThunk(&fable);
        return;
    }

    case 10:
    {
        CCharString* const appended = CCharString__AppendCString(
            &local_10,
            &fable,
            "  was unable to initialise a mouse. Please make sure you have one connected.");
        char const* const message = CCharString_operator_char_const(appended);
        CSystemManager* const systemManager =
            GFHandleSystemInitError_AddChildPrimitiveThunk(reinterpret_cast<CEnginePrimitive*>(const_cast<char*>(message)));
        CSystemManager__DisplayCriticalMessage(systemManager, message);
        GFHandleSystemInitError_CleanupThunk(&local_10);
        GFHandleSystemInitError_CleanupThunk(&fable);
        return;
    }

    case 0x0C:
    {
        CCharString* const appended = CCharString__AppendCString(
            &local_18,
            &fable,
            "  was unable to initialise the input manager.");
        char const* const message = CCharString_operator_char_const(appended);
        CSystemManager* const systemManager =
            GFHandleSystemInitError_AddChildPrimitiveThunk(reinterpret_cast<CEnginePrimitive*>(const_cast<char*>(message)));
        CSystemManager__DisplayCriticalMessage(systemManager, message);
        GFHandleSystemInitError_CleanupThunk(&local_18);
        GFHandleSystemInitError_CleanupThunk(&fable);
        return;
    }

    case 0x0D:
    {
        CCharString* const appended = CCharString__AppendCString(
            &local_c,
            &fable,
            "  was unable to initialise a your 3D card. If you are running in a window, make sure the pixel depth is correct, and that your 3D card can run in a window. Otherwise, make sure you have closed down all other applications and try again.");
        char const* const message = CCharString_operator_char_const(appended);
        CSystemManager* const systemManager =
            GFHandleSystemInitError_AddChildPrimitiveThunk(reinterpret_cast<CEnginePrimitive*>(const_cast<char*>(message)));
        CSystemManager__DisplayCriticalMessage(systemManager, message);
        GFHandleSystemInitError_CleanupThunk(&local_c);
        GFHandleSystemInitError_CleanupThunk(&fable);
        return;
    }

    case 0x0E:
    {
        CCharString* const appended = CCharString__AppendCString(
            &local_8,
            &fable,
            "  was unable to initialise debugging information. Lord knows why.");
        char const* const message = CCharString_operator_char_const(appended);
        CSystemManager* const systemManager =
            GFHandleSystemInitError_AddChildPrimitiveThunk(reinterpret_cast<CEnginePrimitive*>(const_cast<char*>(message)));
        CSystemManager__DisplayCriticalMessage(systemManager, message);
        GFHandleSystemInitError_CleanupThunk(&local_8);
        GFHandleSystemInitError_CleanupThunk(&fable);
        return;
    }

    case 0x0F:
    {
        CCharString* const appended = CCharString__AppendCString(
            &local_4,
            &fable,
            "  was unable to initialise. Lord knows why.");
        char const* const message = CCharString_operator_char_const(appended);
        CSystemManager* const systemManager =
            GFHandleSystemInitError_AddChildPrimitiveThunk(reinterpret_cast<CEnginePrimitive*>(const_cast<char*>(message)));
        CSystemManager__DisplayCriticalMessage(systemManager, message);
        GFHandleSystemInitError_CleanupThunk(&local_4);
        GFHandleSystemInitError_CleanupThunk(&fable);
        return;
    }

    case 0x10:
    {
        CCharString* const appended = CCharString__AppendCString(
            &local_24,
            &fable,
            "  was unable to initialise DirectX. You may need to reboot your machine.");
        char const* const message = CCharString_operator_char_const(appended);
        CSystemManager* const systemManager =
            GFHandleSystemInitError_AddChildPrimitiveThunk(reinterpret_cast<CEnginePrimitive*>(const_cast<char*>(message)));
        CSystemManager__DisplayCriticalMessage(systemManager, message);
        GFHandleSystemInitError_CleanupThunk(&local_24);
        GFHandleSystemInitError_CleanupThunk(&fable);
        return;
    }

    case 0x11:
    {
        CCharString* const appended = CCharString__AppendCString(
            &local_1c,
            &fable,
            "  was unable to initialise the network manager.");
        char const* const message = CCharString_operator_char_const(appended);
        CSystemManager* const systemManager =
            GFHandleSystemInitError_AddChildPrimitiveThunk(reinterpret_cast<CEnginePrimitive*>(const_cast<char*>(message)));
        CSystemManager__DisplayCriticalMessage(systemManager, message);
        GFHandleSystemInitError_CleanupThunk(&local_1c);
        GFHandleSystemInitError_CleanupThunk(&fable);
        return;
    }

    default:
    {
        char const* const message = "Unknown initialisation error. Bugger.";
        CSystemManager* const systemManager =
            GFHandleSystemInitError_AddChildPrimitiveThunk(reinterpret_cast<CEnginePrimitive*>(const_cast<char*>(message)));
        CSystemManager__DisplayCriticalMessage(systemManager, message);
        GFHandleSystemInitError_CleanupThunk(&fable);
        return;
    }
    }
}