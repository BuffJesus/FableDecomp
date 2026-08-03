#include <cstdio>

struct CGameJoystickManager {
    int   pad0;         // +0x0
    int   deviceNumber; // +0x4
    bool  isSet;        // +0x8
};

// Function under test (same lowering as retail 0x0049bb30)
void __fastcall SetMainPlayerJoystickDeviceNumber(CGameJoystickManager *this_, void * /*edx*/, int deviceNumber)
{
    this_->deviceNumber = deviceNumber;
    this_->isSet = true;
}

int main()
{
    CGameJoystickManager mgr;
    mgr.pad0 = 0x11223344;
    mgr.deviceNumber = -1;
    mgr.isSet = false;

    // Invoke via a small __asm shim: this=ecx, one dword stack arg, ret 4.
    int arg = 7;
    CGameJoystickManager *p = &mgr;
    void (__fastcall *fn)(CGameJoystickManager*, void*, int) = SetMainPlayerJoystickDeviceNumber;
    __asm {
        mov  ecx, p
        push arg
        call fn
    }

    if (mgr.deviceNumber != 7) { printf("FAIL deviceNumber=%d\n", mgr.deviceNumber); return 1; }
    if (mgr.isSet != true)     { printf("FAIL isSet=%d\n", (int)mgr.isSet); return 1; }
    if (mgr.pad0 != 0x11223344){ printf("FAIL pad0 clobbered=%08x\n", mgr.pad0); return 1; }

    printf("OK_0x0049bb30 deviceNumber=%d isSet=%d\n", mgr.deviceNumber, (int)mgr.isSet);
    return 0;
}