-- Readable native conversion: ArtifactThief. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_LampCost = 3860,  -- 50.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local holdingArtifact, alreadyTalkedTo, notAttacked

-- ArtifactThief.Main (retail 0x00d62480)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult4, predicateResult20, getStateBool, questionAnswer, questionAnswer2, movie
    local scratchValue, scratchValue41, scratchValue42, scratchValue43, movie4, resource, timerId
    local function ReleaseEverything()
        resources:DestroyMovie(movie)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything2()
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    scratchValue42 = 0
    scratchValue43 = 0
    if not quest:NewScriptFrame(me) then return end
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d63c9f end
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
            if not quest:NewScriptFrame(me) then goto LAB_00d63c96 end
            if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1 then
                local conversationId = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId, hero)
                if not alreadyTalkedTo then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, true, false, false)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_FIRST", me, hero, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_SECOND", me, hero, false)
                end
                quest:SetTimer(timerId, 10)
                scratchValue42 = scratchValue43
            end
            scratchValue = scratchValue42 | 1
            scratchValue43 = scratchValue
            if me:MsgIsHitByHero() then
                goto LAB_00d6280c
            else
                scratchValue = scratchValue42 | 3
                scratchValue43 = scratchValue
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue = scratchValue42 | 7
                    scratchValue43 = scratchValue
                    if not me:MsgIsHitByHeroSpecialAbility(me) then goto LAB_00d6280c end
                end
                predicateResult4 = false
            end
            goto FLOW_past_lab_00d6280c
            ::LAB_00d6280c::
            predicateResult4 = true
            ::FLOW_past_lab_00d6280c::
            if scratchValue & 4 ~= 0 then
                scratchValue = scratchValue & 0xfffffffb
                scratchValue43 = scratchValue
            end
            if scratchValue & 2 ~= 0 then
                scratchValue = scratchValue & 0xfffffffd
                scratchValue43 = scratchValue
            end
            if scratchValue & 1 ~= 0 then
                scratchValue = scratchValue & 0xfffffffe
                scratchValue43 = scratchValue
            end
            scratchValue42 = scratchValue
            if predicateResult4 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                notAttacked = false
                if not alreadyTalkedTo then
                    local movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_PRE_HIT", GROUP_SELECT_FIRST, false, true, false)
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
                            goto LAB_00d63c96
                        end
                    end
                    quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                    quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): this_01 = (CPhysicsMeshInfo *)xStack_138;
                else
                    local movie6 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_HIT", GROUP_SELECT_FIRST, false, true, false)
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
                            goto LAB_00d63c96
                        end
                    end
                    quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                    quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): this_01 = (CPhysicsMeshInfo *)xStack_bc;
                end
                quest:EntitySetAsKillable(me, true, true)
                me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 1.0, ENTITY_MOVE_RUN, false, true)
                scratchValue42 = scratchValue43
            end
            if not me:IsTalkedToByHero() then getStateBool = holdingArtifact; goto continue_3 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
            if not holdingArtifact then goto LAB_00d638ec end
            if not alreadyTalkedTo then
                alreadyTalkedTo = true
                local movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 >= quest:GetHealth(resources:ScriptThing(resource)) then goto LAB_00d62e38 end
                if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_CHAT", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d63a7c end
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
                if not quest:IsActiveThreadTerminating() then
                    local predicateResult = quest:IsActiveThreadTerminating()
                    if questionAnswer == 1 then
                        if not predicateResult then
                            if quest:GetHeroGold() < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost) then
                                if not quest:IsActiveThreadTerminating() then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                        if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d63a7c end
                                        goto LAB_00d632b4
                                    end
                                    goto LAB_00d632c3
                                end
                                goto LAB_00d63a7c
                            end
                            if not quest:IsActiveThreadTerminating() then
                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                    if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d63a7c end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                end
                                quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                                quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                                quest:GiveHeroGold(math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost))))
                                quest:EntityGiveGold(me, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_LampCost))))
                                holdingArtifact = false
                                me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 1.0, ENTITY_MOVE_WALK, false, true)
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie3
                                goto LAB_00d638d8
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie3)
                        goto LAB_00d63c96
                    end
                    if not predicateResult then
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d63a7c end
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
                end
                ::FLOW_past_lab_00d62e38::
                ::LAB_00d63a7c::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                goto LAB_00d63c96
            end
            movie4 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d63ad2 end
                if not quest:IsActiveThreadTerminating() then goto LAB_00d633f7 end
                goto LAB_00d635e3
            end
            goto FLOW_past_lab_00d635e3
            ::LAB_00d635e3::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie4)
            goto LAB_00d63c96
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
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d63aec end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63aec end
                    goto FLOW_past_lab_00d63aec
                    ::LAB_00d63aec::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00d63c96
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
            goto LAB_00d63c96
            ::FLOW_past_lab_00d63ad2::
            -- TODO(native): if *(iVar6 + 0xf14) <= xStack_128 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d635e3 end
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d63ad2 end
                if quest:IsActiveThreadTerminating() then goto LAB_00d635e3 end
            end
            ::LAB_00d638c8::
            quest:PauseAllNonScriptedEntities(false)
            movie = movie4
            ::LAB_00d638d8::
            resources:DestroyMovie(movie)
            scratchValue42 = scratchValue43
            getStateBool = holdingArtifact
            ::continue_3::
        until false
    end
    goto LAB_00d63c9f
    ::LAB_00d638ec::
    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then goto LAB_00d63c96 end
        scratchValue41 = scratchValue42 | 8
        scratchValue43 = scratchValue41
        if me:MsgIsHitByHero() then
            goto LAB_00d63b21
        else
            scratchValue41 = scratchValue42 | 24
            scratchValue43 = scratchValue41
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue41 = scratchValue42 | 56
                scratchValue43 = scratchValue41
                if not me:MsgIsHitByHeroSpecialAbility(me) then goto LAB_00d63b21 end
            end
            predicateResult20 = false
        end
        goto FLOW_past_lab_00d63b21
        ::LAB_00d63b21::
        predicateResult20 = true
        ::FLOW_past_lab_00d63b21::
        if scratchValue41 & 32 ~= 0 then
            scratchValue41 = scratchValue41 & 0xffffffdf
            scratchValue43 = scratchValue41
        end
        if scratchValue41 & 16 ~= 0 then
            scratchValue41 = scratchValue41 & 0xffffffef
            scratchValue43 = scratchValue41
        end
        if scratchValue41 & 8 ~= 0 then
            scratchValue41 = scratchValue41 & 0xfffffff7
            scratchValue43 = scratchValue41
        end
        scratchValue42 = scratchValue41
        if predicateResult20 then
            if notAttacked then
                local conversationId2 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId2, hero)
                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_ATTACK", me, hero, false)
                notAttacked = false
                quest:EntitySetAsKillable(me, true, true)
                me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 1.0, ENTITY_MOVE_RUN, false, true)
                scratchValue42 = scratchValue43
            end
        end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:FadeOutAndKillEntity(me, true, 1.0, true)
    end
    ::LAB_00d63c96::
    quest:DeregisterTimer(timerId)
    ::LAB_00d63c9f::
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

