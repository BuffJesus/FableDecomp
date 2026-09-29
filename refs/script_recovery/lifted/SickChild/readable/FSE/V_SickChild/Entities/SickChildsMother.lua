-- Readable native conversion: SickChildsMother. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST, GROUP_SELECT_RANDOM_NO_REPEAT = 0, 2  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

local helpers = require("V_SickChild.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local self0X14

-- SickChildsMother.Main (retail 0x00ecd9e0)
function Main(quest, me)
    local self_0x14
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local isObjectInThingsPossession, predicateResult7, questionAnswer, scratchValue, movie3, movie4
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie3)
    end
    if not quest:NewScriptFrame(me) then return end
    resources:PrepareResource(resources:MemberResource("seh_Mother"))
    while not resources:TryAcquire(resources:MemberResource("seh_Mother"), me, 4) do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    isObjectInThingsPossession = quest:IsObjectInThingsPossession(quest:ReadGlobalGameDataString(1860), hero)
    if isObjectInThingsPossession or not quest:GetStateBool("MotherIntroDone") then
        isObjectInThingsPossession = true
    end
    if isObjectInThingsPossession then
        quest:SetThingHasInformation(me, false, true, false)
    end
    if quest:IsActiveThreadTerminating() then return end
    repeat
        if not quest:GetStateBool("MotherIntroDone") then
            if quest:IsDistanceBetweenThingsUnder(me, hero, 7.0) then
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                helpers.helper_ECE460(quest, me, "CS_SICK_INTRO", "NULL")
                quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SICK_CHILD", quest:GetActiveQuestName(), false)
                quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_SICK_CHILD_OBJECTIVE_01", "", "")
                quest:SetStateBool("MotherIntroDone", true)
                quest:ClearThingHasInformation(me)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
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
            movie3 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local predicateResult18 = quest:IsActiveThreadTerminating()
            if quest:IsObjectInThingsPossession(quest:ReadGlobalGameDataString(1860), hero) then
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
                        resources:DestroyMovie(movie3)
                        do return end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                    return
                end
                local predicateResult = quest:IsActiveThreadTerminating()
                if questionAnswer == 1 then
                    if predicateResult then quest:PauseAllNonScriptedEntities(false); resources:DestroyMovie(movie3); return end
                    helpers.helper_ECE460(quest, me, "CS_SICK_OUTRO", "NULL")
                    quest:TakeObjectFromHero(quest:ReadGlobalGameDataString(1860))
                    quest:ClearThingHasInformation(me)
                    quest:SetStateBool("FinishedQuest", true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                    if quest:IsActiveThreadTerminating() then return end
                    resources:PrepareResource(resources:MemberResource("seh_Mother"))
                    repeat
                        quest:NewScriptFrame(me)
                    until quest:IsActiveThreadTerminating()
                    do return end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                    return
                end
                if predicateResult then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                    return
                end
                if 0.0 < quest:GetHealth(resources:ScriptThing(resources:MemberResource("seh_Mother"))) then
                    -- TODO(native): iVar14 = *(self_0x14 + 0x48)
                    scratchValue = nil --[[unresolved native value]]
                    me:Speak(hero, "TEXT_QST_B10_MOTHER_ASK_CURE", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie3)
                        return
                    end
                end
            else
                if predicateResult18 then ReleaseEverything(); return end
                if 0.0 < quest:GetHealth(resources:ScriptThing(resources:MemberResource("seh_Mother"))) then
                    -- TODO(native): iVar14 = *(self_0x14 + 0x48)
                    scratchValue = nil --[[unresolved native value]]
                    -- TODO(native): (**(code **)(iVar14 + 0x34))(pCVar13,pcVar15,uVar16,uVar17);
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
        end
        if not not (me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)))) then
            if quest:IsActiveThreadTerminating() then return end
            resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resources:MemberResource("seh_Mother"))) then
                -- TODO(native): iVar14 = *(self_0x14 + 0x48)
                scratchValue = nil --[[unresolved native value]]
                me:Speak(hero, "TEXT_QST_B10_MOTHER_HIT", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie("")
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie("")
                    return
                end
            end
            quest:EntitySetThingAsAllyOfThing(me, hero)
            quest:EntitySetThingAsAllyOfThing(hero, me)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie("")
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

