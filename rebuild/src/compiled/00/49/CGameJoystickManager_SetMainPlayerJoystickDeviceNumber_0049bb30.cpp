
#include "engine/CGameJoystickManager.h"  // retyped onto the PDB layout; byte parity re-verified
void __fastcall SetMainPlayerJoystickDeviceNumber(CGameJoystickManager *this_, void * /*edx*/, int deviceNumber)
{
    this_->MainPlayerJoystickDeviceNumber = deviceNumber;
    this_->MainPlayerJoystickDeviceNumberSet = true;
}