struct CGameTimeManager;

struct CGameScriptInterface_TimeManagerOwnerOverlay
{
    std::byte _pad0[0x1C];
    CGameTimeManager* m_pGameTimeManager;
};
static_assert(offsetof(CGameScriptInterface_TimeManagerOwnerOverlay, m_pGameTimeManager) == 0x1C);

struct CGameScriptInterface_LayoutOverlay
{
    std::byte _pad0[0x04];
    CGameScriptInterface_TimeManagerOwnerOverlay* m_pTimeManagerOwner;
};
static_assert(offsetof(CGameScriptInterface_LayoutOverlay, m_pTimeManagerOwner) == 0x04);

void CGameScriptInterface::SetTimeAsStopped(bool stopped, long& time) const
{
    auto* const self = reinterpret_cast<const CGameScriptInterface_LayoutOverlay*>(this);
    self->m_pTimeManagerOwner->m_pGameTimeManager->SetTimeAsStopped(stopped, time);
}