class CCharString;
class CGameScriptInterface;

void __cdecl CGameScriptInterface_AddGossipVillageEx(
    CCharString gossipVillage,
    CCharString gossipText);

void __thiscall CGameScriptInterface::AddGossipVillage(
    const CCharString& gossipVillage,
    const CCharString& gossipText) const
{
    CCharString gossipTextCopy(gossipText);
    CCharString gossipVillageCopy(gossipVillage);
    CGameScriptInterface_AddGossipVillageEx(gossipVillageCopy, gossipTextCopy);
}