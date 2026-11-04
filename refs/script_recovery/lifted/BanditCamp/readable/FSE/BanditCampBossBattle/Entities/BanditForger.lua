-- Readable native conversion: BanditForger. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    EXPERIENCE_GRANT_SMALL = 60,  -- 10
    BAC_ForgerGoldAmount = 3716,  -- 1000.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local givenPass_

-- BanditForger.Main (retail 0x00d0c410)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult3, predicateResult4, predicateResult, predicateResult20, scratchValue4
    local scratchValue5, scratchValue6, questionAnswer, questionAnswer2, addNewConversation, timerId
    local hero2, hero3, hero17, line, scratchValue30, resource, scriptThing
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
    if not givenPass_ then
        if quest:IsActiveThreadTerminating() then goto LAB_00d0d8c8 end
        quest:GiveThingHeroRewardItem(me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", "")
        givenPass_ = true
    end
    repeat
        if scratchValue6 == 0 then goto LAB_00d0d04e end
        if not quest:NewScriptFrame(me) then goto LAB_00d0d8c8 end
        if me:MsgIsHitByHero() then
            goto LAB_00d0c68f
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0c68f end
            local hero = quest:GetHero()
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
        if not (quest:GetTimer(timerId) < 1) then goto LAB_00d0c766 end
        hero2 = quest:GetHero()
        if not quest:IsDistanceBetweenThingsUnder(hero2, me, 10.0) or quest:GetStateBool("Gate2Open") then goto LAB_00d0c766 end
        hero3 = quest:GetHero()
        predicateResult4 = true
        if quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero3) then goto LAB_00d0c766 end
        goto FLOW_past_lab_00d0c766
        ::LAB_00d0c766::
        predicateResult4 = false
        ::FLOW_past_lab_00d0c766::
        if predicateResult4 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d0d8c8 end
            local hero4 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, hero4, false)
            me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, true, false, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d0d8c8 end
            if scratchValue4 == 0 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d0d8c8 end
                addNewConversation = quest:AddNewConversation(me, false, false)
                local hero5 = quest:GetHero()
                quest:AddPersonToConversation(addNewConversation, hero5)
                local hero6 = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_FORGER_ASIDE_FIRST", me, hero6, false)
                scratchValue4 = 1
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00d0d8c8 end
                addNewConversation = quest:AddNewConversation(me, false, false)
                local hero7 = quest:GetHero()
                quest:AddPersonToConversation(addNewConversation, hero7)
                local hero8 = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_009_FORGER_ASIDE_SECOND", me, hero8, false)
                scratchValue4 = 0
            end
            quest:SetTimer(timerId, 10)
        end
    until me:IsTalkedToByHero()
    if not quest:IsActiveThreadTerminating() then
        local movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        if not quest:GetStateBool("Gate2Open") then
            local hero9 = quest:GetHero()
            local isObjectInThingsPossession = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero9)
            local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if not isObjectInThingsPossession then
                if isActiveThreadTerminating then
                    quest:PauseAllNonScriptedEntities(false)
                else
                    scriptThing = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scriptThing) then
                        line = "TEXT_QST_009_FORGER_CHAT"
                        local hero10 = quest:GetHero()
                        if not me:Speak(hero10, line, 0, false, true, false) then
                            goto LAB_00d0cb83
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d0cb83 end
                        end
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
                        do
                            scriptThing = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scriptThing) then
                                line = "TEXT_QST_009_FORGER_TURNED_DOWN"
                                local hero11 = quest:GetHero()
                                if not me:Speak(hero11, line, 0, false, true, false) then
                                    goto LAB_00d0cb83
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d0cb83 end
                                end
                                goto LAB_00d0d014
                            end
                            goto LAB_00d0d023
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        goto FLOW_after_lab_00d0d377
                    end
                    do
                        if quest:GetHeroGold() < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_ForgerGoldAmount) then
                            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d0d377 end
                            scriptThing = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scriptThing) then
                                line = "TEXT_QST_009_FORGER_FAILED"
                                local hero12 = quest:GetHero()
                                if not me:Speak(hero12, line, 0, false, true, false) then
                                    goto LAB_00d0cb83
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d0cb83 end
                                end
                                goto LAB_00d0d014
                            end
                            goto LAB_00d0d023
                        end
                        if not quest:IsActiveThreadTerminating() then
                            scriptThing = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scriptThing) then
                                line = "TEXT_QST_009_FORGER_SUCCESS"
                                local hero13 = quest:GetHero()
                                if not me:Speak(hero13, line, 0, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto FLOW_after_lab_00d0d377
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d0d377 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d0cb83 end
                            end
                            quest:GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", -1, false)
                            addNewConversation = math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_ForgerGoldAmount)))
                            quest:GiveHeroGold(addNewConversation)
                            addNewConversation = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_ForgerGoldAmount)))
                            quest:EntityGiveGold(me, addNewConversation)
                            quest:RemoveItemFromContainer(me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS")
                            scratchValue5 = 0
                            quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_SMALL))
                            quest:ClearThingHasInformation(me)
                            goto LAB_00d0d023
                        end
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
            scriptThing = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(scriptThing) then
                line = "TEXT_QST_009_FORGER_EARLY_GOT_PASS"
                local hero14 = quest:GetHero()
                if not me:Speak(hero14, line, 0, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    goto LAB_00d0d8c8
                end
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
            scriptThing = resources:ScriptThing(resource)
            if quest:GetHealth(scriptThing) <= 0.0 then goto LAB_00d0d023 end
            line = "TEXT_QST_009_FORGER_EARLY_NOT_NEED"
            local hero15 = quest:GetHero()
            if not me:Speak(hero15, line, 0, false, true, false) then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                ReleaseEverything()
                return
            end
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
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0d12f end
                local hero16 = quest:GetHero()
                predicateResult = false
                if quest:AreEntitiesEnemies(me, hero16) then goto LAB_00d0d12f end
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
        local scratchValue31 = scratchValue30
        quest:PauseAllNonScriptedEntities(true)
        if quest:GetStateBool("Gate2Open") then goto LAB_00d0d27c end
        hero17 = quest:GetHero()
        predicateResult20 = false
        if quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", hero17) then goto LAB_00d0d27c end
        goto FLOW_past_lab_00d0d27c
        ::LAB_00d0d27c::
        predicateResult20 = true
        ::FLOW_past_lab_00d0d27c::
        if predicateResult20 then
            if not quest:IsActiveThreadTerminating() then
                scriptThing = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scriptThing) then
                    line = "TEXT_QST_009_FORGER_LATE_NOT_NEED"
                    local hero18 = quest:GetHero()
                    if not me:Speak(hero18, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        goto FLOW_after_lab_00d0d8e5
                    end
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
                                    local scratchValue33 = resources:ScriptThing(resource)
                                    if 0.0 < quest:GetHealth(scratchValue33) then
                                        line = "TEXT_QST_009_FORGER_SECOND_FAIL"
                                        local hero19 = quest:GetHero()
                                        if not me:Speak(hero19, line, 0, false, true, false) then
                                            goto LAB_00d0d365
                                        end
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d0d365 end
                                        end
                                        goto LAB_00d0d81a
                                    end
                                    goto LAB_00d0d829
                                end
                                goto LAB_00d0d8e5
                            end
                            if not quest:IsActiveThreadTerminating() then
                                local scratchValue34 = resources:ScriptThing(resource)
                                if 0.0 < quest:GetHealth(scratchValue34) then
                                    line = "TEXT_QST_009_FORGER_SUCCESS"
                                    local hero20 = quest:GetHero()
                                    if not me:Speak(hero20, line, 0, false, true, false) then
                                        goto LAB_00d0d8e5
                                    end
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d0d8e5 end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d0d365 end
                                end
                                quest:GiveHeroObject("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", -1, false)
                                addNewConversation = math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_ForgerGoldAmount)))
                                quest:GiveHeroGold(addNewConversation)
                                addNewConversation = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_ForgerGoldAmount)))
                                quest:EntityGiveGold(me, addNewConversation)
                                quest:RemoveItemFromContainer(me, "OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS")
                                quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_SMALL))
                                quest:ClearThingHasInformation(me)
                                scratchValue5 = 0
                                scratchValue30 = scratchValue31
                                goto LAB_00d0d829
                            end
                        end
                        goto LAB_00d0d365
                    end
                    if not predicateResult25 then
                        local scratchValue32 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(scratchValue32) then
                            line = "TEXT_QST_009_FORGER_TURNED_DOWN"
                            local hero21 = quest:GetHero()
                            if not me:Speak(hero21, line, 0, false, true, false) then
                                goto LAB_00d0d365
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d0d365 end
                            end
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
            local hero22 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(me, hero22)
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
    givenPass_ = false
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntitySetInFaction(me, "FACTION_TWINBLADE_CAMP_BANDITS")
    quest:SetIsPushableByHero(me, false)
end

-- BanditForger.OnPersist (retail 0x00d0ee40)
function OnPersist(quest, me, context)
    local givenPass = givenPass_ or false
    givenPass_ = quest:PersistTransferBool(context, "GivenPass", givenPass)
end

-- BanditForger.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

