-- Readable native conversion: Assassin1. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    EXPERIENCE_GRANT_SMALL = 60,  -- 10
    BAC_AssassinGoldAmount = 3712,  -- 2000.0
}

-- Assassin1.Main (retail 0x00d04ae0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, scratchValue, assassinsUnderAttack, scratchValue4, scratchValue5
    local questionAnswer, questionAnswer2, movie2, resource
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
        resources:ReleaseResource(resource)
    end
    if not quest:NewScriptFrame(me) then return end
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d058f3 end
    end
    predicateResult = quest:IsActiveThreadTerminating()
    if predicateResult then goto LAB_00d058f3 end
    assassinsUnderAttack = quest:GetStateBool("AssassinsUnderAttack")
    scratchValue4 = predicateResult
    while not assassinsUnderAttack and not quest:GetStateBool("AssassinCutsceneTriggered") do
        if not quest:NewScriptFrame(me) then goto LAB_00d058f3 end
        if me:MsgIsHitByHero() then goto LAB_00d04cdc end
        -- TODO(native): bVar5 = (**(*me + 0xa8))(me,"SCRIPT_NAME_HERO")
        scratchValue = nil --[[unresolved native value]]
        if scratchValue then
            -- TODO(native): bVar5 = (**(*me + 0xa4))(me,0xe,"SCRIPT_NAME_HERO")
            local scratchValue2 = nil --[[unresolved native value]]
            if not scratchValue2 then goto LAB_00d04cdc end
        end
        scratchValue5 = 0
        goto FLOW_past_lab_00d04cdc
        ::LAB_00d04cdc::
        scratchValue5 = 1
        ::FLOW_past_lab_00d04cdc::
        if scratchValue5 ~= 0 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d058f3 end
            quest:SetStateBool("AssassinsUnderAttack", true)
        end
        -- TODO(native): bVar5 = (**(*me + 0x6c))(me,"SCRIPT_NAME_HERO")
        local scratchValue3 = nil --[[unresolved native value]]
        if not scratchValue3 then assassinsUnderAttack = quest:GetStateBool("AssassinsUnderAttack"); goto continue_1 end
        if quest:IsActiveThreadTerminating() then goto LAB_00d058f3 end
        if not quest:GetStateBool("Gate3Open") then
            if not scratchValue4 then
                if not quest:IsActiveThreadTerminating() then
                    local movie3 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        if not me:Speak(hero, "TEXT_QST_009_ASSASSIN1_INTRO", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0584f end
                        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d0580f end
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_009_ASSASSIN1_INTRO_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d0584f end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0584f end
                    if questionAnswer == 1 then
                        if quest:GetHeroGold() < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_AssassinGoldAmount) then
                            if not quest:IsActiveThreadTerminating() then
                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                    if not me:Speak(hero, "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_POOR", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0584f end
                                    goto LAB_00d05364
                                end
                                goto LAB_00d05373
                            end
                            goto LAB_00d0584f
                        end
                        if not quest:IsActiveThreadTerminating() then
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0584f end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d0584f end
                            end
                            quest:SetStateBool("AssassinCutsceneTriggered", true)
                            quest:GiveHeroGold(math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_AssassinGoldAmount))))
                            quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_SMALL))
                            goto LAB_00d05373
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        goto LAB_00d0580f
                    end
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        if not me:Speak(hero, "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0584f end
                        goto LAB_00d05364
                    end
                    goto FLOW_hoist_lab_00d05364_1
                    goto FLOW_past_lab_00d05364
                    ::LAB_00d05364::
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0584f end
                    ::FLOW_hoist_lab_00d05364_1::
                    ::LAB_00d05373::
                    quest:SetStateBool("TalkedToAssassin", true)
                    scratchValue4 = 1
                    quest:PauseAllNonScriptedEntities(false)
                    goto LAB_00d057a5
                    ::FLOW_past_lab_00d05364::
                    ::LAB_00d0584f::
                    quest:PauseAllNonScriptedEntities(false)
                    ::LAB_00d0580f::
                    resources:DestroyMovie(movie3)
                    resources:ReleaseResource(resource)
                    return
                end
                goto LAB_00d058f3
            end
            movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:GiveHeroYesNoQuestion("TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer2 < 0 do
                if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            if questionAnswer2 == 1 then
                local i_stk_a0_2 = quest:GetHeroGold()
                if quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_AssassinGoldAmount) <= i_stk_a0_2 then
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            resources:ReleaseResource(resource)
                            return
                        end
                    end
                    quest:SetStateBool("AssassinCutsceneTriggered", true)
                    quest:GiveHeroGold(math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_AssassinGoldAmount))))
                    quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_SMALL))
                elseif 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    me:Speak(hero, "TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_POOR", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                    end
                    goto LAB_00d05774
                end
            elseif 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                me:Speak(hero, "TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                end
                goto LAB_00d05774
            end
            goto FLOW_past_lab_00d05774
            ::LAB_00d05774::
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            ::FLOW_past_lab_00d05774::
            quest:PauseAllNonScriptedEntities(false)
        else
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                me:Speak(hero, "TEXT_QST_009_ASSASSIN1_NOT_NEEDED", GROUP_SELECT_FIRST, false, true, false)
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
            quest:PauseAllNonScriptedEntities(false)
        end
        ::LAB_00d057a5::
        assassinsUnderAttack = quest:GetStateBool("AssassinsUnderAttack")
        ::continue_1::
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d058f3 end
    if not quest:GetStateBool("AssassinCutsceneTriggered") and not quest:GetStateBool("AssassinsUnderAttack") then
        repeat
            quest:NewScriptFrame(me)
        until quest:IsActiveThreadTerminating()
        resources:ReleaseResource(resource)
        return
    else
        quest:ClearThingHasInformation(me)
        resources:PrepareResource(resource)
        repeat
            quest:NewScriptFrame(me)
        until quest:IsActiveThreadTerminating()
    end
    ::LAB_00d058f3::
    resources:ReleaseResource(resource)
end

-- Assassin1.Init (retail 0x00d04a60)
function Init(quest, me)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:SetIsPushableByHero(me, false)
end

-- Assassin1.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- Assassin1.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

