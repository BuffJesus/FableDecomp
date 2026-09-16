    local bVar3, cVar2, fVar19, fVar20, fVar25, iVar11, local_native_guard_point, local_native_hidden_point, native_arg_barrel_has_health, native_arg_barrel_hero, native_arg_man_start_position, native_arg_primary_walkoff_position, native_arg_sequence_1, native_arg_switch_2, native_arg_teleport_point, native_arg_walkoff_position, pCVar1, pCVar15, pCVar22, pCVar23, pCVar24, pCVar26, pCVar7, pCVar9, pcVar21, piVar10, piVar8, ppVar12, ppuVar17, ppuVar18, r1, r10, r11, r12, r2, r3, r4, r5, r6, r7, r8, r9, uVar4, uVar6
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    barrel_resource = resources:NewResource()
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    local barrelHomePosition = me:GetHomePos()
    quest:SetWanderCentrePoint(me, barrelHomePosition)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 1.0)
    quest:SetScriptingStateGroup(me, 4)
    warehouseStartMarker = resources:NewThingFromScriptName("M_WHouse_ManStart")
    warehouseGuardMarker = resources:NewThingFromScriptName("M_WHouse_GuardPoint")
    alive = not quest:IsActiveThreadTerminating()
    cVar2 = not alive
    repeat
        if cVar2 then
            return
        end
        if not advanceBarrelPhase() then goto LAB_00db6afd end
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
        ::LAB_00db6933::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        cVar2 = not alive
    until false
    ::LAB_00db6ac0::
    ::LAB_00db6ac9::
    resources:Pause(false)
    ::LAB_00db6af4::
    ::LAB_00db6af8::
    resources:DestroyMovie(barrel_interaction_movie); barrel_interaction_movie = nil
    ::LAB_00db6afd::
