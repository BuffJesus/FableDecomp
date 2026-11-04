-- Readable native conversion: SickChildsMother. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST, GROUP_SELECT_RANDOM_NO_REPEAT = 0, 2  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    EndPotion = 1860,  -- 'OBJECT_MUSHROOM_POTION_01'
}

local helpers = require("V_SickChild.native_quest_helpers")

-- SickChildsMother.Main (retail 0x00ecd9e0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local isObjectInThingsPossession, predicateResult7, questionAnswer, motherTalkMovie, hero5
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(motherTalkMovie)
    end
    if not quest:NewScriptFrame(me) then return end
    local scratchValue = resources:MemberResource("seh_Mother")
    resources:PrepareResource(scratchValue)
    while not resources:TryAcquire(resources:MemberResource("seh_Mother"), me, 4) do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local scratchValue6 = quest:ReadGlobalGameDataString(SCRIPT_DEF.EndPotion)
    local hero = quest:GetHero()
    isObjectInThingsPossession = quest:IsObjectInThingsPossession(scratchValue6, hero)
    if isObjectInThingsPossession or not quest:GetStateBool("MotherIntroDone") then
        isObjectInThingsPossession = true
    end
    if isObjectInThingsPossession then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetThingHasInformation(me, false, true, false)
    end
    if quest:IsActiveThreadTerminating() then return end
    repeat
        if not quest:GetStateBool("MotherIntroDone") then
            local hero2 = quest:GetHero()
            if quest:IsDistanceBetweenThingsUnder(me, hero2, 7.0) then
                if quest:IsActiveThreadTerminating() then return end
                local motherIntroMovie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                helpers.helper_ECE460(quest, me, "CS_SICK_INTRO", "NULL")
                local getActiveQuestName = quest:GetActiveQuestName()
                quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SICK_CHILD", getActiveQuestName, false)
                local pQuestName = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_SICK_CHILD_OBJECTIVE_01", "", "")
                quest:SetStateBool("MotherIntroDone", true)
                quest:ClearThingHasInformation(me)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(motherIntroMovie)
            end
        end
        if not quest:GetStateBool("MotherIntroDone") then
            goto LAB_00ecdd11
        else
            predicateResult7 = true
            if not me:IsTalkedToByHero() then goto LAB_00ecdd11 end
        end
        goto FLOW_past_lab_00ecdd11
        ::LAB_00ecdd11::
        predicateResult7 = false
        ::FLOW_past_lab_00ecdd11::
        if predicateResult7 then
            if quest:IsActiveThreadTerminating() then return end
            motherTalkMovie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local scratchValue4 = quest:ReadGlobalGameDataString(SCRIPT_DEF.EndPotion)
            hero5 = quest:GetHero()
            local isObjectInThingsPossession2 = quest:IsObjectInThingsPossession(scratchValue4, hero5)
            local predicateResult18 = quest:IsActiveThreadTerminating()
            if isObjectInThingsPossession2 then
                if predicateResult18 then
                    ReleaseEverything()
                    return
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_B10_MOTHER_CURE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                while questionAnswer < 0 do
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    else
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(motherTalkMovie)
                        do return end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(motherTalkMovie)
                    return
                end
                local predicateResult = quest:IsActiveThreadTerminating()
                if questionAnswer == 1 then
                    if predicateResult then quest:PauseAllNonScriptedEntities(false); resources:DestroyMovie(motherTalkMovie); return end
                    helpers.helper_ECE460(quest, me, "CS_SICK_OUTRO", "NULL")
                    local scratchValue5 = quest:ReadGlobalGameDataString(SCRIPT_DEF.EndPotion)
                    quest:TakeObjectFromHero(scratchValue5)
                    quest:ClearThingHasInformation(me)
                    quest:SetStateBool("FinishedQuest", true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(motherTalkMovie)
                    if quest:IsActiveThreadTerminating() then return end
                    local scratchValue3 = resources:MemberResource("seh_Mother")
                    resources:PrepareResource(scratchValue3)
                    repeat
                        quest:NewScriptFrame(me)
                    until quest:IsActiveThreadTerminating()
                    do return end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(motherTalkMovie)
                    return
                end
                if predicateResult then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(motherTalkMovie)
                    return
                end
                hero5 = nil
                if resources:ResourceThingHealthAbove(resources:MemberResource("seh_Mother"), 0.0) then
                    hero5 = quest:GetHero()
                    if not me:Speak(hero5, "TEXT_QST_B10_MOTHER_ASK_CURE", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(motherTalkMovie)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(motherTalkMovie)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(motherTalkMovie)
                        return
                    end
                end
            else
                if predicateResult18 then ReleaseEverything(); return end
                hero5 = nil
                if resources:ResourceThingHealthAbove(resources:MemberResource("seh_Mother"), 0.0) then
                    hero5 = quest:GetHero()
                    if not me:Speak(hero5, "TEXT_QST_B10_MOTHER_CURE_REMIND_10", GROUP_SELECT_FIRST, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(motherTalkMovie)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(motherTalkMovie)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(motherTalkMovie)
        end
        if not not (me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)))) then
            if quest:IsActiveThreadTerminating() then return end
            local motherHitMovie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            hero5 = nil
            if resources:ResourceThingHealthAbove(resources:MemberResource("seh_Mother"), 0.0) then
                hero5 = quest:GetHero()
                if not me:Speak(hero5, "TEXT_QST_B10_MOTHER_HIT", GROUP_SELECT_FIRST, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(motherHitMovie)
                    return
                end
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(motherHitMovie)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(motherHitMovie)
                    return
                end
            end
            local hero3 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(me, hero3)
            local hero4 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(hero4, me)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(motherHitMovie)
        end
        if not quest:NewScriptFrame(me) then return end
    until false
end

-- SickChildsMother.Init (retail 0x00ec5c90)
function Init(quest, me)
    quest:SetIsPushableByHero(me, false)
end

-- SickChildsMother.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- SickChildsMother.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

