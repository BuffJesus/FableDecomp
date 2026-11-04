// Behaviour fixture for CKeyboardDX::GetKeyName (provider CKeyboardDX_GetKeyName_00ab6180.cpp; it does not define it).
// The keyboard is raw storage at the retail offsets fable_keyboard_dx.h pins; the DirectInput device double checks the
// DIPROP_KEYNAME request and fills in a name; CWideString's assignment from text is a recording double.
#include "fable_dinput8.h"
#include "fable_keyboard_dx.h"
#include "fable_string.h"
#include <stdio.h>
#include <string.h>

static int g_fail, g_assigns;
static unsigned short g_assigned[64];
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

const CWideString& CWideString::operator=(const wchar_t* text)
{
    ++g_assigns;
    int i = 0;
    for (; text[i] && i < 63; ++i) g_assigned[i] = text[i];
    g_assigned[i] = 0;
    return *this;
}

struct CTestDevice : IDirectInputDevice8W
{
    HRESULT Result;
    DWORD Size, HeaderSize, Obj, How;
    const GUID* Property;
    virtual HRESULT __stdcall QueryInterface(REFIID, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { return 0; }
    virtual HRESULT __stdcall GetCapabilities(void*) { return 0; }
    virtual HRESULT __stdcall EnumObjects(void*, void*, DWORD) { return 0; }
    virtual HRESULT __stdcall GetProperty(REFIID guid, void* header)
    {
        DIPROPSTRING* p = static_cast<DIPROPSTRING*>(header);
        Property = &guid; Size = p->diph.dwSize; HeaderSize = p->diph.dwHeaderSize; Obj = p->diph.dwObj; How = p->diph.dwHow;
        const char* name = "Left Shift";
        int i = 0;
        for (; name[i]; ++i) p->wsz[i] = (unsigned short)name[i];
        p->wsz[i] = 0;
        return Result;
    }
    virtual HRESULT __stdcall SetProperty(REFIID, const void*) { return 0; }
    virtual HRESULT __stdcall Acquire() { return 0; }
    virtual HRESULT __stdcall Unacquire() { return 0; }
    virtual HRESULT __stdcall GetDeviceState(DWORD, void*) { return 0; }
    virtual HRESULT __stdcall GetDeviceData(DWORD, DIDEVICEOBJECTDATA*, DWORD*, DWORD) { return 0; }
};

static unsigned char storage[sizeof(CKeyboardDX)];
static unsigned char nameStorage[16];

int main()
{
    memset(storage, 0, sizeof(storage));
    CKeyboardDX* k = reinterpret_cast<CKeyboardDX*>(storage);
    CTestDevice device; device.Result = 0;
    IDirectInputDevice8W* dev = &device; memcpy(storage + 0x3418, &dev, 4);
    const unsigned short dik = 0x2A;                         // DIK_LSHIFT for KB_A in this test
    memcpy(storage + 0x351C + KB_A * 2, &dik, 2);
    CWideString& name = *reinterpret_cast<CWideString*>(nameStorage);

    CHECK(k->CKeyboardDX::GetKeyName(KB_A, name));
    CHECK(device.Property == &DIPROP_KEYNAME);
    CHECK(device.Size == sizeof(DIPROPSTRING) && device.HeaderSize == sizeof(DIPROPHEADER));
    CHECK(device.Obj == 0x2A && device.How == DIPH_BYOFFSET);
    CHECK(g_assigns == 1 && g_assigned[0] == 'L' && g_assigned[5] == 'S' && g_assigned[10] == 0);

    device.Result = (HRESULT)0x80070057L;                    // DirectInput failure: no name
    CHECK(!k->CKeyboardDX::GetKeyName(KB_A, name));
    CHECK(g_assigns == 1);

    printf(g_fail ? "FAIL\n" : "KEYBOARD_DX_GETKEYNAME_OK\n");
    return g_fail != 0;
}
