-- Readable native conversion: BanditForger. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    EXPERIENCE_GRANT_SMALL = 60,  -- 10
    BAC_ForgerGoldAmount = 3716,  -- 1000.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local givenPass

-- BanditForger.Main (retail 0x00d0c410)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult3, predicateResult, predicateResult20, scratchValue4, scratchValue5
    local scratchValue6, questionAnswer, questionAnswer2, addNewConversation, timerId, resource
    local function ReleaseEverything()
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    scratchValue5 = 1
    scratchValue4 = 0
    scratchValue6 = 1
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    if not quest:NewScriptFrame(me) then goto LAB_00d0d8d1 end
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    if not givenPass then
        if quest:IsActiveThreadTerminating() then goto LAB_00d0d8c8 end
        quest:GiveThingHeroRewardItem(me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", "")
        givenPass = true
    end
    repeat
        if scratchValue6 == 0 then goto LAB_00d0d04e end
        if not quest:NewScriptFrame(me) then goto LAB_00d0d8c8 end
        if me:MsgIsHitByHero() then
            goto LAB_00d0c68f
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0c68f end
            end
            predicateResult3 = false
            if quest:AreEntitiesEnemies(me, hero) then goto LAB_00d0c68f end
        end
        goto FLOW_past_lab_00d0c68f
        ::LAB_00d0c68f::
        predicateResult3 = true
        ::FLOW_past_lab_00d0c68f::
        if predicateResult3 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d0d8c8 end
            scratchValue6 = 0
        end
        if not ((quest:GetTimer(timerId) >= 1) or ((not quest:IsDistanceBetweenThingsUnder(hero, me, 10.0)) or quest:GetStateBool("Gate2Open")) or quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero)) then
            if quest:IsActiveThreadTerminating() then goto LAB_00d0d8c8 end
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
            me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, true, false, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d0d8c8 end
            if scratchValue4 == 0 then
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_FORGER_ASIDE_FIRST", me, hero, false)
                scratchValue4 = 1
            else
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_FORGER_ASIDE_SECOND", me, hero, false)
                scratchValue4 = 0
            end
            quest:SetTimer(timerId, 10)
        end
    until me:IsTalkedToByHero()
    if not quest:IsActiveThreadTerminating() then
        local movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        if not quest:GetStateBool("Gate2Open") then
            local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if not quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero) then
                if isActiveThreadTerminating then
                    quest:PauseAllNonScriptedEntities(false)
                else
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        if not me:Speak(hero, "TEXT_QST_009_FORGER_CHAT", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0cb83 end
                        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d0d377 end
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_009_FORGER_CHAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d0cb83 end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d0d377 end
                    if questionAnswer ~= 1 then
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_009_FORGER_TURNED_DOWN", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0cb83 end
                            goto LAB_00d0d014
                        end
                        goto LAB_00d0d023
                        quest:PauseAllNonScriptedEntities(false)
                        goto FLOW_after_lab_00d0d377
                    end
                    if quest:GetHeroGold() < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_ForgerGoldAmount) then
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_009_FORGER_FAILED", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0cb83 end
                            goto LAB_00d0d014
                        end
                        goto LAB_00d0d023
                    end
                    if not quest:IsActiveThreadTerminating() then
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            me:Speak(hero, "TEXT_QST_009_FORGER_SUCCESS", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d0d377 end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d0cb83 end
                        end
                        quest:GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", -1, false)
                        quest:GiveHeroGold(math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_ForgerGoldAmount))))
                        quest:EntityGiveGold(me, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_ForgerGoldAmount))))
                        quest:RemoveItemFromContainer(me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS")
                        scratchValue5 = 0
                        quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_SMALL))
                        quest:ClearThingHasInformation(me)
                        goto LAB_00d0d023
                    end
                    ::LAB_00d0cb83::
                    quest:PauseAllNonScriptedEntities(false)
                end
                ::FLOW_after_lab_00d0d377::
                resources:DestroyMovie(movie2)
                goto LAB_00d0d8c8
            end
            if isActiveThreadTerminating then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                goto LAB_00d0d8c8
            end
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                me:Speak(hero, "TEXT_QST_009_FORGER_EARLY_GOT_PASS", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        goto LAB_00d0d8c8
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    goto LAB_00d0d8c8
                end
            end
        else
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                goto LAB_00d0d8c8
            end
            if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then goto LAB_00d0d023 end
            me:Speak(hero, "TEXT_QST_009_FORGER_EARLY_NOT_NEED", GROUP_SELECT_FIRST, false, true, false)
            while me:IsPerformingScriptTask() do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    ReleaseEverything()
                    do return end
                end
            end
            goto LAB_00d0d014
        end
        goto FLOW_past_lab_00d0d014
        ::LAB_00d0d014::
        if quest:IsActiveThreadTerminating() then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            goto LAB_00d0d8c8
        end
        ::FLOW_past_lab_00d0d014::
        ::LAB_00d0d023::
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
        goto LAB_00d0d04e
    end
    goto FLOW_past_lab_00d0d04e
    ::LAB_00d0d04e::
    if not quest:IsActiveThreadTerminating() then
        ::LAB_00d0d063::
        repeat
            if scratchValue5 == 0 or scratchValue6 == 0 then goto LAB_00d0d85f end
            if not quest:NewScriptFrame(me) then goto LAB_00d0d8c8 end
            if me:MsgIsHitByHero() then
                goto LAB_00d0d12f
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0d12f end
                end
                predicateResult = false
                if quest:AreEntitiesEnemies(me, hero) then goto LAB_00d0d12f end
            end
            goto FLOW_past_lab_00d0d12f
            ::LAB_00d0d12f::
            predicateResult = true
            ::FLOW_past_lab_00d0d12f::
            if predicateResult then
                if quest:IsActiveThreadTerminating() then goto LAB_00d0d8c8 end
                scratchValue6 = 0
            end
        until me:IsTalkedToByHero()
        if quest:IsActiveThreadTerminating() then goto LAB_00d0d8c8 end
        local movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        if quest:GetStateBool("Gate2Open") then goto LAB_00d0d27c end
        -- TODO(native): xStack_8c = xStack_8c | 0x80;
        predicateResult20 = false
        if quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero) then goto LAB_00d0d27c end
        goto FLOW_past_lab_00d0d27c
        ::LAB_00d0d27c::
        predicateResult20 = true
        ::FLOW_past_lab_00d0d27c::
            -- TODO(native): xStack_8c = xStack_8c & 0xffffff7f;
        if predicateResult20 then
            if not quest:IsActiveThreadTerminating() then
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    me:Speak(hero, "TEXT_QST_009_FORGER_LATE_NOT_NEED", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d0d8e5 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0d365 end
                end
                goto LAB_00d0d829
            end
            goto FLOW_hoist_lab_00d0d829_1
        else
            if not quest:IsActiveThreadTerminating() then
                quest:GiveHeroYesNoQuestion("TEXT_QST_009_FORGER_SECOND_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                while questionAnswer2 < 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d0d365 end
                    questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                if not quest:IsActiveThreadTerminating() then
                    local predicateResult25 = quest:IsActiveThreadTerminating()
                    if questionAnswer2 == 1 then
                        if not predicateResult25 then
                            if quest:GetHeroGold() < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_ForgerGoldAmount) then
                                if not quest:IsActiveThreadTerminating() then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                        if not me:Speak(hero, "TEXT_QST_009_FORGER_SECOND_FAIL", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0d365 end
                                        goto LAB_00d0d81a
                                    end
                                    goto LAB_00d0d829
                                end
                                goto LAB_00d0d8e5
                            end
                            if not quest:IsActiveThreadTerminating() then
                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                    if not me:Speak(hero, "TEXT_QST_009_FORGER_SUCCESS", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0d8e5 end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d0d365 end
                                end
                                quest:GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", -1, false)
                                quest:GiveHeroGold(math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_ForgerGoldAmount))))
                                quest:EntityGiveGold(me, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_ForgerGoldAmount))))
                                quest:RemoveItemFromContainer(me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS")
                                quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_SMALL))
                                quest:ClearThingHasInformation(me)
                                scratchValue5 = 0
                                goto LAB_00d0d829
                            end
                        end
                        goto LAB_00d0d365
                    end
                    if not predicateResult25 then
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_009_FORGER_TURNED_DOWN", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0d365 end
                            goto LAB_00d0d81a
                        end
                        goto FLOW_hoist_lab_00d0d81a_1
                    end
                    goto FLOW_past_lab_00d0d81a
                    ::LAB_00d0d81a::
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0d8e5 end
                    ::FLOW_hoist_lab_00d0d81a_1::
                    goto LAB_00d0d829
                    ::FLOW_past_lab_00d0d81a::
                end
            end
            ::LAB_00d0d8e5::
            quest:PauseAllNonScriptedEntities(false)
        end
        ::FLOW_after_lab_00d0d8e5::
        goto FLOW_past_lab_00d0d829
        ::LAB_00d0d829::
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        goto LAB_00d0d063
        ::FLOW_hoist_lab_00d0d829_1::
        ::LAB_00d0d365::
        quest:PauseAllNonScriptedEntities(false)
        ::FLOW_past_lab_00d0d829::
        resources:DestroyMovie(movie)
    end
    ::FLOW_past_lab_00d0d04e::
    ::LAB_00d0d8c8::
    quest:DeregisterTimer(timerId)
    ::LAB_00d0d8d1::
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d0d85f::
    if not quest:IsActiveThreadTerminating() then
        if scratchValue6 == 0 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d0d8c8 end
            quest:ClearThingHasInformation(me)
            quest:GiveThingBestEnemyTarget(me, hero)
        end
        resources:PrepareResource(resource)
        repeat
            quest:NewScriptFrame(me)
        until quest:IsActiveThreadTerminating()
    end
    goto LAB_00d0d8c8
end

-- BanditForger.Init (retail 0x00d0c390)
function Init(quest, me)
    givenPass = false
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntitySetInFaction(me, "FACTION_TWINBLADE_CAMP_BANDITS")
    quest:SetIsPushableByHero(me, false)
end

-- BanditForger.OnPersist (retail 0x00d0ee40)
function OnPersist(quest, me, context)
    quest:SetStateBool("GivenPass", quest:PersistTransferBool(context, "GivenPass", quest:GetStateBool("GivenPass")))
end

-- BanditForger.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

