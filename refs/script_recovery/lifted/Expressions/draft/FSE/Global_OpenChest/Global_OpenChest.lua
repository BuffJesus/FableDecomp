-- Generated native draft: Global_OpenChest. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local __native_condition_1, bVar11, bVar2, bVar4, bVar5, bVar6, cVar3, delay, iStack_2c, iVar10, iVar9, i_stk_28, native_arg_sequence_1, pCVar12, pCVar7, pCVar8, r1, r2, xStack_10, xStack_20
    local alive = true
    pCVar7 = quest:GetHero()
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    bVar2 = false
    pCVar8 = quest:GetActiveQuestName()
    quest:SetQuestAsPersistent(pCVar8, bVar2)
    bVar2 = quest:IsHeroControlledByPlayer()
    if not bVar2 then goto LAB_00eece68 end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    r1 = quest:GetMostRecentValidUsedTarget()
    __native_condition_1 = (r1 ~= nil and not r1:IsNull())
    if __native_condition_1 then
        cVar3 = (r1 ~= nil and r1:IsAlive())
        __native_condition_1 = cVar3
    end
    if __native_condition_1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00eec9c9 end
        iVar9 = quest:GetNumberOfKeysNeededToUnlockChest(r1)
        if iVar9 < 1 then
            goto LAB_00eec9da
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                iVar10 = quest:GetNumberOfItemsOfTypeInInventory("")
                if iVar9 <= iVar10 then goto LAB_00eec9da end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                native_arg_sequence_1 = false
                if bVar2 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if not native_arg_sequence_1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then goto LAB_00eec9c0 end
                r2 = quest:PlayCriteriaSoundOnThing(r1, "CHEST_OPEN_FAIL")
                quest:DisplayLockedChestMessage(r1)
                goto LAB_00eece56
            end
        end
        goto FLOW_past_lab_00eec9da
        ::LAB_00eec9da::
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            xStack_20 = resources:NewResource()
            iVar10 = 4
            pCVar12 = xStack_20
            pCVar7 = quest:GetHero()
            bVar2 = resources:TryAcquire(pCVar12, pCVar7, iVar10)
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00eecdb1 end
                iVar10 = 4
                pCVar12 = xStack_20
                pCVar7 = quest:GetHero()
                bVar2 = resources:TryAcquire(pCVar12, pCVar7, iVar10)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                pCVar7 = quest:GetHero()
                bVar4 = quest:IsEntityWieldingMeleeWeapon(pCVar7)
                pCVar7 = quest:GetHero()
                bVar5 = quest:IsEntityWieldingRangedWeapon(pCVar7)
                quest:SetToKeepHeroAbilitiesDuringCutscenes(true)
                quest:SetToDisplayTutorialsDuringCutscenes(true)
                resources:ClearAllActionsIncludingLoopingAnimations(xStack_20)
                xStack_10 = resources:StartMovie("")
                quest:SetCutsceneMode(true, false)
                quest:EntitySetCutsceneBehaviour(r1, 2)
                quest:PauseAllEntities(true)
                iStack_2c = quest:GetItemDefNamesFromContainer(r1)
                bVar11 = iStack_2c == i_stk_28
                bVar2 = true
                bVar6 = quest:OpenChest(r1, true)
                if bVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        bVar2 = quest:MsgOnChestOpeningCancelled()
                        bVar6 = quest:IsChestOpen(r1)
                        while (not bVar6 and (bVar2 == false)) do
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00eecd9f end
                            bVar2 = quest:MsgOnChestOpeningCancelled()
                            if not bVar2 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00eecd9f end
                                pCVar7 = quest:GetHero()
                                bVar6 = (pCVar7 ~= nil and pCVar7:MsgIsHitBy(""))
                                if bVar6 then
                                    goto LAB_00eecc47
                                else
                                    pCVar7 = quest:GetHero()
                                    bVar6 = (pCVar7 ~= nil and pCVar7:MsgIsHitByAnySpecialAbilityFrom(""))
                                    if bVar6 then goto LAB_00eecc47 end
                                end
                                goto FLOW_past_lab_00eecc47
                                ::LAB_00eecc47::
                                bVar2 = true
                                ::FLOW_past_lab_00eecc47::
                            end
                            bVar6 = quest:IsChestOpen(r1)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then goto LAB_00eecca6 end
                    end
                else
                    goto LAB_00eecca6
                end
                goto FLOW_past_lab_00eecca6
                ::LAB_00eecca6::
                quest:PauseAllEntities(false)
                quest:EntitySetCutsceneBehaviour(r1, 0)
                quest:SetCutsceneMode(false, true)
                quest:CameraDefault()
                if (bVar11) or (bVar2 ~= false) then
                    goto LAB_00eecd4f
                end
                goto FLOW_past_lab_00eecd4f
                ::LAB_00eecd4f::
                quest:SetToKeepHeroAbilitiesDuringCutscenes(false)
                quest:SetToDisplayTutorialsDuringCutscenes(false)
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00eecd9f end
                    bVar2 = false
                    pCVar7 = quest:GetHero()
                    quest:EntityUnsheatheMeleeWeapon(pCVar7, bVar2)
                elseif bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00eecd9f end
                    bVar2 = false
                    pCVar7 = quest:GetHero()
                    quest:EntityUnsheatheRangedWeapon(pCVar7, bVar2)
                end
                -- TODO(native): CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)&iStack_2c);
                resources:DestroyMovie(xStack_10)
                resources:ReleaseResource(xStack_20)
                goto LAB_00eece56
                ::FLOW_past_lab_00eecd4f::
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    bVar2 = quest:MsgOnHeroRewardedWithItemsFrom()
                    while not bVar2 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00eecd9a end
                        bVar2 = quest:MsgOnHeroRewardedWithItemsFrom()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        goto LAB_00eecd4f
                    end
                    ::LAB_00eecd9a::
                end
                ::FLOW_past_lab_00eecca6::
                ::LAB_00eecd9f::
                -- TODO(native): CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)&iStack_2c);
                resources:DestroyMovie(xStack_10)
            end
            ::LAB_00eecdb1::
            resources:ReleaseResource(xStack_20)
        end
        ::FLOW_past_lab_00eec9da::
        goto FLOW_past_lab_00eece56
        ::LAB_00eece56::
        goto LAB_00eece5f
        ::FLOW_past_lab_00eece56::
        ::LAB_00eec9c0::
        ::LAB_00eec9c9::
        return
    end
    ::LAB_00eece5f::
    ::LAB_00eece68::
    delay = 0
    pCVar8 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(pCVar8, delay)
end

function Init(quest)
end

function OnPersist(quest, context)
end

