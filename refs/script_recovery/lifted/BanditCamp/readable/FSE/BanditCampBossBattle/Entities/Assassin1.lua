-- Readable native conversion: Assassin1. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    EXPERIENCE_GRANT_SMALL = 60,  -- 10
    BAC_AssassinGoldAmount = 3712,  -- 2000.0
}

-- Assassin1.Main (retail 0x00d04ae0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local assassinsUnderAttack, scratchValue, questionAnswer, questionAnswer2, movie2, resource
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
        resources:ReleaseResource(resource)
    end
    if not quest:NewScriptFrame(me) then return end
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    local predicateResult = quest:IsActiveThreadTerminating()
    if predicateResult then resources:ReleaseResource(resource); return end
    assassinsUnderAttack = quest:GetStateBool("AssassinsUnderAttack")
    scratchValue = predicateResult and 1 or 0
    while not assassinsUnderAttack and not quest:GetStateBool("AssassinCutsceneTriggered") do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        if not not (me:MsgIsHitByHero() or (me:MsgIsHitByAnySpecialAbilityFromHero() and (not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)))) then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            quest:SetStateBool("AssassinsUnderAttack", true)
        end
        if not me:IsTalkedToByHero() then assassinsUnderAttack = quest:GetStateBool("AssassinsUnderAttack"); goto continue_1 end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        if not quest:GetStateBool("Gate3Open") then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            if scratchValue == 0 then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue59 = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scratchValue59) then
                    local line = "TEXT_QST_009_ASSASSIN1_INTRO"
                    local hero = quest:GetHero()
                    if not me:Speak(hero, line, 0, false, true, false) then
                        goto LAB_00d0584f
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d0584f end
                    end
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
                    do
                        if quest:GetHeroGold() < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_AssassinGoldAmount) then
                            if not quest:IsActiveThreadTerminating() then
                                local scratchValue57 = resources:ScriptThing(resource)
                                if 0.0 < quest:GetHealth(scratchValue57) then
                                    local line2 = "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_POOR"
                                    local hero2 = quest:GetHero()
                                    if not me:Speak(hero2, line2, 0, false, true, false) then
                                        goto LAB_00d0584f
                                    end
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d0584f end
                                    end
                                    goto LAB_00d05364
                                end
                                goto LAB_00d05373
                            end
                            goto LAB_00d0584f
                        end
                        if not quest:IsActiveThreadTerminating() then
                            local scratchValue55 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue55) then
                                local line3 = "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_YES"
                                local hero3 = quest:GetHero()
                                if not me:Speak(hero3, line3, 0, false, true, false) then
                                    goto LAB_00d0584f
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d0584f end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d0584f end
                            end
                            quest:SetStateBool("AssassinCutsceneTriggered", true)
                            local scratchValue23 = math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_AssassinGoldAmount)))
                            quest:GiveHeroGold(scratchValue23)
                            quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_SMALL))
                            goto LAB_00d05373
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    goto LAB_00d0580f
                end
                do
                    local scratchValue61 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue61) then
                        local line4 = "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_NO"
                        local hero4 = quest:GetHero()
                        if not me:Speak(hero4, line4, 0, false, true, false) then
                            goto LAB_00d0584f
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d0584f end
                        end
                        goto LAB_00d05364
                    end
                    goto FLOW_hoist_lab_00d05364_1
                end
                goto FLOW_past_lab_00d05364
                ::LAB_00d05364::
                if quest:IsActiveThreadTerminating() then goto LAB_00d0584f end
                ::FLOW_hoist_lab_00d05364_1::
                ::LAB_00d05373::
                quest:SetStateBool("TalkedToAssassin", true)
                scratchValue = 1
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                goto LAB_00d057a5
                ::FLOW_past_lab_00d05364::
                ::LAB_00d0584f::
                quest:PauseAllNonScriptedEntities(false)
                ::LAB_00d0580f::
                resources:DestroyMovie(movie3)
                resources:ReleaseResource(resource)
                do return end
                resources:ReleaseResource(resource)
                return
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
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
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    local scratchValue54 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue54) then
                        local line5 = "TEXT_QST_009_ASSASSIN1_INTRO_QUESTION_YES"
                        local hero5 = quest:GetHero()
                        if not me:Speak(hero5, line5, 0, false, true, false) then
                            ReleaseEverything(); return
                        end
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
                    local scratchValue26 = math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BAC_AssassinGoldAmount)))
                    quest:GiveHeroGold(scratchValue26)
                    quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_SMALL))
                else
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    local scratchValue58 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue58) then
                        local line6 = "TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_POOR"
                        local hero6 = quest:GetHero()
                        if not me:Speak(hero6, line6, 0, false, true, false) then
                            ReleaseEverything(); return
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        goto LAB_00d05774
                    end
                end
            else
                local scratchValue56 = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scratchValue56) then
                    local line7 = "TEXT_QST_009_ASSASSIN1_REPEAT_QUESTION_NO"
                    local hero7 = quest:GetHero()
                    if not me:Speak(hero7, line7, 0, false, true, false) then
                        ReleaseEverything(); return
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                    end
                    goto LAB_00d05774
                end
            end
            goto FLOW_past_lab_00d05774
            ::LAB_00d05774::
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            ::FLOW_past_lab_00d05774::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
        else
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local scratchValue60 = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(scratchValue60) then
                local line8 = "TEXT_QST_009_ASSASSIN1_NOT_NEEDED"
                local hero8 = quest:GetHero()
                if not me:Speak(hero8, line8, 0, false, true, false) then
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
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        ::LAB_00d057a5::
        assassinsUnderAttack = quest:GetStateBool("AssassinsUnderAttack")
        ::continue_1::
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    if not quest:GetStateBool("AssassinCutsceneTriggered") and not quest:GetStateBool("AssassinsUnderAttack") then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        repeat
            quest:NewScriptFrame(me)
        until quest:IsActiveThreadTerminating()
        resources:ReleaseResource(resource)
        return
    else
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        quest:ClearThingHasInformation(me)
        resources:PrepareResource(resource)
        repeat
            quest:NewScriptFrame(me)
        until quest:IsActiveThreadTerminating()
    end
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

