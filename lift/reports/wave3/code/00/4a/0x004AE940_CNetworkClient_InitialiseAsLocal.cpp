bool CNetworkClient::InitialiseAsLocal(CMainGameComponent* component)
{
    using CXMVPlaybackInfoInitFn = char(__thiscall*)(CXMVPlaybackInfo*);
    const auto initialisePlaybackInfo =
        reinterpret_cast<CXMVPlaybackInfoInitFn>(0x0099A350);

    if (initialisePlaybackInfo(reinterpret_cast<CXMVPlaybackInfo*>(this)) == '\x01')
    {
        Host = false;
        LocalPlayer = nullptr;
        HostPlayer = nullptr;
        LocalFrame = 0;
        GameComponent = component;
        LocalGame = true;
        FirstTime = true;
        return true;
    }

    return false;
}