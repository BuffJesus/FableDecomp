#include "fable_gfuninitialise.h"

#include <new>

extern "C" CCharString* __fastcall
FableCharStringAppendCString_0099F600(
    CCharString* result,
    const CCharString* left,
    const char* right);

struct FableSystemInitErrorStorage
{
    CCharStringData* slots[11];
};

FABLE_STATIC_ASSERT(sizeof(FableSystemInitErrorStorage) == 0x2C);

#define FABLE_DISPLAY_INIT_ERROR(result_pointer, suffix_text) \
    do \
    { \
        GFGetSystemManager()->DisplayCriticalMessage( \
            static_cast<const char*>( \
                *FableCharStringAppendCString_0099F600( \
                    result_pointer, \
                    reinterpret_cast<const CCharString*>( \
                        &storage.slots[0]), \
                    suffix_text))); \
        (result_pointer)->~CCharString(); \
        reinterpret_cast<CCharString*>( \
            &storage.slots[0])->~CCharString(); \
        return; \
    } while (0)

void FABLE_STDCALL GFHandleSystemInitError(unsigned long errorCode)
{
    FableSystemInitErrorStorage storage;
    new (&storage.slots[0]) CCharString("Fable", -1);

    switch (errorCode)
    {
    case 2:
        FABLE_DISPLAY_INIT_ERROR(
            reinterpret_cast<CCharString*>(&errorCode),
            " was unable to initialise the display. If you are running in a window, make sure the pixel depth is correct, and that your 3D card can run in a window. Use the -fullscreen option to force fullscreen, and/or -forceprimary if you have a secondary 3D accelerator you wish to ignore. Otherwise, make sure you have closed down all other applications and try again.");
    case 4:
        FABLE_DISPLAY_INIT_ERROR(
            reinterpret_cast<CCharString*>(&storage.slots[1]),
            "  was unable to initialise a window. You may need to reboot your machine.");
    case 5:
        FABLE_DISPLAY_INIT_ERROR(
            reinterpret_cast<CCharString*>(&storage.slots[2]),
            "  was unable to access the hard disc. Lord knows why.");
    case 6:
        FABLE_DISPLAY_INIT_ERROR(
            reinterpret_cast<CCharString*>(&storage.slots[3]),
            "  was unable to initialise a joystick. Please make sure you have one connected.");
    case 10:
        FABLE_DISPLAY_INIT_ERROR(
            reinterpret_cast<CCharString*>(&storage.slots[4]),
            "  was unable to initialise a mouse. Please make sure you have one connected.");
    case 12:
        FABLE_DISPLAY_INIT_ERROR(
            reinterpret_cast<CCharString*>(&storage.slots[5]),
            "  was unable to initialise the input manager.");
    case 13:
        FABLE_DISPLAY_INIT_ERROR(
            reinterpret_cast<CCharString*>(&storage.slots[6]),
            "  was unable to initialise a your 3D card. If you are running in a window, make sure the pixel depth is correct, and that your 3D card can run in a window. Otherwise, make sure you have closed down all other applications and try again.");
    case 14:
        FABLE_DISPLAY_INIT_ERROR(
            reinterpret_cast<CCharString*>(&storage.slots[7]),
            "  was unable to initialise debugging information. Lord knows why.");
    case 15:
        FABLE_DISPLAY_INIT_ERROR(
            reinterpret_cast<CCharString*>(&storage.slots[8]),
            "  was unable to initialise. Lord knows why.");
    case 16:
        FABLE_DISPLAY_INIT_ERROR(
            reinterpret_cast<CCharString*>(&storage.slots[9]),
            "  was unable to initialise DirectX. You may need to reboot your machine.");
    case 17:
        FABLE_DISPLAY_INIT_ERROR(
            reinterpret_cast<CCharString*>(&storage.slots[10]),
            "  was unable to initialise the network manager.");
    default:
        GFGetSystemManager()->DisplayCriticalMessage(
            "Unknown initialisation error. Bugger.");
        reinterpret_cast<CCharString*>(
            &storage.slots[0])->~CCharString();
        return;
    }
}

#undef FABLE_DISPLAY_INIT_ERROR
