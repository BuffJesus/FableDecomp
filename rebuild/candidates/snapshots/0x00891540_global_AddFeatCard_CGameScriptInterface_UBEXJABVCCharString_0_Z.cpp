class CCharString;
class CGameScriptInterface;

namespace NUISystem
{
class CManager
{
public:
    void __thiscall RemoveThingActionParam(long actionParam);
};
}

void __thiscall CGameScriptInterface::AddFeatCard(
    long actionParam,
    const CCharString&,
    const CCharString&) const
{
    auto* const uiManager = *reinterpret_cast<NUISystem::CManager**>(0x013B89FC);
    uiManager->RemoveThingActionParam(actionParam);
}