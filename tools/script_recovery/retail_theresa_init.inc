    void InitializeTheresaActor(CScriptThing* actor) {
        CheckOpen();
        if (!actor || !EntitySetAsDamageable_API || !EntitySetAsKillable_API ||
            !EntitySetAsToAddToComboMultiplierWhenHit_API || !SetThingHasInformation_API ||
            !SetIsPushableByHero_API || !EntitySetAsUseMovementInActions_API)
            throw std::runtime_error("Theresa Init APIs unavailable");
        EntitySetAsDamageable_API(m_game,actor,false);
        EntitySetAsKillable_API(m_game,actor,false,false);
        EntitySetAsToAddToComboMultiplierWhenHit_API(m_game,actor,false);
        SetThingHasInformation_API(m_game,actor,false,true,false);
        CScriptThing copy = *actor;
        if (copy.pImp.Info) ++copy.pImp.Info->RefCount;
        // Native D30 consumes this retained by-value Thing.
        SetIsPushableByHero_API(m_game,copy,false);
        EntitySetAsUseMovementInActions_API(m_game,actor,false);
    }
