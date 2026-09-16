// Unapplied methods inside LuaRetailResources; no native IsNull filtering.
void InitializeWifeActor(CScriptThing* actor) {
    CheckOpen();
    if(!actor || !g_pCScriptThingVTable || !EntitySetAsDamageable_API || !EntitySetAsKillable_API ||
       !EntitySetAsToAddToComboMultiplierWhenHit_API || !SetThingHasInformation_API ||
       !EntitySetAsUseMovementInActions_API || !SetIsPushableByHero_API)
        throw std::runtime_error("Wife initialization API unavailable");
    EntitySetAsDamageable_API(m_game,actor,false);
    EntitySetAsKillable_API(m_game,actor,false,false);
    EntitySetAsToAddToComboMultiplierWhenHit_API(m_game,actor,false);
    SetThingHasInformation_API(m_game,actor,false,true,false);
    EntitySetAsUseMovementInActions_API(m_game,actor,false);
    CScriptThing copy=*actor;
    copy.pVTable=g_pCScriptThingVTable;
    if(copy.pImp.Info) ++copy.pImp.Info->RefCount;
    SetIsPushableByHero_API(m_game,copy,false); // Native callee consumes this copy.
}
void SetWifeDeedReactionsDisabled(CScriptThing* actor) {
    CheckOpen();
    if(!EntitySetDeedReactionsEnabled_API) throw std::runtime_error("Wife deed reaction API unavailable");
    EntitySetDeedReactionsEnabled_API(m_game,actor,false);
}
