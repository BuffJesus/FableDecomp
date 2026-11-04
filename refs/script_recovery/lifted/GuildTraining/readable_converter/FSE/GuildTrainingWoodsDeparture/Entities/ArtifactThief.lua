-- Readable native conversion: ArtifactThief. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_LampCost = 3860,  -- 50.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local holdingArtifact, alreadyTalkedTo, notAttacked

-- ArtifactThief.Main (retail 0x00d62480)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult4, predicateResult, getStateBool, questionAnswer, questionAnswer2, timerId
    local hero, movie, line, getHeroGold, movie4, resource, scratchValue41, scratchValue49
    local function ReleaseEverything()
        resources:DestroyMovie(movie)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything2()
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    if not quest:NewScriptFrame(me) then return end
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:EntitySetAsKillable(me, false, true)
        quest:SetThingHasInformation(me, false, true, false)
        holdingArtifact = true
        alreadyTalkedTo = false
        notAttacked = true
        timerId = quest:RegisterTimer()
        quest:SetTimer(timerId, 0)
        getStateBool = holdingArtifact
        repeat
            if not getStateBool or not notAttacked then goto LAB_00d638ec end
            if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            hero = quest:GetHero()
            if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                local conversationId = quest:AddNewConversation(me, false, false)
                hero = quest:GetHero()
                quest:AddPersonToConversation(conversationId, hero)
                if not alreadyTalkedTo then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    hero = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    resources:PlayAnimationWithNativeArgument5(resource, "ST_WAVE_SPECIAL_02", false, false, false, true, false, false)
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_FIRST", me, hero, false)
                else
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    hero = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_SECOND", me, hero, false)
                end
                quest:SetTimer(timerId, 10)
            end
            if me:MsgIsHitByHero() then
                goto LAB_00d6280c
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d6280c end
                predicateResult4 = false
            end
            goto FLOW_past_lab_00d6280c
            ::LAB_00d6280c::
            predicateResult4 = true
            ::FLOW_past_lab_00d6280c::
            if predicateResult4 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                notAttacked = false
                if not alreadyTalkedTo then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    local movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue47 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue47) then
                        line = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_PRE_HIT"
                        hero = quest:GetHero()
                        if not me:Speak(hero, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            ReleaseEverything2(); return
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                ReleaseEverything2(); do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource)
                            return
                        end
                    end
                    quest:GiveHeroObject("OBJECT_HAND_LAMP", -1, false)
                    quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                else
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                    local movie6 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue48 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue48) then
                        line = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_HIT"
                        hero = quest:GetHero()
                        if not me:Speak(hero, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie6)
                            ReleaseEverything2(); return
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie6)
                                ReleaseEverything2(); do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie6)
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource)
                            return
                        end
                    end
                    quest:GiveHeroObject("OBJECT_HAND_LAMP", -1, false)
                    quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie6)
                end
                quest:EntitySetAsKillable(me, true, true)
                local position = quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos()
                me:MoveToPosition(position, 1.0, ENTITY_MOVE_RUN, false, true)
                hero = nil
            end
            if not me:IsTalkedToByHero() then getStateBool = holdingArtifact; goto continue_3 end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            if not holdingArtifact then goto LAB_00d638ec end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            if not alreadyTalkedTo then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                alreadyTalkedTo = true
                local movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue45 = resources:ScriptThing(resource)
                if not (0.0 < quest:GetHealth(scratchValue45)) then goto LAB_00d62e38 end
                line = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_CHAT"
                hero = quest:GetHero()
                if not me:Speak(hero, line, 0, false, true, false) then
                    goto LAB_00d63a7c
                end
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                end
                if not quest:IsActiveThreadTerminating() then goto LAB_00d62e38 end
                goto FLOW_past_lab_00d62e38
                ::LAB_00d62e38::
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                while questionAnswer < 0 do
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    else
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie3
                        ReleaseEverything()
                        do return end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                end
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d62e38 end
                if questionAnswer == 1 then
                    do
                        if quest:GetHeroGold() < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost) then
                            if not quest:IsActiveThreadTerminating() then
                                local scratchValue43 = resources:ScriptThing(resource)
                                if 0.0 < quest:GetHealth(scratchValue43) then
                                    line = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD"
                                    hero = quest:GetHero()
                                    if not me:Speak(hero, line, 0, false, true, false) then
                                        goto LAB_00d63a7c
                                    end
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                    end
                                    goto LAB_00d632b4
                                end
                                goto LAB_00d632c3
                            end
                            goto LAB_00d63a7c
                        end
                        if not quest:IsActiveThreadTerminating() then
                            local scratchValue42 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue42) then
                                line = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES"
                                hero = quest:GetHero()
                                if not me:Speak(hero, line, 0, false, true, false) then
                                    goto LAB_00d63a7c
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                            end
                            quest:GiveHeroObject("OBJECT_HAND_LAMP", -1, false)
                            quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                            local scratchValue = math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost)))
                            quest:GiveHeroGold(scratchValue)
                            local scratchValue28 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost)))
                            quest:EntityGiveGold(me, scratchValue28)
                            holdingArtifact = false
                            local position2 = quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos()
                            me:MoveToPosition(position2, 1.0, ENTITY_MOVE_WALK, false, true)
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie3
                            goto LAB_00d638d8
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
                do
                    local scratchValue44 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue44) then
                        line = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO"
                        hero = quest:GetHero()
                        if not me:Speak(hero, line, 0, false, true, false) then
                            goto LAB_00d63a7c
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                        end
                        goto LAB_00d632b4
                    end
                    goto FLOW_hoist_lab_00d632b4_1
                end
                goto FLOW_past_lab_00d632b4
                ::LAB_00d632b4::
                if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                ::FLOW_hoist_lab_00d632b4_1::
                ::LAB_00d632c3::
                quest:PauseAllNonScriptedEntities(false)
                movie = movie3
                goto LAB_00d638d8
                ::FLOW_past_lab_00d632b4::
                ::FLOW_past_lab_00d62e38::
                ::LAB_00d63a7c::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            movie4 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            scratchValue49 = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(scratchValue49) then
                line = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN"
                hero = quest:GetHero()
                if not me:Speak(hero, line, 0, false, true, false) then
                    goto LAB_00d63ad2
                end
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00d63ad2 end
                end
                if not quest:IsActiveThreadTerminating() then goto LAB_00d633f7 end
                goto LAB_00d635e3
            end
            goto FLOW_past_lab_00d635e3
            ::LAB_00d635e3::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie4)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            do return end
            ::FLOW_past_lab_00d635e3::
            ::LAB_00d633f7::
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer2 < 0 do
                quest:NewScriptFrame(me)
                if not quest:IsActiveThreadTerminating() then
                    questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                else
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie4
                    ReleaseEverything(); do return end
                    questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d635e3 end
            if questionAnswer2 ~= 1 then
                local movie5 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue40 = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scratchValue40) then
                    line = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION_NO"
                    hero = quest:GetHero()
                    if not me:Speak(hero, line, 0, false, true, false) then
                        goto LAB_00d63aec
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d63aec end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63aec end
                    goto FLOW_past_lab_00d63aec
                    ::LAB_00d63aec::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    do return end
                    ::FLOW_past_lab_00d63aec::
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie5)
                goto LAB_00d638c8
            end
            goto FLOW_past_lab_00d63ad2
            ::LAB_00d63ad2::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie4)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            do return end
            ::FLOW_past_lab_00d63ad2::
            getHeroGold = quest:GetHeroGold()
            if quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost) <= getHeroGold then
                if not quest:IsActiveThreadTerminating() then
                    local scratchValue46 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue46) then
                        line = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES"
                        hero = quest:GetHero()
                        if not me:Speak(hero, line, 0, false, true, false) then
                            goto LAB_00d635e3
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d635e3 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d63ad2 end
                    end
                    quest:GiveHeroObject("OBJECT_HAND_LAMP", -1, false)
                    quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                    local scratchValue34 = math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost)))
                    quest:GiveHeroGold(scratchValue34)
                    local scratchValue35 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost)))
                    quest:EntityGiveGold(me, scratchValue35)
                    holdingArtifact = false
                    local position3 = quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos()
                    me:MoveToPosition(position3, 1.0, ENTITY_MOVE_WALK, false, true)
                    goto LAB_00d638c8
                end
                goto LAB_00d63ad2
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d635e3 end
            scratchValue41 = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(scratchValue41) then
                line = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD"
                hero = quest:GetHero()
                if not me:Speak(hero, line, 0, false, true, false) then
                    goto LAB_00d63ad2
                end
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00d63ad2 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d635e3 end
            end
            ::LAB_00d638c8::
            quest:PauseAllNonScriptedEntities(false)
            movie = movie4
            ::LAB_00d638d8::
            resources:DestroyMovie(movie)
            getStateBool = holdingArtifact
            ::continue_3::
        until false
    end
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d638ec::
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
        if me:MsgIsHitByHero() then
            goto LAB_00d63b21
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d63b21 end
            predicateResult = false
        end
        goto FLOW_past_lab_00d63b21
        ::LAB_00d63b21::
        predicateResult = true
        ::FLOW_past_lab_00d63b21::
        if predicateResult then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            if notAttacked then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
                local conversationId2 = quest:AddNewConversation(me, false, false)
                hero = quest:GetHero()
                quest:AddPersonToConversation(conversationId2, hero)
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_ATTACK", me, hero, false)
                notAttacked = false
                quest:EntitySetAsKillable(me, true, true)
                local position4 = quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos()
                me:MoveToPosition(position4, 1.0, ENTITY_MOVE_RUN, false, true)
            end
        end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:FadeOutAndKillEntity(me, true, 1.0, true)
    end
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
end

-- ArtifactThief.Init (retail 0x00d62410)
function Init(quest, me)
    quest:AddItemToContainer(me, "OBJECT_HAND_LAMP")
end

-- ArtifactThief.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- ArtifactThief.OnPredicateFail (retail 0x00d62450)
function OnPredicateFail(quest, me)
end

