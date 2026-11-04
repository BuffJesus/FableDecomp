-- Readable native conversion: Witch. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST, GROUP_SELECT_RANDOM_NO_REPEAT = 0, 2  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    IngredientObject = 1848,  -- 'OBJECT_MUSHROOM_BLUE_01'
    EndPotion = 1860,  -- 'OBJECT_MUSHROOM_POTION_01'
}

local helpers = require("V_SickChild.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local thingsForPotionGot_, introDone_

-- Witch.Main (retail 0x00ece7f0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local thingsForPotionGot, questionAnswer, scratchValue, getHero, scratchValue5, scratchValue6
    local this_01, witchAfterCureHitMovie, witchBeforeCureHitMovie, witchHitDialogueIndex
    local witchTalkMovie, scratchValue11, scratchValue12
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(witchTalkMovie)
    end
    local function ReleaseEverything2()
        local this_01 = witchAfterCureHitMovie
        resources:DestroyMovie(this_01)
    end
    if not quest:NewScriptFrame(me) then return end
    local this_00 = resources:MemberResource("seh_Witch")
    resources:PrepareResource(this_00)
    while not resources:TryAcquire(resources:MemberResource("seh_Witch"), me, 4) do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    thingsForPotionGot = thingsForPotionGot_
    witchHitDialogueIndex = 0
    while thingsForPotionGot < 4 do
        if not quest:NewScriptFrame(me) then return end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then return end
            witchTalkMovie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if not introDone_ then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                helpers.helper_ECE460(quest, me, "CS_SICK_WITCH", "NULL")
                local getActiveQuestName = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(getActiveQuestName, "TEXT_QUEST_SICK_CHILD_OBJECTIVE_02", "", "")
                introDone_ = true
            else
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(witchTalkMovie)
                    return
                end
                local scratchValue7 = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject)
                getHero = quest:GetHero()
                local predicateResult4 = quest:IsObjectInThingsPossession(scratchValue7, getHero) and thingsForPotionGot_ < 4
                local predicateResult = quest:IsActiveThreadTerminating()
                if predicateResult4 then
                    if predicateResult then
                        ReleaseEverything()
                        return
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_B10_QUESTION_GIVE_MUSHROOM", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        quest:NewScriptFrame(me)
                        if not quest:IsActiveThreadTerminating() then
                            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        else
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(witchTalkMovie)
                            do return end
                            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(witchTalkMovie)
                        return
                    end
                    local predicateResult7 = quest:IsActiveThreadTerminating()
                    if questionAnswer == 1 then
                        if predicateResult7 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(witchTalkMovie)
                            return
                        end
                        while true do
                            local scratchValue8 = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject)
                            getHero = quest:GetHero()
                            if not quest:IsObjectInThingsPossession(scratchValue8, getHero) then break end
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(witchTalkMovie)
                                return
                            end
                            local scratchValue9 = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject)
                            quest:TakeObjectFromHero(scratchValue9)
                            thingsForPotionGot_ = (thingsForPotionGot_ + 1 + 2147483648) % 4294967296 - 2147483648
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(witchTalkMovie)
                            return
                        end
                        if 3 < thingsForPotionGot_ then
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(witchTalkMovie)
                                return
                            end
                            local stack0xffffff = quest:ReadGlobalGameDataString(SCRIPT_DEF.EndPotion)
                            helpers.helper_ECE460(quest, me, "CS_SICK_POTION", stack0xffffff)
                            local scratchValue3 = "TEXT_QUEST_SICK_CHILD_OBJECTIVE_03"
                            local getActiveQuestName2 = quest:GetActiveQuestName()
                            quest:SetQuestCardObjective(getActiveQuestName2, scratchValue3, "", "")
                            quest:ClearThingHasInformation(me)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(witchTalkMovie)
                            break
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(witchTalkMovie)
                            return
                        end
                        getHero = nil
                        if resources:ResourceThingHealthAbove(resources:MemberResource("seh_Witch"), 0.0) then
                            getHero = quest:GetHero()
                            if not me:Speak(getHero, "TEXT_QST_B10_WITCH_NEED_MORE_CURE_10", GROUP_SELECT_FIRST, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(witchTalkMovie)
                                return
                            end
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(witchTalkMovie)
                                    do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(witchTalkMovie)
                                return
                            end
                        end
                    else
                        if predicateResult7 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(witchTalkMovie)
                            return
                        end
                        getHero = nil
                        if resources:ResourceThingHealthAbove(resources:MemberResource("seh_Witch"), 0.0) then
                            getHero = quest:GetHero()
                            if not me:Speak(getHero, "TEXT_QST_B10_WITCH_NOT_GIVEN_MORE_MUSHROOMS_10", GROUP_SELECT_FIRST, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(witchTalkMovie)
                                return
                            end
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(witchTalkMovie)
                                    do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(witchTalkMovie)
                                return
                            end
                        end
                    end
                else
                    if predicateResult then ReleaseEverything(); return end
                    getHero = nil
                    if resources:ResourceThingHealthAbove(resources:MemberResource("seh_Witch"), 0.0) then
                        getHero = quest:GetHero()
                        if not me:Speak(getHero, "TEXT_QST_B10_WITCH_WAITING_FOR_CURE", GROUP_SELECT_FIRST, false, true, false) then
                            ReleaseEverything(); return
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    end
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(witchTalkMovie)
        end
        if (resources:IsHitByHeroExceptAbility(me, 14) and 1 or 0) == 0 then thingsForPotionGot = thingsForPotionGot_; goto continue_4 end
        scratchValue = witchHitDialogueIndex
        if quest:IsActiveThreadTerminating() then return end
        witchHitDialogueIndex = witchHitDialogueIndex + 1
        if scratchValue == 0 then
            scratchValue5 = "TEXT_QST_B10_WITCH_ONHIT_10"
            goto LAB_00ecf00d
        else
            if scratchValue == 1 then
                scratchValue5 = "TEXT_QST_B10_WITCH_ONHIT_20"
                goto LAB_00ecf00d
            end
            if scratchValue == 2 then
                scratchValue11 = "TEXT_QST_B10_WITCH_ONHIT_30"
                witchHitDialogueIndex = 0
            else
                scratchValue11 = "TEXT_QST_B10_WITCH_ONHIT_20"
                witchHitDialogueIndex = 0
            end
        end
        goto FLOW_past_lab_00ecf00d
        ::LAB_00ecf00d::
        scratchValue11 = scratchValue5
        ::FLOW_past_lab_00ecf00d::
        witchBeforeCureHitMovie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        getHero = nil
        if resources:ResourceThingHealthAbove(resources:MemberResource("seh_Witch"), 0.0) then
            -- reviewed native member speech table: xStack_60 = *(int *)(*(int *)(this + 0x14) + 0x58);
            local line = scratchValue11
            getHero = quest:GetHero()
            if not me:Speak(getHero, line, 0, false, true, false) then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(witchBeforeCureHitMovie)
                return
            end
            while me:IsPerformingScriptTask() do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(witchBeforeCureHitMovie)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(witchBeforeCureHitMovie)
                return
            end
        end
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(witchBeforeCureHitMovie)
        thingsForPotionGot = thingsForPotionGot_
        ::continue_4::
    end
    if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
    repeat
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then return end
            local witchCureMovie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if resources:ResourceThingHealthAbove(resources:MemberResource("seh_Witch"), 0.0) then
                getHero = quest:GetHero()
                if not me:Speak(getHero, "TEXT_QST_B10_WITCH_GOT_CURE", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(witchCureMovie)
                    return
                end
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(witchCureMovie)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(witchCureMovie)
                    return
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(witchCureMovie)
        end
        if (resources:IsHitByHeroExceptAbility(me, 14) and 1 or 0) ~= 0 then
            local scratchValue2 = witchHitDialogueIndex
            if quest:IsActiveThreadTerminating() then return end
            witchHitDialogueIndex = witchHitDialogueIndex + 1
            if scratchValue2 == 0 then
                scratchValue6 = "TEXT_QST_B10_WITCH_ONHIT_10"
                goto LAB_00ecf622
            else
                if scratchValue2 == 1 then
                    scratchValue6 = "TEXT_QST_B10_WITCH_ONHIT_20"
                    goto LAB_00ecf622
                end
                if scratchValue2 == 2 then
                    scratchValue12 = "TEXT_QST_B10_WITCH_ONHIT_30"
                    witchHitDialogueIndex = 0
                else
                    scratchValue12 = "TEXT_QST_B10_WITCH_ONHIT_20"
                    witchHitDialogueIndex = 0
                end
            end
            goto FLOW_past_lab_00ecf622
            ::LAB_00ecf622::
            scratchValue12 = scratchValue6
            ::FLOW_past_lab_00ecf622::
            witchAfterCureHitMovie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            getHero = nil
            if resources:ResourceThingHealthAbove(resources:MemberResource("seh_Witch"), 0.0) then
                local line2 = scratchValue12
                local hero = quest:GetHero()
                if not me:Speak(hero, line2, 0, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    ReleaseEverything2(); return
                end
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        ReleaseEverything2(); do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    ReleaseEverything2()
                    return
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(witchAfterCureHitMovie)
        end
        if not quest:NewScriptFrame(me) then return end
    until false
end

-- Witch.Init (retail 0x00ec80c0)
function Init(quest, me)
    introDone_ = false
    thingsForPotionGot_ = 0
    if not quest:GetStateBool("MotherIntroDone") then
        quest:RemoveThing(me, false, true)
        return
    end
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
end

-- Witch.OnPersist (retail 0x00ecd890)
function OnPersist(quest, me, context)
    local introDone = introDone_ or false
    introDone_ = quest:PersistTransferBool(context, "IntroDone", introDone)
    local thingsForPotionGot = thingsForPotionGot_ or 0
    thingsForPotionGot_ = quest:PersistTransferInt(context, "ThingsForPotionGot", thingsForPotionGot)
end

-- Witch.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

