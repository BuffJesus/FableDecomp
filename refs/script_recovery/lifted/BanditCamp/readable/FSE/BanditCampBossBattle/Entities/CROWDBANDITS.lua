-- Readable native conversion: CROWDBANDITS. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_PAUSED = 1  -- ECutsceneBehaviour (Ego_r.pdb)
local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CROWDBANDITS.Main (retail 0x00d0b3e0)
function Main(quest, me)
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
    local predicateResult, scratchValue2, scratchValue3, scratchValue5, scratchValue6, scratchValue7
    local scratchValue8, scratchValue9, scratchValue, kingHealth, timerId, retailRandModulo
    local retailRandModulo2, retailRandModulo3, retailRandModulo4, randomChoice3, randomChoice4
    local retailRandModulo5, retailRandModulo6
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
    local getNearestWithDefName = quest:GetNearestWithDefName(me, "VILLAGE_BANDIT_CAMP_BOSS")
    quest:EntityAttachToVillage(me, getNearestWithDefName)
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
        local hero = quest:GetHero()
        if quest:IsDistanceBetweenThingsUnder(me, hero, 3.5) then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
            local hero2 = quest:GetHero()
            quest:EntitySetAttackThingImmediately(me, hero2, false, true)
        else
            local hero3 = quest:GetHero()
            if not (quest:IsDistanceBetweenThingsOver(me, hero3, 6.5) and quest:GetTimer(timerId) < 1) then quest:NewScriptFrame(me); goto continue_1 end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
            if not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                if quest:GetStateInt("KingHealth") < kingHealth then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                    kingHealth = quest:GetStateInt("KingHealth")
                    local hero4 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, hero4, true)
                    local randomChoice5 = quest:RetailRandModulo(32768) & 0x80000001
                    scratchValue5 = randomChoice5 == 0
                    if randomChoice5 < 0 then
                        scratchValue5 = (randomChoice5 - 1 | 0xfffffffe) == 0xffffffff
                    end
                    if not scratchValue5 then goto LAB_00d0bd52 end
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                    local randomChoice = quest:RetailRandModulo(3)
                    if randomChoice == 0 then
                        resources:PlayAnimationWithNativeArgument5(resource, "SPECIAL_BOAST", false, false, false, true, false, false)
                    else
                        if randomChoice == 1 then
                            resources:PlayAnimationWithNativeArgument5(resource, "ST_THREATEN", false, false, false, true, false, false)
                            goto LAB_00d0b927
                        end
                        if randomChoice == 2 then
                            resources:PlayAnimationWithNativeArgument5(resource, "SCRIPT_SHOUT", false, false, false, true, false, false)
                            goto LAB_00d0b927
                        end
                    end
                    ::LAB_00d0b927::
                    local randomChoice6 = quest:RetailRandModulo(32768) & 0x80000003
                    scratchValue6 = randomChoice6 == 0
                    if randomChoice6 < 0 then
                        scratchValue6 = (randomChoice6 - 1 | 0xfffffffc) == 0xffffffff
                    end
                    if scratchValue6 then
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                        retailRandModulo = quest:RetailRandModulo(32768) & 0x80000001
                        if retailRandModulo < 0 then
                            retailRandModulo = (retailRandModulo - 1 | 0xfffffffe) + 1
                        end
                        if retailRandModulo == 0 then
                            quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                        elseif retailRandModulo == 1 then
                            quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_02")
                        end
                    end
                else
                    local hero5 = quest:GetHero()
                    if not quest:IsDistanceBetweenThingsOver(hero5, banditKing, 8.0) then
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                        local hero6 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, hero6, true)
                        local randomChoice7 = quest:RetailRandModulo(32768) & 0x80000001
                        scratchValue7 = randomChoice7 == 0
                        if randomChoice7 < 0 then
                            scratchValue7 = (randomChoice7 - 1 | 0xfffffffe) == 0xffffffff
                        end
                        if not scratchValue7 then goto LAB_00d0bd52 end
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                        retailRandModulo2 = quest:RetailRandModulo(32768) & 0x80000001
                        if retailRandModulo2 < 0 then
                            retailRandModulo2 = (retailRandModulo2 - 1 | 0xfffffffe) + 1
                        end
                        if retailRandModulo2 == 0 then
                            resources:PlayAnimationWithNativeArgument5(resource, "COCKY", false, false, false, true, false, false)
                        elseif retailRandModulo2 == 1 then
                            resources:PlayAnimationWithNativeArgument5(resource, "ST_UNPERTURBED_GESTURE", false, false, false, true, false, false)
                            goto LAB_00d0bcdc
                        end
                        ::LAB_00d0bcdc::
                        retailRandModulo3 = quest:RetailRandModulo(32768) & 0x80000001
                        if retailRandModulo3 < 0 then
                            retailRandModulo3 = (retailRandModulo3 - 1 | 0xfffffffe) + 1
                        end
                        if retailRandModulo3 == 0 then
                            quest:PlaySoundOnThing(me, "SND_CROWDLARGEBOASTREACT_01")
                        elseif retailRandModulo3 == 1 then
                            quest:PlaySoundOnThing(me, "SND_CROWDLARGEBOASTREACT_02")
                        end
                        goto LAB_00d0bd70
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                    local hero7 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, hero7, true)
                    local randomChoice8 = quest:RetailRandModulo(32768) & 0x80000001
                    scratchValue8 = randomChoice8 == 0
                    if randomChoice8 < 0 then
                        scratchValue8 = (randomChoice8 - 1 | 0xfffffffe) == 0xffffffff
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
                retailRandModulo4 = quest:RetailRandModulo(32768) & 0x80000001
                if retailRandModulo4 < 0 then
                    retailRandModulo4 = (retailRandModulo4 - 1 | 0xfffffffe) + 1
                end
                if retailRandModulo4 == 0 then
                    resources:PlayAnimationWithNativeArgument5(resource, "SPECIAL_BOAST", false, false, false, true, false, false)
                elseif retailRandModulo4 == 1 then
                    resources:PlayAnimationWithNativeArgument5(resource, "ST_WAVE_SPECIAL_01", false, false, false, true, false, false)
                    goto LAB_00d0bacf
                end
                ::LAB_00d0bacf::
                randomChoice3 = quest:RetailRandModulo(32768) & 0x80000007
                scratchValue9 = randomChoice3 == 0
                if randomChoice3 < 0 then
                    scratchValue9 = (randomChoice3 - 1 | 0xfffffff8) == 0xffffffff
                end
                if scratchValue9 then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                    local conversationID = quest:AddNewConversation(me, false, false)
                    local hero8 = quest:GetHero()
                    quest:AddPersonToConversation(conversationID, hero8)
                    local hero9 = quest:GetHero()
                    quest:AddLineToConversation(conversationID, "TEXT_QST_009_BOSSFIGHT_TAUNTING", me, hero9, false)
                    timerId = timerId2
                end
                randomChoice4 = quest:RetailRandModulo(32768) & 0x80000003
                scratchValue = randomChoice4 == 0
                if randomChoice4 < 0 then
                    scratchValue = (randomChoice4 - 1 | 0xfffffffc) == 0xffffffff
                end
                if scratchValue then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                    retailRandModulo5 = quest:RetailRandModulo(32768) & 0x80000001
                    if retailRandModulo5 < 0 then
                        retailRandModulo5 = (retailRandModulo5 - 1 | 0xfffffffe) + 1
                    end
                    if retailRandModulo5 == 0 then
                        quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                    elseif retailRandModulo5 == 1 then
                        quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_02")
                    end
                end
                ::FLOW_past_lab_00d0bd52::
            end
            ::LAB_00d0bd70::
            if quest:GetStateBool("BanditKingFightEnded") then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                local dataString = me:GetDataString()
                if parseGameInteger(dataString) == 1 then
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
                if quest:IsActiveThreadTerminating() then break end
                local randomChoice9 = quest:RetailRandModulo(32768) & 0x80000001
                scratchValue2 = randomChoice9 == 0
                if randomChoice9 < 0 then
                    scratchValue2 = (randomChoice9 - 1 | 0xfffffffe) == 0xffffffff
                end
                if scratchValue2 then
                    if quest:IsActiveThreadTerminating() then break end
                    local randomChoice2 = quest:RetailRandModulo(3)
                    if randomChoice2 == 0 then
                        resources:PlayAnimationWithNativeArgument5(resource, "SPECIAL_BOAST", false, false, false, true, false, false)
                    else
                        if randomChoice2 == 1 then
                            resources:PlayAnimationWithNativeArgument5(resource, "ST_THREATEN", false, false, false, true, false, false)
                            goto LAB_00d0c05b
                        end
                        if randomChoice2 == 2 then
                            resources:PlayAnimationWithNativeArgument5(resource, "SCRIPT_SHOUT", false, false, false, true, false, false)
                            goto LAB_00d0c05b
                        end
                    end
                    ::LAB_00d0c05b::
                    local randomChoice10 = quest:RetailRandModulo(32768) & 0x80000003
                    scratchValue3 = randomChoice10 == 0
                    if randomChoice10 < 0 then
                        scratchValue3 = (randomChoice10 - 1 | 0xfffffffc) == 0xffffffff
                    end
                    if scratchValue3 then
                        if quest:IsActiveThreadTerminating() then break end
                        retailRandModulo6 = quest:RetailRandModulo(32768) & 0x80000001
                        if retailRandModulo6 < 0 then
                            retailRandModulo6 = (retailRandModulo6 - 1 | 0xfffffffe) + 1
                        end
                        if retailRandModulo6 == 0 then
                            quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_01")
                        elseif retailRandModulo6 == 1 then
                            quest:PlaySoundOnThing(me, "SND_CROWDBOASTBOOS_02")
                        end
                    end
                else
                    if quest:IsActiveThreadTerminating() then break end
                    quest:SetTimer(timerId, 2)
                end
            end
        end
        if not not ((not (quest:GetStateInt("AngryBanditNeeded") < 1)) or me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)))) then
            if not quest:IsActiveThreadTerminating() then
                local hero10 = quest:GetHero()
                quest:GiveThingBestEnemyTarget(me, hero10)
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

