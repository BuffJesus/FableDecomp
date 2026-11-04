-- Readable native conversion: BB_BeardyBaldyMan. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    BeardyBaldyHaircutStyle1 = 1104,  -- 'OBJECT_HERO_HAIR_BASIN_01'
    BeardyBaldyHaircutCard1 = 1108,  -- 'OBJECT_HAIRCUT_CARD_MOP'
    BeardyBaldyHaircutStyle2 = 1112,  -- 'OBJECT_HERO_HAIR_MOHAWK_01'
    BeardyBaldyHaircutCard2 = 1116,  -- 'OBJECT_HAIRCUT_CARD_07'
    BeardyBaldyBeardStyle1 = 1120,  -- 'OBJECT_HERO_BEARD_MUTTON_01'
    BeardyBaldyBeardCard1 = 1124,  -- 'OBJECT_BEARD_CARD_04'
    BeardyBaldyBeardStyle2 = 1128,  -- 'OBJECT_HERO_BEARD_LONG_01'
    BeardyBaldyBeardCard2 = 1132,  -- 'OBJECT_BEARD_CARD_05'
    BeardyBaldyTashStyle1 = 1136,  -- 'OBJECT_HERO_TASHTRADER_01'
    BeardyBaldyTashCard1 = 1140,  -- 'OBJECT_TASH_CARD_02'
    BeardyBaldyTashStyle2 = 1144,  -- 'OBJECT_HERO_TASHSHERIFF_01'
    BeardyBaldyTashCard2 = 1148,  -- 'OBJECT_TASH_CARD_04'
    BeardyBaldyRewardObjectFullComplete = 1152,  -- 'OBJECT_TATTOO_CARD_BS_BACK_01'
    BeardyBaldyRewardObjectHalfComplete = 1156,  -- 'OBJECT_BEARD_CARD_08'
    BeardyBaldyExperience = 1160,  -- 100
}

