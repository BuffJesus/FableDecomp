-- Readable native conversion: ManInLove. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)
local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local givenLetter, gotReward, helpedGuyOut

-- ManInLove.Main (retail 0x00ec8300)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, predicateResult3, readGlobalGameDataFloat, readGlobalGameDataFloat2
    local questionAnswer, line, getNearestWithDefName, this_00, scratchValue, u_stk_15c_2
    local scratchValue16, scratchValue21, startMovie, resource
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(startMovie)
        resources:ReleaseResource(resource)
    end
    if not quest:NewScriptFrame(me) then return end
    resource = resources:NewResource()
    u_stk_15c_2 = 1
    if not quest:IsObjectInThingsPossession(quest:ReadGlobalGameDataString(1856), hero) then
        u_stk_15c_2 = 3
        if ((not quest:IsObjectInThingsPossession("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER", hero)) and givenLetter) and quest:GetStateInt("MansLoverState") ~= 3 then
            predicateResult = false
            goto LAB_00ec8400
        end
    end
    predicateResult = true
    ::LAB_00ec8400::
    if u_stk_15c_2 & 2 ~= 0 then
        u_stk_15c_2 = u_stk_15c_2 & 0xfffffffd
    end
    if u_stk_15c_2 & 1 ~= 0 then
        u_stk_15c_2 = u_stk_15c_2 & 0xfffffffe
    end
    if predicateResult then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        quest:SetThingHasInformation(me, true, true, false)
    end
    quest:SetIsPushableByHero(me, false)
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    local position = me:GetPos()
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
            if not givenLetter and quest:GetStateInt("MansLoverState") ~= 3 then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    line = "TEXT_QST_B10_MAN_IN_LOVE_INTRO"
                    me:Speak(hero, line, 0, false, true, false)
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
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    line = "TEXT_QST_B10_MAN_IN_LOVE_INTRO_50"
                    me:Speak(hero, line, 0, false, true, false)
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
                quest:GiveHeroObject(quest:ReadGlobalGameDataString(1852), -1, false)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    line = "TEXT_QST_B10_MAN_IN_LOVE_INTRO_60"
                    me:Speak(hero, line, 0, false, true, false)
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
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    line = "TEXT_QST_B10_MAN_IN_LOVE_INTRO_70"
                    me:Speak(hero, line, 0, false, true, false)
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
                givenLetter = true
                quest:ClearThingHasInformation(me)
                quest:PauseAllNonScriptedEntities(false)
                this_00 = movie3
            else
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                startMovie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x740),(int)xStack_138);
                local movie = startMovie
                -- TODO(native): xStack_158 = xStack_158 | 4;
                if quest:IsObjectInThingsPossession("", hero) then
                    goto LAB_00ec8a86
                else
                    scratchValue = movie | 12
                    startMovie = scratchValue
                    if quest:IsObjectInThingsPossession("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER", hero) then goto LAB_00ec8a86 end
                    goto LAB_00ec8a92
                end
                goto FLOW_past_lab_00ec8a86
                ::LAB_00ec8a86::
                -- TODO(native): xStack_170 = (int *)CONCAT13(1,(undefined3)xStack_170);
                if gotReward then goto LAB_00ec8a92 end
                ::FLOW_past_lab_00ec8a86::
                goto FLOW_past_lab_00ec8a92
                ::LAB_00ec8a92::
                ::FLOW_past_lab_00ec8a92::
                if scratchValue & 8 ~= 0 then
                    scratchValue = scratchValue & 0xfffffff7
                    startMovie = scratchValue
                end
                if scratchValue & 4 ~= 0 then
                    -- TODO(native): xStack_158 = uVar11 & 0xfffffffb;
                end
                if true then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    if not gotReward then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(startMovie)
                            resources:ReleaseResource(resource)
                            return
                        end
                        if quest:GetStateInt("MansLoverState") == 3 then
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(startMovie)
                                resources:ReleaseResource(resource)
                                return
                            end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                line = "TEXT_QST_B10_MAN_IN_LOVE_MYRA_DIED"
                                me:Speak(hero, line, 0, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(startMovie)
                                        resources:ReleaseResource(resource)
                                        do return end
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(startMovie)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                            end
                            -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)xStack_140);
                            quest:GiveHeroObject(line, scratchValue21, -1)
                            quest:RemoveItemFromContainer(me, quest:ReadGlobalGameDataString(1848))
                            gotReward = true
                        else
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(startMovie)
                                resources:ReleaseResource(resource)
                                return
                            end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                line = "TEXT_QST_B10_MAN_IN_LOVE_DELIVER_LETTER"
                                me:Speak(hero, line, 0, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(startMovie)
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
                            resources:DestroyMovie(startMovie)
                            resources:ReleaseResource(resource)
                            return
                        end
                        me:ClearCommands()
                        if not helpedGuyOut then
                            if quest:GetStateInt("MansLoverState") == 3 then
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(startMovie)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                    line = "TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_MYRA_DIED_10"
                                    me:Speak(hero, line, 0, false, true, false)
                                    while me:IsPerformingScriptTask() do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(startMovie)
                                            resources:ReleaseResource(resource)
                                            do return end
                                        end
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(startMovie)
                                        resources:ReleaseResource(resource)
                                        return
                                    end
                                end
                            else
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(startMovie)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                    line = "TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_UNHAPPY_10"
                                    me:Speak(hero, line, 0, false, true, false)
                                    while me:IsPerformingScriptTask() do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(startMovie)
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
                                resources:DestroyMovie(startMovie)
                                resources:ReleaseResource(resource)
                                return
                            end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                line = "TEXT_QST_B10_MAN_IN_LOVE_POST_QUEST_HAPPY_10"
                                me:Speak(hero, line, 0, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(startMovie)
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
                        resources:DestroyMovie(startMovie)
                        resources:ReleaseResource(resource)
                        return
                    end
                    ::FLOW_past_lab_00ec96a4::
                else
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_B10_MAN_IN_LOVE_RETURNING_OBJECT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    local predicateResult33 = quest:IsActiveThreadTerminating()
                    if questionAnswer == 1 then
                        if predicateResult33 then ReleaseEverything(); return end
                        quest:TakeObjectFromHero(quest:ReadGlobalGameDataString(1856))
                        quest:TakeObjectFromHero("OBJECT_SICK_CHILD_WOMANS_HERO_LETTER")
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            line = "TEXT_QST_B10_MAN_IN_LOVE_RETURNED_OBJECT_TO_ME"
                            me:Speak(hero, line, 0, false, true, false)
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                            end
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                        end
                        quest:GiveHeroObject(quest:ReadGlobalGameDataString(1848), -1, false)
                        -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)xStack_dc);
                        quest:RemoveItemFromContainer(me, line)
                        if quest:GetStateInt("MansLoverState") == 2 then
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                            readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(1880)
                        else
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                            readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(1892)
                        end
                        quest:GiveHeroMorality(readGlobalGameDataFloat)
                        gotReward = true
                        helpedGuyOut = true
                        quest:ClearThingHasInformation(me)
                    else
                        if predicateResult33 then ReleaseEverything(); return end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            line = "TEXT_QST_B10_MAN_IN_LOVE_DIDNT_RETURN_OBJECT_TO_ME"
                            me:Speak(hero, line, 0, false, true, false)
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                            end
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_B10_MAN_IN_LOVE_WOMANS_OPINION_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        while questionAnswer < 0 do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                        local predicateResult34 = quest:IsActiveThreadTerminating()
                        if questionAnswer == 1 then
                            if predicateResult34 then ReleaseEverything(); return end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                line = "TEXT_QST_B10_MAN_IN_LOVE_HAPPY"
                                me:Speak(hero, line, 0, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                                end
                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                            end
                            -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(DAT_0143e90c + 0x738),(int)xStack_104);
                            quest:GiveHeroObject(line, scratchValue16, -1)
                            quest:RemoveItemFromContainer(me, quest:ReadGlobalGameDataString(1848))
                            gotReward = true
                            helpedGuyOut = true
                            if quest:GetStateInt("MansLoverState") == 2 then
                                goto LAB_00ec91ef
                            else
                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                                readGlobalGameDataFloat2 = quest:ReadGlobalGameDataFloat(1892)
                            end
                        else
                            if predicateResult34 then ReleaseEverything(); return end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                line = "TEXT_QST_B10_MAN_IN_LOVE_UNHAPPY"
                                me:Speak(hero, line, 0, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                                end
                                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                            end
                            quest:GiveHeroObject(quest:ReadGlobalGameDataString(1848), -1, false)
                            quest:RemoveItemFromContainer(me, quest:ReadGlobalGameDataString(1848))
                            gotReward = true
                            if quest:GetStateInt("MansLoverState") ~= 2 then goto LAB_00ec91ef end
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                            readGlobalGameDataFloat2 = quest:ReadGlobalGameDataFloat(1888)
                        end
                        goto FLOW_past_lab_00ec91ef
                        ::LAB_00ec91ef::
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(startMovie)
                            resources:ReleaseResource(resource)
                            return
                        end
                        readGlobalGameDataFloat2 = quest:ReadGlobalGameDataFloat(1880)
                        ::FLOW_past_lab_00ec91ef::
                        quest:GiveHeroMorality(readGlobalGameDataFloat2)
                        quest:ClearThingHasInformation(me)
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                this_00 = startMovie
            end
            resources:ReleaseResource(this_00)
            resources:PrepareResource(resource)
        end
        local scratchValue15 = u_stk_15c_2
        u_stk_15c_2 = u_stk_15c_2 | 16
        if me:MsgIsHitByHero() then goto LAB_00ec9774 end
        scratchValue = scratchValue15 | 48
        u_stk_15c_2 = scratchValue
        if me:MsgIsHitByAnySpecialAbilityFromHero() then
            scratchValue = scratchValue15 | 112
            u_stk_15c_2 = scratchValue
            if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00ec9774 end
        end
        goto FLOW_past_lab_00ec9774
        ::LAB_00ec9774::
        ::FLOW_past_lab_00ec9774::
        if scratchValue & 64 ~= 0 then
            scratchValue = scratchValue & 0xffffffbf
            u_stk_15c_2 = scratchValue
        end
        if scratchValue & 32 ~= 0 then
            scratchValue = scratchValue & 0xffffffdf
            u_stk_15c_2 = scratchValue
        end
        if scratchValue & 16 ~= 0 then
            u_stk_15c_2 = scratchValue & 0xffffffef
        end
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
        local movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
            line = "TEXT_QST_B10_MAN_IN_LOVE_ON_HIT"
            me:Speak(hero, line, 0, false, true, false)
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
        resources:PrepareResource(resource)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
        local getStateBool = gotReward and not me:IsPerformingScriptTask()
        if not getStateBool then quest:NewScriptFrame(me); predicateResult3 = quest:IsActiveThreadTerminating(); goto continue_13 end
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
    givenLetter = false
    gotReward = false
    helpedGuyOut = false
    quest:SetThingPersistent(me, true)
end

-- ManInLove.OnPersist (retail 0x00ecd8e0)
function OnPersist(quest, me, context)
    quest:SetStateBool("GivenLetter", quest:PersistTransferBool(context, "GivenLetter", quest:GetStateBool("GivenLetter")))
    quest:SetStateBool("GotReward", quest:PersistTransferBool(context, "GotReward", quest:GetStateBool("GotReward")))
end

-- ManInLove.OnPredicateFail (retail 0x00ec8230)
function OnPredicateFail(quest, me)
    local predicateResult
    if not gotReward then
        predicateResult = true
        if me:MsgIsKilledBy("") then goto LAB_00ec8274 end
    end
    predicateResult = false
    ::LAB_00ec8274::
    if not predicateResult then return end
    quest:GiveHeroObject(quest:ReadGlobalGameDataString(1848), -1, false)
    quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(1896))
end

