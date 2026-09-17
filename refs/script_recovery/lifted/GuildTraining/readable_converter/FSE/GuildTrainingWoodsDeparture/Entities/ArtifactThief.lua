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
    local resources = quest:RetailResources()
    local scratchValue2, scratchValue3, scratchValue4, scratchValue7, scratchValue8, scratchValue10
    local scratchValue12, scratchValue13, scratchValue14, scratchValue15, scratchValue16
    local scratchValue17, scratchValue18, timerId, scratchValue19, scratchValue21
    local function __region_LAB_00d632c3_c2()
        quest:PauseAllNonScriptedEntities(false)
        scratchValue10 = scratchValue16
    end
    local function __cleanup_LAB_00d63aa6()
        resources:DestroyMovie(scratchValue10)
        quest:DeregisterTimer(timerId)
        resources:DestroyMovie(scratchValue18)
    end
    local function __cleanup_LAB_00d63aab()
        quest:DeregisterTimer(timerId)
        resources:DestroyMovie(scratchValue18)
    end
    scratchValue13 = 0
    scratchValue14 = 0
    if not quest:NewScriptFrame(me) then return end
    scratchValue18 = resources:NewResource()
    scratchValue2 = resources:TryAcquire(scratchValue18, me, 4)
    while not scratchValue2 do
        if not quest:NewScriptFrame(me) then goto LAB_00d63c9f end
        scratchValue2 = resources:TryAcquire(scratchValue18, me, 4)
    end
    if not quest:IsActiveThreadTerminating() then
        quest:EntitySetAsKillable(me, false, true)
        quest:SetThingHasInformation(me, false, true, false)
        state:SetBool("HoldingArtifact", true)
        state:SetBool("AlreadyTalkedTo", false)
        state:SetBool("NotAttacked", true)
        timerId = quest:RegisterTimer()
        quest:SetTimer(timerId, 0)
        scratchValue3 = state:GetBool("HoldingArtifact")
        repeat
            if not scratchValue3 or not state:GetBool("NotAttacked") then goto LAB_00d638ec end
            if not quest:NewScriptFrame(me) then goto LAB_00d63c96 end
            if quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(timerId) < 1 then
                scratchValue8 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue8, quest:GetHero())
                if not state:GetBool("AlreadyTalkedTo") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                    me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, true, false, false)
                    quest:AddLineToConversation(scratchValue8, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_FIRST", me, quest:GetHero(), false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                    quest:AddLineToConversation(scratchValue8, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_SECOND", me, quest:GetHero(), false)
                end
                quest:SetTimer(timerId, 10)
                scratchValue13 = scratchValue14
            end
            scratchValue12 = scratchValue13 | 1
            scratchValue14 = scratchValue12
            if me:MsgIsHitByHero() then
                scratchValue2 = true
            else
                scratchValue12 = scratchValue13 | 3
                scratchValue14 = scratchValue12
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue12 = scratchValue13 | 7
                    scratchValue14 = scratchValue12
                    if not me:MsgIsHitByHeroSpecialAbility(me) then
                        scratchValue2 = true
                        goto FLOW_after_lab_00d6280c
                    end
                end
                scratchValue2 = false
            end
            ::FLOW_after_lab_00d6280c::
            if scratchValue12 & 4 ~= 0 then
                scratchValue12 = scratchValue12 & 0xfffffffb
                scratchValue14 = scratchValue12
            end
            if scratchValue12 & 2 ~= 0 then
                scratchValue12 = scratchValue12 & 0xfffffffd
                scratchValue14 = scratchValue12
            end
            if scratchValue12 & 1 ~= 0 then
                scratchValue12 = scratchValue12 & 0xfffffffe
                scratchValue14 = scratchValue12
            end
            scratchValue13 = scratchValue12
            if scratchValue2 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                state:SetBool("NotAttacked", false)
                if not state:GetBool("AlreadyTalkedTo") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                    scratchValue15 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(5.5 ~= 0)
                    scratchValue4 = 0.0
                    if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue18)) then
                        me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_PRE_HIT", 0, false, true, false)
                        scratchValue3 = me:IsPerformingScriptTask()
                        while scratchValue3 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue15)
                                __cleanup_LAB_00d63aab(); return
                            end
                            scratchValue3 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue15)
                            goto LAB_00d63c96
                        end
                    end
                    quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                    quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (xStack_138);
                    -- TODO(native): this_01 = (CPhysicsMeshInfo *)xStack_138;
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                    scratchValue21 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(scratchValue4 ~= 0)
                    scratchValue4 = 0.0
                    if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue18)) then
                        me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_HIT", 0, false, true, false)
                        scratchValue3 = me:IsPerformingScriptTask()
                        while scratchValue3 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue21)
                                __cleanup_LAB_00d63aab(); return
                            end
                            scratchValue3 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue21)
                            goto LAB_00d63c96
                        end
                    end
                    quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                    quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (xStack_bc);
                    -- TODO(native): this_01 = (CPhysicsMeshInfo *)xStack_bc;
                end
                quest:EntitySetAsKillable(me, true, true)
                me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 0x3f800000, 1, false, true)
                scratchValue13 = scratchValue14
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                if not state:GetBool("HoldingArtifact") then goto LAB_00d638ec end
                if not state:GetBool("AlreadyTalkedTo") then
                    state:SetBool("AlreadyTalkedTo", true)
                    scratchValue16 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue18)) then
                        me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_CHAT", 0, false, true, false)
                        scratchValue3 = me:IsPerformingScriptTask()
                        while scratchValue3 do
                            if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                            scratchValue3 = me:IsPerformingScriptTask()
                        end
                        if not quest:IsActiveThreadTerminating() then
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while scratchValue7 < 0 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    scratchValue10 = scratchValue16
                                    resources:DestroyMovie(scratchValue10)
                                    quest:DeregisterTimer(timerId)
                                    resources:DestroyMovie(scratchValue18)
                                    return
                                end
                                scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            if not quest:IsActiveThreadTerminating() then
                                scratchValue2 = quest:IsActiveThreadTerminating()
                                if scratchValue7 == 1 then
                                    if not scratchValue2 then
                                        if quest:GetHeroGold() < quest:ReadGlobalGameDataFloat(3860) then
                                            if not quest:IsActiveThreadTerminating() then
                                                scratchValue4 = 0.0
                                                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue18)) then
                                                    me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", 0, false, true, false)
                                                    scratchValue3 = me:IsPerformingScriptTask()
                                                    while scratchValue3 do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                                        scratchValue3 = me:IsPerformingScriptTask()
                                                    end
                                                    -- TODO(native): goto LAB_00d632b4_c2
                                                end
                                                __region_LAB_00d632c3_c2(); goto LAB_00d638d8
                                            end
                                            goto LAB_00d63a7c
                                        end
                                        if not quest:IsActiveThreadTerminating() then
                                            scratchValue4 = 0.0
                                            if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue18)) then
                                                me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES", 0, false, true, false)
                                                scratchValue3 = me:IsPerformingScriptTask()
                                                while scratchValue3 do
                                                    if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                                    scratchValue3 = me:IsPerformingScriptTask()
                                                end
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                            end
                                            quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                                            quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                                            quest:GiveHeroGold(math.modf(-quest:ReadGlobalGameDataFloat(3860)))
                                            quest:EntityGiveGold(me, math.modf(quest:ReadGlobalGameDataFloat(3860)))
                                            state:SetBool("HoldingArtifact", false)
                                            me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 0x3f800000, 0, false, true)
                                            quest:PauseAllNonScriptedEntities(false)
                                            scratchValue10 = scratchValue16
                                            goto LAB_00d638d8
                                        end
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue16)
                                    goto LAB_00d63c96
                                end
                                if not scratchValue2 then
                                    scratchValue4 = 0.0
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue18)) then
                                        me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO", 0, false, true, false)
                                        scratchValue3 = me:IsPerformingScriptTask()
                                        while scratchValue3 do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                            scratchValue3 = me:IsPerformingScriptTask()
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
                        scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while scratchValue7 < 0 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                scratchValue10 = scratchValue16
                                __cleanup_LAB_00d63aa6()
                                return
                            end
                            scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if not quest:IsActiveThreadTerminating() then
                            scratchValue2 = quest:IsActiveThreadTerminating()
                            if scratchValue7 == 1 then
                                if not scratchValue2 then
                                    if quest:GetHeroGold() < quest:ReadGlobalGameDataFloat(3860) then
                                        if not quest:IsActiveThreadTerminating() then
                                            scratchValue4 = 0.0
                                            if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue18)) then
                                                me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", 0, false, true, false)
                                                scratchValue3 = me:IsPerformingScriptTask()
                                                while scratchValue3 do
                                                    if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                                    scratchValue3 = me:IsPerformingScriptTask()
                                                end
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                                quest:PauseAllNonScriptedEntities(false)
                                                scratchValue10 = scratchValue16
                                                goto LAB_00d638d8
                                            end
                                            quest:PauseAllNonScriptedEntities(false)
                                            scratchValue10 = scratchValue16
                                            goto LAB_00d638d8
                                        end
                                        goto LAB_00d63a7c
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        scratchValue4 = 0.0
                                        if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue18)) then
                                            me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES", 0, false, true, false)
                                            scratchValue3 = me:IsPerformingScriptTask()
                                            while scratchValue3 do
                                                if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                                scratchValue3 = me:IsPerformingScriptTask()
                                            end
                                            if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                        end
                                        quest:GiveHeroObject("OBJECT_HAND_LAMP", -1)
                                        quest:RemoveItemFromContainer(me, "OBJECT_HAND_LAMP")
                                        quest:GiveHeroGold(math.modf(-quest:ReadGlobalGameDataFloat(3860)))
                                        quest:EntityGiveGold(me, math.modf(quest:ReadGlobalGameDataFloat(3860)))
                                        state:SetBool("HoldingArtifact", false)
                                        me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 0x3f800000, 0, false, true)
                                        quest:PauseAllNonScriptedEntities(false)
                                        scratchValue10 = scratchValue16
                                        goto LAB_00d638d8
                                    end
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue16)
                                goto LAB_00d63c96
                            end
                            if not scratchValue2 then
                                scratchValue4 = 0.0
                                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue18)) then
                                    me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO", 0, false, true, false)
                                    scratchValue3 = me:IsPerformingScriptTask()
                                    while scratchValue3 do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                        scratchValue3 = me:IsPerformingScriptTask()
                                    end
                                    -- LAB_00d632b4: (native jump target)
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                scratchValue10 = scratchValue16
                                goto LAB_00d638d8
                            end
                        end
                    end
                    ::FLOW_after_lab_00d62e38::
                    ::LAB_00d63a7c::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue16)
                    goto LAB_00d63c96
                end
                scratchValue17 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue18)) then
                    me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN", 0, false, true, false)
                    scratchValue3 = me:IsPerformingScriptTask()
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue17)
                            goto LAB_00d63c96
                        end
                        scratchValue3 = me:IsPerformingScriptTask()
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue17)
                        goto LAB_00d63c96
                    end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                while scratchValue7 < 0 do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        scratchValue10 = scratchValue17
                        __cleanup_LAB_00d63aa6(); return
                    end
                    scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue17)
                    goto LAB_00d63c96
                end
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue7 ~= 1 then
                    if scratchValue2 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue17)
                        goto LAB_00d63c96
                    end
                    scratchValue19 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(0.0 ~= 0)
                    scratchValue4 = 0.0
                    if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue18)) then
                        me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION_NO", 0, false, true, false)
                        scratchValue3 = me:IsPerformingScriptTask()
                        while scratchValue3 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue19)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:ReleaseResource(scratchValue17)
                                goto LAB_00d63c96
                            end
                            scratchValue3 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue19)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(scratchValue17)
                            goto LAB_00d63c96
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue19)
                    goto LAB_00d638c8
                end
                if scratchValue2 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue17)
                    goto LAB_00d63c96
                end
                -- TODO(native): if *(iVar6 + 0xf14) <= xStack_128 then
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue17)
                    goto LAB_00d63c96
                end
                scratchValue4 = 0.0
                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue18)) then
                    me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", 0, false, true, false)
                    scratchValue3 = me:IsPerformingScriptTask()
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue17)
                            goto LAB_00d63c96
                        end
                        scratchValue3 = me:IsPerformingScriptTask()
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue17)
                        goto LAB_00d63c96
                    end
                end
                ::LAB_00d638c8::
                quest:PauseAllNonScriptedEntities(false)
                scratchValue10 = scratchValue17
                ::LAB_00d638d8::
                resources:DestroyMovie(scratchValue10)
                scratchValue13 = scratchValue14
            end
            scratchValue3 = state:GetBool("HoldingArtifact")
        until false
    end
    goto LAB_00d63c9f
    ::LAB_00d638ec::
    if not quest:IsActiveThreadTerminating() then
        scratchValue3 = me:IsPerformingScriptTask()
        while scratchValue3 do
            if not quest:NewScriptFrame(me) then goto LAB_00d63c96 end
            scratchValue12 = scratchValue13 | 8
            scratchValue14 = scratchValue12
            if me:MsgIsHitByHero() then
                scratchValue2 = true
            else
                scratchValue12 = scratchValue13 | 24
                scratchValue14 = scratchValue12
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue12 = scratchValue13 | 56
                    scratchValue14 = scratchValue12
                    if not me:MsgIsHitByHeroSpecialAbility(me) then
                        scratchValue2 = true
                        goto FLOW_after_lab_00d63b21
                    end
                end
                scratchValue2 = false
            end
            ::FLOW_after_lab_00d63b21::
            if scratchValue12 & 32 ~= 0 then
                scratchValue12 = scratchValue12 & 0xffffffdf
                scratchValue14 = scratchValue12
            end
            if scratchValue12 & 16 ~= 0 then
                scratchValue12 = scratchValue12 & 0xffffffef
                scratchValue14 = scratchValue12
            end
            if scratchValue12 & 8 ~= 0 then
                scratchValue12 = scratchValue12 & 0xfffffff7
                scratchValue14 = scratchValue12
            end
            scratchValue13 = scratchValue12
            if scratchValue2 then
                if state:GetBool("NotAttacked") then
                    scratchValue8 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue8, quest:GetHero())
                    quest:AddLineToConversation(scratchValue8, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_ATTACK", me, quest:GetHero(), false)
                    state:SetBool("NotAttacked", false)
                    quest:EntitySetAsKillable(me, true, true)
                    me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 0x3f800000, 1, false, true)
                    scratchValue13 = scratchValue14
                end
            end
            scratchValue3 = me:IsPerformingScriptTask()
        end
        if not quest:IsActiveThreadTerminating() then
            quest:FadeOutAndKillEntity(me, true, 1.0, true)
        end
    end
    ::LAB_00d63c96::
    quest:DeregisterTimer(timerId)
    ::LAB_00d63c9f::
    resources:ReleaseResource(scratchValue18)
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

