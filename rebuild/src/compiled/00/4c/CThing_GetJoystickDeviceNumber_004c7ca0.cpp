// CThing::GetJoystickDeviceNumber @ 0x004c7ca0

struct CInputProvider {
    long GetDeviceNumber();          // 0x487510 __fastcall -> long
};

struct CInputManager {
    CInputProvider* GetProvider(int deviceIndex); // 0x449910 __fastcall(byte arg)
};

struct CInputManagerHolder {
    char pad[0x30];
    CInputManager* mgr;   // +0x30
};

extern CInputManagerHolder* g_inputHolder; // [0x13b8a1c]

struct CThing {
    char pad[0x90];
    signed char joystickDeviceNumber; // +0x90
    long GetJoystickDeviceNumber();
};

long CThing::GetJoystickDeviceNumber()
{
    return g_inputHolder->mgr->GetProvider(this->joystickDeviceNumber)->GetDeviceNumber();
}