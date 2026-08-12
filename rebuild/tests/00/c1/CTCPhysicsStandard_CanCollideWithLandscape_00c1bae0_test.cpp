#include <cstdio>

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

int main()
{
	CTCPhysicsStandard o;
	int fails = 0;
	for (int v = 0; v < 256; ++v)
	{
		o.flags2 = (unsigned char)v;
		bool expect = ((v >> 1) & 1) != 0;
		if (o.CanCollideWithLandscape() != expect)
			++fails;
	}
	if (fails == 0)
		printf("CANCOLLIDE_OK\n");
	else
		printf("CANCOLLIDE_FAIL %d\n", fails);
	return fails == 0 ? 0 : 1;
}