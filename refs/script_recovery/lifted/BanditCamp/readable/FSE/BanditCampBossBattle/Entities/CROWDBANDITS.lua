-- Readable native conversion: CROWDBANDITS. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_PAUSED = 1  -- ECutsceneBehaviour (Ego_r.pdb)
local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CROWDBANDITS.Main (retail 0x00d0b3e0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local isActiveThreadTerminating, scratchValue3, scratchValue4, scratchValue5, scratchValue6
    local scratchValue7, scratchValue8, scratchValue9, kingHealth, banditKing, timerId
    local scratchValue20, scratchValue21, scratchValue22, timerId2
    scratchValue22 = 0
    if not quest:NewScriptFrame(me) then return end
    quest:EntitySetTargetable(me, false)
    quest:EntitySetAsDamageable(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAbleToBeEngagedInCombat(me, false)
    while not quest:GetStateBool("BanditKingFightStarted") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:EntitySetAsUseMovementInActions(me, false)
    quest:EntityAttachToVillage(me, quest:GetNearestWithDefName(me, "VILLAGE_BANDIT_CAMP_BOSS"))
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d0c2a1 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d0c2a1 end
    me:ClearCommands()
    kingHealth = quest:GetStateInt("KingHealth")
    banditKing = quest:GetThingWithScriptName("BanditKing")
    timerId2 = quest:RegisterTimer()
    timerId = timerId2
    isActiveThreadTerminating = false
    quest:SetTimer(timerId2, 0)
    while not quest:IsActiveThreadTerminating() do
        if quest:GetStateBool("BanditsNeededForCutscene") then
            quest:EntitySetAsUseMovementInActions(me, true)
            resources:PrepareResource(resource)
            while quest:GetStateBool("BanditsNeededForCutscene") do
                if not quest:NewScriptFrame(me) then goto LAB_00d0c28b end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00d0c28b end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
            quest:EntitySetAsUseMovementInActions(me, false)
        end
        if not isActiveThreadTerminating and quest:GetStateBool("ItsAllOver") then
            if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
            quest:EntitySetCutsceneBehaviour(me, CUTSCENE_BEHAVIOUR_PAUSED)
            isActiveThreadTerminating = true
        end
        if quest:IsDistanceBetweenThingsUnder(me, hero, 3.5) then
            if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
            quest:EntitySetAttackThingImmediately(me, hero, false, true)
        else
            if not (quest:IsDistanceBetweenThingsOver(me, hero, 6.5) and quest:GetTimer(timerId) < 1) then quest:NewScriptFrame(me); goto continue_1 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
            if not me:IsPerformingScriptTask() then
                if quest:GetStateInt("KingHealth") < kingHealth then
                    kingHealth = quest:GetStateInt("KingHealth")
                    quest:EntitySetFacingAngleTowardsThing(me, hero, true)
                    scratchValue21 = math.random(0, 32767) & 0x80000001
                    scratchValue3 = scratchValue21 == 0
                    if scratchValue21 < 0 then
                        scratchValue3 = (scratchValue21 - 1 | 0xfffffffe) == 0xffffffff
                    end
                    if not scratchValue3 then goto LAB_00d0bd52 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
                    scratchValue9 = math.random(0, 32767) % 3
                    if scratchValue9 == 0 then
                        me:PlayAnimation("SPECIAL_BOAST", false, false, false, true, true, false, false)
                    else
                        if scratchValue9 == 1 then
                            me:PlayAnimation("ST_THREATEN", false, false, false, true, true, false, false)
                            goto LAB_00d0b927
                        end
                        if scratchValue9 == 2 then
                            me:PlayAnimation("SCRIPT_SHOUT", false, false, false, true, true, false, false)
                            goto LAB_00d0b927
                        end
                    end
                    ::LAB_00d0b927::
                    scratchValue21 = math.random(0, 32767) & 0x80000003
                    scratchValue4 = scratchValue21 == 0
                    if scratchValue21 < 0 then
                        scratchValue4 = (scratchValue21 - 1 | 0xfffffffc) == 0xffffffff
                    end
                    if scratchValue4 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
                        scratchValue21 = math.random(0, 32767) & 0x80000001
                        if scratchValue21 < 0 then
                            scratchValue21 = (scratchValue21 - 1 | 0xfffffffe) + 1
                        end
                        if scratchValue21 == 0 then
                            quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                        elseif scratchValue21 == 1 then
                            quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_02")
                        end
                    end
                else
                    if not quest:IsDistanceBetweenThingsOver(hero, banditKing, 8.0) then
                        if not quest:IsActiveThreadTerminating() then
                            quest:EntitySetFacingAngleTowardsThing(me, hero, true)
                            scratchValue21 = math.random(0, 32767) & 0x80000001
                            scratchValue5 = scratchValue21 == 0
                            if scratchValue21 < 0 then
                                scratchValue5 = (scratchValue21 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if not scratchValue5 then goto LAB_00d0bd52 end
                            if not quest:IsActiveThreadTerminating() then
                                scratchValue21 = math.random(0, 32767) & 0x80000001
                                if scratchValue21 < 0 then
                                    scratchValue21 = (scratchValue21 - 1 | 0xfffffffe) + 1
                                end
                                if scratchValue21 == 0 then
                                    me:PlayAnimation("COCKY", false, false, false, true, true, false, false)
                                elseif scratchValue21 == 1 then
                                    me:PlayAnimation("ST_UNPERTURBED_GESTURE", false, false, false, true, true, false, false)
                                    goto LAB_00d0bcdc
                                end
                                ::LAB_00d0bcdc::
                                scratchValue21 = math.random(0, 32767) & 0x80000001
                                if scratchValue21 < 0 then
                                    scratchValue21 = (scratchValue21 - 1 | 0xfffffffe) + 1
                                end
                                if scratchValue21 == 0 then
                                    quest:PlaySoundOnThing(me, "SND_CROWDLARGEBOASTREACT_01")
                                elseif scratchValue21 == 1 then
                                    quest:PlaySoundOnThing(me, "SND_CROWDLARGEBOASTREACT_02")
                                end
                                goto LAB_00d0bd70
                            end
                        end
                        goto LAB_00d0c28b
                    end
                    quest:EntitySetFacingAngleTowardsThing(me, hero, true)
                    scratchValue21 = math.random(0, 32767) & 0x80000001
                    scratchValue6 = scratchValue21 == 0
                    if scratchValue21 < 0 then
                        scratchValue6 = (scratchValue21 - 1 | 0xfffffffe) == 0xffffffff
                    end
                    if not scratchValue6 then
                        goto LAB_00d0bd52
                    end
                    goto FLOW_hoist_lab_00d0bd52_1
                end
                goto FLOW_past_lab_00d0bd52
                ::LAB_00d0bd52::
                if not quest:IsActiveThreadTerminating() then quest:SetTimer(timerId, 2); goto LAB_00d0bd70 end
                goto LAB_00d0c28b
                ::FLOW_hoist_lab_00d0bd52_1::
                if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
                scratchValue21 = math.random(0, 32767) & 0x80000001
                if scratchValue21 < 0 then
                    scratchValue21 = (scratchValue21 - 1 | 0xfffffffe) + 1
                end
                if scratchValue21 == 0 then
                    me:PlayAnimation("SPECIAL_BOAST", false, false, false, true, true, false, false)
                elseif scratchValue21 == 1 then
                    me:PlayAnimation("ST_WAVE_SPECIAL_01", false, false, false, true, true, false, false)
                    goto LAB_00d0bacf
                end
                ::LAB_00d0bacf::
                scratchValue21 = math.random(0, 32767) & 0x80000007
                scratchValue7 = scratchValue21 == 0
                if scratchValue21 < 0 then
                    scratchValue7 = (scratchValue21 - 1 | 0xfffffff8) == 0xffffffff
                end
                if scratchValue7 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
                    local conversationID = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationID, hero)
                    quest:AddLineToConversation(conversationID, "TEXT_QST_009_BOSSFIGHT_TAUNTING", me, hero, false)
                    timerId = timerId2
                end
                scratchValue21 = math.random(0, 32767) & 0x80000003
                scratchValue8 = scratchValue21 == 0
                if scratchValue21 < 0 then
                    scratchValue8 = (scratchValue21 - 1 | 0xfffffffc) == 0xffffffff
                end
                if scratchValue8 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
                    scratchValue21 = math.random(0, 32767) & 0x80000001
                    if scratchValue21 < 0 then
                        scratchValue21 = (scratchValue21 - 1 | 0xfffffffe) + 1
                    end
                    if scratchValue21 == 0 then
                        quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                    elseif scratchValue21 == 1 then
                        quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_02")
                    end
                end
                ::FLOW_past_lab_00d0bd52::
            end
            ::LAB_00d0bd70::
            if quest:GetStateBool("BanditKingFightEnded") then
                if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
                if tonumber(me:GetDataString()) == 1 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
                    quest:RemoveThing(me, false, true)
                end
                quest:EntitySetTargetable(me, true)
                quest:EntitySetAsDamageable(me, true)
                quest:EntitySetAsUseMovementInActions(me, true)
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if not isActiveThreadTerminating then
                    if quest:GetStateBool("BanditsNeededForCutscene") then
                        if quest:IsActiveThreadTerminating() then break end
                        quest:EntitySetAsUseMovementInActions(me, true)
                        resources:PrepareResource(resource)
                        while quest:GetStateBool("BanditsNeededForCutscene") do
                            if not quest:NewScriptFrame(me) then goto LAB_00d0c28b end
                        end
                        if quest:IsActiveThreadTerminating() then break end
                        resources:PrepareResource(resource)
                        while not resources:TryAcquire(resource, me, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00d0c28b end
                        end
                        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                        if isActiveThreadTerminating then break end
                        quest:EntitySetAsUseMovementInActions(me, false)
                    end
                    if not quest:GetStateBool("TwinBladeKilled") then
                        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                        if isActiveThreadTerminating then break end
                        if quest:GetTimer(timerId) < 1 then
                            if quest:IsActiveThreadTerminating() then break end
                            scratchValue21 = math.random(0, 32767) & 0x80000001
                            isActiveThreadTerminating = scratchValue21 == 0
                            if scratchValue21 < 0 then
                                isActiveThreadTerminating = (scratchValue21 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if isActiveThreadTerminating then
                                if quest:IsActiveThreadTerminating() then break end
                                scratchValue9 = math.random(0, 32767) % 3
                                if scratchValue9 == 0 then
                                    me:PlayAnimation("SPECIAL_BOAST", false, false, false, true, true, false, false)
                                else
                                    if scratchValue9 == 1 then
                                        me:PlayAnimation("ST_THREATEN", false, false, false, true, true, false, false)
                                        goto LAB_00d0c05b_c1
                                    end
                                    if scratchValue9 == 2 then
                                        me:PlayAnimation("SCRIPT_SHOUT", false, false, false, true, true, false, false)
                                        goto LAB_00d0c05b_c1
                                    end
                                end
                                ::LAB_00d0c05b_c1::
                                scratchValue21 = math.random(0, 32767) & 0x80000003
                                isActiveThreadTerminating = scratchValue21 == 0
                                if scratchValue21 < 0 then
                                    isActiveThreadTerminating = (scratchValue21 - 1 | 0xfffffffc) == 0xffffffff
                                end
                                if isActiveThreadTerminating then
                                    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                                    if isActiveThreadTerminating then break end
                                    scratchValue21 = math.random(0, 32767) & 0x80000001
                                    if scratchValue21 < 0 then
                                        scratchValue21 = (scratchValue21 - 1 | 0xfffffffe) + 1
                                    end
                                    if scratchValue21 == 0 then
                                        quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                                    elseif scratchValue21 == 1 then
                                        quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_02")
                                    end
                                end
                            else
                                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                                if isActiveThreadTerminating then break end
                                quest:SetTimer(timerId, 2)
                            end
                        end
                    end
                    if quest:GetStateInt("AngryBanditNeeded") < 1 then
                        scratchValue22 = scratchValue22 | 1
                        if me:MsgIsHitByHero() then goto LAB_00d0c1af_c1 end
                        scratchValue21 = 0 | 3
                        if me:MsgIsHitByAnySpecialAbilityFromHero() then
                            scratchValue21 = 0 | 7
                            if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0c1af_c1 end
                        end
                        isActiveThreadTerminating = false
                    end
                    goto FLOW_past_lab_00d0c1af_c1
                    ::LAB_00d0c1af_c1::
                    isActiveThreadTerminating = true
                    ::FLOW_past_lab_00d0c1af_c1::
                    if scratchValue21 & 4 ~= 0 then
                        scratchValue21 = scratchValue21 & 0xfffffffb
                    end
                    if scratchValue21 & 2 ~= 0 then
                        scratchValue21 = scratchValue21 & 0xfffffffd
                    end
                    if isActiveThreadTerminating then
                        if not quest:IsActiveThreadTerminating() then
                            quest:GiveThingBestEnemyTarget(me, hero)
                            quest:EntitySetAbleToBeEngagedInCombat(me, true)
                            resources:PrepareResource(resource)
                            if quest:GetStateInt("AngryBanditNeeded") == 0 then goto LAB_00d0c278_c1 end
                            if not quest:IsActiveThreadTerminating() then quest:SetStateInt("AngryBanditNeeded", quest:GetStateInt("AngryBanditNeeded") - 1); goto LAB_00d0c278_c1 end
                            goto FLOW_past_lab_00d0c278_c1
                            ::LAB_00d0c278_c1::
                            repeat
                                quest:NewScriptFrame(me)
                            until quest:IsActiveThreadTerminating()
                            ::FLOW_past_lab_00d0c278_c1::
                        end
                        break
                    end
                    goto FLOW_after_lab_00d0be35
                end
                goto LAB_00d0c28b
            end
        end
        quest:NewScriptFrame(me)
        ::continue_1::
    end
    quest:DeregisterTimer(timerId)
    goto LAB_00d0c294
    while true do
        if not quest:NewScriptFrame(me) then break end
        if quest:GetStateBool("BanditsNeededForCutscene") then
            quest:EntitySetAsUseMovementInActions(me, true)
            resources:PrepareResource(resource)
            while quest:GetStateBool("BanditsNeededForCutscene") do
                if not quest:NewScriptFrame(me) then goto LAB_00d0c28b end
            end
            if quest:IsActiveThreadTerminating() then break end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00d0c28b end
            end
            if quest:IsActiveThreadTerminating() then break end
            quest:EntitySetAsUseMovementInActions(me, false)
        end
        if not quest:GetStateBool("TwinBladeKilled") then
            if quest:IsActiveThreadTerminating() then break end
            if quest:GetTimer(timerId) < 1 then
                scratchValue21 = math.random(0, 32767) & 0x80000001
                isActiveThreadTerminating = scratchValue21 == 0
                if scratchValue21 < 0 then
                    isActiveThreadTerminating = (scratchValue21 - 1 | 0xfffffffe) == 0xffffffff
                end
                if isActiveThreadTerminating then
                    if quest:IsActiveThreadTerminating() then break end
                    scratchValue9 = math.random(0, 32767) % 3
                    if scratchValue9 == 0 then
                        me:PlayAnimation("SPECIAL_BOAST", false, false, false, true, true, false, false)
                    else
                        if scratchValue9 == 1 then
                            me:PlayAnimation("ST_THREATEN", false, false, false, true, true, false, false)
                            goto LAB_00d0c05b
                        end
                        if scratchValue9 == 2 then
                            me:PlayAnimation("SCRIPT_SHOUT", false, false, false, true, true, false, false)
                            goto LAB_00d0c05b
                        end
                    end
                    ::LAB_00d0c05b::
                    scratchValue21 = math.random(0, 32767) & 0x80000003
                    isActiveThreadTerminating = scratchValue21 == 0
                    if scratchValue21 < 0 then
                        isActiveThreadTerminating = (scratchValue21 - 1 | 0xfffffffc) == 0xffffffff
                    end
                    if isActiveThreadTerminating then
                        if quest:IsActiveThreadTerminating() then break end
                        scratchValue21 = math.random(0, 32767) & 0x80000001
                        if scratchValue21 < 0 then
                            scratchValue21 = (scratchValue21 - 1 | 0xfffffffe) + 1
                        end
                        if scratchValue21 == 0 then
                            quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                        elseif scratchValue21 == 1 then
                            quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_02")
                        end
                    end
                else
                    if quest:IsActiveThreadTerminating() then break end
                    quest:SetTimer(timerId, 2)
                end
            end
        end
        if quest:GetStateInt("AngryBanditNeeded") >= 1 then goto LAB_00d0c1af end
        scratchValue20 = scratchValue22
        scratchValue22 = scratchValue22 | 1
        if me:MsgIsHitByHero() then goto LAB_00d0c1af end
        scratchValue21 = scratchValue20 | 3
        scratchValue22 = scratchValue21
        if me:MsgIsHitByAnySpecialAbilityFromHero() then
            scratchValue21 = scratchValue20 | 7
            scratchValue22 = scratchValue21
            if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0c1af end
        end
        isActiveThreadTerminating = false
        goto FLOW_past_lab_00d0c1af
        ::LAB_00d0c1af::
        isActiveThreadTerminating = true
        ::FLOW_past_lab_00d0c1af::
        if scratchValue21 & 4 ~= 0 then
            scratchValue21 = scratchValue21 & 0xfffffffb
            scratchValue22 = scratchValue21
        end
        if scratchValue21 & 2 ~= 0 then
            scratchValue21 = scratchValue21 & 0xfffffffd
            scratchValue22 = scratchValue21
        end
        if scratchValue21 & 1 ~= 0 then
            scratchValue22 = scratchValue21 & 0xfffffffe
        end
        if not isActiveThreadTerminating then goto continue_2 end
        if not quest:IsActiveThreadTerminating() then
            quest:GiveThingBestEnemyTarget(me, hero)
            quest:EntitySetAbleToBeEngagedInCombat(me, true)
            resources:PrepareResource(resource)
            if quest:GetStateInt("AngryBanditNeeded") == 0 then goto LAB_00d0c278 end
            if not quest:IsActiveThreadTerminating() then quest:SetStateInt("AngryBanditNeeded", quest:GetStateInt("AngryBanditNeeded") - 1); goto LAB_00d0c278 end
            goto FLOW_past_lab_00d0c278
            ::LAB_00d0c278::
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
            ::FLOW_past_lab_00d0c278::
        end
        break
        ::continue_2::
    end
    ::FLOW_after_lab_00d0be35::
    ::LAB_00d0c28b::
    quest:DeregisterTimer(timerId2)
    ::LAB_00d0c294::
    ::LAB_00d0c2a1::
    resources:ReleaseResource(resource)
end

-- CROWDBANDITS.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- CROWDBANDITS.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- CROWDBANDITS.OnPredicateFail (retail 0x00d0b380)
function OnPredicateFail(quest, me)
    if me:MsgIsKilledBy("SCRIPT_NAME_HERO") then
        quest:SetStateInt("AngryBanditNeeded", quest:GetStateInt("AngryBanditNeeded") + 1)
    end
end

