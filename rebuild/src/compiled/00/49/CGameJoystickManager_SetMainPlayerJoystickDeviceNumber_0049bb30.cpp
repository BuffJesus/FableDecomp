struct CGameJoystickManager {
    int   pad0;         // +0x0
    int   deviceNumber; // +0x4
    bool  isSet;        // +0x8
};

void __fastcall SetMainPlayerJoystickDeviceNumber(CGameJoystickManager *this_, void * /*edx*/, int deviceNumber)
{
    this_->deviceNumber = deviceNumber;
    this_->isSet = true;
}