local helpers = require("V_BeardyBaldy.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local lastRandomSpeechIdx1, lastRandomSpeechIdx2

-- BB_BeardyBaldyMan.Main (retail 0x00e50fd0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue, predicateResult4, questionAnswer, addNewConversation, p0, hero, hero2
    local getStateString, line, scratchValue22
    local resource = resources:NewResource()
    quest:CreateThread("WatchForAttack", {args = {me}})  -- native parent-quest worker WatchForAttack, the entity's own thing captured
    scratchValue = quest:IsActiveThreadTerminating()
    repeat
        if scratchValue then
            resources:ReleaseResource(resource)
            return
        end
        if not quest:GetStateBool("AttackedByHero") then
            if quest:GetTimer(quest:GetStateInt("RandomSpeechTimer")) == 0 then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local scratchValue25 = GetRandomSpeech(quest, me)
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                scratchValue = false
                local pThing = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(pThing, me, false)
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
                addNewConversation = quest:AddNewConversation(me, false, false)
                local hero3 = quest:GetHero()
                quest:AddPersonToConversation(addNewConversation, hero3)
                hero2 = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, scratchValue25, me, hero2, false)
                helpers.ResetRandomSpeechTime(quest, me)
                goto LAB_00e51417
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
            hero = nil
            if resources:ResourceThingHealthAbove(resource, 0.0) then
                line = "TEXT_QST_014_ATTACK"
                hero = quest:GetHero()
                if not me:Speak(hero, line, 2, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(resource)
                    return
                end
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
        local questPhase = quest:GetStateInt("QuestPhase")
        if questPhase == 0 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            if quest:GetStateBool("InitialisePhase") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local position = me:GetPos()
                helpers.SetWanderPointAndDistance(quest, me, me, position)
                quest:SetStateBool("InitialisePhase", false)
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local movie4 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e51c43 end
                end
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e51c43 end
                if quest:GetStateBool("Phase1RequirementsComplete") then
                    if not quest:IsActiveThreadTerminating() then
                        hero = nil
                        if resources:ResourceThingHealthAbove(resource, 0.0) then
                            line = "TEXT_QST_014_PHASE1_REMINDER"
                            hero = quest:GetHero()
                            if not me:Speak(hero, line, 2, false, true, false) then
                                goto FLOW_past_lab_00e51c43
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e51c43 end
                            end
                            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e51c43 end
                        end
                        goto LAB_00e51c43
                    end
                    goto FLOW_past_lab_00e51c43
                end
                if not quest:IsActiveThreadTerminating() then
                    if not quest:GetStateBool("IntroComplete") then
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e51c43 end
                        hero = nil
                        if resources:ResourceThingHealthAbove(resource, 0.0) then
                            line = "TEXT_QST_014_PHASE1_INTRO1"
                            hero = quest:GetHero()
                            if not me:Speak(hero, line, 0, false, true, false) then
                                goto FLOW_past_lab_00e51c43
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e51c43 end
                            end
                            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e51c43 end
                        end
                        quest:SetStateBool("IntroComplete", true)
                    else
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e51c43 end
                        hero = nil
                        if resources:ResourceThingHealthAbove(resource, 0.0) then
                            line = "TEXT_QST_014_PHASE1_INTRO_REMINDER"
                            hero = quest:GetHero()
                            if not me:Speak(hero, line, 2, false, true, false) then
                                goto FLOW_past_lab_00e51c43
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e51c43 end
                            end
                            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e51c43 end
                        end
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_014_QUEST_ON_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e51c43 end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        helpers.ResetRandomSpeechTime(quest, me)
                        scratchValue = quest:IsActiveThreadTerminating()
                        if questionAnswer == 1 then
                            if scratchValue then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                resources:ReleaseResource(resource)
                                return
                            end
                            hero = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                line = "TEXT_QST_014_PHASE1_INTRO2"
                                hero = quest:GetHero()
                                if not me:Speak(hero, line, 0, false, true, false) then
                                    goto FLOW_past_lab_00e51c43
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e51c43 end
                                end
                                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e51c43 end
                            end
                            local scratchValue18 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyHaircutStyle1)
                            hero = quest:GetHero()
                            scratchValue = quest:IsWearingHairstyle(hero, scratchValue18)
                            local predicateResult2 = quest:IsActiveThreadTerminating()
                            if scratchValue then
                                if predicateResult2 then goto FLOW_past_lab_00e51c43 end
                                local scratchValue9 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyHaircutCard2)
                                quest:GiveHeroObject(scratchValue9, -1, false)
                                local scratchValue8 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyHaircutStyle2)
                                quest:SetStateString("RequiredHairdo", scratchValue8)
                            else
                                if predicateResult2 then goto FLOW_past_lab_00e51c43 end
                                local scratchValue16 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyHaircutCard1)
                                quest:GiveHeroObject(scratchValue16, -1, false)
                                local scratchValue14 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyHaircutStyle1)
                                quest:SetStateString("RequiredHairdo", scratchValue14)
                            end
                            hero = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                line = "TEXT_QST_014_PHASE1_INTRO3"
                                hero = quest:GetHero()
                                if not me:Speak(hero, line, 0, false, true, false) then
                                    goto FLOW_past_lab_00e51c43
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e51c43 end
                                end
                                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e51c43 end
                            end
                            quest:SetStateBool("Phase1RequirementsComplete", true)
                        else
                            if scratchValue then goto FLOW_past_lab_00e51c43 end
                            hero = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                line = "TEXT_QST_014_PHASE1_DENIED"
                                hero = quest:GetHero()
                                if not me:Speak(hero, line, 2, false, true, false) then
                                    goto FLOW_past_lab_00e51c43
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e51c43 end
                                end
                                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e51c43 end
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
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                resources:ReleaseResource(resource)
                return
            end
            ::LAB_00e51c6f::
            if not quest:GetStateBool("Phase1RequirementsComplete") then
                quest:NewScriptFrame(me);
                scratchValue = quest:IsActiveThreadTerminating();
                goto continue_3
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            quest:SetStateInt("QuestPhase", 1)
            quest:SetStateBool("InitialisePhase", true)
        elseif questPhase == 1 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            if quest:GetStateBool("InitialisePhase") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local position2 = me:GetPos()
                helpers.SetWanderPointAndDistance(quest, me, me, position2)
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
                    getStateString = quest:GetStateString("RequiredHairdo")
                    hero = quest:GetHero()
                    if quest:IsWearingHairstyle(hero, getStateString) then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            resources:ReleaseResource(resource)
                            return
                        end
                        hero = nil
                        if resources:ResourceThingHealthAbove(resource, 0.0) then
                            line = "TEXT_QST_014_PHASE2_INTRO1"
                            hero = quest:GetHero()
                            if not me:Speak(hero, line, 0, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                resources:ReleaseResource(resource)
                                return
                            end
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
                        local scratchValue13 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyBeardStyle1)
                        hero = quest:GetHero()
                        scratchValue = quest:IsWearingHairstyle(hero, scratchValue13)
                        local predicateResult3 = quest:IsActiveThreadTerminating()
                        if scratchValue then
                            if predicateResult3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                resources:ReleaseResource(resource)
                                return
                            end
                            local scratchValue17 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyBeardCard2)
                            quest:GiveHeroObject(scratchValue17, -1, false)
                            local scratchValue12 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyBeardStyle2)
                            quest:SetStateString("RequiredBeard", scratchValue12)
                        else
                            if predicateResult3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                resources:ReleaseResource(resource)
                                return
                            end
                            local scratchValue19 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyBeardCard1)
                            quest:GiveHeroObject(scratchValue19, -1, false)
                            local scratchValue10 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyBeardStyle1)
                            quest:SetStateString("RequiredBeard", scratchValue10)
                        end
                        hero = nil
                        if resources:ResourceThingHealthAbove(resource, 0.0) then
                            line = "TEXT_QST_014_PHASE2_INTRO2"
                            hero = quest:GetHero()
                            if not me:Speak(hero, line, 0, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                resources:ReleaseResource(resource)
                                return
                            end
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
                        hero = nil
                        if resources:ResourceThingHealthAbove(resource, 0.0) then
                            line = "TEXT_QST_014_PHASE2_WRONG_HAIR"
                            hero = quest:GetHero()
                            if not me:Speak(hero, line, 2, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                resources:ReleaseResource(resource)
                                return
                            end
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
                    hero = nil
                    if resources:ResourceThingHealthAbove(resource, 0.0) then
                        line = "TEXT_QST_014_PHASE2_REMINDER"
                        hero = quest:GetHero()
                        if not me:Speak(hero, line, 2, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            resources:ReleaseResource(resource)
                            return
                        end
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
        elseif questPhase == 2 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            if quest:GetStateBool("InitialisePhase") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local position3 = me:GetPos()
                helpers.SetWanderPointAndDistance(quest, me, me, position3)
                quest:SetStateBool("InitialisePhase", false)
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local movie5 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e52d54 end
                end
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e52d54 end
                if not quest:GetStateBool("Phase3RequirementsComplete") then
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e52d54 end
                    if not IsHeroWearingBeard(quest, me) then
                        if not quest:IsActiveThreadTerminating() then
                            hero = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                line = "TEXT_QST_014_PHASE3_NO_BEARD"
                                hero = quest:GetHero()
                                if not me:Speak(hero, line, 2, false, true, false) then
                                    goto FLOW_past_lab_00e52d54
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e52d54 end
                                end
                                goto LAB_00e52d54
                            end
                            goto LAB_00e52d63
                        end
                        goto FLOW_past_lab_00e52d54
                    end
                    getStateString = quest:GetStateString("RequiredHairdo")
                    hero = quest:GetHero()
                    if quest:IsWearingHairstyle(hero, getStateString) then
                        goto FLOW_past_lab_00e52a37
                    end
                    ::LAB_00e52a37::
                    getStateString = quest:GetStateString("RequiredHairdo")
                    hero = quest:GetHero()
                    if quest:IsWearingHairstyle(hero, getStateString) then
                        getStateString = quest:GetStateString("RequiredBeard")
                        hero = quest:GetHero()
                        if not quest:IsWearingHairstyle(hero, getStateString) then
                            if not quest:IsActiveThreadTerminating() then
                                hero = nil
                                if resources:ResourceThingHealthAbove(resource, 0.0) then
                                    line = "TEXT_QST_014_PHASE3_WRONG_BEARD"
                                    hero = quest:GetHero()
                                    if not me:Speak(hero, line, 2, false, true, false) then
                                        goto FLOW_past_lab_00e52d54
                                    end
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e52d54 end
                                    end
                                    goto LAB_00e52d54
                                end
                                goto LAB_00e52d63
                            end
                            goto FLOW_past_lab_00e52d54
                        end
                    end
                    getStateString = quest:GetStateString("RequiredHairdo")
                    hero = quest:GetHero()
                    if quest:IsWearingHairstyle(hero, getStateString) then
                        goto LAB_00e52c5e
                    else
                        getStateString = quest:GetStateString("RequiredBeard")
                        hero = quest:GetHero()
                        if not quest:IsWearingHairstyle(hero, getStateString) then goto LAB_00e52c5e end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie5)
                            resources:ReleaseResource(resource)
                            return
                        end
                        hero = nil
                        if resources:ResourceThingHealthAbove(resource, 0.0) then
                            line = "TEXT_QST_014_PHASE3_WRONG_HAIR"
                            hero = quest:GetHero()
                            if not me:Speak(hero, line, 2, false, true, false) then
                                goto FLOW_past_lab_00e52d54
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e52d54 end
                            end
                            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e52d54 end
                        end
                    end
                    goto FLOW_past_lab_00e52c5e
                    ::LAB_00e52c5e::
                    getStateString = quest:GetStateString("RequiredHairdo")
                    hero = quest:GetHero()
                    if not quest:IsWearingHairstyle(hero, getStateString) then
                        getStateString = quest:GetStateString("RequiredBeard")
                        hero = quest:GetHero()
                        if not quest:IsWearingHairstyle(hero, getStateString) then
                            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e52d54 end
                            hero = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                line = "TEXT_QST_014_PHASE3_WRONG_HAIR_AND_BEARD"
                                hero = quest:GetHero()
                                if not me:Speak(hero, line, 2, false, true, false) then
                                    goto FLOW_past_lab_00e52d54
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e52d54 end
                                end
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
                    getStateString = quest:GetStateString("RequiredBeard")
                    hero = quest:GetHero()
                    if not quest:IsWearingHairstyle(hero, getStateString) then goto LAB_00e52a37 end
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e52d54 end
                    hero = nil
                    if resources:ResourceThingHealthAbove(resource, 0.0) then
                        line = "TEXT_QST_014_PHASE3_INTRO1"
                        hero = quest:GetHero()
                        if not me:Speak(hero, line, 0, false, true, false) then
                            goto FLOW_past_lab_00e52d54
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e52d54 end
                        end
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e52d54 end
                    end
                    if not helpers.IsHeroWearingAnyTash(quest, me) then
                        if not quest:IsActiveThreadTerminating() then
                            hero = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                line = "TEXT_QST_014_PHASE3_INTRO3"
                                hero = quest:GetHero()
                                if not me:Speak(hero, line, 0, false, true, false) then
                                    goto FLOW_past_lab_00e52d54
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e52d54 end
                                end
                                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e52d54 end
                            end
                            goto LAB_00e52838
                        end
                        goto FLOW_past_lab_00e52d54
                    end
                    if not quest:IsActiveThreadTerminating() then
                        hero = nil
                        if resources:ResourceThingHealthAbove(resource, 0.0) then
                            line = "TEXT_QST_014_PHASE3_INTRO2"
                            hero = quest:GetHero()
                            if not me:Speak(hero, line, 0, false, true, false) then
                                goto FLOW_past_lab_00e52d54
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e52d54 end
                            end
                            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e52d54 end
                        end
                    else
                        goto FLOW_past_lab_00e52838
                    end
                    ::LAB_00e52838::
                    scratchValue22 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyTashStyle1)
                    hero = quest:GetHero()
                    scratchValue = quest:IsWearingHairstyle(hero, scratchValue22)
                    predicateResult4 = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        if predicateResult4 then goto FLOW_past_lab_00e52d54 end
                        local scratchValue21 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyTashCard2)
                        quest:GiveHeroObject(scratchValue21, -1, false)
                        local scratchValue20 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyTashStyle2)
                        quest:SetStateString("RequiredTash", scratchValue20)
                    else
                        if predicateResult4 then goto FLOW_past_lab_00e52d54 end
                        local scratchValue11 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyTashCard1)
                        quest:GiveHeroObject(scratchValue11, -1, false)
                        local scratchValue15 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyTashStyle1)
                        quest:SetStateString("RequiredTash", scratchValue15)
                    end
                    hero = nil
                    if resources:ResourceThingHealthAbove(resource, 0.0) then
                        line = "TEXT_QST_014_PHASE3_INTRO4"
                        hero = quest:GetHero()
                        if not me:Speak(hero, line, 0, false, true, false) then
                            goto FLOW_past_lab_00e52d54
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e52d54 end
                        end
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e52d54 end
                    end
                    quest:SetStateBool("Phase3RequirementsComplete", true)
                    goto LAB_00e52d63
                    ::FLOW_past_lab_00e52838::
                else
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e52d54 end
                    hero = nil
                    if not resources:ResourceThingHealthAbove(resource, 0.0) then
                        resources:PrepareResource(resource)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie5)
                        goto LAB_00e52d8f
                    end
                    line = "TEXT_QST_014_PHASE3_REMINDER"
                    hero = quest:GetHero()
                    if not me:Speak(hero, line, 2, false, true, false) then
                        goto FLOW_past_lab_00e52d54
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e52d54 end
                    end
                    goto LAB_00e52d54
                end
                goto FLOW_past_lab_00e52d54
                ::LAB_00e52d54::
                if not (quest:IsActiveThreadTerminating()) then
                    resources:PrepareResource(resource)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    goto LAB_00e52d8f
                end
                ::FLOW_past_lab_00e52d54::
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
        elseif questPhase == 3 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            if quest:GetStateBool("InitialisePhase") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local position4 = me:GetPos()
                helpers.SetWanderPointAndDistance(quest, me, me, position4)
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
                    scratchValue = IsHeroWearingBeard(quest, me)
                    local sequence = scratchValue and helpers.IsHeroWearingAnyTash(quest, me) and helpers.IsHeroWearingAnyOddHairdo(quest, me)
                    if sequence then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e53630 end
                        hero2 = nil
                        if resources:ResourceThingHealthAbove(resource, 0.0) then
                            line = "TEXT_QST_014_END_FINALE1"
                            hero2 = quest:GetHero()
                            if not me:Speak(hero2, line, 0, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                resources:ReleaseResource(resource)
                                return
                            end
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
                        local scratchValue23 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyRewardObjectFullComplete)
                        quest:GiveHeroObject(scratchValue23, -1, false)
                        quest:SetStateBool("QuestComplete", true)
                        goto LAB_00e532e3
                    else
                        if 3 < quest:GetStateInt("IncorrectHairComboCount") and quest:GetStateBool("AllHairChanged") then
                            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); resources:DestroyMovie(movie2); resources:ReleaseResource(resource); return end
                            hero2 = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                line = "TEXT_QST_014_END_FINALE2"
                                hero2 = quest:GetHero()
                                if not me:Speak(hero2, line, 0, false, true, false) then
                                    goto LAB_00e53630
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e53630 end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                            end
                            local scratchValue24 = quest:ReadGlobalGameDataString(SCRIPT_DEF.BeardyBaldyRewardObjectHalfComplete)
                            quest:GiveHeroObject(scratchValue24, -1, false)
                            quest:SetStateBool("QuestComplete", true)
                            goto LAB_00e532e3
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e53630 end
                        quest:SetStateInt("IncorrectHairComboCount", quest:GetStateInt("IncorrectHairComboCount") + 1)
                        hero2 = nil
                        if resources:ResourceThingHealthAbove(resource, 0.0) then
                            line = "TEXT_QST_014_END_WRONG"
                            hero2 = quest:GetHero()
                            if not me:Speak(hero2, line, 2, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                resources:ReleaseResource(resource)
                                return
                            end
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
                hero2 = nil
                if resources:ResourceThingHealthAbove(resource, 0.0) then
                    line = "TEXT_QST_014_END_NO_TASH"
                    hero2 = quest:GetHero()
                    if not me:Speak(hero2, line, 2, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        resources:ReleaseResource(resource)
                        return
                    end
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
                quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.BeardyBaldyExperience))
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
                    if not (quest:IsActiveThreadTerminating()) then
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
                    end
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
    local flags, scratchValue, predicateResult
    predicateResult = true
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_01") then
        local hero2 = quest:GetHero()
        if not quest:IsWearingHairstyle(hero2, "OBJECT_HERO_BEARD_02") then
            local hero3 = quest:GetHero()
            if not quest:IsWearingHairstyle(hero3, "OBJECT_HERO_BEARD_03") then
                local hero4 = quest:GetHero()
                if not quest:IsWearingHairstyle(hero4, "OBJECT_HERO_BEARD_MUTTON_01") then
                    local hero5 = quest:GetHero()
                    if not quest:IsWearingHairstyle(hero5, "OBJECT_HERO_BEARD_LONG_01") then
                        local hero6 = quest:GetHero()
                        if not quest:IsWearingHairstyle(hero6, "OBJECT_HERO_BEARD_CHIN_01") then
                            local hero7 = quest:GetHero()
                            if not quest:IsWearingHairstyle(hero7, "OBJECT_HERO_BEARD_TRAMP_01") then
                                local hero8 = quest:GetHero()
                                predicateResult = not not quest:IsWearingHairstyle(hero8, "OBJECT_HERO_BEARD_WATSON_01")
                            end
                        end
                    end
                end
            end
        end
    end
    return predicateResult
end

-- BB_BeardyBaldyMan.GetRandomSpeech (retail 0x00e53cb0)
-- E53CB0: bsim names this body NScript::CV_ArcheryCompetitionScript::CAC_Owner::GetRandomSpeech (a homologous script member); no PDB name
function GetRandomSpeech(quest, me)
    local predicateResult = quest:IsActiveThreadTerminating()
    while not predicateResult do
        local randomChoice = quest:RetailRandModulo(10)
        if (randomChoice ~= lastRandomSpeechIdx1) and (randomChoice ~= lastRandomSpeechIdx2) then
            if quest:IsActiveThreadTerminating() then return "" end
            lastRandomSpeechIdx2 = lastRandomSpeechIdx1
            lastRandomSpeechIdx1 = randomChoice
            if not quest:IsActiveThreadTerminating() then return (quest:GetStateString("RandomSpeech_" .. randomChoice)) end
            return ""
        end
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
    end
    return ""
end

