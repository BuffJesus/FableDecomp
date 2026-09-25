-- Generated native draft: CROWDBANDITS. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local resources = quest:RetailResources()
    local __native_condition_1, b2, bVar5, bVar6, cVar1, conversationID, fVar11, iVar8, i_stk_74, p0, pCVar10, pCVar7, r1, r10, r11, r12, r2, r3, r4, r5, r6, r7, r8, r9, timerId, uVar4, uVar9, u_stk_78, xStack_20, xStack_7c
    local alive = true
    u_stk_78 = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        return
    end
    quest:EntitySetTargetable(me, false)
    quest:EntitySetAsDamageable(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAbleToBeEngagedInCombat(me, false)
    cVar1 = quest:GetStateBool("BanditKingFightStarted")
    while not cVar1 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            return
        end
        cVar1 = quest:GetStateBool("BanditKingFightStarted")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        return
    end
    quest:EntitySetAsUseMovementInActions(me, false)
    r1 = quest:GetNearestWithDefName(me, "VILLAGE_BANDIT_CAMP_BOSS")
    quest:EntityAttachToVillage(me, r1)
    xStack_20 = resources:NewResource()
    resources:PrepareResource(xStack_20)
    bVar5 = resources:TryAcquire(xStack_20, me, 4)
    while not bVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then goto LAB_00d0c2a1 end
        bVar5 = resources:TryAcquire(xStack_20, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then goto LAB_00d0c2a1 end
    me:ClearCommands()
    i_stk_74 = quest:GetStateInt("KingHealth")
    r2 = quest:GetThingWithScriptName("BanditKing")
    xStack_7c = quest:RegisterTimer()
    timerId = xStack_7c
    bVar5 = false
    quest:SetTimer(xStack_7c, 0)
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    while not bVar6 do
        if quest:GetStateBool("BanditsNeededForCutscene") then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then goto LAB_00d0c28b end
            quest:EntitySetAsUseMovementInActions(me, true)
            resources:PrepareResource(xStack_20)
            cVar1 = quest:GetStateBool("BanditsNeededForCutscene")
            while cVar1 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00d0c28b end
                cVar1 = quest:GetStateBool("BanditsNeededForCutscene")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then goto LAB_00d0c28b end
            resources:PrepareResource(xStack_20)
            bVar6 = resources:TryAcquire(xStack_20, me, 4)
            while not bVar6 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00d0c28b end
                bVar6 = resources:TryAcquire(xStack_20, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then goto LAB_00d0c28b end
            quest:EntitySetAsUseMovementInActions(me, false)
        end
        if (not bVar5) and (quest:GetStateBool("ItsAllOver")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00d0c28b end
            quest:EntitySetCutsceneBehaviour(me, 1)
            bVar5 = true
        end
        fVar11 = 3.5
        pCVar7 = quest:GetHero()
        bVar6 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, fVar11)
        if bVar6 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then goto LAB_00d0c28b end
            b2 = true
            bVar6 = false
            pCVar7 = quest:GetHero()
            quest:EntitySetAttackThingImmediately(me, pCVar7, bVar6, b2)
        else
            fVar11 = 6.5
            pCVar7 = quest:GetHero()
            bVar6 = quest:IsDistanceBetweenThingsOver(me, pCVar7, fVar11)
            __native_condition_1 = bVar6
            if __native_condition_1 then
                iVar8 = quest:GetTimer(timerId)
                __native_condition_1 = iVar8 < 1
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00d0c28b end
                iVar8 = me:IsPerformingScriptTask()
                if not iVar8 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d0c28b end
                    if quest:GetStateInt("KingHealth") < i_stk_74 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d0c28b end
                        i_stk_74 = quest:GetStateInt("KingHealth")
                        bVar6 = true
                        pCVar7 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, pCVar7, bVar6)
                        uVar9 = math.random(0, 32767)
                        uVar9 = uVar9 & 0x80000001
                        bVar6 = uVar9 == 0
                        if uVar9 < 0 then
                            bVar6 = (uVar9 - 1 | 0xfffffffe) == 0xffffffff
                        end
                        if not bVar6 then goto LAB_00d0bd52 end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d0c28b end
                        iVar8 = math.random(0, 32767)
                        iVar8 = iVar8 % 3
                        if iVar8 == 0 then
                            me:PlayAnimation("SPECIAL_BOAST", false, false, false, true, true, false, false)
                        else
                            if iVar8 == 1 then
                                me:PlayAnimation("ST_THREATEN", false, false, false, true, true, false, false)
                                goto LAB_00d0b927
                            end
                            if iVar8 == 2 then
                                me:PlayAnimation("SCRIPT_SHOUT", false, false, false, true, true, false, false)
                                goto LAB_00d0b927
                            end
                        end
                        ::LAB_00d0b927::
                        uVar9 = math.random(0, 32767)
                        uVar9 = uVar9 & 0x80000003
                        bVar6 = uVar9 == 0
                        if uVar9 < 0 then
                            bVar6 = (uVar9 - 1 | 0xfffffffc) == 0xffffffff
                        end
                        if bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00d0c28b end
                            uVar9 = math.random(0, 32767)
                            uVar9 = uVar9 & 0x80000001
                            if uVar9 < 0 then
                                uVar9 = (uVar9 - 1 | 0xfffffffe) + 1
                            end
                            if uVar9 == 0 then
                                r3 = quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                            elseif uVar9 == 1 then
                                r4 = quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_02")
                            end
                        end
                    else
                        fVar11 = 8.0
                        pCVar7 = quest:GetHero()
                        bVar6 = quest:IsDistanceBetweenThingsOver(pCVar7, r2, fVar11)
                        if not bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if not bVar6 then
                                bVar6 = true
                                pCVar7 = quest:GetHero()
                                quest:EntitySetFacingAngleTowardsThing(me, pCVar7, bVar6)
                                uVar9 = math.random(0, 32767)
                                uVar9 = uVar9 & 0x80000001
                                bVar6 = uVar9 == 0
                                if uVar9 < 0 then
                                    bVar6 = (uVar9 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not bVar6 then goto LAB_00d0bd52 end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if not bVar6 then
                                    uVar9 = math.random(0, 32767)
                                    uVar9 = uVar9 & 0x80000001
                                    if uVar9 < 0 then
                                        uVar9 = (uVar9 - 1 | 0xfffffffe) + 1
                                    end
                                    if uVar9 == 0 then
                                        me:PlayAnimation("COCKY", false, false, false, true, true, false, false)
                                    elseif uVar9 == 1 then
                                        me:PlayAnimation("ST_UNPERTURBED_GESTURE", false, false, false, true, true, false, false)
                                        goto LAB_00d0bcdc
                                    end
                                    ::LAB_00d0bcdc::
                                    uVar9 = math.random(0, 32767)
                                    uVar9 = uVar9 & 0x80000001
                                    if uVar9 < 0 then
                                        uVar9 = (uVar9 - 1 | 0xfffffffe) + 1
                                    end
                                    if uVar9 == 0 then
                                        r5 = quest:PlaySoundOnThing(me, "SND_CROWDLARGEBOASTREACT_01")
                                    elseif uVar9 == 1 then
                                        r6 = quest:PlaySoundOnThing(me, "SND_CROWDLARGEBOASTREACT_02")
                                    end
                                    goto LAB_00d0bd70
                                end
                            end
                            goto LAB_00d0c28b
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d0c28b end
                        bVar6 = true
                        pCVar7 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, pCVar7, bVar6)
                        uVar9 = math.random(0, 32767)
                        uVar9 = uVar9 & 0x80000001
                        bVar6 = uVar9 == 0
                        if uVar9 < 0 then
                            bVar6 = (uVar9 - 1 | 0xfffffffe) == 0xffffffff
                        end
                        if not bVar6 then
                            goto LAB_00d0bd52
                        end
                        goto FLOW_hoist_lab_00d0bd52_1
                    end
                    goto FLOW_past_lab_00d0bd52
                    ::LAB_00d0bd52::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if not bVar6 then
                        quest:SetTimer(timerId, 2)
                        goto LAB_00d0bd70
                    end
                    goto LAB_00d0c28b
                    ::FLOW_hoist_lab_00d0bd52_1::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d0c28b end
                    uVar9 = math.random(0, 32767)
                    uVar9 = uVar9 & 0x80000001
                    if uVar9 < 0 then
                        uVar9 = (uVar9 - 1 | 0xfffffffe) + 1
                    end
                    if uVar9 == 0 then
                        me:PlayAnimation("SPECIAL_BOAST", false, false, false, true, true, false, false)
                    elseif uVar9 == 1 then
                        me:PlayAnimation("ST_WAVE_SPECIAL_01", false, false, false, true, true, false, false)
                        goto LAB_00d0bacf
                    end
                    ::LAB_00d0bacf::
                    uVar9 = math.random(0, 32767)
                    uVar9 = uVar9 & 0x80000007
                    bVar6 = uVar9 == 0
                    if uVar9 < 0 then
                        bVar6 = (uVar9 - 1 | 0xfffffff8) == 0xffffffff
                    end
                    if bVar6 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d0c28b end
                        conversationID = quest:AddNewConversation(me, false, false)
                        pCVar7 = quest:GetHero()
                        quest:AddPersonToConversation(conversationID, pCVar7)
                        pCVar7 = quest:GetHero()
                        quest:AddLineToConversation(conversationID, "TEXT_QST_009_BOSSFIGHT_TAUNTING", me, pCVar7, false)
                        timerId = xStack_7c
                    end
                    uVar9 = math.random(0, 32767)
                    uVar9 = uVar9 & 0x80000003
                    bVar6 = uVar9 == 0
                    if uVar9 < 0 then
                        bVar6 = (uVar9 - 1 | 0xfffffffc) == 0xffffffff
                    end
                    if bVar6 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d0c28b end
                        uVar9 = math.random(0, 32767)
                        uVar9 = uVar9 & 0x80000001
                        if uVar9 < 0 then
                            uVar9 = (uVar9 - 1 | 0xfffffffe) + 1
                        end
                        if uVar9 == 0 then
                            r7 = quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                        elseif uVar9 == 1 then
                            r8 = quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_02")
                        end
                    end
                    ::FLOW_past_lab_00d0bd52::
                end
                ::LAB_00d0bd70::
                if quest:GetStateBool("BanditKingFightEnded") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00d0c28b end
                    pCVar10 = me:GetDataString()
                    iVar8 = tonumber(pCVar10)
                    if iVar8 == 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00d0c28b end
                        quest:RemoveThing(me, false, true)
                    end
                    quest:EntitySetTargetable(me, true)
                    quest:EntitySetAsDamageable(me, true)
                    quest:EntitySetAsUseMovementInActions(me, true)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        if quest:GetStateBool("BanditsNeededForCutscene") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then break end
                            quest:EntitySetAsUseMovementInActions(me, true)
                            resources:PrepareResource(xStack_20)
                            cVar1 = quest:GetStateBool("BanditsNeededForCutscene")
                            while cVar1 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00d0c28b end
                                cVar1 = quest:GetStateBool("BanditsNeededForCutscene")
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then break end
                            resources:PrepareResource(xStack_20)
                            bVar5 = resources:TryAcquire(xStack_20, me, 4)
                            while not bVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00d0c28b end
                                bVar5 = resources:TryAcquire(xStack_20, me, 4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then break end
                            quest:EntitySetAsUseMovementInActions(me, false)
                        end
                        if not quest:GetStateBool("TwinBladeKilled") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then break end
                            iVar8 = quest:GetTimer(timerId)
                            if iVar8 < 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then break end
                                uVar9 = math.random(0, 32767)
                                uVar9 = uVar9 & 0x80000001
                                bVar5 = uVar9 == 0
                                if uVar9 < 0 then
                                    bVar5 = (uVar9 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if bVar5 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then break end
                                    iVar8 = math.random(0, 32767)
                                    iVar8 = iVar8 % 3
                                    if iVar8 == 0 then
                                        me:PlayAnimation("SPECIAL_BOAST", false, false, false, true, true, false, false)
                                    else
                                        if iVar8 == 1 then
                                            me:PlayAnimation("ST_THREATEN", false, false, false, true, true, false, false)
                                            goto LAB_00d0c05b_c1
                                        end
                                        if iVar8 == 2 then
                                            me:PlayAnimation("SCRIPT_SHOUT", false, false, false, true, true, false, false)
                                            goto LAB_00d0c05b_c1
                                        end
                                    end
                                    ::LAB_00d0c05b_c1::
                                    uVar9 = math.random(0, 32767)
                                    uVar9 = uVar9 & 0x80000003
                                    bVar5 = uVar9 == 0
                                    if uVar9 < 0 then
                                        bVar5 = (uVar9 - 1 | 0xfffffffc) == 0xffffffff
                                    end
                                    if bVar5 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then break end
                                        uVar9 = math.random(0, 32767)
                                        uVar9 = uVar9 & 0x80000001
                                        if uVar9 < 0 then
                                            uVar9 = (uVar9 - 1 | 0xfffffffe) + 1
                                        end
                                        if uVar9 == 0 then
                                            r9 = quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                                        elseif uVar9 == 1 then
                                            r10 = quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_02")
                                        end
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then break end
                                    quest:SetTimer(timerId, 2)
                                end
                            end
                        end
                        if quest:GetStateInt("AngryBanditNeeded") < 1 then
                            uVar4 = u_stk_78
                            u_stk_78 = u_stk_78 | 1
                            bVar5 = me:MsgIsHitByHero()
                            if bVar5 then goto LAB_00d0c1af_c1 end
                            uVar9 = uVar4 | 3
                            u_stk_78 = uVar9
                            bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                            if bVar5 then
                                uVar9 = uVar4 | 7
                                u_stk_78 = uVar9
                                bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                if not bVar5 then goto LAB_00d0c1af_c1 end
                            end
                            bVar5 = false
                        end
                        goto FLOW_past_lab_00d0c1af_c1
                        ::LAB_00d0c1af_c1::
                        bVar5 = true
                        ::FLOW_past_lab_00d0c1af_c1::
                        if (uVar9 & 4) ~= 0 then
                            uVar9 = uVar9 & 0xfffffffb
                            u_stk_78 = uVar9
                        end
                        if (uVar9 & 2) ~= 0 then
                            uVar9 = uVar9 & 0xfffffffd
                            u_stk_78 = uVar9
                        end
                        if (uVar9 & 1) ~= 0 then
                            u_stk_78 = uVar9 & 0xfffffffe
                        end
                        if bVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then
                                pCVar7 = quest:GetHero()
                                quest:GiveThingBestEnemyTarget(me, pCVar7)
                                quest:EntitySetAbleToBeEngagedInCombat(me, true)
                                resources:PrepareResource(xStack_20)
                                if quest:GetStateInt("AngryBanditNeeded") == 0 then goto LAB_00d0c278_c1 end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if not bVar5 then
                                    quest:SetStateInt("AngryBanditNeeded", quest:GetStateInt("AngryBanditNeeded") + -1)
                                    goto LAB_00d0c278_c1
                                end
                                goto FLOW_past_lab_00d0c278_c1
                                ::LAB_00d0c278_c1::
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                until not (not bVar5)
                                ::FLOW_past_lab_00d0c278_c1::
                            end
                            break
                        end
                        goto FLOW_after_lab_00d0be35
                    end
                    goto LAB_00d0c28b
                end
            end
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
    end
    quest:DeregisterTimer(timerId)
    goto LAB_00d0c294
    while true do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then break end
        -- LAB_00d0be35: (native jump target)
        if quest:GetStateBool("BanditsNeededForCutscene") then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then break end
            quest:EntitySetAsUseMovementInActions(me, true)
            resources:PrepareResource(xStack_20)
            cVar1 = quest:GetStateBool("BanditsNeededForCutscene")
            while cVar1 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00d0c28b end
                cVar1 = quest:GetStateBool("BanditsNeededForCutscene")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then break end
            resources:PrepareResource(xStack_20)
            bVar5 = resources:TryAcquire(xStack_20, me, 4)
            while not bVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00d0c28b end
                bVar5 = resources:TryAcquire(xStack_20, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then break end
            quest:EntitySetAsUseMovementInActions(me, false)
        end
        if not quest:GetStateBool("TwinBladeKilled") then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then break end
            iVar8 = quest:GetTimer(timerId)
            if iVar8 < 1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then break end
                uVar9 = math.random(0, 32767)
                uVar9 = uVar9 & 0x80000001
                bVar5 = uVar9 == 0
                if uVar9 < 0 then
                    bVar5 = (uVar9 - 1 | 0xfffffffe) == 0xffffffff
                end
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then break end
                    iVar8 = math.random(0, 32767)
                    iVar8 = iVar8 % 3
                    if iVar8 == 0 then
                        me:PlayAnimation("SPECIAL_BOAST", false, false, false, true, true, false, false)
                    else
                        if iVar8 == 1 then
                            me:PlayAnimation("ST_THREATEN", false, false, false, true, true, false, false)
                            goto LAB_00d0c05b
                        end
                        if iVar8 == 2 then
                            me:PlayAnimation("SCRIPT_SHOUT", false, false, false, true, true, false, false)
                            goto LAB_00d0c05b
                        end
                    end
                    ::LAB_00d0c05b::
                    uVar9 = math.random(0, 32767)
                    uVar9 = uVar9 & 0x80000003
                    bVar5 = uVar9 == 0
                    if uVar9 < 0 then
                        bVar5 = (uVar9 - 1 | 0xfffffffc) == 0xffffffff
                    end
                    if bVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then break end
                        uVar9 = math.random(0, 32767)
                        uVar9 = uVar9 & 0x80000001
                        if uVar9 < 0 then
                            uVar9 = (uVar9 - 1 | 0xfffffffe) + 1
                        end
                        if uVar9 == 0 then
                            r11 = quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                        elseif uVar9 == 1 then
                            r12 = quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_02")
                        end
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then break end
                    quest:SetTimer(timerId, 2)
                end
            end
        end
        if quest:GetStateInt("AngryBanditNeeded") < 1 then
            uVar4 = u_stk_78
            u_stk_78 = u_stk_78 | 1
            bVar5 = me:MsgIsHitByHero()
            if bVar5 then goto LAB_00d0c1af end
            uVar9 = uVar4 | 3
            u_stk_78 = uVar9
            bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if bVar5 then
                uVar9 = uVar4 | 7
                u_stk_78 = uVar9
                bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not bVar5 then goto LAB_00d0c1af end
            end
            bVar5 = false
        else
            goto LAB_00d0c1af
        end
        goto FLOW_past_lab_00d0c1af
        ::LAB_00d0c1af::
        bVar5 = true
        ::FLOW_past_lab_00d0c1af::
        if (uVar9 & 4) ~= 0 then
            uVar9 = uVar9 & 0xfffffffb
            u_stk_78 = uVar9
        end
        if (uVar9 & 2) ~= 0 then
            uVar9 = uVar9 & 0xfffffffd
            u_stk_78 = uVar9
        end
        if (uVar9 & 1) ~= 0 then
            u_stk_78 = uVar9 & 0xfffffffe
        end
        if bVar5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if not bVar5 then
                pCVar7 = quest:GetHero()
                quest:GiveThingBestEnemyTarget(me, pCVar7)
                quest:EntitySetAbleToBeEngagedInCombat(me, true)
                resources:PrepareResource(xStack_20)
                if quest:GetStateInt("AngryBanditNeeded") == 0 then goto LAB_00d0c278 end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    quest:SetStateInt("AngryBanditNeeded", quest:GetStateInt("AngryBanditNeeded") + -1)
                    goto LAB_00d0c278
                end
                goto FLOW_past_lab_00d0c278
                ::LAB_00d0c278::
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                until not (not bVar5)
                ::FLOW_past_lab_00d0c278::
            end
            break
        end
    end
    ::FLOW_after_lab_00d0be35::
    ::LAB_00d0c28b::
    quest:DeregisterTimer(xStack_7c)
    ::LAB_00d0c294::
    ::LAB_00d0c2a1::
    resources:ReleaseResource(xStack_20)
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
    local cVar2 = me:MsgIsKilledBy("SCRIPT_NAME_HERO")
    if cVar2 then
        quest:SetStateInt("AngryBanditNeeded", quest:GetStateInt("AngryBanditNeeded") + 1)
    end
end

