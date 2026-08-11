struct CIDrawEnvironment;

struct CWorld {
	char pad0[0xC];
	CIDrawEnvironment* m_pDrawEnvironment;
	CIDrawEnvironment* DrawGetEnvironment();
};

CIDrawEnvironment* CWorld::DrawGetEnvironment()
{
	return this->m_pDrawEnvironment;
}