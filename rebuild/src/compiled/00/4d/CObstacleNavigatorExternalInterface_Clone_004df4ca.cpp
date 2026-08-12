#pragma optimize("s",on)
// CObstacleNavigatorExternalInterface::Clone  @ 004df4ca
// Pattern: return new CObstacleNavigatorExternalInterface(*this);
//   -> operator new(0x14); if (p) copy-ctor(p, this); return p;

struct CNavigatorExternalInterface { int dummy; };

struct CObstacleNavigatorExternalInterface
{
    char data[0x14];
    CObstacleNavigatorExternalInterface() {}
    CObstacleNavigatorExternalInterface(const CObstacleNavigatorExternalInterface& src);
    CNavigatorExternalInterface* Clone();
};

CNavigatorExternalInterface* CObstacleNavigatorExternalInterface::Clone()
{
    return (CNavigatorExternalInterface*)new CObstacleNavigatorExternalInterface(*this);
}