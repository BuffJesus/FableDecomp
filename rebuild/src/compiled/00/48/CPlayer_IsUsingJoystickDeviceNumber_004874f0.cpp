// CPlayer::IsUsingJoystickDeviceNumber  retail 0x004874f0
// __fastcall bool(CPlayer*, long)

struct CInputThing {
	long GetDeviceNumber(long which);   // retail 0x449990, __fastcall
};

struct CPlayer {
	char pad0[0x10];
	CInputThing *m10;   // +0x10
	char pad14[0x28 - 0x14];
	long m28;           // +0x28  stored joystick device number
	bool IsUsingJoystickDeviceNumber(long which);
};

bool CPlayer::IsUsingJoystickDeviceNumber(long which)
{
	long stored = this->m28;
	if (this->m10->GetDeviceNumber(which) == stored)
		return true;
	return false;
}