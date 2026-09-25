-- Readable native conversion: BanditKing. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local healthBarIndex

-- BanditKing.Main (retail 0x00d0a830)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, scratchValue2, scratchValue3, scratchValue4, predicateResult3
    local msgIsHitByAnySpecialAbilityFromHero, getStateBool, scratchValue5, fret_0, conversationId
    local conversationId2, timerId, getHero, health, health2, speechResult, movie, resource
    local scratchValue7, scratchValue8
    msgIsHitByAnySpecialAbilityFromHero = false
    quest:NewScriptFrame(me)
    scratchValue4 = quest:IsActiveThreadTerminating()
    if not scratchValue4 then
        quest:SetQuestCardObjective("Q_BanditCamp", "TEXT_QUEST_BANDIT_CAMP_OBJECTIVE_07", "", "BanditCampMain")
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
        quest:EntitySetAbleToBeEngagedInCombat(me, false)
        health = quest:GetHealth(me)
        scratchValue = math.tointeger(math.modf(value))
        scratchValue4 = false
        predicateResult3 = false
        local scratchValue6 = scratchValue
        conversationId2 = quest:RegisterTimer()
        timerId = conversationId2
        quest:SetTimer(timerId, 0)
        getStateBool = quest:GetStateBool("ItsAllOver")
        while not getStateBool do
            quest:NewScriptFrame(me)
            local predicateResult = quest:IsActiveThreadTerminating()
            if predicateResult then
                quest:DeregisterTimer(timerId)
                return
            end
            health2 = quest:GetHealth(me)
            conversationId = math.tointeger(math.modf(value_00))
            quest:SetStateInt("KingHealth", conversationId)
            local getStateInt = quest:GetStateInt("KingHealth") < (scratchValue * 3 + (scratchValue * 3 >> 31 & 3U)) >> 2 and not scratchValue4
            scratchValue2 = getStateInt
            if scratchValue2 then
                conversationId = quest:GetTimer(timerId)
                scratchValue2 = conversationId < 1
            end
            if scratchValue2 then
                scratchValue4 = quest:IsActiveThreadTerminating()
                if scratchValue4 then
                    quest:DeregisterTimer(timerId)
                    return
                end
                conversationId = quest:AddNewConversation(me, false, false)
                getHero = hero
                quest:AddPersonToConversation(conversationId, getHero)
                getHero = hero
                quest:AddLineToConversation(conversationId, "TEXT_QST_009_TWINBLADE_TAUNT_10", me, getHero, false)
                scratchValue4 = true
                quest:SetTimer(timerId, 8)
                conversationId2 = timerId
                scratchValue = scratchValue6
            end
            local getStateInt2 = quest:GetStateInt("KingHealth") < scratchValue / 2 and not predicateResult3
            scratchValue3 = getStateInt2
            if scratchValue3 then
                conversationId = quest:GetTimer(timerId)
                scratchValue3 = conversationId < 1
            end
            if not scratchValue3 then
                getStateBool = quest:GetStateBool("ItsAllOver")
            else
                predicateResult3 = quest:IsActiveThreadTerminating()
                if predicateResult3 then
                    quest:DeregisterTimer(timerId)
                    return
                end
                conversationId = quest:AddNewConversation(me, false, false)
                getHero = hero
                quest:AddPersonToConversation(conversationId, getHero)
                getHero = hero
                quest:AddLineToConversation(conversationId, "TEXT_QST_009_TWINBLADE_TAUNT_20", me, getHero, false)
                predicateResult3 = true
                quest:SetTimer(timerId, 8)
                conversationId2 = timerId
                scratchValue = scratchValue6
                getStateBool = quest:GetStateBool("ItsAllOver")
            end
        end
        scratchValue4 = quest:IsActiveThreadTerminating()
        if scratchValue4 then
            quest:DeregisterTimer(timerId)
            return
        end
        scratchValue = 0
        resource = resources:NewResource()
        resources:PrepareResource(resource)
        scratchValue4 = resources:TryAcquire(resource, me, 4)
        while not scratchValue4 do
            quest:NewScriptFrame(me)
            scratchValue4 = quest:IsActiveThreadTerminating()
            if scratchValue4 then goto LAB_00d0b2a0 end
            scratchValue4 = resources:TryAcquire(resource, me, 4)
        end
        scratchValue4 = quest:IsActiveThreadTerminating()
        if not scratchValue4 then
            local timerId2 = quest:RegisterTimer()
            quest:SetTimer(timerId2, 0)
            scratchValue4 = false
            getHero = hero
            quest:EntitySetFacingAngleTowardsThing(me, getHero, scratchValue4)
            scratchValue4 = false
            local pThing = hero
            quest:EntitySetFacingAngleTowardsThing(pThing, me, scratchValue4)
            me:PlayLoopingAnimation("DEFEATED_POSE", -1, false, false, false)
            scratchValue4 = quest:IsActiveThreadTerminating()
            repeat
                if scratchValue4 then
                    quest:DeregisterTimer(timerId2)
                    resources:ReleaseResource(resource)
                    quest:DeregisterTimer(timerId)
                    return
                end
                conversationId2 = quest:GetTimer(timerId2)
                if 0 < conversationId2 then goto LAB_00d0ae81 end
                scratchValue4 = quest:IsActiveThreadTerminating()
                if scratchValue4 then goto LAB_00d0b297 end
                conversationId2 = quest:AddNewConversation(me, false, false)
                getHero = hero
                quest:AddPersonToConversation(conversationId2, getHero)
                -- TODO(native): switch(CVar8) {
                -- TODO(native): case (CCharString)0x0:
                getHero = hero
                quest:AddLineToConversation(conversationId2, "TEXT_QST_009_TWINBLADE_BEGGING_10", me, getHero, false)
                scratchValue = 1
                scratchValue7 = scratchValue
                break
                -- TODO(native): case (CCharString)0x1:
                getHero = hero
                quest:AddLineToConversation(conversationId2, "TEXT_QST_009_TWINBLADE_BEGGING_20", me, getHero, false)
                scratchValue = 2
                scratchValue7 = scratchValue
                break
                -- TODO(native): case (CCharString)0x2:
                getHero = hero
                quest:AddLineToConversation(conversationId2, "TEXT_QST_009_TWINBLADE_BEGGING_30", me, getHero, false)
                goto LAB_00d0ae5e
                -- TODO(native): case (CCharString)0x3:
                getHero = hero
                quest:AddLineToConversation(conversationId2, "TEXT_QST_009_TWINBLADE_BEGGING_40", me, getHero, false)
                scratchValue = 4
                scratchValue7 = scratchValue
                break
                -- TODO(native): case (CCharString)0x4:
                getHero = hero
                quest:AddLineToConversation(conversationId2, "TEXT_QST_009_TWINBLADE_BEGGING_50", me, getHero, false)
                ::LAB_00d0ae5e::
                scratchValue = 3
                scratchValue7 = scratchValue
            end
            quest:SetTimer(timerId2, 10)
            ::LAB_00d0ae81::
            scratchValue4 = me:MsgIsHitByHero()
            if scratchValue4 then
                goto LAB_00d0af0f
            else
                msgIsHitByAnySpecialAbilityFromHero = me:MsgIsHitByAnySpecialAbilityFromHero()
                if msgIsHitByAnySpecialAbilityFromHero then
                    msgIsHitByAnySpecialAbilityFromHero = true
                    scratchValue4 = me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)
                    if not scratchValue4 then goto LAB_00d0af0f end
                end
                msgIsHitByAnySpecialAbilityFromHero = true
                scratchValue4 = false
            end
            goto FLOW_past_lab_00d0af0f
            ::LAB_00d0af0f::
            scratchValue4 = true
            ::FLOW_past_lab_00d0af0f::
            msgIsHitByAnySpecialAbilityFromHero = msgIsHitByAnySpecialAbilityFromHero and false
            if scratchValue4 then
                scratchValue4 = quest:IsActiveThreadTerminating()
                if scratchValue4 then
                    goto LAB_00d0b297
                end
                goto FLOW_hoist_lab_00d0b297_1
            end
            goto FLOW_past_lab_00d0b297
            ::LAB_00d0b297::
            quest:DeregisterTimer(timerId2)
            break
            ::FLOW_hoist_lab_00d0b297_1::
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            me:ClearAllActionsIncludingLoopingAnimations()
            scratchValue8 = resources:ScriptThing(resource)
            getHero = scratchValue8
            fret_0 = quest:GetHealth(getHero)
            scratchValue5 = 0.0
            if scratchValue5 < fret_0 then
                local p5 = 0
                local p4 = 1
                conversationId = 0
                conversationId2 = 0
                local p1 = "TEXT_QST_009_TWINBLADE_OVER"
                getHero = hero
                speechResult = me:Speak(getHero, p1, conversationId2, conversationId ~= 0, p4 ~= 0, p5 ~= 0)
                conversationId2 = me:IsPerformingScriptTask()
                getStateBool = conversationId2
                while getStateBool do
                    quest:NewScriptFrame(me)
                    scratchValue4 = quest:IsActiveThreadTerminating()
                    if scratchValue4 then
                        quest:PauseAllNonScriptedEntities(false)
                        goto LAB_00d0b27a
                    end
                    conversationId2 = me:IsPerformingScriptTask()
                    getStateBool = conversationId2
                end
                scratchValue4 = quest:IsActiveThreadTerminating()
                if scratchValue4 then
                    quest:PauseAllNonScriptedEntities(false)
                    goto LAB_00d0b27a
                end
                goto FLOW_past_lab_00d0b27a
                ::LAB_00d0b27a::
                resources:DestroyMovie(movie)
                goto LAB_00d0b297
                ::FLOW_past_lab_00d0b27a::
            end
            quest:SetStateBool("TwinBladeAttacked", true)
            quest:SetStateInt("AngryBanditNeeded", 4)
            resources:PrepareResource(resource)
            getHero = hero
            quest:GiveThingBestEnemyTarget(me, getHero)
            quest:EntitySetAsKillable(me, true, true)
            quest:EntitySetAbleToBeEngagedInCombat(me, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            quest:DisplayQuestInfo(true)
            conversationId2 = quest:AddQuestInfoBar(scratchValue6, 0.0, {R = 255, G = 0, B = 0, A = 255}, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_TWINBLADE", "", 1.0)
            healthBarIndex = conversationId2
            scratchValue4 = me:IsAlive()
            scratchValue = scratchValue7
            while scratchValue4 do
                quest:NewScriptFrame(me)
                scratchValue4 = quest:IsActiveThreadTerminating()
                if scratchValue4 then goto LAB_00d0b297 end
                local scale = -1.0
                local max = -1.0
                local fret_00 = quest:GetHealth(me)
                quest:UpdateQuestInfoBar(healthBarIndex, fret_00, max, scale)
                scratchValue4 = me:IsAlive()
                scratchValue = scratchValue7
            end
            ::FLOW_past_lab_00d0b297::
            quest:NewScriptFrame(me)
            scratchValue4 = quest:IsActiveThreadTerminating()
        until false
    end
    ::LAB_00d0b2a0::
    resources:ReleaseResource(resource)
    quest:DeregisterTimer(timerId)
    end
end

-- BanditKing.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- BanditKing.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- BanditKing.OnPredicateFail (retail 0x00d0a7b0)
function OnPredicateFail(quest, me)
    if me:MsgIsKilledBy("") then
        quest:SetStateBool("TwinBladeKilled", true)
        quest:SetMasterGameState("BanditCampTwinbladeKilled", true)
    end
    quest:DisplayQuestInfo(false)
    quest:RemoveQuestInfoElement(healthBarIndex)
end

