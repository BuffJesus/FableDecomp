#include <stdio.h>
#include <string.h>

#include "fable_system.h"

namespace
{
    enum ConstructionEvent
    {
        kWideStringConstructed = 1,
        kCharStringConstructed = 2,
        kRegionConstructed = 3,
        kWideStringAssigned = 4
    };

    struct Event
    {
        ConstructionEvent kind;
        const void* object;
        wchar_t value;
    };

    Event g_events[8];
    int g_eventCount = 0;

    void Record(
        ConstructionEvent kind,
        const void* object,
        wchar_t value)
    {
        g_events[g_eventCount].kind = kind;
        g_events[g_eventCount].object = object;
        g_events[g_eventCount].value = value;
        ++g_eventCount;
    }
}

void* operator new(size_t, void* address)
{
    return address;
}

CWideString::CWideString()
    : storage_(0)
{
    Record(kWideStringConstructed, this, L'\0');
}

CWideString::~CWideString()
{
}

const CWideString& CWideString::operator=(const wchar_t* text)
{
    storage_ = reinterpret_cast<CWideStringData*>(
        text[0] == L'b' ? 0x0000000BUL : 0x0000000CUL);
    Record(kWideStringAssigned, this, text[0]);
    return *this;
}

CCharString::CCharString()
    : storage_(0)
{
    Record(kCharStringConstructed, this, L'\0');
}

CCharString::~CCharString()
{
}

void FABLE_FASTCALL FableConstructWideString(CWideString* value)
{
    new (value) CWideString;
}

void FABLE_FASTCALL FableConstructCharString(CCharString* value)
{
    new (value) CCharString;
}

const CWideString& FABLE_FASTCALL FableAssignWideString(
    CWideString* value,
    void*,
    const wchar_t* text)
{
    return *value = text;
}

void CRegionDef::ConstructStorage()
{
    Record(kRegionConstructed, this, L'\0');
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
    const fable_u8* region = &value->regionStorage[0];

    if (g_eventCount != 8)
        return 1;
    if (g_events[0].kind != kWideStringConstructed ||
        g_events[0].object != base + 0x0C)
        return 2;
    if (g_events[1].kind != kCharStringConstructed ||
        g_events[1].object != base + 0x2C)
        return 3;
    if (g_events[2].kind != kCharStringConstructed ||
        g_events[2].object != base + 0x30)
        return 4;
    if (g_events[3].kind != kRegionConstructed ||
        g_events[3].object != base + 0x5C)
        return 5;
    if (g_events[4].kind != kWideStringConstructed ||
        g_events[4].object != base + 0xB8)
        return 6;
    if (g_events[5].kind != kWideStringConstructed ||
        g_events[5].object != base + 0xBC)
        return 7;
    if (g_events[6].kind != kWideStringAssigned ||
        g_events[6].object != base + 0xB8 ||
        g_events[6].value != L'b')
        return 8;
    if (g_events[7].kind != kWideStringAssigned ||
        g_events[7].object != base + 0xBC ||
        g_events[7].value != L'C')
        return 9;

    if (value->displayWidth != 128 ||
        value->displayHeight != 64)
        return 10;
    if (value->displayDepth != 64.0f ||
        value->nearPlane != 2.0f ||
        value->scale != 1.0f)
        return 11;
    if (value->memoryBudget != 0x00040000UL)
        return 12;
    if (value->enabled != 1 ||
        value->useDefaultDisplay != 1)
        return 13;
    if (value->unknown14 != 0 ||
        value->unknown1C != 0 ||
        value->unknown24 != 0 ||
        value->unknown28 != 0 ||
        value->unknownC0 != 0 ||
        value->unknownC4 != 0)
        return 14;
    if (value->unknown11 != 0 ||
        value->unknown19 != 0 ||
        value->unknown20 != 0 ||
        value->unknown21 != 0 ||
        value->unknown58 != 0 ||
        value->unknown59 != 0 ||
        value->unknown5A != 0)
        return 15;
    if (memcmp(region + 0x44, "BBB GUID", 8) != 0 ||
        region[0x4C] != '\0')
        return 16;
    if (*reinterpret_cast<const fable_u32*>(region + 0x40) != 0 ||
        *reinterpret_cast<const fable_u32*>(region + 0x58) !=
            0x00020000UL)
        return 17;
    if (*reinterpret_cast<const fable_u32*>(base + 0xB8) !=
            0x0000000BUL ||
        *reinterpret_cast<const fable_u32*>(base + 0xBC) !=
            0x0000000CUL)
        return 18;

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
