-- Readable native conversion: BB_BeardyBaldyMan. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST, GROUP_SELECT_RANDOM_NO_REPEAT = 0, 2  -- ETextGroupSelectionMethod

local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

local helpers = require("V_BeardyBaldy.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local lastRandomSpeechIdx1, lastRandomSpeechIdx2

-- BB_BeardyBaldyMan.Main (retail 0x00e50fd0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, predicateResult2, scratchValue3, p0, this_00
    local resource = resources:NewResource()
    if this_00 ~= nil then
        -- TODO(native): xStack_1c8._4_4_ = *(undefined4 *)(this + 0xc);
        if nil ~= nil then
            -- TODO(native): *xStack_1c8 = *xStack_1c8 + 1;
        end
        -- TODO(native): CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript> (this_00,pCVar9,0);
        -- TODO(native): *(code **)(this_00 + 0x34) = NScript::CV_BeardyBaldyScript::WatchForAttack;
        -- TODO(native): *(undefined4 *)(this_00 + 0x38) = uVar1;
    end
    -- TODO(native): CGuiVarTransferStruct::Add(*(CGuiVarTransferStruct **)(this + 0x14),this_00,sectionName);
    scratchValue = quest:IsActiveThreadTerminating()
    repeat
        if scratchValue then
            resources:ReleaseResource(resource)
            return
        end
        if not quest:GetStateBool("AttackedByHero") then
            if quest:GetTimer(quest:GetStateInt("RandomSpeechTimer")) == 0 then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local scratchValue47 = GetRandomSpeech(quest, me)
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                quest:EntitySetFacingAngleTowardsThing(hero, me, false)
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                local addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, scratchValue47, me, hero, false)
                helpers.ResetRandomSpeechTime(quest, me)
                goto LAB_00e51417
                resources:ReleaseResource(resource)
                return
            end
        else
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:ReleaseResource(resource)
                return
            end
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                me:Speak(hero, "TEXT_QST_014_ATTACK", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        resources:ReleaseResource(resource)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(resource)
                    return
                end
            end
            resources:PrepareResource(resource)
            quest:SetStateBool("AttackedByHero", false)
            helpers.ResetRandomSpeechTime(quest, me)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        ::LAB_00e51417::
        scratchValue3 = quest:GetStateInt("QuestPhase")
        if scratchValue3 == 0 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            if quest:GetStateBool("InitialisePhase") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                -- TODO(native): SetWanderPointAndDistance(quest, me, *(this + 0x14), me, iVar13)
                quest:SetStateBool("InitialisePhase", false)
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local movie4 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00e53600 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e53600 end
                if quest:GetStateBool("Phase1RequirementsComplete") then
                    if not quest:IsActiveThreadTerminating() then
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_014_PHASE1_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false) then goto LAB_00e53600 end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e53600 end
                        end
                        goto LAB_00e51c43
                    end
                    goto LAB_00e53600
                end
                if not quest:IsActiveThreadTerminating() then
                    if not quest:GetStateBool("IntroComplete") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e53600 end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_014_PHASE1_INTRO1", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e53600 end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e53600 end
                        end
                        quest:SetStateBool("IntroComplete", true)
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00e53600 end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_014_PHASE1_INTRO_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false) then goto LAB_00e53600 end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e53600 end
                        end
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_014_QUEST_ON_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    scratchValue3 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while scratchValue3 < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00e53600 end
                        scratchValue3 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        helpers.ResetRandomSpeechTime(quest, me)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue3 == 1 then
                            if scratchValue then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                resources:ReleaseResource(resource)
                                return
                            end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_014_PHASE1_INTRO2", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e53600 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e53600 end
                            end
                            predicateResult2 = quest:IsActiveThreadTerminating()
                            if quest:IsWearingHairstyle(hero, quest:ReadGlobalGameDataString(1104)) then
                                if predicateResult2 then goto LAB_00e53600 end
                                quest:GiveHeroObject(quest:ReadGlobalGameDataString(1116), -1, false)
                                quest:SetStateString("RequiredHairdo", quest:ReadGlobalGameDataString(1112))
                            else
                                if predicateResult2 then goto LAB_00e53600 end
                                quest:GiveHeroObject(quest:ReadGlobalGameDataString(1108), -1, false)
                                quest:SetStateString("RequiredHairdo", quest:ReadGlobalGameDataString(1104))
                            end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_014_PHASE1_INTRO3", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e53600 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e53600 end
                            end
                            quest:SetStateBool("Phase1RequirementsComplete", true)
                        else
                            if scratchValue then goto LAB_00e53600 end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_014_PHASE1_DENIED", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false) then goto LAB_00e53600 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e53600 end
                            end
                        end
                        goto LAB_00e51c43
                    end
                end
                goto FLOW_past_lab_00e51c43
                ::LAB_00e51c43::
                resources:PrepareResource(resource)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                goto LAB_00e51c6f
                ::FLOW_past_lab_00e51c43::
                ::LAB_00e53600::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                resources:ReleaseResource(resource)
                return
            end
            ::LAB_00e51c6f::
            if not quest:GetStateBool("Phase1RequirementsComplete") then quest:NewScriptFrame(me); scratchValue = quest:IsActiveThreadTerminating(); goto continue_3 end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            quest:SetStateInt("QuestPhase", 1)
            quest:SetStateBool("InitialisePhase", true)
        elseif scratchValue3 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            if quest:GetStateBool("InitialisePhase") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                -- TODO(native): SetWanderPointAndDistance(quest, me, *(this + 0x14), me, iVar13)
                quest:SetStateBool("InitialisePhase", false)
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie3)
                        resources:ReleaseResource(resource)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                    resources:ReleaseResource(resource)
                    return
                end
                if not quest:GetStateBool("Phase2RequirementsComplete") then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie3)
                        resources:ReleaseResource(resource)
                        return
                    end
                    if quest:IsWearingHairstyle(hero, quest:GetStateString("RequiredHairdo")) then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            resources:ReleaseResource(resource)
                            return
                        end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            me:Speak(hero, "TEXT_QST_014_PHASE2_INTRO1", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    resources:ReleaseResource(resource)
                                    do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                resources:ReleaseResource(resource)
                                return
                            end
                        end
                        predicateResult2 = quest:IsActiveThreadTerminating()
                        if quest:IsWearingHairstyle(hero, quest:ReadGlobalGameDataString(1120)) then
                            if predicateResult2 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                resources:ReleaseResource(resource)
                                return
                            end
                            quest:GiveHeroObject(quest:ReadGlobalGameDataString(1132), -1, false)
                            quest:ReadGlobalGameDataString(1128)
                            -- TODO(native): quest:SetStateString("RequiredBeard", &xStack_148)
                        else
                            if predicateResult2 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                resources:ReleaseResource(resource)
                                return
                            end
                            quest:GiveHeroObject(quest:ReadGlobalGameDataString(1124), -1, false)
                            quest:ReadGlobalGameDataString(1120)
                            -- TODO(native): quest:SetStateString("RequiredBeard", &xStack_138)
                        end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            me:Speak(hero, "TEXT_QST_014_PHASE2_INTRO2", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    resources:ReleaseResource(resource)
                                    do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                resources:ReleaseResource(resource)
                                return
                            end
                        end
                        quest:SetStateBool("Phase2RequirementsComplete", true)
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            resources:ReleaseResource(resource)
                            return
                        end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            me:Speak(hero, "TEXT_QST_014_PHASE2_WRONG_HAIR", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    resources:ReleaseResource(resource)
                                    do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                resources:ReleaseResource(resource)
                                return
                            end
                        end
                    end
                else
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie3)
                        resources:ReleaseResource(resource)
                        return
                    end
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_014_PHASE2_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                resources:ReleaseResource(resource)
                                do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            resources:ReleaseResource(resource)
                            return
                        end
                    end
                end
                resources:PrepareResource(resource)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
            end
            if quest:GetStateBool("Phase2RequirementsComplete") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                quest:SetStateInt("QuestPhase", 2)
                quest:SetStateBool("InitialisePhase", true)
            end
        elseif scratchValue3 == 2 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            if quest:GetStateBool("InitialisePhase") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                -- TODO(native): SetWanderPointAndDistance(quest, me, *(this + 0x14), me, iVar13)
                quest:SetStateBool("InitialisePhase", false)
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local movie5 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00e5276b end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e5276b end
                if not quest:GetStateBool("Phase3RequirementsComplete") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5276b end
                    -- TODO(native): bVar6 = IsHeroWearingBeard(quest, me, *(this + 0x14))
                    scratchValue = nil --[[unresolved native value]]
                    if not scratchValue then
                        if not quest:IsActiveThreadTerminating() then
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_014_PHASE3_NO_BEARD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false) then goto LAB_00e5276b end
                                goto LAB_00e52d54
                            end
                            goto LAB_00e52d63
                        end
                        goto LAB_00e5276b
                    end
                    if not quest:IsWearingHairstyle(hero, quest:GetStateString("RequiredHairdo")) then
                        goto LAB_00e52a37
                    end
                    goto FLOW_past_lab_00e52a37
                    ::LAB_00e52a37::
                    if quest:IsWearingHairstyle(hero, quest:GetStateString("RequiredHairdo")) then
                        if not quest:IsWearingHairstyle(hero, quest:GetStateString("RequiredBeard")) then
                            if not quest:IsActiveThreadTerminating() then
                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                    if not me:Speak(hero, "TEXT_QST_014_PHASE3_WRONG_BEARD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false) then goto LAB_00e5276b end
                                    goto LAB_00e52d54
                                end
                                goto LAB_00e52d63
                            end
                            goto LAB_00e5276b
                        end
                    end
                    if quest:IsWearingHairstyle(hero, quest:GetStateString("RequiredHairdo")) then
                        goto LAB_00e52c5e
                    else
                        if not quest:IsWearingHairstyle(hero, quest:GetStateString("RequiredBeard")) then goto LAB_00e52c5e end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie5)
                            resources:ReleaseResource(resource)
                            return
                        end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_014_PHASE3_WRONG_HAIR", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false) then goto LAB_00e5276b end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e5276b end
                        end
                    end
                    goto FLOW_past_lab_00e52c5e
                    ::LAB_00e52c5e::
                    if not quest:IsWearingHairstyle(hero, quest:GetStateString("RequiredHairdo")) then
                        if not quest:IsWearingHairstyle(hero, quest:GetStateString("RequiredBeard")) then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e5276b end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_014_PHASE3_WRONG_HAIR_AND_BEARD", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false) then goto LAB_00e5276b end
                                goto LAB_00e52d54
                            end
                        end
                    end
                    ::FLOW_past_lab_00e52c5e::
                    goto LAB_00e52d63
                    ::FLOW_past_lab_00e52a37::
                    goto FLOW_past_lab_00e52d63
                    ::LAB_00e52d63::
                    resources:PrepareResource(resource)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    goto LAB_00e52d8f
                    ::FLOW_past_lab_00e52d63::
                    if not quest:IsWearingHairstyle(hero, quest:GetStateString("RequiredBeard")) then goto LAB_00e52a37 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5276b end
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        if not me:Speak(hero, "TEXT_QST_014_PHASE3_INTRO1", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5276b end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5276b end
                    end
                    if not helpers.IsHeroWearingAnyTash(quest, me) then
                        if not quest:IsActiveThreadTerminating() then
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_014_PHASE3_INTRO3", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5276b end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e5276b end
                            end
                            goto LAB_00e52838
                        end
                        goto LAB_00e5276b
                    end
                    if not quest:IsActiveThreadTerminating() then
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_014_PHASE3_INTRO2", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5276b end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e5276b end
                        end
                        goto LAB_00e52838
                    end
                    goto FLOW_past_lab_00e52838
                    ::LAB_00e52838::
                    predicateResult2 = quest:IsActiveThreadTerminating()
                    if quest:IsWearingHairstyle(hero, quest:ReadGlobalGameDataString(1136)) then
                        if predicateResult2 then goto LAB_00e5276b end
                        quest:GiveHeroObject(quest:ReadGlobalGameDataString(1148), -1, false)
                        quest:ReadGlobalGameDataString(1144)
                        -- TODO(native): quest:SetStateString("RequiredTash", &xStack_184)
                    else
                        if predicateResult2 then goto LAB_00e5276b end
                        quest:GiveHeroObject(quest:ReadGlobalGameDataString(1140), -1, false)
                        quest:ReadGlobalGameDataString(1136)
                        -- TODO(native): quest:SetStateString("RequiredTash", &xStack_158)
                    end
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        if not me:Speak(hero, "TEXT_QST_014_PHASE3_INTRO4", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5276b end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5276b end
                    end
                    quest:SetStateBool("Phase3RequirementsComplete", true)
                    goto LAB_00e52d63
                    ::FLOW_past_lab_00e52838::
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5276b end
                    if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then
                        resources:PrepareResource(resource)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie5)
                        goto LAB_00e52d8f
                    end
                    if not me:Speak(hero, "TEXT_QST_014_PHASE3_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false) then goto LAB_00e5276b end
                    goto LAB_00e52d54
                end
                goto FLOW_past_lab_00e52d54
                ::LAB_00e52d54::
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e52d54 end
                resources:PrepareResource(resource)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie5)
                goto LAB_00e52d8f
                ::FLOW_past_lab_00e52d54::
                ::LAB_00e5276b::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie5)
                resources:ReleaseResource(resource)
                return
            end
            ::LAB_00e52d8f::
            if quest:GetStateBool("Phase3RequirementsComplete") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                quest:SetStateInt("QuestPhase", 3)
                quest:SetStateBool("InitialisePhase", true)
            end
        elseif scratchValue3 == 3 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            if quest:GetStateBool("InitialisePhase") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                -- TODO(native): SetWanderPointAndDistance(quest, me, *(this + 0x14), me, iVar13)
                quest:SetStateBool("InitialisePhase", false)
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        resources:ReleaseResource(resource)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    resources:ReleaseResource(resource)
                    return
                end
                if quest:GetStateBool("AllHairChanged") then
                    -- TODO(native): bVar6 = IsHeroWearingBeard(quest, me, *(this + 0x14))
                    scratchValue = nil --[[unresolved native value]]
                    local sequence1 = scratchValue and helpers.IsHeroWearingAnyTash(quest, me) and helpers.IsHeroWearingAnyOddHairdo(quest, me)
                    if sequence1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e53630 end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            me:Speak(hero, "TEXT_QST_014_END_FINALE1", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    resources:ReleaseResource(resource)
                                    do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e53630 end
                        end
                        quest:GiveHeroObject(quest:ReadGlobalGameDataString(1152), -1, false)
                        quest:SetStateBool("QuestComplete", true)
                        goto LAB_00e532e3
                    else
                        if 3 < quest:GetStateInt("IncorrectHairComboCount") and quest:GetStateBool("AllHairChanged") then
                            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); resources:DestroyMovie(movie2); resources:ReleaseResource(resource); return end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_014_END_FINALE2", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e53630 end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                            end
                            quest:GiveHeroObject(quest:ReadGlobalGameDataString(1156), -1, false)
                            quest:SetStateBool("QuestComplete", true)
                            goto LAB_00e532e3
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            resources:ReleaseResource(resource)
                            return
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e53630 end
                        quest:SetStateInt("IncorrectHairComboCount", quest:GetStateInt("IncorrectHairComboCount") + 1)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            me:Speak(hero, "TEXT_QST_014_END_WRONG", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    resources:ReleaseResource(resource)
                                    do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e53630 end
                        end
                        goto LAB_00e532e3
                    end
                    ::LAB_00e53630::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    resources:ReleaseResource(resource)
                    return
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    resources:ReleaseResource(resource)
                    return
                end
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    me:Speak(hero, "TEXT_QST_014_END_NO_TASH", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                ::LAB_00e532e3::
                resources:PrepareResource(resource)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
            end
            if quest:GetStateBool("QuestComplete") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                quest:GiveHeroExperience(quest:ReadGlobalGameData(1160))
                quest:ClearThingHasInformation(me)
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        resources:ReleaseResource(resource)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local exitPoint = quest:GetNearestWithScriptName(me, "M_BB_ExitPoint")
                if exitPoint ~= nil and exitPoint:IsAlive() then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e53647 end
                    if not resources:ScriptThing(resource):IsNull() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e53647 end
                        if not (exitPoint ~= nil and not exitPoint:IsNull()) then
                            p0 = {x = 0, y = 0, z = 0}
                        else
                            p0 = exitPoint:GetPos()
                        end
                        me:MoveToPosition(p0, 1.0, ENTITY_MOVE_RUN, false, true)
                    end
                    scratchValue = quest:IsDistanceBetweenThingsUnder(me, exitPoint, 2.0)
                    while true do
                        local predicateResult = not scratchValue and not quest:MsgOnRegionLoaded()
                        if not predicateResult then break end
                        if not quest:NewScriptFrame(me) then goto LAB_00e53647 end
                        scratchValue = quest:IsDistanceBetweenThingsUnder(me, exitPoint, 2.0)
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e53647 end
                    quest:FadeOutAndKillEntity(me, false, 1.0, true)
                    quest:SetStateBool("BeardyBaldyLeft", true)
                    goto LAB_00e534c2
                    ::LAB_00e53647::
                    resources:ReleaseResource(resource)
                    return
                end
                ::LAB_00e534c2::
            end
        end
        quest:NewScriptFrame(me)
        scratchValue = quest:IsActiveThreadTerminating()
        ::continue_3::
    until false
end

-- BB_BeardyBaldyMan.Init (retail 0x00e50890)
function Init(quest, me)
    quest:SetThingHasInformation(me, true, true, false)
    quest:SetThingPersistent(me, true)
    lastRandomSpeechIdx1 = 10
    lastRandomSpeechIdx2 = 10
end

-- BB_BeardyBaldyMan.OnPersist (retail 0x00e508d0)
function OnPersist(quest, me, context)
end

-- BB_BeardyBaldyMan.OnPredicateFail (retail 0x00e508e0)
function OnPredicateFail(quest, me)
end

-- BB_BeardyBaldyMan.IsHeroWearingBeard (retail 0x00e53670)
-- E53670: bsim names this body NScript::CV_BordelloScript::IsHeroWearingBeard (a homologous script member); no PDB name
function IsHeroWearingBeard(quest, me)
    local flags, predicateResult
    local hero = quest:GetHero()
    flags = 1
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_01") then
        flags = 3
        if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_02") then
            flags = 7
            if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_03") then
                flags = 15
                if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_MUTTON_01") then
                    flags = 31
                    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_LONG_01") then
                        flags = 63
                        if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_CHIN_01") then
                            flags = 127
                            if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_TRAMP_01") then
                                flags = 255
                                predicateResult = false
                                if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_WATSON_01") then goto LAB_00e53856 end
                            end
                        end
                    end
                end
            end
        end
    end
    predicateResult = true
    ::LAB_00e53856::
    if flags < 0 then
        flags = flags & 127
    end
    if flags & 64 ~= 0 then
        flags = flags & 191
    end
    if flags & 32 ~= 0 then
        flags = flags & 223
    end
    if flags & 16 ~= 0 then
        flags = flags & 239
    end
    if flags & 8 ~= 0 then
        flags = flags & 247
    end
    if flags & 4 ~= 0 then
        flags = flags & 251
    end
    return predicateResult
end

-- BB_BeardyBaldyMan.GetRandomSpeech (retail 0x00e53cb0)
-- E53CB0: bsim names this body NScript::CV_ArcheryCompetitionScript::CAC_Owner::GetRandomSpeech (a homologous script member); no PDB name
function GetRandomSpeech(quest, me)
    local predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            return ""
        end
        ::FLOW_after_lab_00e53d52::
        local scratchValue = math.random(0, 32767) % 10
        if not ((scratchValue ~= lastRandomSpeechIdx1) and (scratchValue ~= lastRandomSpeechIdx2)) then quest:NewScriptFrame(me); predicateResult = quest:IsActiveThreadTerminating(); goto continue_1 end
        if quest:IsActiveThreadTerminating() then do return "" end; goto FLOW_after_lab_00e53d52 end
        lastRandomSpeechIdx2 = lastRandomSpeechIdx1
        lastRandomSpeechIdx1 = scratchValue
        if not quest:IsActiveThreadTerminating() then do return quest:GetStateString("RandomSpeech_" .. scratchValue) end; goto FLOW_after_lab_00e53d52 end
        do return "" end
        goto FLOW_after_lab_00e53d52
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
        ::continue_1::
    until false
end

-- BB_BeardyBaldyMan.SetWanderPointAndDistance (retail 0x00e53f60)
-- E53F60: bsim names this body NScript::CV_BeardyBaldyScript::SetWanderPointAndDistance (a homologous script member); no PDB name
function SetWanderPointAndDistance(quest, me, param1, param2)
    -- TODO(native): local center = *native_arg_param_2
    quest:SetWanderCentrePoint(nil --[[missing]], nil --[[missing]])
    quest:ReadGlobalGameDataFloat(1096)
    quest:SetWanderMinDistance(nil --[[missing]], param1)
    quest:ReadGlobalGameDataFloat(1100)
    quest:SetWanderMaxDistance(nil --[[missing]], param1)
    quest:SetScriptingStateGroup(nil --[[missing]], param1)
end

