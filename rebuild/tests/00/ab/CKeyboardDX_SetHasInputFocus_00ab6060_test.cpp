// Behaviour fixture for CKeyboardDX::SetHasInputFocus (provider CKeyboardDX_SetHasInputFocus_00ab6060.cpp; it does
// not define it). The keyboard is raw storage at the retail offsets fable_keyboard_dx.h pins; the DirectInput device
// is a double that hands out a scripted sequence of buffered-record counts.
#include "fable_dinput8.h"
#include "fable_keyboard_dx.h"
#include <stdio.h>
#include <string.h>

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

struct CTestDevice : IDirectInputDevice8W
{
    HRESULT AcquireResult, UnacquireResult;
    int Acquires, Unacquires, Reads, ReadsBeforeEmpty;
    DWORD LastRequested, LastSize;
    virtual HRESULT __stdcall QueryInterface(REFIID, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { return 0; }
    virtual HRESULT __stdcall GetCapabilities(void*) { return 0; }
    virtual HRESULT __stdcall EnumObjects(void*, void*, DWORD) { return 0; }
    virtual HRESULT __stdcall GetProperty(REFIID, void*) { return 0; }
    virtual HRESULT __stdcall SetProperty(REFIID, const void*) { return 0; }
    virtual HRESULT __stdcall Acquire() { ++Acquires; return AcquireResult; }
    virtual HRESULT __stdcall Unacquire() { ++Unacquires; return UnacquireResult; }
    virtual HRESULT __stdcall GetDeviceState(DWORD, void*) { return 0; }
    virtual HRESULT __stdcall GetDeviceData(DWORD size, DIDEVICEOBJECTDATA*, DWORD* count, DWORD)
    {
        ++Reads; LastSize = size; LastRequested = *count;
        *count = Reads < ReadsBeforeEmpty ? 256 : 0;          // full buffers, then an empty one
        return 0;
    }
};

static unsigned char storage[sizeof(CKeyboardDX)];

int main()
{
    memset(storage, 0, sizeof(storage));
    CKeyboardDX* k = reinterpret_cast<CKeyboardDX*>(storage);
    CTestDevice device;
    memset(&device.AcquireResult, 0, sizeof(CTestDevice) - sizeof(void*));
    IDirectInputDevice8W* dev = &device; memcpy(storage + 0x3418, &dev, 4);

    // Gaining focus while not acquired: acquire, then drain the buffer until DirectInput returns no records.
    device.ReadsBeforeEmpty = 3;
    k->CKeyboardDX::SetHasInputFocus(true);
    CHECK(device.Acquires == 1 && storage[0x3610] == 1);
    CHECK(device.Reads == 3 && device.LastRequested == 256 && device.LastSize == sizeof(DIDEVICEOBJECTDATA));

    // Already acquired: acquire again, no drain.
    k->CKeyboardDX::SetHasInputFocus(true);
    CHECK(device.Acquires == 2 && device.Reads == 3 && storage[0x3610] == 1);

    // Losing focus: unacquire; DI_NOEFFECT also clears Acquired.
    device.UnacquireResult = DI_NOEFFECT;
    k->CKeyboardDX::SetHasInputFocus(false);
    CHECK(device.Unacquires == 1 && storage[0x3610] == 0);
    k->CKeyboardDX::SetHasInputFocus(false);
    CHECK(device.Unacquires == 1);

    // A failed acquire leaves it unacquired (but the buffer is still flushed, as it was not held before).
    device.AcquireResult = (HRESULT)0x80070005L; device.Reads = 0; device.ReadsBeforeEmpty = 1;
    k->CKeyboardDX::SetHasInputFocus(true);
    CHECK(device.Acquires == 3 && storage[0x3610] == 0 && device.Reads == 1);

    printf(g_fail ? "FAIL\n" : "KEYBOARD_DX_FOCUS_OK\n");
    return g_fail != 0;
}
