        bVar3 = resources:IsHitByHeroExceptAbility(me, 14)
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db6afd end
            resources:SetBarrelManHeroAllies(me)
            require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
            if not acquireBarrelControl() then goto LAB_00db6afd end
            if not playCarefulMovie() then goto LAB_00db6afd end
            goto LAB_00db6933
        end
        bVar3 = false
        if __native_entity_state:GetStateInt("MyPhase") == 0 then
            bVar3 = resources:IsHeroWithinBarrelApproachDistance(me)
        end
        if not bVar3 then
            bVar3 = me:IsTalkedToByHero()
        end
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db6afd end
            if not acquireBarrelControl() then goto LAB_00db6afd end
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&piStack_1c0);
            pCVar22 = ""
            barrel_interaction_movie = resources:StartMovie("")
            uVar6 = 1
            resources:Pause(true)
            uVar4 = uVar6
            iVar11 = __native_entity_state:GetStateInt("MyPhase")
            if iVar11 ~= 0 then
                local continued = playReturnInteraction(barrel_interaction_movie, iVar11)
                barrel_interaction_movie = nil
                if not continued then goto LAB_00db6afd end
                goto LAB_00db6933
            end
            local continued = playInitialInteraction(barrel_interaction_movie)
            barrel_interaction_movie = nil
            if not continued then goto LAB_00db6afd end
            goto LAB_00db6933
        end
        if __native_entity_state:GetStateInt("MyPhase") == 0
            and resources:ShouldBarrelOverhear(me, __native_entity_state:GetStateBool("OverheardYet")) then
            if quest:IsActiveThreadTerminating() then goto LAB_00db6afd end
            __native_entity_state:SetStateBool("OverheardYet", true)
            resources:AddBarrelConversation(me, "TEXT_QST_048_BARRELMAN_OVERHEAR")
        end
