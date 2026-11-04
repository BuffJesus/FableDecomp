#include <stdio.h>
#include <string.h>

#include "fable_system.h"

namespace
{
    enum ConstructionEvent
    {
        kWideStringConstructed = 1,
        kCharStringConstructed = 2,
        kDisplayInitConstructed = 3,
        kWideStringAssigned = 4
    };

    struct Event
    {
        ConstructionEvent kind;
        const void* object;
        const wchar_t* text;
    };

    Event g_events[16];
    int g_eventCount = 0;

    void Record(
        ConstructionEvent kind,
        const void* object,
        const wchar_t* text)
    {
        if (g_eventCount < 16)
        {
            g_events[g_eventCount].kind = kind;
            g_events[g_eventCount].object = object;
            g_events[g_eventCount].text = text;
        }
        ++g_eventCount;
    }

    bool IsEvent(
        int index,
        ConstructionEvent kind,
        const void* object)
    {
        return g_events[index].kind == kind &&
            g_events[index].object == object;
    }
}

void* operator new(size_t, void* address)
{
    return address;
}

CWideString::CWideString()
    : storage_(0)
{
    Record(kWideStringConstructed, this, 0);
}

CWideString::~CWideString()
{
}

const CWideString& CWideString::operator=(const wchar_t* text)
{
    Record(kWideStringAssigned, this, text);
    return *this;
}

CCharString::CCharString()
    : storage_(0)
{
    Record(kCharStringConstructed, this, 0);
}

CCharString::~CCharString()
{
}

CDisplayManagerInit::CDisplayManagerInit()
{
    Record(kDisplayInitConstructed, this, 0);
}

static int VerifyConstruction()
{
    union AlignedStorage
    {
        double alignment;
        fable_u8 bytes[sizeof(CSystemManagerInit)];
    } storage;

    memset(storage.bytes, 0xCD, sizeof(storage.bytes));
    g_eventCount = 0;

    CSystemManagerInit* value =
        new (storage.bytes) CSystemManagerInit;

    const fable_u8* base = storage.bytes;

    // The display init's own ShaderResourcePath is built before its body runs.
    if (g_eventCount != 9)
        return 1;
    if (!IsEvent(0, kWideStringConstructed, base + 0x0C))
        return 2;
    if (!IsEvent(1, kCharStringConstructed, base + 0x2C))
        return 3;
    if (!IsEvent(2, kCharStringConstructed, base + 0x30))
        return 4;
    if (!IsEvent(3, kWideStringConstructed, base + 0x5C + 0x38) ||
        !IsEvent(4, kDisplayInitConstructed, base + 0x5C))
        return 5;
    if (!IsEvent(5, kWideStringConstructed, base + 0xB8))
        return 6;
    if (!IsEvent(6, kWideStringConstructed, base + 0xBC))
        return 7;
    if (!IsEvent(7, kWideStringAssigned, base + 0xB8) ||
        wcscmp(g_events[7].text, L"bbb.log") != 0)
        return 8;
    if (!IsEvent(8, kWideStringAssigned, base + 0xBC) ||
        wcscmp(g_events[8].text, L"C:\\temp") != 0)
        return 9;

    const CSoundManagerInit& sound = value->SoundManagerInit;
    if (sound.unknown00 != 128 ||
        sound.unknown04 != 64 ||
        sound.unknown08 != 64.0f ||
        sound.unknown0C != 2.0f ||
        sound.unknown10 != 1.0f ||
        sound.unknown14 != 0 ||
        sound.unknown18 != 0 ||
        sound.unknown1C != 0x40000)
        return 10;
    if (value->InputManagerInit.UseMouse ||
        value->InputManagerInit.UseJoystick ||
        value->InputManagerInit.UseKeyboard)
        return 11;
    if (value->InstanceHandle != 0 ||
        value->CommandLine != 0 ||
        value->Flags != 0 ||
        value->ApplicationWindowHandle != 0 ||
        value->AppReinitFunc != 0 ||
        value->AppReinitContext != 0 ||
        value->IsErrorFailureFunc != 0 ||
        value->IMEMessageFunc != 0 ||
        value->MessageFilter != 0)
        return 12;
    if (base[0x10] != 1 ||
        base[0x34] != 1 ||
        value->PreferPrimaryDisplayDevice ||
        value->AttachToExternalWindow ||
        value->PreventExclusiveModeChanges ||
        value->UseAccurateTimer)
        return 13;
    if (value->ScratchBufferSize != 0x20000)
        return 14;
    if (memcmp(value->Identifier, "BBB GUID", 9) != 0)
        return 15;
    for (int i = 9; i < 17; ++i)
    {
        if (value->Identifier[i] != '\0')
            return 16;
    }

    // Left for the caller to fill.
    if (base[0x08] != 0xCD ||
        base[0x18] != 0xCD ||
        base[0x35] != 0xCD ||
        base[0x98] != 0xCD)
        return 17;

    return 0;
}

int main()
{
    const int result = VerifyConstruction();
    if (result != 0)
    {
        printf(
            "FABLETLC_SYSTEM_MANAGER_INIT_BEHAVIOR FAIL code=%d\n",
            result);
        return result;
    }

    printf("FABLETLC_SYSTEM_MANAGER_INIT_BEHAVIOR PASS\n");
    return 0;
}
