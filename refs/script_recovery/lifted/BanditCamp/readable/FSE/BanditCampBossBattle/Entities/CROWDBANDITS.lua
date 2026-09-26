-- Readable native conversion: CROWDBANDITS. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_PAUSED = 1  -- ECutsceneBehaviour (Ego_r.pdb)
local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CROWDBANDITS.Main (retail 0x00d0b3e0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, scratchValue2, scratchValue3, predicateResult3, scratchValue5
    local scratchValue6, scratchValue7, scratchValue8, scratchValue9, scratchValue, kingHealth
    local banditKing, timerId, scratchValue24, scratchValue25, scratchValue31, scratchValue35
    local scratchValue37, scratchValue43, timerId2
    scratchValue43 = 0
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
    predicateResult = false
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
        if not predicateResult and quest:GetStateBool("ItsAllOver") then
            if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
            quest:EntitySetCutsceneBehaviour(me, CUTSCENE_BEHAVIOUR_PAUSED)
            predicateResult = true
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
                    scratchValue25 = math.random(0, 32767) & 0x80000001
                    scratchValue5 = scratchValue25 == 0
                    if scratchValue25 < 0 then
                        scratchValue5 = (scratchValue25 - 1 | 0xfffffffe) == 0xffffffff
                    end
                    if not scratchValue5 then goto LAB_00d0bd52 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
                    local scratchValue12 = math.random(0, 32767) % 3
                    if scratchValue12 == 0 then
                        me:PlayAnimation("SPECIAL_BOAST", false, false, false, true, true, false, false)
                    else
                        if scratchValue12 == 1 then
                            me:PlayAnimation("ST_THREATEN", false, false, false, true, true, false, false)
                            goto LAB_00d0b927
                        end
                        if scratchValue12 == 2 then
                            me:PlayAnimation("SCRIPT_SHOUT", false, false, false, true, true, false, false)
                            goto LAB_00d0b927
                        end
                    end
                    ::LAB_00d0b927::
                    scratchValue25 = math.random(0, 32767) & 0x80000003
                    scratchValue6 = scratchValue25 == 0
                    if scratchValue25 < 0 then
                        scratchValue6 = (scratchValue25 - 1 | 0xfffffffc) == 0xffffffff
                    end
                    if scratchValue6 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
                        scratchValue25 = math.random(0, 32767) & 0x80000001
                        if scratchValue25 < 0 then
                            scratchValue25 = (scratchValue25 - 1 | 0xfffffffe) + 1
                        end
                        if scratchValue25 == 0 then
                            quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                        elseif scratchValue25 == 1 then
                            quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_02")
                        end
                    end
                else
                    if not quest:IsDistanceBetweenThingsOver(hero, banditKing, 8.0) then
                        if not quest:IsActiveThreadTerminating() then
                            quest:EntitySetFacingAngleTowardsThing(me, hero, true)
                            scratchValue25 = math.random(0, 32767) & 0x80000001
                            scratchValue7 = scratchValue25 == 0
                            if scratchValue25 < 0 then
                                scratchValue7 = (scratchValue25 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if not scratchValue7 then goto LAB_00d0bd52 end
                            if not quest:IsActiveThreadTerminating() then
                                scratchValue31 = math.random(0, 32767) & 0x80000001
                                if scratchValue31 < 0 then
                                    scratchValue31 = (scratchValue31 - 1 | 0xfffffffe) + 1
                                end
                                if scratchValue31 == 0 then
                                    me:PlayAnimation("COCKY", false, false, false, true, true, false, false)
                                elseif scratchValue31 == 1 then
                                    me:PlayAnimation("ST_UNPERTURBED_GESTURE", false, false, false, true, true, false, false)
                                    goto LAB_00d0bcdc
                                end
                                ::LAB_00d0bcdc::
                                scratchValue25 = math.random(0, 32767) & 0x80000001
                                if scratchValue25 < 0 then
                                    scratchValue25 = (scratchValue25 - 1 | 0xfffffffe) + 1
                                end
                                if scratchValue25 == 0 then
                                    quest:PlaySoundOnThing(me, "SND_CROWDLARGEBOASTREACT_01")
                                elseif scratchValue25 == 1 then
                                    quest:PlaySoundOnThing(me, "SND_CROWDLARGEBOASTREACT_02")
                                end
                                goto LAB_00d0bd70
                            end
                        end
                        goto LAB_00d0c28b
                    end
                    quest:EntitySetFacingAngleTowardsThing(me, hero, true)
                    scratchValue25 = math.random(0, 32767) & 0x80000001
                    scratchValue8 = scratchValue25 == 0
                    if scratchValue25 < 0 then
                        scratchValue8 = (scratchValue25 - 1 | 0xfffffffe) == 0xffffffff
                    end
                    if not scratchValue8 then
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
                scratchValue35 = math.random(0, 32767) & 0x80000001
                if scratchValue35 < 0 then
                    scratchValue35 = (scratchValue35 - 1 | 0xfffffffe) + 1
                end
                if scratchValue35 == 0 then
                    me:PlayAnimation("SPECIAL_BOAST", false, false, false, true, true, false, false)
                elseif scratchValue35 == 1 then
                    me:PlayAnimation("ST_WAVE_SPECIAL_01", false, false, false, true, true, false, false)
                    goto LAB_00d0bacf
                end
                ::LAB_00d0bacf::
                scratchValue37 = math.random(0, 32767) & 0x80000007
                scratchValue9 = scratchValue37 == 0
                if scratchValue37 < 0 then
                    scratchValue9 = (scratchValue37 - 1 | 0xfffffff8) == 0xffffffff
                end
                if scratchValue9 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
                    local conversationID = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationID, hero)
                    quest:AddLineToConversation(conversationID, "TEXT_QST_009_BOSSFIGHT_TAUNTING", me, hero, false)
                    timerId = timerId2
                end
                scratchValue25 = math.random(0, 32767) & 0x80000003
                scratchValue = scratchValue25 == 0
                if scratchValue25 < 0 then
                    scratchValue = (scratchValue25 - 1 | 0xfffffffc) == 0xffffffff
                end
                if scratchValue then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0c28b end
                    scratchValue25 = math.random(0, 32767) & 0x80000001
                    if scratchValue25 < 0 then
                        scratchValue25 = (scratchValue25 - 1 | 0xfffffffe) + 1
                    end
                    if scratchValue25 == 0 then
                        quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                    elseif scratchValue25 == 1 then
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
                if not quest:IsActiveThreadTerminating() then goto LAB_00d0be35 end
                goto LAB_00d0c28b
            end
        end
        quest:NewScriptFrame(me)
        ::continue_1::
    end
    quest:DeregisterTimer(timerId)
    goto LAB_00d0c294
    ::LAB_00d0be35::
    while true do
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
            if quest:IsActiveThreadTerminating() then break end
            quest:EntitySetAsUseMovementInActions(me, false)
        end
        if not quest:GetStateBool("TwinBladeKilled") then
            if quest:IsActiveThreadTerminating() then break end
            if quest:GetTimer(timerId) < 1 then
                scratchValue25 = math.random(0, 32767) & 0x80000001
                scratchValue2 = scratchValue25 == 0
                if scratchValue25 < 0 then
                    scratchValue2 = (scratchValue25 - 1 | 0xfffffffe) == 0xffffffff
                end
                if scratchValue2 then
                    if quest:IsActiveThreadTerminating() then break end
                    local scratchValue15 = math.random(0, 32767) % 3
                    if scratchValue15 == 0 then
                        me:PlayAnimation("SPECIAL_BOAST", false, false, false, true, true, false, false)
                    else
                        if scratchValue15 == 1 then
                            me:PlayAnimation("ST_THREATEN", false, false, false, true, true, false, false)
                            goto LAB_00d0c05b
                        end
                        if scratchValue15 == 2 then
                            me:PlayAnimation("SCRIPT_SHOUT", false, false, false, true, true, false, false)
                            goto LAB_00d0c05b
                        end
                    end
                    ::LAB_00d0c05b::
                    scratchValue25 = math.random(0, 32767) & 0x80000003
                    scratchValue3 = scratchValue25 == 0
                    if scratchValue25 < 0 then
                        scratchValue3 = (scratchValue25 - 1 | 0xfffffffc) == 0xffffffff
                    end
                    if scratchValue3 then
                        if quest:IsActiveThreadTerminating() then break end
                        scratchValue25 = math.random(0, 32767) & 0x80000001
                        if scratchValue25 < 0 then
                            scratchValue25 = (scratchValue25 - 1 | 0xfffffffe) + 1
                        end
                        if scratchValue25 == 0 then
                            quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                        elseif scratchValue25 == 1 then
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
        scratchValue24 = scratchValue43
        scratchValue43 = scratchValue43 | 1
        if me:MsgIsHitByHero() then goto LAB_00d0c1af end
        scratchValue25 = scratchValue24 | 3
        scratchValue43 = scratchValue25
        if me:MsgIsHitByAnySpecialAbilityFromHero() then
            scratchValue25 = scratchValue24 | 7
            scratchValue43 = scratchValue25
            if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0c1af end
        end
        predicateResult3 = false
        goto FLOW_past_lab_00d0c1af
        ::LAB_00d0c1af::
        predicateResult3 = true
        ::FLOW_past_lab_00d0c1af::
        if scratchValue25 & 4 ~= 0 then
            scratchValue25 = scratchValue25 & 0xfffffffb
            scratchValue43 = scratchValue25
        end
        if scratchValue25 & 2 ~= 0 then
            scratchValue25 = scratchValue25 & 0xfffffffd
            scratchValue43 = scratchValue25
        end
        if scratchValue25 & 1 ~= 0 then
            scratchValue43 = scratchValue25 & 0xfffffffe
        end
        if predicateResult3 then
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
        end
        if not quest:NewScriptFrame(me) then break end
    end
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

