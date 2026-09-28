-- Readable native conversion: Witch. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local helpers = require("V_SickChild.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local self0X14, thingsForPotionGot_, introDone

-- Witch.Main (retail 0x00ece7f0)
function Main(quest, me)
    local self_0x14
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue4, scratchValue6, ctr_94_2, thingsForPotionGot, questionAnswer, scratchValue14
    local scratchValue15, getHero, scratchValue19, scratchValue20, this_01, movie, movie4
    local scratchValue28, movie5
    local function ReleaseEverything()
        resources:DestroyMovie(movie)
    end
    local function ReleaseEverything2()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
    end
    local function ReleaseEverything3()
        local this_01 = scratchValue28 + 4
        resources:DestroyMovie(this_01)
    end
    if not quest:NewScriptFrame(me) then return end
    resources:PrepareResource(resources:MemberResource("seh_Witch"))
    while not resources:TryAcquire(resources:MemberResource("seh_Witch"), me, 4) do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    thingsForPotionGot = thingsForPotionGot_
    ctr_94_2 = 0
    while thingsForPotionGot < 4 do
        if not quest:NewScriptFrame(me) then return end
        if me:IsTalkedToByHero() then
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if not introDone then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                helpers.helper_ECE460(quest, me)
                quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_SICK_CHILD_OBJECTIVE_02", "", "")
                introDone = true
            else
                if quest:IsActiveThreadTerminating() then
                    -- TODO(native): quest:PauseAllNonScriptedEntities(*(this + 4))
                    resources:DestroyMovie(movie)
                    return
                end
                local predicateResult = quest:IsActiveThreadTerminating()
                if quest:IsObjectInThingsPossession(quest:ReadGlobalGameDataString(1848), hero) and thingsForPotionGot_ < 4 then
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
                            resources:DestroyMovie(movie)
                            do return end
                            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        -- TODO(native): quest:PauseAllNonScriptedEntities(*(this + 4))
                        resources:DestroyMovie(movie)
                        return
                    end
                    local predicateResult7 = quest:IsActiveThreadTerminating()
                    if questionAnswer == 1 then
                        if predicateResult7 then
                            ReleaseEverything2()
                            return
                        end
                        while true do
                            if not quest:IsObjectInThingsPossession(quest:ReadGlobalGameDataString(1848), hero) then break end
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                -- TODO(native): quest:PauseAllNonScriptedEntities(*(this + 4))
                                resources:DestroyMovie(movie)
                                return
                            end
                            quest:TakeObjectFromHero(quest:ReadGlobalGameDataString(1848))
                            thingsForPotionGot_ = thingsForPotionGot_ + 1
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            return
                        end
                        if 3 < thingsForPotionGot_ then
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                return
                            end
                            quest:ReadGlobalGameDataString(1860)
                            helpers.helper_ECE460(quest, me)
                            quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_SICK_CHILD_OBJECTIVE_03", "", "")
                            quest:ClearThingHasInformation(me)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            break
                        end
                        if quest:IsActiveThreadTerminating() then
                            -- TODO(native): quest:PauseAllNonScriptedEntities(*(this + 4))
                            resources:DestroyMovie(movie)
                            return
                        end
                        -- TODO(native): pCVar12 = (**(*(self_0x14 + 0x58) + 0x30))()
                        scratchValue15 = nil --[[unresolved native value]]
                        local fret_0 = quest:GetHealth(nil --[[missing]])
                        if 0.0 < fret_0 then
                            -- TODO(native): iVar10 = *(self_0x14 + 0x58)
                            scratchValue14 = nil --[[unresolved native value]]
                            -- TODO(native): (**(code **)(iVar10 + 0x34))(pCVar20,pcVar14,uVar16,uVar17);
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie)
                                    do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                -- TODO(native): quest:PauseAllNonScriptedEntities(*(this + 4))
                                resources:DestroyMovie(movie)
                                return
                            end
                        end
                    else
                        if predicateResult7 then
                            -- TODO(native): quest:PauseAllNonScriptedEntities(*(this + 4))
                            resources:DestroyMovie(movie)
                            return
                        end
                        -- TODO(native): pCVar12 = (**(*(self_0x14 + 0x58) + 0x30))()
                        scratchValue15 = nil --[[unresolved native value]]
                        if 0.0 < quest:GetHealth(1) then
                            -- TODO(native): iVar10 = *(self_0x14 + 0x58)
                            scratchValue14 = nil --[[unresolved native value]]
                            -- TODO(native): (**(code **)(iVar10 + 0x34))(pCVar20,pcVar14,uVar16,uVar17);
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
                            end
                            if quest:IsActiveThreadTerminating() then
                                -- TODO(native): quest:PauseAllNonScriptedEntities(*(this + 4))
                                resources:DestroyMovie(movie)
                                return
                            end
                        end
                    end
                else
                    if predicateResult then ReleaseEverything2(); return end
                    -- TODO(native): pCVar12 = (**(*(self_0x14 + 0x58) + 0x30))()
                    scratchValue15 = nil --[[unresolved native value]]
                    if 0.0 < quest:GetHealth(1) then
                        -- TODO(native): iVar10 = *(self_0x14 + 0x58)
                        scratchValue14 = nil --[[unresolved native value]]
                        -- TODO(native): (**(code **)(iVar10 + 0x34))(pCVar20,pcVar14,uVar16,uVar17);
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                    end
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        -- TODO(native): xStack_a0 = xStack_a0 | 2;
        if me:MsgIsHitByHero() then goto LAB_00ecef61 end
        -- TODO(native): bVar6 = (**(*me + 0xa8))(me,"SCRIPT_NAME_HERO")
        scratchValue4 = nil --[[unresolved native value]]
        if scratchValue4 then
            -- TODO(native): bVar6 = (**(*me + 0xa4))(me,0xe,"SCRIPT_NAME_HERO")
            local scratchValue5 = nil --[[unresolved native value]]
            if not scratchValue5 then goto LAB_00ecef61 end
        end
        goto FLOW_past_lab_00ecef61
        ::LAB_00ecef61::
        ::FLOW_past_lab_00ecef61::
        local scratchValue = ctr_94_2
        if quest:IsActiveThreadTerminating() then return end
        ctr_94_2 = ctr_94_2 + 1
        if scratchValue == nil then
            scratchValue19 = "TEXT_QST_B10_WITCH_ONHIT_10"
            goto LAB_00ecf00d
        else
            if scratchValue == 1 then
                scratchValue19 = "TEXT_QST_B10_WITCH_ONHIT_20"
                goto LAB_00ecf00d
            end
            if scratchValue == 2 then
                -- TODO(native): CCharString::operator=(&xStack_a0,"TEXT_QST_B10_WITCH_ONHIT_30");
                ctr_94_2 = 0
            else
                -- TODO(native): CCharString::operator=(&xStack_a0,"TEXT_QST_B10_WITCH_ONHIT_20");
                ctr_94_2 = 0
            end
        end
        goto FLOW_past_lab_00ecf00d
        ::LAB_00ecf00d::
        movie5 = scratchValue19
        ::FLOW_past_lab_00ecf00d::
        resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        -- TODO(native): pCVar20 = (**(*(self_0x14 + 0x58) + 0x30))()
        getHero = nil --[[unresolved native value]]
        local fret_02 = quest:GetHealth(nil --[[missing]])
        if 0.0 < fret_02 then
            -- TODO(native): xStack_58 = *(int *)(*(int *)(this + 0x14) + 0x58);
            -- TODO(native): (**(code **)((int)xStack_58 + 0x34))(pCVar20,pvVar11);
            while me:IsPerformingScriptTask() do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                return
            end
        end
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        thingsForPotionGot = thingsForPotionGot_
        ::continue_3::
    end
    if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
    repeat
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then return end
            resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            -- TODO(native): pCVar12 = (**(*(self_0x14 + 0x58) + 0x30))()
            scratchValue15 = nil --[[unresolved native value]]
            local fret_03 = quest:GetHealth(nil --[[missing]])
            if 0.0 < fret_03 then
                -- TODO(native): iVar10 = *(self_0x14 + 0x58)
                scratchValue14 = nil --[[unresolved native value]]
                -- TODO(native): (**(code **)(iVar10 + 0x34))(pCVar20,pcVar14,uVar16);
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    return
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        -- TODO(native): xStack_a0 = xStack_a0 | 0x10;
        if me:MsgIsHitByHero() then goto LAB_00ecf576 end
        -- TODO(native): bVar6 = (**(*me + 0xa8))(me,"SCRIPT_NAME_HERO")
        scratchValue6 = nil --[[unresolved native value]]
        if scratchValue6 then
            -- TODO(native): bVar6 = (**(*me + 0xa4))(me,0xe,"SCRIPT_NAME_HERO")
            local scratchValue7 = nil --[[unresolved native value]]
            if not scratchValue7 then goto LAB_00ecf576 end
        end
        goto FLOW_past_lab_00ecf576
        ::LAB_00ecf576::
        ::FLOW_past_lab_00ecf576::
        local scratchValue2 = ctr_94_2
        if quest:IsActiveThreadTerminating() then return end
        ctr_94_2 = ctr_94_2 + 1
        if scratchValue2 == nil then
            scratchValue20 = "TEXT_QST_B10_WITCH_ONHIT_10"
            goto LAB_00ecf622
        else
            if scratchValue2 == 1 then
                scratchValue20 = "TEXT_QST_B10_WITCH_ONHIT_20"
                goto LAB_00ecf622
            end
            if scratchValue2 == 2 then
                -- TODO(native): CCharString::operator=(&xStack_a0,"TEXT_QST_B10_WITCH_ONHIT_30");
                ctr_94_2 = 0
            else
                -- TODO(native): CCharString::operator=(&xStack_a0,"TEXT_QST_B10_WITCH_ONHIT_20");
                ctr_94_2 = 0
            end
        end
        goto FLOW_past_lab_00ecf622
        ::LAB_00ecf622::
        movie5 = scratchValue20
        ::FLOW_past_lab_00ecf622::
        resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        -- TODO(native): pCVar12 = (**(*(self_0x14 + 0x58) + 0x30))()
        scratchValue15 = nil --[[unresolved native value]]
        local fret_04 = quest:GetHealth(nil --[[missing]])
        if 0.0 < fret_04 then
            -- TODO(native): iVar10 = *(self_0x14 + 0x58)
            scratchValue14 = nil --[[unresolved native value]]
            -- TODO(native): (**(code **)(iVar10 + 0x34))(pCVar20,pvVar11,uVar16,uVar17);
            while me:IsPerformingScriptTask() do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    ReleaseEverything3(); do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                ReleaseEverything3()
                return
            end
        end
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie5)
        if not quest:NewScriptFrame(me) then return end
    until false
end

-- Witch.Init (retail 0x00ec80c0)
function Init(quest, me)
    introDone = false
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
    quest:SetStateBool("IntroDone", quest:PersistTransferBool(context, "IntroDone", quest:GetStateBool("IntroDone")))
    quest:SetStateInt("ThingsForPotionGot", quest:PersistTransferInt(context, "ThingsForPotionGot", quest:GetStateInt("ThingsForPotionGot") or 0))
end

-- Witch.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

