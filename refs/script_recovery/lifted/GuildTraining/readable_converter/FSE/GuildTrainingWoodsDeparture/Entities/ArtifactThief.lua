-- Readable native conversion: ArtifactThief. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- ArtifactThief.Main (retail 0x00d62480)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue2, getStateBool, scratchValue3, scratchValue6, addNewConversation, movie
    local scratchValue9, scratchValue, scratchValue11, movie2, movie3, movie4, resource, timerId
    local movie5, movie6
    local function __region_LAB_00d632c3_c2()
        quest:PauseAllNonScriptedEntities(false)
        movie = movie3
    end
    local function __cleanup_LAB_00d63aa6()
        resources:DestroyMovie(movie)
        quest:DeregisterTimer(timerId)
        resources:DestroyMovie(resource)
    end
    local function __cleanup_LAB_00d63aab()
        quest:DeregisterTimer(timerId)
        resources:DestroyMovie(resource)
    end
    scratchValue = 0
    scratchValue11 = 0
    if not quest:NewScriptFrame(me) then return end
    resource = resources:NewResource()
    scratchValue2 = resources:TryAcquire(resource, me, 4)
    while not scratchValue2 do
        if not quest:NewScriptFrame(me) then goto LAB_00d63c9f end
        scratchValue2 = resources:TryAcquire(resource, me, 4)
    end
    if not quest:IsActiveThreadTerminating() then
        quest:EntitySetAsKillable(me, false, true)
        quest:SetThingHasInformation(me, false, true, false)
        state:SetBool("HoldingArtifact", true)
        state:SetBool("AlreadyTalkedTo", false)
        state:SetBool("NotAttacked", true)
        timerId = quest:RegisterTimer()
        quest:SetTimer(timerId, 0)
        getStateBool = state:GetBool("HoldingArtifact")
        repeat
            if not getStateBool or not state:GetBool("NotAttacked") then goto LAB_00d638ec end
            if not quest:NewScriptFrame(me) then goto LAB_00d63c96 end
            if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1 then
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                if not state:GetBool("AlreadyTalkedTo") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, true, false, false)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_FIRST", me, hero, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_SECOND", me, hero, false)
                end
                quest:SetTimer(timerId, 10)
                scratchValue = scratchValue11
            end
            scratchValue9 = scratchValue | 1
            scratchValue11 = scratchValue9
            if me:MsgIsHitByHero() then
                scratchValue2 = true
            else
                scratchValue9 = scratchValue | 3
                scratchValue11 = scratchValue9
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue9 = scratchValue | 7
                    scratchValue11 = scratchValue9
                    if not me:MsgIsHitByHeroSpecialAbility(me) then
                        scratchValue2 = true
                        goto FLOW_after_lab_00d6280c
                    end
                end
                scratchValue2 = false
            end
            ::FLOW_after_lab_00d6280c::
            if scratchValue9 & 4 ~= 0 then
                scratchValue9 = scratchValue9 & 0xfffffffb
                scratchValue11 = scratchValue9
            end
            if scratchValue9 & 2 ~= 0 then
                scratchValue9 = scratchValue9 & 0xfffffffd
                scratchValue11 = scratchValue9
            end
            if scratchValue9 & 1 ~= 0 then
                scratchValue9 = scratchValue9 & 0xfffffffe
                scratchValue11 = scratchValue9
            end
            scratchValue = scratchValue9
            if scratchValue2 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                state:SetBool("NotAttacked", false)
                if not state:GetBool("AlreadyTalkedTo") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                    movie2 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(5.5 ~= 0)
                    scratchValue3 = 0.0
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_PRE_HIT", 0, false, true, false)
                        getStateBool = me:IsPerformingScriptTask()
                        while getStateBool do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                __cleanup_LAB_00d63aab(); return
                            end
                            getStateBool = me:IsPerformingScriptTask()
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
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                    movie6 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(scratchValue3 ~= 0)
                    scratchValue3 = 0.0
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_HIT", 0, false, true, false)
                        getStateBool = me:IsPerformingScriptTask()
                        while getStateBool do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie6)
                                __cleanup_LAB_00d63aab(); return
                            end
                            getStateBool = me:IsPerformingScriptTask()
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
                me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 0x3f800000, 1, false, true)
                scratchValue = scratchValue11
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                if not state:GetBool("HoldingArtifact") then goto LAB_00d638ec end
                if not state:GetBool("AlreadyTalkedTo") then
                    state:SetBool("AlreadyTalkedTo", true)
                    movie3 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_CHAT", 0, false, true, false)
                        getStateBool = me:IsPerformingScriptTask()
                        while getStateBool do
                            if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                            getStateBool = me:IsPerformingScriptTask()
                        end
                        if not quest:IsActiveThreadTerminating() then
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            scratchValue6 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while scratchValue6 < 0 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    movie = movie3
                                    resources:DestroyMovie(movie)
                                    quest:DeregisterTimer(timerId)
                                    resources:DestroyMovie(resource)
                                    return
                                end
                                scratchValue6 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            if not quest:IsActiveThreadTerminating() then
                                scratchValue2 = quest:IsActiveThreadTerminating()
                                if scratchValue6 == 1 then
                                    if not scratchValue2 then
                                        if quest:GetHeroGold() < quest:ReadGlobalGameDataFloat(3860) then
                                            if not quest:IsActiveThreadTerminating() then
                                                scratchValue3 = 0.0
                                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                                    me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", 0, false, true, false)
                                                    getStateBool = me:IsPerformingScriptTask()
                                                    while getStateBool do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                                        getStateBool = me:IsPerformingScriptTask()
                                                    end
                                                    -- TODO(native): goto LAB_00d632b4_c2
                                                end
                                                __region_LAB_00d632c3_c2(); goto LAB_00d638d8
                                            end
                                            goto LAB_00d63a7c
                                        end
                                        if not quest:IsActiveThreadTerminating() then
                                            scratchValue3 = 0.0
                                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                                me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES", 0, false, true, false)
                                                getStateBool = me:IsPerformingScriptTask()
                                                while getStateBool do
                                                    if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                                    getStateBool = me:IsPerformingScriptTask()
                                                end
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                            end
                                            quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                                            quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                                            quest:GiveHeroGold(math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(3860))))
                                            quest:EntityGiveGold(me, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(3860))))
                                            state:SetBool("HoldingArtifact", false)
                                            me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 0x3f800000, 0, false, true)
                                            quest:PauseAllNonScriptedEntities(false)
                                            movie = movie3
                                            goto LAB_00d638d8
                                        end
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d63c96
                                end
                                if not scratchValue2 then
                                    scratchValue3 = 0.0
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                        me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO", 0, false, true, false)
                                        getStateBool = me:IsPerformingScriptTask()
                                        while getStateBool do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                            getStateBool = me:IsPerformingScriptTask()
                                        end
                                        -- LAB_00d632b4_c2: (native jump target)
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                    end
                                    __region_LAB_00d632c3_c2()
                                    goto LAB_00d638d8
                                end
                            end
                            goto FLOW_after_lab_00d62e38
                        end
                    else
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        scratchValue6 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while scratchValue6 < 0 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie3
                                __cleanup_LAB_00d63aa6()
                                return
                            end
                            scratchValue6 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if not quest:IsActiveThreadTerminating() then
                            scratchValue2 = quest:IsActiveThreadTerminating()
                            if scratchValue6 == 1 then
                                if not scratchValue2 then
                                    if quest:GetHeroGold() < quest:ReadGlobalGameDataFloat(3860) then
                                        if not quest:IsActiveThreadTerminating() then
                                            scratchValue3 = 0.0
                                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                                me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", 0, false, true, false)
                                                getStateBool = me:IsPerformingScriptTask()
                                                while getStateBool do
                                                    if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                                    getStateBool = me:IsPerformingScriptTask()
                                                end
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                                quest:PauseAllNonScriptedEntities(false)
                                                movie = movie3
                                                goto LAB_00d638d8
                                            end
                                            quest:PauseAllNonScriptedEntities(false)
                                            movie = movie3
                                            goto LAB_00d638d8
                                        end
                                        goto LAB_00d63a7c
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        scratchValue3 = 0.0
                                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                            me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES", 0, false, true, false)
                                            getStateBool = me:IsPerformingScriptTask()
                                            while getStateBool do
                                                if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                                getStateBool = me:IsPerformingScriptTask()
                                            end
                                            if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                        end
                                        quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                                        quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                                        quest:GiveHeroGold(math.tointeger(math.modf(-quest:ReadGlobalGameDataFloat(3860))))
                                        quest:EntityGiveGold(me, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(3860))))
                                        state:SetBool("HoldingArtifact", false)
                                        me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 0x3f800000, 0, false, true)
                                        quest:PauseAllNonScriptedEntities(false)
                                        movie = movie3
                                        goto LAB_00d638d8
                                    end
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00d63c96
                            end
                            if not scratchValue2 then
                                scratchValue3 = 0.0
                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                    me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO", 0, false, true, false)
                                    getStateBool = me:IsPerformingScriptTask()
                                    while getStateBool do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                        getStateBool = me:IsPerformingScriptTask()
                                    end
                                    -- LAB_00d632b4: (native jump target)
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                movie = movie3
                                goto LAB_00d638d8
                            end
                        end
                    end
                    ::FLOW_after_lab_00d62e38::
                    ::LAB_00d63a7c::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                    goto LAB_00d63c96
                end
                movie4 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN", 0, false, true, false)
                    getStateBool = me:IsPerformingScriptTask()
                    while getStateBool do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            goto LAB_00d63c96
                        end
                        getStateBool = me:IsPerformingScriptTask()
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        goto LAB_00d63c96
                    end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                scratchValue6 = quest:MsgIsQuestionAnsweredYesOrNo()
                while scratchValue6 < 0 do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie4
                        __cleanup_LAB_00d63aa6(); return
                    end
                    scratchValue6 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00d63c96
                end
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue6 ~= 1 then
                    if scratchValue2 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        goto LAB_00d63c96
                    end
                    movie5 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(0.0 ~= 0)
                    scratchValue3 = 0.0
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION_NO", 0, false, true, false)
                        getStateBool = me:IsPerformingScriptTask()
                        while getStateBool do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie5)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:ReleaseResource(movie4)
                                goto LAB_00d63c96
                            end
                            getStateBool = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie5)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(movie4)
                            goto LAB_00d63c96
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    goto LAB_00d638c8
                end
                if scratchValue2 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00d63c96
                end
                -- TODO(native): if *(iVar6 + 0xf14) <= xStack_128 then
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00d63c96
                end
                scratchValue3 = 0.0
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", 0, false, true, false)
                    getStateBool = me:IsPerformingScriptTask()
                    while getStateBool do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            goto LAB_00d63c96
                        end
                        getStateBool = me:IsPerformingScriptTask()
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        goto LAB_00d63c96
                    end
                end
                ::LAB_00d638c8::
                quest:PauseAllNonScriptedEntities(false)
                movie = movie4
                ::LAB_00d638d8::
                resources:DestroyMovie(movie)
                scratchValue = scratchValue11
            end
            getStateBool = state:GetBool("HoldingArtifact")
        until false
    end
    goto LAB_00d63c9f
    ::LAB_00d638ec::
    if not quest:IsActiveThreadTerminating() then
        getStateBool = me:IsPerformingScriptTask()
        while getStateBool do
            if not quest:NewScriptFrame(me) then goto LAB_00d63c96 end
            scratchValue9 = scratchValue | 8
            scratchValue11 = scratchValue9
            if me:MsgIsHitByHero() then
                scratchValue2 = true
            else
                scratchValue9 = scratchValue | 24
                scratchValue11 = scratchValue9
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue9 = scratchValue | 56
                    scratchValue11 = scratchValue9
                    if not me:MsgIsHitByHeroSpecialAbility(me) then
                        scratchValue2 = true
                        goto FLOW_after_lab_00d63b21
                    end
                end
                scratchValue2 = false
            end
            ::FLOW_after_lab_00d63b21::
            if scratchValue9 & 32 ~= 0 then
                scratchValue9 = scratchValue9 & 0xffffffdf
                scratchValue11 = scratchValue9
            end
            if scratchValue9 & 16 ~= 0 then
                scratchValue9 = scratchValue9 & 0xffffffef
                scratchValue11 = scratchValue9
            end
            if scratchValue9 & 8 ~= 0 then
                scratchValue9 = scratchValue9 & 0xfffffff7
                scratchValue11 = scratchValue9
            end
            scratchValue = scratchValue9
            if scratchValue2 then
                if state:GetBool("NotAttacked") then
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_ATTACK", me, hero, false)
                    state:SetBool("NotAttacked", false)
                    quest:EntitySetAsKillable(me, true, true)
                    me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 0x3f800000, 1, false, true)
                    scratchValue = scratchValue11
                end
            end
            getStateBool = me:IsPerformingScriptTask()
        end
        if not quest:IsActiveThreadTerminating() then
            quest:FadeOutAndKillEntity(me, true, 1.0, true)
        end
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
function OnPersist(quest, context)
end

-- ArtifactThief.OnPredicateFail (retail 0x00d62450)
function OnPredicateFail(quest, me)
end

