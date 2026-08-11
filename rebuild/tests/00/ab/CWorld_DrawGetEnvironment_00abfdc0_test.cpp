#include <cstdio>

struct CIDrawEnvironment;

struct CWorld {
	char pad0[0x10];
	CIDrawEnvironment* m_pDrawEnvironment;
	CIDrawEnvironment* DrawGetEnvironment();
};

CIDrawEnvironment* CWorld::DrawGetEnvironment()
{
	return this->m_pDrawEnvironment;
}

int main()
{
	CWorld w;
	CIDrawEnvironment* marker = (CIDrawEnvironment*)0xDEADBEEF;
	w.m_pDrawEnvironment = marker;
	CIDrawEnvironment* got = w.DrawGetEnvironment();
	if (got == marker) {
		std::printf("DRAWENV_OK\n");
		return 0;
	}
	std::printf("DRAWENV_FAIL\n");
	return 1;
}