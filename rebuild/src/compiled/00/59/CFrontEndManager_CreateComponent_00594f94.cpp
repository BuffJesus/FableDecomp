// NUISystem::CFrontEndManager::CreateComponent @ 0x00594F94
//
// Ghidra confirms this is a one-argument __thiscall wrapper around the base
// manager factory.  Size optimization preserves the retail push order and
// stack cleanup without an assembly shim.

#pragma optimize("s", on)

class CCharString;

namespace NUISystem
{
class CComponent;

class CManager
{
public:
    CComponent* CreateComponent(CCharString* definition, bool frontEnd);
};

class CFrontEndManager : public CManager
{
public:
    static CFrontEndManager* GetInstance();
    CComponent* CreateComponent(CCharString* definition);
};

CComponent* CFrontEndManager::CreateComponent(CCharString* definition)
{
    return GetInstance()->CManager::CreateComponent(definition, true);
}
}

#pragma optimize("s", off)
