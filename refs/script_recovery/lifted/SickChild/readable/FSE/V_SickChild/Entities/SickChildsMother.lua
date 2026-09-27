-- Readable native conversion: SickChildsMother. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

local helpers = require("V_SickChild.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local self0X14

-- SickChildsMother.Main (retail 0x00ecd9e0)
function Main(quest, me)
    local self_0x14
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local isObjectInThingsPossession, predicateResult8, predicateResult16, questionAnswer
    local scratchValue8, scratchValue, hero6, movie3, movie4, movie5
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(resources:MemberResource("seh_Mother"))
        resources:DestroyMovie(movie4)
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
                helpers.helper_ECE460(quest, me)
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
            predicateResult8 = true
            if not me:IsTalkedToByHero() then goto LAB_00ecdd11 end
        end
        goto FLOW_past_lab_00ecdd11
        ::LAB_00ecdd11::
        predicateResult8 = false
        ::FLOW_past_lab_00ecdd11::
        if predicateResult8 then
            if quest:IsActiveThreadTerminating() then return end
            movie4 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x744),(int)xStack_70);
            local predicateResult = quest:IsActiveThreadTerminating()
            if quest:IsObjectInThingsPossession("", hero) then
                if predicateResult then
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
                        resources:DestroyMovie(movie4)
                        do return end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    return
                end
                local predicateResult11 = quest:IsActiveThreadTerminating()
                if questionAnswer == 1 then
                    if predicateResult11 then quest:PauseAllNonScriptedEntities(false); resources:DestroyMovie(movie4); return end
                    helpers.helper_ECE460(quest, me)
                    quest:TakeObjectFromHero(quest:ReadGlobalGameDataString(1860))
                    quest:ClearThingHasInformation(me)
                    quest:SetStateBool("FinishedQuest", true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    if quest:IsActiveThreadTerminating() then return end
                    resources:PrepareResource(resources:MemberResource("seh_Mother"))
                    repeat
                        quest:NewScriptFrame(me)
                    until quest:IsActiveThreadTerminating()
                    do return end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    return
                end
                if predicateResult11 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    return
                end
                -- TODO(native): pCVar11 = (**(*(self_0x14 + 0x48) + 0x30))()
                scratchValue = nil --[[unresolved native value]]
                local fret_0 = quest:GetHealth(nil --[[missing]])
                if 0.0 < fret_0 then
                    -- TODO(native): iVar13 = *(self_0x14 + 0x48)
                    scratchValue8 = nil --[[unresolved native value]]
                    -- TODO(native): (**(code **)(iVar13 + 0x34))(pCVar17,pcVar14,uVar15,uVar16);
                    -- TODO(native): cVar9 = (**(*(self_0x14 + 0x48) + 0x68))(pCVar17)
                    while nil --[[unresolved native value]] do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            return
                        end
                        -- TODO(native): cVar9 = (**(*(self_0x14 + 0x48) + 0x68))()
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        return
                    end
                end
            else
                if predicateResult then ReleaseEverything(); return end
                -- TODO(native): pCVar11 = (**(*(self_0x14 + 0x48) + 0x30))()
                scratchValue = nil --[[unresolved native value]]
                if 0.0 < quest:GetHealth(1) then
                    -- TODO(native): iVar13 = *(self_0x14 + 0x48)
                    scratchValue8 = nil --[[unresolved native value]]
                    -- TODO(native): (**(code **)(iVar13 + 0x34))(pCVar17,pcVar14,uVar15,uVar16);
                    -- TODO(native): cVar9 = (**(*(self_0x14 + 0x48) + 0x68))(pCVar17)
                    while nil --[[unresolved native value]] do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(resources:MemberResource("seh_Mother"))
                            resources:DestroyMovie(movie5)
                            return
                        end
                        -- TODO(native): cVar9 = (**(*(self_0x14 + 0x48) + 0x68))()
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie4)
        end
        if me:MsgIsHitByHero() then goto LAB_00ece11a end
        if me:MsgIsHitByAnySpecialAbilityFromHero() then
            if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00ece11a end
        end
        predicateResult16 = false
        goto FLOW_past_lab_00ece11a
        ::LAB_00ece11a::
        predicateResult16 = true
        ::FLOW_past_lab_00ece11a::
        if predicateResult16 then
            if quest:IsActiveThreadTerminating() then return end
            resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            -- TODO(native): pCVar17 = (**(*(self_0x14 + 0x48) + 0x30))()
            hero6 = nil --[[unresolved native value]]
            local fret_01 = quest:GetHealth(nil --[[missing]])
            if 0.0 < fret_01 then
                -- TODO(native): iVar13 = *(self_0x14 + 0x48)
                scratchValue8 = nil --[[unresolved native value]]
                -- TODO(native): (**(code **)(iVar13 + 0x34))(pCVar17,pcVar14,uVar15);
                -- TODO(native): cVar9 = (**(*(self_0x14 + 0x48) + 0x68))(pCVar17)
                while nil --[[unresolved native value]] do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie3)
                        return
                    end
                    -- TODO(native): cVar9 = (**(*(self_0x14 + 0x48) + 0x68))()
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                    return
                end
            end
            quest:EntitySetThingAsAllyOfThing(me, hero)
            quest:EntitySetThingAsAllyOfThing(hero, me)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
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

