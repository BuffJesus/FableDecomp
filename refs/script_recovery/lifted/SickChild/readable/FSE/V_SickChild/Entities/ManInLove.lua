-- Readable native conversion: ManInLove. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)
local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    IngredientObject = 1848,  -- 'OBJECT_MUSHROOM_BLUE_01'
    ObjectToGiveToMansLover = 1852,  -- 'OBJECT_SICK_CHILD_MANS_LETTER'
    ObjectFromMansLover = 1856,  -- 'OBJECT_SICK_CHILD_WOMANS_LETTER'
    TellingTheTruth = 1880,  -- 0.009999999776482582
    LyingToManAboutWomanNotLikingHer = 1888,  -- -0.019999999552965164
    LyingToManAboutWomanLikingHer = 1892,  -- -0.019999999552965164
    KilledSomeoneForMushroom = 1896,  -- -0.029999999329447746
}

-- per-entity fields (native class members; one Lua state per entity instance)
local givenLetter_, gotReward_, helpedGuyOut

-- ManInLove.Main (retail 0x00ec8300)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, predicateResult3, center, readGlobalGameDataFloat
    local msgIsQuestionAnsweredYesOrNo, manConversationMovie, manHasUnrewardedLetter
    local manShouldReactToHit, manTalkMovie, hero3, getHero, line, getNearestWithDefName, resource
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(manTalkMovie)
        resources:ReleaseResource(resource)
    end
    if not quest:NewScriptFrame(me) then return end
    resource = resources:NewResource()
    local scratchValue50 = quest:ReadGlobalGameDataString(SCRIPT_DEF.ObjectFromMansLover)
    local hero = quest:GetHero()
    if not quest:IsObjectInThingsPossession(scratchValue50, hero) then
        local hero2 = quest:GetHero()
        if ((not quest:IsObjectInThingsPossession("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER", hero2)) and givenLetter_) and quest:GetStateInt("MansLoverState") ~= 3 then
            predicateResult = false
            goto LAB_00ec8400
        end
    end
    predicateResult = true
    ::LAB_00ec8400::
    if predicateResult then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        quest:SetThingHasInformation(me, true, true, false)
    end
    quest:SetIsPushableByHero(me, false)
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    local position = me:GetPos()
    center = position.x
    quest:SetWanderCentrePoint(me, position)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 10.0)
    quest:SetScriptingStateGroup(me, 4)
    predicateResult3 = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult3 then
            resources:ReleaseResource(resource)
            return
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            if not givenLetter_ and quest:GetStateInt("MansLoverState") ~= 3 then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local manIntroMovie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                getHero = nil
                if resources:ResourceThingHealthAbove(resource, 0.0) then
                    msgIsQuestionAnsweredYesOrNo = 0
                    line = "TEXT_QST_B10_MAN_IN_LOVE_INTRO"
                    getHero = quest:GetHero()
                    if not me:Speak(getHero, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(resource)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(manIntroMovie)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                getHero = nil
                if resources:ResourceThingHealthAbove(resource, 0.0) then
                    msgIsQuestionAnsweredYesOrNo = 0
                    line = "TEXT_QST_B10_MAN_IN_LOVE_INTRO_50"
                    getHero = quest:GetHero()
                    if not me:Speak(getHero, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(resource)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(manIntroMovie)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                local scratchValue47 = quest:ReadGlobalGameDataString(SCRIPT_DEF.ObjectToGiveToMansLover)
                quest:GiveHeroObject(scratchValue47, -1, false)
                getHero = nil
                if resources:ResourceThingHealthAbove(resource, 0.0) then
                    msgIsQuestionAnsweredYesOrNo = 0
                    line = "TEXT_QST_B10_MAN_IN_LOVE_INTRO_60"
                    getHero = quest:GetHero()
                    if not me:Speak(getHero, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(resource)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(manIntroMovie)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                getHero = nil
                if resources:ResourceThingHealthAbove(resource, 0.0) then
                    msgIsQuestionAnsweredYesOrNo = 0
                    line = "TEXT_QST_B10_MAN_IN_LOVE_INTRO_70"
                    getHero = quest:GetHero()
                    if not me:Speak(getHero, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(resource)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(manIntroMovie)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manIntroMovie)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                givenLetter_ = true
                quest:ClearThingHasInformation(me)
                quest:PauseAllNonScriptedEntities(false)
                manConversationMovie = manIntroMovie
            else
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                manTalkMovie = resources:StartMovie("")
                getHero = 1
                quest:PauseAllNonScriptedEntities(true)
                local local_manLetterDefinition = quest:ReadGlobalGameDataString(SCRIPT_DEF.ObjectFromMansLover)
                hero3 = quest:GetHero()
                if quest:IsObjectInThingsPossession(local_manLetterDefinition, hero3) then
                    goto LAB_00ec8a86
                else
                    hero3 = quest:GetHero()
                    if quest:IsObjectInThingsPossession("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER", hero3) then goto LAB_00ec8a86 end
                    goto LAB_00ec8a92
                end
                goto FLOW_past_lab_00ec8a86
                ::LAB_00ec8a86::
                manHasUnrewardedLetter = true
                if gotReward_ then goto LAB_00ec8a92 end
                ::FLOW_past_lab_00ec8a86::
                goto FLOW_past_lab_00ec8a92
                ::LAB_00ec8a92::
                manHasUnrewardedLetter = false
                ::FLOW_past_lab_00ec8a92::
                if not manHasUnrewardedLetter then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    if not gotReward_ then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(manTalkMovie)
                            resources:ReleaseResource(resource)
                            return
                        end
                        if quest:GetStateInt("MansLoverState") == 3 then
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(manTalkMovie)
                                resources:ReleaseResource(resource)
                                return
                            end
                            hero3 = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                msgIsQuestionAnsweredYesOrNo = 0
                                line = "TEXT_QST_B10_MAN_IN_LOVE_MYRA_DIED"
                                getHero = quest:GetHero()
                                if not me:Speak(getHero, line, 0, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(manTalkMovie)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(manTalkMovie)
                                        resources:ReleaseResource(resource)
                                        do return end
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(manTalkMovie)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                            end
                            local local_manRewardDefinitionFirst = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject)
                            quest:GiveHeroObject(local_manRewardDefinitionFirst, -1, false)
                            local scratchValue46 = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject)
                            quest:RemoveItemFromContainer(me, scratchValue46)
                            gotReward_ = true
                        else
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(manTalkMovie)
                                resources:ReleaseResource(resource)
                                return
                            end
                            hero3 = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                msgIsQuestionAnsweredYesOrNo = 0
                                line = "TEXT_QST_B10_MAN_IN_LOVE_DELIVER_LETTER"
                                getHero = quest:GetHero()
                                if not me:Speak(getHero, line, 0, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(manTalkMovie)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(manTalkMovie)
                                        resources:ReleaseResource(resource)
                                        do return end
                                    end
                                end
                                goto LAB_00ec96a4
                            end
                        end
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(manTalkMovie)
                            resources:ReleaseResource(resource)
                            return
                        end
                        me:ClearCommands()
                        if not helpedGuyOut then
                            if quest:GetStateInt("MansLoverState") == 3 then
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(manTalkMovie)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                hero3 = nil
                                if resources:ResourceThingHealthAbove(resource, 0.0) then
                                    msgIsQuestionAnsweredYesOrNo = 0
                                    line = "TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_MYRA_DIED_10"
                                    getHero = quest:GetHero()
                                    if not me:Speak(getHero, line, 0, false, true, false) then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(manTalkMovie)
                                        resources:ReleaseResource(resource)
                                        return
                                    end
                                    while me:IsPerformingScriptTask() do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(manTalkMovie)
                                            resources:ReleaseResource(resource)
                                            do return end
                                        end
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(manTalkMovie)
                                        resources:ReleaseResource(resource)
                                        return
                                    end
                                end
                            else
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(manTalkMovie)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                hero3 = nil
                                if resources:ResourceThingHealthAbove(resource, 0.0) then
                                    msgIsQuestionAnsweredYesOrNo = 0
                                    line = "TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_UNHAPPY_10"
                                    getHero = quest:GetHero()
                                    if not me:Speak(getHero, line, 0, false, true, false) then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(manTalkMovie)
                                        resources:ReleaseResource(resource)
                                        return
                                    end
                                    while me:IsPerformingScriptTask() do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(manTalkMovie)
                                            resources:ReleaseResource(resource)
                                            do return end
                                        end
                                    end
                                    goto LAB_00ec96a4
                                end
                            end
                        else
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(manTalkMovie)
                                resources:ReleaseResource(resource)
                                return
                            end
                            hero3 = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                msgIsQuestionAnsweredYesOrNo = 0
                                line = "TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_HAPPY_10"
                                getHero = quest:GetHero()
                                if not me:Speak(getHero, line, 0, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(manTalkMovie)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(manTalkMovie)
                                        resources:ReleaseResource(resource)
                                        do return end
                                    end
                                end
                                goto LAB_00ec96a4
                            end
                        end
                    end
                    goto FLOW_past_lab_00ec96a4
                    ::LAB_00ec96a4::
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manTalkMovie)
                        resources:ReleaseResource(resource)
                        return
                    end
                    ::FLOW_past_lab_00ec96a4::
                else
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_B10_MAN_IN_LOVE_RETURNING_OBJECT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    msgIsQuestionAnsweredYesOrNo = quest:MsgIsQuestionAnsweredYesOrNo()
                    while msgIsQuestionAnsweredYesOrNo < 0 do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        msgIsQuestionAnsweredYesOrNo = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    local predicateResult33 = quest:IsActiveThreadTerminating()
                    if msgIsQuestionAnsweredYesOrNo == 1 then
                        if predicateResult33 then ReleaseEverything(); return end
                        local scratchValue51 = quest:ReadGlobalGameDataString(SCRIPT_DEF.ObjectFromMansLover)
                        quest:TakeObjectFromHero(scratchValue51)
                        quest:TakeObjectFromHero("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER")
                        hero3 = nil
                        if resources:ResourceThingHealthAbove(resource, 0.0) then
                            msgIsQuestionAnsweredYesOrNo = 0
                            line = "TEXT_QST_B10_MAN_IN_LOVE_RETURNED_OBJECT_TO_ME"
                            getHero = quest:GetHero()
                            if not me:Speak(getHero, line, 0, false, true, false) then
                                ReleaseEverything(); return
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                            end
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                        end
                        local scratchValue48 = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject)
                        quest:GiveHeroObject(scratchValue48, -1, false)
                        local local_manReturnedItemDefinition = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject)
                        quest:RemoveItemFromContainer(me, local_manReturnedItemDefinition)
                        if quest:GetStateInt("MansLoverState") == 2 then
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                            readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.TellingTheTruth)
                        else
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                            readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.LyingToManAboutWomanLikingHer)
                        end
                        quest:GiveHeroMorality(readGlobalGameDataFloat)
                        gotReward_ = true
                        helpedGuyOut = true
                        quest:ClearThingHasInformation(me)
                    else
                        if predicateResult33 then ReleaseEverything(); return end
                        hero3 = nil
                        if resources:ResourceThingHealthAbove(resource, 0.0) then
                            msgIsQuestionAnsweredYesOrNo = 0
                            line = "TEXT_QST_B10_MAN_IN_LOVE_DIDNT_RETURN_OBJECT_TO_ME"
                            hero3 = quest:GetHero()
                            if not me:Speak(hero3, line, 0, false, true, false) then
                                ReleaseEverything(); return
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                            end
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_B10_MAN_IN_LOVE_WOMANS_OPINION_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        msgIsQuestionAnsweredYesOrNo = quest:MsgIsQuestionAnsweredYesOrNo()
                        while msgIsQuestionAnsweredYesOrNo < 0 do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                            msgIsQuestionAnsweredYesOrNo = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                        local predicateResult34 = quest:IsActiveThreadTerminating()
                        if msgIsQuestionAnsweredYesOrNo == 1 then
                            if predicateResult34 then ReleaseEverything(); return end
                            hero3 = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                msgIsQuestionAnsweredYesOrNo = 0
                                line = "TEXT_QST_B10_MAN_IN_LOVE_HAPPY"
                                getHero = quest:GetHero()
                                if not me:Speak(getHero, line, 0, false, true, false) then
                                    ReleaseEverything(); return
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                                end
                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                            end
                            local local_manRewardDefinitionSecond = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject)
                            quest:GiveHeroObject(local_manRewardDefinitionSecond, -1, false)
                            local scratchValue52 = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject)
                            quest:RemoveItemFromContainer(me, scratchValue52)
                            gotReward_ = true
                            helpedGuyOut = true
                            if quest:GetStateInt("MansLoverState") == 2 then
                                goto LAB_00ec91ef
                            else
                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                                readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.LyingToManAboutWomanLikingHer)
                            end
                        else
                            if predicateResult34 then ReleaseEverything(); return end
                            hero3 = nil
                            if resources:ResourceThingHealthAbove(resource, 0.0) then
                                msgIsQuestionAnsweredYesOrNo = 0
                                line = "TEXT_QST_B10_MAN_IN_LOVE_UNHAPPY"
                                getHero = quest:GetHero()
                                if not me:Speak(getHero, line, 0, false, true, false) then
                                    ReleaseEverything(); return
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                                end
                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                            end
                            local scratchValue53 = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject)
                            quest:GiveHeroObject(scratchValue53, -1, false)
                            local scratchValue49 = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject)
                            quest:RemoveItemFromContainer(me, scratchValue49)
                            gotReward_ = true
                            if quest:GetStateInt("MansLoverState") ~= 2 then goto LAB_00ec91ef end
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                            readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.LyingToManAboutWomanNotLikingHer)
                        end
                        goto FLOW_past_lab_00ec91ef
                        ::LAB_00ec91ef::
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(manTalkMovie)
                            resources:ReleaseResource(resource)
                            return
                        end
                        readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.TellingTheTruth)
                        ::FLOW_past_lab_00ec91ef::
                        quest:GiveHeroMorality(readGlobalGameDataFloat)
                        quest:ClearThingHasInformation(me)
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                manConversationMovie = manTalkMovie
            end
            resources:DestroyMovie(manConversationMovie)
            resources:PrepareResource(resource)
        end
        if me:MsgIsHitByHero() then goto LAB_00ec9774 end
        if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00ec9774 end
        manShouldReactToHit = false
        goto FLOW_past_lab_00ec9774
        ::LAB_00ec9774::
        manShouldReactToHit = true
        ::FLOW_past_lab_00ec9774::
        if manShouldReactToHit then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            me:ClearCommands()
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            local manHitMovie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            getHero = nil
            if resources:ResourceThingHealthAbove(resource, 0.0) then
                msgIsQuestionAnsweredYesOrNo = 0
                line = "TEXT_QST_B10_MAN_IN_LOVE_ON_HIT"
                getHero = quest:GetHero()
                if not me:Speak(getHero, line, 0, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(manHitMovie)
                    resources:ReleaseResource(resource)
                    return
                end
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(manHitMovie)
                        resources:ReleaseResource(resource)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(manHitMovie)
                    resources:ReleaseResource(resource)
                    return
                end
            end
            resources:PrepareResource(resource)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(manHitMovie)
        end
        local getStateBool = gotReward_ and not me:IsPerformingScriptTask()
        if not getStateBool then
            quest:NewScriptFrame(me);
            predicateResult3 = quest:IsActiveThreadTerminating();
            goto continue_13
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        getNearestWithDefName = quest:GetNearestWithDefName(me, "REGION_EXIT_POINT")
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource)
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        if quest:IsDistanceBetweenThingsOver(me, getNearestWithDefName, 2.0) then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            me:MoveToThing(getNearestWithDefName, 1.0, ENTITY_MOVE_WALK)
        else
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            quest:FadeOutAndKillEntity(me, true, 1.5, true)
        end
        quest:NewScriptFrame(me)
        predicateResult3 = quest:IsActiveThreadTerminating()
        ::continue_13::
    until false
end

-- ManInLove.Init (retail 0x00ec8210)
function Init(quest, me)
    givenLetter_ = false
    gotReward_ = false
    helpedGuyOut = false
    quest:SetThingPersistent(me, true)
end

-- ManInLove.OnPersist (retail 0x00ecd8e0)
function OnPersist(quest, me, context)
    local givenLetter = givenLetter_ or false
    givenLetter_ = quest:PersistTransferBool(context, "GivenLetter", givenLetter)
    local gotReward = gotReward_ or false
    gotReward_ = quest:PersistTransferBool(context, "GotReward", gotReward)
end

-- ManInLove.OnPredicateFail (retail 0x00ec8230)
function OnPredicateFail(quest, me)
    local predicateResult, predicateResult2
    predicateResult2 = false
    if not gotReward_ then
        predicateResult2 = true
        predicateResult = true
        if me:MsgIsKilledBy("") then goto LAB_00ec8274 end
    end
    predicateResult = false
    ::LAB_00ec8274::
    if predicateResult2 then
    end
    if not predicateResult then return end
    local scratchValue = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject)
    quest:GiveHeroObject(scratchValue, -1, false)
    quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.KilledSomeoneForMushroom))
end

