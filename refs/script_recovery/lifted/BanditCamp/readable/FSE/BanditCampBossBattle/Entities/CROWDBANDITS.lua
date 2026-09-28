-- Readable native conversion: CROWDBANDITS. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_PAUSED = 1  -- ECutsceneBehaviour (Ego_r.pdb)
local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CROWDBANDITS.Main (retail 0x00d0b3e0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local function parseGameInteger(text)
        local value, negative = 0, false
        for position = 1, #text do
            local character = text:sub(position, position)
            if character == "." then break end
            if character == "-" then
                negative = true
            elseif character >= "0" and character <= "9" then
                value = (value * 10 + tonumber(character)) % 4294967296
            end
        end
        if negative then value = (-value) % 4294967296 end
        -- Match the game's signed 32-bit result, including overflow.
        if value >= 2147483648 then value = value - 4294967296 end
        return value
    end
    local predicateResult, scratchValue2, scratchValue3, predicateResult3, scratchValue5
    local scratchValue6, scratchValue7, scratchValue8, scratchValue9, scratchValue, kingHealth
    local timerId, scratchValue32, scratchValue36, scratchValue38, scratchValue42, scratchValue44
    local scratchValue46, scratchValue48, scratchValue54
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
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    me:ClearCommands()
    kingHealth = quest:GetStateInt("KingHealth")
    local banditKing = quest:GetThingWithScriptName("BanditKing")
    local timerId2 = quest:RegisterTimer()
    timerId = timerId2
    predicateResult = false
    quest:SetTimer(timerId2, 0)
    while not quest:IsActiveThreadTerminating() do
        if quest:GetStateBool("BanditsNeededForCutscene") then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
            quest:EntitySetAsUseMovementInActions(me, true)
            resources:PrepareResource(resource)
            while quest:GetStateBool("BanditsNeededForCutscene") do
                if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
            quest:EntitySetAsUseMovementInActions(me, false)
        end
        if not predicateResult and quest:GetStateBool("ItsAllOver") then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
            quest:EntitySetCutsceneBehaviour(me, CUTSCENE_BEHAVIOUR_PAUSED)
            predicateResult = true
        end
        if quest:IsDistanceBetweenThingsUnder(me, hero, 3.5) then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
            quest:EntitySetAttackThingImmediately(me, hero, false, true)
        else
            if not (quest:IsDistanceBetweenThingsOver(me, hero, 6.5) and quest:GetTimer(timerId) < 1) then quest:NewScriptFrame(me); goto continue_1 end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
            if not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                if quest:GetStateInt("KingHealth") < kingHealth then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                    kingHealth = quest:GetStateInt("KingHealth")
                    quest:EntitySetFacingAngleTowardsThing(me, hero, true)
                    local scratchValue28 = math.random(0, 32767) & 0x80000001
                    scratchValue5 = scratchValue28 == 0
                    if scratchValue28 < 0 then
                        scratchValue5 = (scratchValue28 - 1 | 0xfffffffe) == 0xffffffff
                    end
                    if not scratchValue5 then goto LAB_00d0bd52 end
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                    local scratchValue15 = math.random(0, 32767) % 3
                    if scratchValue15 == 0 then
                        me:PlayAnimation("SPECIAL_BOAST", false, false, false, true, true, false, false)
                    else
                        if scratchValue15 == 1 then
                            me:PlayAnimation("ST_THREATEN", false, false, false, true, true, false, false)
                            goto LAB_00d0b927
                        end
                        if scratchValue15 == 2 then
                            me:PlayAnimation("SCRIPT_SHOUT", false, false, false, true, true, false, false)
                            goto LAB_00d0b927
                        end
                    end
                    ::LAB_00d0b927::
                    local scratchValue30 = math.random(0, 32767) & 0x80000003
                    scratchValue6 = scratchValue30 == 0
                    if scratchValue30 < 0 then
                        scratchValue6 = (scratchValue30 - 1 | 0xfffffffc) == 0xffffffff
                    end
                    if scratchValue6 then
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                        scratchValue32 = math.random(0, 32767) & 0x80000001
                        if scratchValue32 < 0 then
                            scratchValue32 = (scratchValue32 - 1 | 0xfffffffe) + 1
                        end
                        if scratchValue32 == 0 then
                            quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                        elseif scratchValue32 == 1 then
                            quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_02")
                        end
                    end
                else
                    if not quest:IsDistanceBetweenThingsOver(hero, banditKing, 8.0) then
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                        quest:EntitySetFacingAngleTowardsThing(me, hero, true)
                        local scratchValue34 = math.random(0, 32767) & 0x80000001
                        scratchValue7 = scratchValue34 == 0
                        if scratchValue34 < 0 then
                            scratchValue7 = (scratchValue34 - 1 | 0xfffffffe) == 0xffffffff
                        end
                        if not scratchValue7 then goto LAB_00d0bd52 end
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                        scratchValue36 = math.random(0, 32767) & 0x80000001
                        if scratchValue36 < 0 then
                            scratchValue36 = (scratchValue36 - 1 | 0xfffffffe) + 1
                        end
                        if scratchValue36 == 0 then
                            me:PlayAnimation("COCKY", false, false, false, true, true, false, false)
                        elseif scratchValue36 == 1 then
                            me:PlayAnimation("ST_UNPERTURBED_GESTURE", false, false, false, true, true, false, false)
                            goto LAB_00d0bcdc
                        end
                        ::LAB_00d0bcdc::
                        scratchValue38 = math.random(0, 32767) & 0x80000001
                        if scratchValue38 < 0 then
                            scratchValue38 = (scratchValue38 - 1 | 0xfffffffe) + 1
                        end
                        if scratchValue38 == 0 then
                            quest:PlaySoundOnThing(me, "SND_CROWDLARGEBOASTREACT_01")
                        elseif scratchValue38 == 1 then
                            quest:PlaySoundOnThing(me, "SND_CROWDLARGEBOASTREACT_02")
                        end
                        goto LAB_00d0bd70
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                    quest:EntitySetFacingAngleTowardsThing(me, hero, true)
                    local scratchValue40 = math.random(0, 32767) & 0x80000001
                    scratchValue8 = scratchValue40 == 0
                    if scratchValue40 < 0 then
                        scratchValue8 = (scratchValue40 - 1 | 0xfffffffe) == 0xffffffff
                    end
                    if not scratchValue8 then
                        goto LAB_00d0bd52
                    end
                    goto FLOW_hoist_lab_00d0bd52_1
                end
                goto FLOW_past_lab_00d0bd52
                ::LAB_00d0bd52::
                if not quest:IsActiveThreadTerminating() then quest:SetTimer(timerId, 2); goto LAB_00d0bd70 end
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(resource)
                do return end
                ::FLOW_hoist_lab_00d0bd52_1::
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                scratchValue42 = math.random(0, 32767) & 0x80000001
                if scratchValue42 < 0 then
                    scratchValue42 = (scratchValue42 - 1 | 0xfffffffe) + 1
                end
                if scratchValue42 == 0 then
                    me:PlayAnimation("SPECIAL_BOAST", false, false, false, true, true, false, false)
                elseif scratchValue42 == 1 then
                    me:PlayAnimation("ST_WAVE_SPECIAL_01", false, false, false, true, true, false, false)
                    goto LAB_00d0bacf
                end
                ::LAB_00d0bacf::
                scratchValue44 = math.random(0, 32767) & 0x80000007
                scratchValue9 = scratchValue44 == 0
                if scratchValue44 < 0 then
                    scratchValue9 = (scratchValue44 - 1 | 0xfffffff8) == 0xffffffff
                end
                if scratchValue9 then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                    local conversationID = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationID, hero)
                    quest:AddLineToConversation(conversationID, "TEXT_QST_009_BOSSFIGHT_TAUNTING", me, hero, false)
                    timerId = timerId2
                end
                scratchValue46 = math.random(0, 32767) & 0x80000003
                scratchValue = scratchValue46 == 0
                if scratchValue46 < 0 then
                    scratchValue = (scratchValue46 - 1 | 0xfffffffc) == 0xffffffff
                end
                if scratchValue then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                    scratchValue48 = math.random(0, 32767) & 0x80000001
                    if scratchValue48 < 0 then
                        scratchValue48 = (scratchValue48 - 1 | 0xfffffffe) + 1
                    end
                    if scratchValue48 == 0 then
                        quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                    elseif scratchValue48 == 1 then
                        quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_02")
                    end
                end
                ::FLOW_past_lab_00d0bd52::
            end
            ::LAB_00d0bd70::
            if quest:GetStateBool("BanditKingFightEnded") then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                if parseGameInteger(me:GetDataString()) == 1 then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                    quest:RemoveThing(me, false, true)
                end
                quest:EntitySetTargetable(me, true)
                quest:EntitySetAsDamageable(me, true)
                quest:EntitySetAsUseMovementInActions(me, true)
                if not quest:IsActiveThreadTerminating() then goto LAB_00d0be35 end
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(resource)
                return
            end
        end
        quest:NewScriptFrame(me)
        ::continue_1::
    end
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d0be35::
    while true do
        if quest:GetStateBool("BanditsNeededForCutscene") then
            if quest:IsActiveThreadTerminating() then break end
            quest:EntitySetAsUseMovementInActions(me, true)
            resources:PrepareResource(resource)
            while quest:GetStateBool("BanditsNeededForCutscene") do
                if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then break end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then break end
            quest:EntitySetAsUseMovementInActions(me, false)
        end
        if not quest:GetStateBool("TwinBladeKilled") then
            if quest:IsActiveThreadTerminating() then break end
            if quest:GetTimer(timerId) < 1 then
                local scratchValue50 = math.random(0, 32767) & 0x80000001
                scratchValue2 = scratchValue50 == 0
                if scratchValue50 < 0 then
                    scratchValue2 = (scratchValue50 - 1 | 0xfffffffe) == 0xffffffff
                end
                if scratchValue2 then
                    if quest:IsActiveThreadTerminating() then break end
                    local scratchValue18 = math.random(0, 32767) % 3
                    if scratchValue18 == 0 then
                        me:PlayAnimation("SPECIAL_BOAST", false, false, false, true, true, false, false)
                    else
                        if scratchValue18 == 1 then
                            me:PlayAnimation("ST_THREATEN", false, false, false, true, true, false, false)
                            goto LAB_00d0c05b
                        end
                        if scratchValue18 == 2 then
                            me:PlayAnimation("SCRIPT_SHOUT", false, false, false, true, true, false, false)
                            goto LAB_00d0c05b
                        end
                    end
                    ::LAB_00d0c05b::
                    local scratchValue52 = math.random(0, 32767) & 0x80000003
                    scratchValue3 = scratchValue52 == 0
                    if scratchValue52 < 0 then
                        scratchValue3 = (scratchValue52 - 1 | 0xfffffffc) == 0xffffffff
                    end
                    if scratchValue3 then
                        if quest:IsActiveThreadTerminating() then break end
                        scratchValue54 = math.random(0, 32767) & 0x80000001
                        if scratchValue54 < 0 then
                            scratchValue54 = (scratchValue54 - 1 | 0xfffffffe) + 1
                        end
                        if scratchValue54 == 0 then
                            quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                        elseif scratchValue54 == 1 then
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
        if me:MsgIsHitByHero() then goto LAB_00d0c1af end
        if me:MsgIsHitByAnySpecialAbilityFromHero() then
            if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0c1af end
        end
        predicateResult3 = false
        goto FLOW_past_lab_00d0c1af
        ::LAB_00d0c1af::
        predicateResult3 = true
        ::FLOW_past_lab_00d0c1af::
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
    quest:DeregisterTimer(timerId2)
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

