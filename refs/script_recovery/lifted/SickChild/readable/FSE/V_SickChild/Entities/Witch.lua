-- Readable native conversion: Witch. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

local helpers = require("V_SickChild.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local self0X14, thingsForPotionGot_, introDone

-- Witch.Main (retail 0x00ece7f0)
function Main(quest, me)
    local self_0x14
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local thingsForPotionGot, questionAnswer, scratchValue9, scratchValue12, scratchValue20
    local scratchValue21, this_01, movie, movie4
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
    end
    local function ReleaseEverything2()
        local this_01 = movie4
        resources:DestroyMovie(this_01)
    end
    if not quest:NewScriptFrame(me) then return end
    resources:PrepareResource(resources:MemberResource("seh_Witch"))
    while not resources:TryAcquire(resources:MemberResource("seh_Witch"), me, 4) do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    thingsForPotionGot = thingsForPotionGot_
    scratchValue12 = 0
    while thingsForPotionGot < 4 do
        if not quest:NewScriptFrame(me) then return end
        if me:IsTalkedToByHero() then
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if not introDone then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                helpers.helper_ECE460(quest, me, "CS_SICK_WITCH", "NULL")
                quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_SICK_CHILD_OBJECTIVE_02", "", "")
                introDone = true
            else
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
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
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        return
                    end
                    local predicateResult7 = quest:IsActiveThreadTerminating()
                    if questionAnswer == 1 then
                        if predicateResult7 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            return
                        end
                        while true do
                            if not quest:IsObjectInThingsPossession(quest:ReadGlobalGameDataString(1848), hero) then break end
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
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
                            helpers.helper_ECE460(quest, me, "CS_SICK_POTION", quest:ReadGlobalGameDataString(1860))
                            quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_SICK_CHILD_OBJECTIVE_03", "", "")
                            quest:ClearThingHasInformation(me)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            break
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            return
                        end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resources:MemberResource("seh_Witch"))) then
                            -- TODO(native): iVar10 = *(self_0x14 + 0x58)
                            scratchValue9 = nil --[[unresolved native value]]
                            me:Speak(hero, "TEXT_QST_B10_WITCH_NEED_MORE_CURE_10", GROUP_SELECT_FIRST, false, true, false)
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
                    else
                        if predicateResult7 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            return
                        end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resources:MemberResource("seh_Witch"))) then
                            -- TODO(native): iVar10 = *(self_0x14 + 0x58)
                            scratchValue9 = nil --[[unresolved native value]]
                            me:Speak(hero, "TEXT_QST_B10_WITCH_NOT_GIVEN_MORE_MUSHROOMS_10", GROUP_SELECT_FIRST, false, true, false)
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
                    end
                else
                    if predicateResult then ReleaseEverything(); return end
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resources:MemberResource("seh_Witch"))) then
                        -- TODO(native): iVar10 = *(self_0x14 + 0x58)
                        scratchValue9 = nil --[[unresolved native value]]
                        me:Speak(hero, "TEXT_QST_B10_WITCH_WAITING_FOR_CURE", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    end
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        if me:MsgIsHitByHero() then goto LAB_00ecef61 end
        if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00ecef61 end
        goto FLOW_past_lab_00ecef61
        ::LAB_00ecef61::
        ::FLOW_past_lab_00ecef61::
        local scratchValue = scratchValue12
        if quest:IsActiveThreadTerminating() then return end
        scratchValue12 = scratchValue12 + 1
        if scratchValue == 0 then
            scratchValue20 = "TEXT_QST_B10_WITCH_ONHIT_10"
            goto LAB_00ecf00d
        else
            if scratchValue == 1 then
                scratchValue20 = "TEXT_QST_B10_WITCH_ONHIT_20"
                goto LAB_00ecf00d
            end
            if scratchValue == 2 then
                -- TODO(native): CCharString::operator=(xStack_a0,"TEXT_QST_B10_WITCH_ONHIT_30");
                scratchValue12 = 0
            else
                -- TODO(native): CCharString::operator=(xStack_a0,"TEXT_QST_B10_WITCH_ONHIT_20");
                scratchValue12 = 0
            end
        end
        goto FLOW_past_lab_00ecf00d
        ::LAB_00ecf00d::
        movie4 = scratchValue20
        ::FLOW_past_lab_00ecf00d::
        resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        if 0.0 < quest:GetHealth(resources:ScriptThing(resources:MemberResource("seh_Witch"))) then
            -- TODO(native): xStack_60 = *(int *)(*(int *)(this + 0x14) + 0x58);
            -- TODO(native): (**(code **)((int)xStack_60 + 0x34))(pCVar11,pvVar12,uVar15,uVar16);
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
        ::continue_4::
    end
    if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
    repeat
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then return end
            resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resources:MemberResource("seh_Witch"))) then
                -- TODO(native): iVar10 = *(self_0x14 + 0x58)
                scratchValue9 = nil --[[unresolved native value]]
                -- TODO(native): (**(code **)(iVar10 + 0x34))(pCVar11,pcVar14,uVar15,uVar16,uVar18);
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
        if me:MsgIsHitByHero() then goto LAB_00ecf576 end
        if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00ecf576 end
        -- TODO(native): uStack_a8 = uStack_a8 & 0xffffff | 0x10;
        goto FLOW_past_lab_00ecf576
        ::LAB_00ecf576::
        -- TODO(native): uStack_a8._0_3_ = (undefined3)(uStack_a8 | 0x10);
        ::FLOW_past_lab_00ecf576::
        local scratchValue11 = scratchValue12
        if quest:IsActiveThreadTerminating() then return end
        scratchValue12 = scratchValue12 + 1
        if scratchValue11 == 0 then
            scratchValue21 = "TEXT_QST_B10_WITCH_ONHIT_10"
            goto LAB_00ecf622
        else
            if scratchValue11 == 1 then
                scratchValue21 = "TEXT_QST_B10_WITCH_ONHIT_20"
                goto LAB_00ecf622
            end
            if scratchValue11 == 2 then
                movie4 = "TEXT_QST_B10_WITCH_ONHIT_30"
                scratchValue12 = 0
            else
                movie4 = "TEXT_QST_B10_WITCH_ONHIT_20"
                scratchValue12 = 0
            end
        end
        goto FLOW_past_lab_00ecf622
        ::LAB_00ecf622::
        movie4 = scratchValue21
        ::FLOW_past_lab_00ecf622::
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        if 0.0 < quest:GetHealth(resources:ScriptThing(resources:MemberResource("seh_Witch"))) then
            -- TODO(native): iVar10 = *(self_0x14 + 0x58)
            scratchValue9 = nil --[[unresolved native value]]
            -- TODO(native): (**(code **)(iVar10 + 0x34))(pCVar11,pvVar12,uVar15,uVar16);
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
        resources:DestroyMovie(movie4)
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
    introDone = quest:PersistTransferBool(context, "IntroDone", introDone)
    thingsForPotionGot_ = quest:PersistTransferInt(context, "ThingsForPotionGot", thingsForPotionGot_ or 0)
end

-- Witch.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

