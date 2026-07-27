void CGameScriptInterface::SetSaveGameMarkerPos(const C3DVector& pos)
{
    struct CGameScriptInterface_Overlay
    {
        std::byte pad_0[0x4];
        CWorld* world;
    };
    static_assert(offsetof(CGameScriptInterface_Overlay, world) == 0x4);

    const auto* self = reinterpret_cast<const CGameScriptInterface_Overlay*>(this);
    CWorld::SetSaveGameMarkerPos(self->world, &pos);
}