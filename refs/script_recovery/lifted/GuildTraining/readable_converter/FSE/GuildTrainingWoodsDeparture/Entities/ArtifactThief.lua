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
    local scratchValue2, scratchValue3, scratchValue7, scratchValue8, scratchValue11, scratchValue13
    local scratchValue14, scratchValue15, scratchValue16, scratchValue17, scratchValue18
    local scratchValue20, timerId, scratchValue28, scratchValue30
    local function __region_LAB_00d632c3_c2()
        quest:PauseAllNonScriptedEntities(false)
        scratchValue11 = scratchValue17
    end
    local function __cleanup_LAB_00d63aa6()
        resources:DestroyMovie(scratchValue11)
        quest:DeregisterTimer(timerId)
        resources:DestroyMovie(scratchValue20)
    end
    local function __cleanup_LAB_00d63aab()
        quest:DeregisterTimer(timerId)
        resources:DestroyMovie(scratchValue20)
    end
    scratchValue14 = 0
    scratchValue15 = 0
    if not quest:NewScriptFrame(me) then return end
    scratchValue20 = resources:NewResource()
    scratchValue2 = resources:TryAcquire(scratchValue20, me, 4)
    while not scratchValue2 do
        if not quest:NewScriptFrame(me) then goto LAB_00d63c9f end
        scratchValue2 = resources:TryAcquire(scratchValue20, me, 4)
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
                    me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, DAT_01375748, false, false)
                    quest:AddLineToConversation(scratchValue8, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_FIRST", me, quest:GetHero(), false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                    quest:AddLineToConversation(scratchValue8, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_SECOND", me, quest:GetHero(), false)
                end
                quest:SetTimer(timerId, 10)
                scratchValue14 = scratchValue15
            end
            scratchValue13 = scratchValue14 | 1
            scratchValue15 = scratchValue13
            if me:MsgIsHitByHero() then
                scratchValue2 = true
            else
                scratchValue13 = scratchValue14 | 3
                scratchValue15 = scratchValue13
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue13 = scratchValue14 | 7
                    scratchValue15 = scratchValue13
                    if not me:MsgIsHitByHeroSpecialAbility(me) then
                        scratchValue2 = true
                        goto FLOW_after_lab_00d6280c
                    end
                end
                scratchValue2 = false
            end
            ::FLOW_after_lab_00d6280c::
            if scratchValue13 & 4 ~= 0 then
                scratchValue13 = scratchValue13 & 0xfffffffb
                scratchValue15 = scratchValue13
            end
            if scratchValue13 & 2 ~= 0 then
                scratchValue13 = scratchValue13 & 0xfffffffd
                scratchValue15 = scratchValue13
            end
            if scratchValue13 & 1 ~= 0 then
                scratchValue13 = scratchValue13 & 0xfffffffe
                scratchValue15 = scratchValue13
            end
            scratchValue14 = scratchValue13
            if scratchValue2 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                state:SetBool("NotAttacked", false)
                if not state:GetBool("AlreadyTalkedTo") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                    scratchValue16 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:GetHealth(resources:ScriptThing(scratchValue20))
                    if 0.0 < fret_00 then
                        me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_PRE_HIT", 0, false, true, false)
                        scratchValue3 = me:IsPerformingScriptTask()
                        while scratchValue3 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue16)
                                __cleanup_LAB_00d63aab(); return
                            end
                            scratchValue3 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue16)
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
                    scratchValue30 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:GetHealth(resources:ScriptThing(scratchValue20))
                    if 0.0 < fret_0 then
                        me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_HIT", 0, false, true, false)
                        scratchValue3 = me:IsPerformingScriptTask()
                        while scratchValue3 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue30)
                                __cleanup_LAB_00d63aab(); return
                            end
                            scratchValue3 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue30)
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
                scratchValue14 = scratchValue15
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d63c96 end
                if not state:GetBool("HoldingArtifact") then goto LAB_00d638ec end
                if not state:GetBool("AlreadyTalkedTo") then
                    state:SetBool("AlreadyTalkedTo", true)
                    scratchValue17 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:GetHealth(resources:ScriptThing(scratchValue20))
                    if 0.0 < fret_01 then
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
                                    scratchValue11 = scratchValue17
                                    resources:DestroyMovie(scratchValue11)
                                    quest:DeregisterTimer(timerId)
                                    resources:DestroyMovie(scratchValue20)
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
                                                quest:GetHealth(resources:ScriptThing(scratchValue20))
                                                if 0.0 < fret_02 then
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
                                            quest:GetHealth(resources:ScriptThing(scratchValue20))
                                            if 0.0 < fret_03 then
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
                                            scratchValue11 = scratchValue17
                                            goto LAB_00d638d8
                                        end
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue17)
                                    goto LAB_00d63c96
                                end
                                if not scratchValue2 then
                                    quest:GetHealth(resources:ScriptThing(scratchValue20))
                                    if 0.0 < fret_04 then
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
                                scratchValue11 = scratchValue17
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
                                            quest:GetHealth(resources:ScriptThing(scratchValue20))
                                            if 0.0 < fret_02 then
                                                me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", 0, false, true, false)
                                                scratchValue3 = me:IsPerformingScriptTask()
                                                while scratchValue3 do
                                                    if not quest:NewScriptFrame(me) then goto LAB_00d63a7c end
                                                    scratchValue3 = me:IsPerformingScriptTask()
                                                end
                                                if quest:IsActiveThreadTerminating() then goto LAB_00d63a7c end
                                                quest:PauseAllNonScriptedEntities(false)
                                                scratchValue11 = scratchValue17
                                                goto LAB_00d638d8
                                            end
                                            quest:PauseAllNonScriptedEntities(false)
                                            scratchValue11 = scratchValue17
                                            goto LAB_00d638d8
                                        end
                                        goto LAB_00d63a7c
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        quest:GetHealth(resources:ScriptThing(scratchValue20))
                                        if 0.0 < fret_03 then
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
                                        scratchValue11 = scratchValue17
                                        goto LAB_00d638d8
                                    end
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue17)
                                goto LAB_00d63c96
                            end
                            if not scratchValue2 then
                                quest:GetHealth(resources:ScriptThing(scratchValue20))
                                if 0.0 < fret_04 then
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
                                scratchValue11 = scratchValue17
                                goto LAB_00d638d8
                            end
                        end
                    end
                    ::FLOW_after_lab_00d62e38::
                    ::LAB_00d63a7c::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue17)
                    goto LAB_00d63c96
                end
                scratchValue18 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:GetHealth(resources:ScriptThing(scratchValue20))
                if 0.0 < fret_05 then
                    me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN", 0, false, true, false)
                    scratchValue3 = me:IsPerformingScriptTask()
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue18)
                            goto LAB_00d63c96
                        end
                        scratchValue3 = me:IsPerformingScriptTask()
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue18)
                        goto LAB_00d63c96
                    end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                while scratchValue7 < 0 do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        scratchValue11 = scratchValue18
                        __cleanup_LAB_00d63aa6(); return
                    end
                    scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue18)
                    goto LAB_00d63c96
                end
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue7 ~= 1 then
                    if scratchValue2 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue18)
                        goto LAB_00d63c96
                    end
                    scratchValue28 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:GetHealth(resources:ScriptThing(scratchValue20))
                    if 0.0 < fret_08 then
                        me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION_NO", 0, false, true, false)
                        scratchValue3 = me:IsPerformingScriptTask()
                        while scratchValue3 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue28)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:ReleaseResource(scratchValue18)
                                goto LAB_00d63c96
                            end
                            scratchValue3 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue28)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(scratchValue18)
                            goto LAB_00d63c96
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue28)
                    goto LAB_00d638c8
                end
                if scratchValue2 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue18)
                    goto LAB_00d63c96
                end
                -- TODO(native): if *(iVar6 + 0xf14) <= xStack_128 then
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue18)
                    goto LAB_00d63c96
                end
                quest:GetHealth(resources:ScriptThing(scratchValue20))
                if 0.0 < fret_06 then
                    me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD", 0, false, true, false)
                    scratchValue3 = me:IsPerformingScriptTask()
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue18)
                            goto LAB_00d63c96
                        end
                        scratchValue3 = me:IsPerformingScriptTask()
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue18)
                        goto LAB_00d63c96
                    end
                end
                ::LAB_00d638c8::
                quest:PauseAllNonScriptedEntities(false)
                scratchValue11 = scratchValue18
                ::LAB_00d638d8::
                resources:DestroyMovie(scratchValue11)
                scratchValue14 = scratchValue15
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
            scratchValue13 = scratchValue14 | 8
            scratchValue15 = scratchValue13
            if me:MsgIsHitByHero() then
                scratchValue2 = true
            else
                scratchValue13 = scratchValue14 | 24
                scratchValue15 = scratchValue13
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue13 = scratchValue14 | 56
                    scratchValue15 = scratchValue13
                    if not me:MsgIsHitByHeroSpecialAbility(me) then
                        scratchValue2 = true
                        goto FLOW_after_lab_00d63b21
                    end
                end
                scratchValue2 = false
            end
            ::FLOW_after_lab_00d63b21::
            if scratchValue13 & 32 ~= 0 then
                scratchValue13 = scratchValue13 & 0xffffffdf
                scratchValue15 = scratchValue13
            end
            if scratchValue13 & 16 ~= 0 then
                scratchValue13 = scratchValue13 & 0xffffffef
                scratchValue15 = scratchValue13
            end
            if scratchValue13 & 8 ~= 0 then
                scratchValue13 = scratchValue13 & 0xfffffff7
                scratchValue15 = scratchValue13
            end
            scratchValue14 = scratchValue13
            if scratchValue2 then
                if state:GetBool("NotAttacked") then
                    scratchValue8 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue8, quest:GetHero())
                    quest:AddLineToConversation(scratchValue8, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_ATTACK", me, quest:GetHero(), false)
                    state:SetBool("NotAttacked", false)
                    quest:EntitySetAsKillable(me, true, true)
                    me:MoveToPosition(quest:GetThingWithScriptName("ArtifactThiefRunMarker"):GetPos(), 0x3f800000, 1, false, true)
                    scratchValue14 = scratchValue15
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
    resources:ReleaseResource(scratchValue20)
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

