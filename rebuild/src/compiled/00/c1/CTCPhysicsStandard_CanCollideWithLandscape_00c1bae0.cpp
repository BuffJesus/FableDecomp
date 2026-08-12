// CTCPhysicsStandard::CanCollideWithLandscape  @ 0x00c1bae0
// Retail: xor eax,eax; mov al,[ecx+2]; shr eax,1; and eax,1; ret
// Returns bit 1 of the flags byte at offset +2.

struct CTCPhysicsStandard
{
	char pad0[2];
	unsigned char flags2;

	bool CanCollideWithLandscape();
};

bool CTCPhysicsStandard::CanCollideWithLandscape()
{
	return ((unsigned int)flags2 >> 1) & 1;
}