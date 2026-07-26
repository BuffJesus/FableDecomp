bool CNetworkClient::GetGameEventPackageSet(CGameEventPackageSet* event_package)
{
    if (LocalGame != false)
    {
        return GetLocalGameEventPackageSet(event_package);
    }

    return false;
}