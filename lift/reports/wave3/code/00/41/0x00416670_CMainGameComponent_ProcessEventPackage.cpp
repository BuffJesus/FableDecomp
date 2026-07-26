namespace
{
using CGameEventPackage_GetCountFn = std::uint32_t(__thiscall*)(CGameEventPackage*);
using CGameEventPackage_GetEventFn = CGameEvent*(__thiscall*)(CGameEventPackage*, std::uint32_t);

constexpr auto CGameEventPackage_GetCount =
    reinterpret_cast<CGameEventPackage_GetCountFn>(0x009F16E0);

constexpr auto CGameEventPackage_GetEvent =
    reinterpret_cast<CGameEventPackage_GetEventFn>(0x009F16D7);
}

void CMainGameComponent::ProcessEventPackage(CGameEventPackage* event_package)
{
    std::uint32_t event_index = 0;
    std::uint32_t event_count = CGameEventPackage_GetCount(event_package);

    if (event_count != 0) {
        do {
            ProcessEvent(CGameEventPackage_GetEvent(event_package, event_index));
            ++event_index;
            event_count = CGameEventPackage_GetCount(event_package);
        } while (event_index < event_count);
    }
}