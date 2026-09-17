-- Readable native conversion: BirdKiller. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- BirdKiller.Main (retail 0x00d4dea0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue4, scratchValue5, ctr_90, scratchValue7, scratchValue10, scratchValue11
    local scratchValue16, timerId, scratchValue17, scratchValue18, scratchValue19, scratchValue20
    local scratchValue21
    local function __region_LAB_00d4e853_c2()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(scratchValue20)
    end
    local function __cleanup_LAB_00d4e91f()
        quest:PauseAllNonScriptedEntities(false)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(scratchValue21)
    end
    scratchValue17 = 0
    scratchValue21 = resources:NewResource()
    scratchValue4 = resources:TryAcquire(scratchValue21, me, 4)
    while not scratchValue4 do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(scratchValue21); return end
        scratchValue4 = resources:TryAcquire(scratchValue21, me, 4)
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(scratchValue21); return end
    quest:EntitySetAsKillable(me, false, true)
    quest:SetThingHasInformation(me, false, true, false)
    me:SetFriendsWithEverythingFlag(me)
    if state:GetInt("BirdMode") == 0 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d4ef90 end
        scratchValue19 = quest:GetAllThingsWithScriptName("BirdMarker")
        if #scratchValue19 ~= 0 then
            ctr_90 = 0
            repeat
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(scratchValue21); return end
                quest:SetThingPersistent(quest:CreateCreature("CREATURE_BIRD_GUILD_SPARROW", scratchValue19[ctr_90 / 12 + 1]:GetPos(), "KillBird"), true)
                ctr_90 = ctr_90 + 12
                scratchValue17 = scratchValue17 + 1
            until scratchValue17 >= ((#scratchValue19 * 12) / 12)
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(scratchValue21); return end
        quest:SetStateInt("CurrentBirdsKilled", 0)
        state:SetInt("BirdMode", 2)
    end
    timerId = quest:RegisterTimer()
    ctr_90 = timerId
    quest:SetTimer(timerId, 15)
    scratchValue10 = state:GetInt("BirdMode")
    while scratchValue10 == 2 do
        if not quest:NewScriptFrame(me) then goto LAB_00d4ef87 end
        if quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(timerId) < 1 then
            scratchValue11 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(scratchValue11, quest:GetHero())
            quest:AddLineToConversation(scratchValue11, "TEXT_QST_028_BIRD_KILLER_HELP", me, quest:GetHero(), false)
            quest:SetTimer(ctr_90, 15)
        end
        if me:IsTalkedToByHero() then
            if not quest:IsActiveThreadTerminating() then
                scratchValue20 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if not state:GetBool("HaveChatted") then
                    if not quest:IsActiveThreadTerminating() then
                        state:SetBool("HaveChatted", true)
                        quest:GetHealth(resources:ScriptThing(scratchValue21))
                        scratchValue7 = 0.0
                        if 0.0 < fret_0 then
                            me:Speak(quest:GetHero(), "TEXT_QST_028_BIRD_KILLER_GREET", 0, false, true, false)
                            scratchValue5 = me:IsPerformingScriptTask()
                            while scratchValue5 do
                                if not quest:NewScriptFrame(me) then __cleanup_LAB_00d4e91f(); return end
                                scratchValue5 = me:IsPerformingScriptTask()
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4e978 end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_BIRD_KILLER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while scratchValue10 < 0 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (xStack_78);
                                quest:DeregisterTimer(timerId)
                                return
                            end
                            scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if not quest:IsActiveThreadTerminating() then
                            scratchValue4 = quest:IsActiveThreadTerminating()
                            if scratchValue10 == 1 then
                                if not scratchValue4 then
                                    state:SetInt("BirdMode", 1)
                                    scratchValue4 = resources:TryAcquire(0, quest:GetHero(), 4)
                                    while not scratchValue4 do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            resources:ReleaseResource(0)
                                            quest:PauseAllNonScriptedEntities(false)
                                            -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (xStack_78);
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(scratchValue21)
                                            return
                                        end
                                        scratchValue4 = resources:TryAcquire(0, quest:GetHero(), 4)
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        scratchValue16 = resources:NewActorMap()
                                        resources:SetActor(scratchValue16, "HERO", 0)
                                        resources:SetActor(scratchValue16, "ME", scratchValue21)
                                        resources:RunMacro("CS_GUILD_GULLS_INTRO", scratchValue16, false, true)
                                        resources:DestroyActorMap(scratchValue16)
                                        resources:ReleaseResource(0)
                                        __region_LAB_00d4e853_c2(); goto LAB_00d4e87a
                                    end
                                    resources:DestroyMovie(0)
                                end
                                goto LAB_00d4e978
                            end
                            if not scratchValue4 then
                                quest:GetHealth(resources:ScriptThing(scratchValue21))
                                scratchValue7 = 0.0
                                if 0.0 < fret_00 then
                                    me:Speak(quest:GetHero(), "TEXT_QST_028_BIRD_KILLER_REFUSE", 0, false, true, false)
                                    scratchValue5 = me:IsPerformingScriptTask()
                                    while scratchValue5 do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (xStack_78);
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(scratchValue21)
                                            return
                                        end
                                        scratchValue5 = me:IsPerformingScriptTask()
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d4e9e1_c2 end
                                end
                                __region_LAB_00d4e853_c2()
                                goto LAB_00d4e87a
                            end
                        end
                        ::LAB_00d4e9e1_c2::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_7c)
                        goto FLOW_after_lab_00d4e5e3
                    end
                    ::LAB_00d4e978::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(scratchValue20)
                else
                    if not quest:IsActiveThreadTerminating() then
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_BIRD_KILLER_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while scratchValue10 < 0 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (xStack_78);
                                quest:DeregisterTimer(timerId)
                                return
                            end
                            scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if not quest:IsActiveThreadTerminating() then
                            scratchValue4 = quest:IsActiveThreadTerminating()
                            if scratchValue10 == 1 then
                                if not scratchValue4 then
                                    state:SetInt("BirdMode", 1)
                                    scratchValue4 = resources:TryAcquire(0, quest:GetHero(), 4)
                                    while not scratchValue4 do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            resources:ReleaseResource(0)
                                            __cleanup_LAB_00d4e91f()
                                            return
                                        end
                                        scratchValue4 = resources:TryAcquire(0, quest:GetHero(), 4)
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        scratchValue16 = resources:NewActorMap()
                                        resources:SetActor(scratchValue16, "HERO", 0)
                                        resources:SetActor(scratchValue16, "ME", scratchValue21)
                                        resources:RunMacro("CS_GUILD_GULLS_INTRO", scratchValue16, false, true)
                                        resources:DestroyActorMap(scratchValue16)
                                        resources:ReleaseResource(0)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue20)
                                        goto LAB_00d4e87a
                                    end
                                    resources:DestroyMovie(0)
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:ReleaseResource(scratchValue20)
                                goto FLOW_after_lab_00d4e5e3
                            end
                            if not scratchValue4 then
                                quest:GetHealth(resources:ScriptThing(scratchValue21))
                                scratchValue7 = 0.0
                                if 0.0 < fret_00 then
                                    me:Speak(quest:GetHero(), "TEXT_QST_028_BIRD_KILLER_REFUSE", 0, false, true, false)
                                    scratchValue5 = me:IsPerformingScriptTask()
                                    while scratchValue5 do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            -- TODO(native): std::_Dest_val<std::allocator<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_>,CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>_> (xStack_78);
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(scratchValue21)
                                            return
                                        end
                                        scratchValue5 = me:IsPerformingScriptTask()
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d4e9e1 end
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue20)
                                goto LAB_00d4e87a
                            end
                        end
                    end
                    ::LAB_00d4e9e1::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_7c)
                end
                ::FLOW_after_lab_00d4e5e3::
            end
            goto LAB_00d4ef87
        end
        ::LAB_00d4e87a::
        timerId = ctr_90
        scratchValue10 = state:GetInt("BirdMode")
    end
    if not quest:IsActiveThreadTerminating() then
        scratchValue10 = state:GetInt("BirdMode")
        while scratchValue10 == 1 do
            if not quest:NewScriptFrame(me) then goto LAB_00d4ef87 end
            if quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(ctr_90) < 1 then
                if state:GetInt("CurrentBirds") == 0 then
                    scratchValue11 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue11, quest:GetHero())
                    quest:AddLineToConversation(scratchValue11, "TEXT_QST_028_BIRD_KILLER_ANY", me, quest:GetHero(), false)
                else
                    scratchValue11 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue11, quest:GetHero())
                    quest:AddLineToConversation(scratchValue11, "TEXT_QST_028_BIRD_KILLER_ANY_MORE", me, quest:GetHero(), false)
                end
                quest:SetTimer(ctr_90, 15)
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d4ef87 end
                scratchValue10 = quest:GetStateInt("CurrentBirdsKilled")
                if scratchValue10 == 1 then
                    scratchValue11 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue11, quest:GetHero())
                    quest:AddLineToConversation(scratchValue11, "TEXT_QST_028_BIRD_KILLER_ONE", me, quest:GetHero(), false)
                    quest:Pause(1.0)
                    quest:GiveHeroGold(math.modf(quest:ReadGlobalGameDataFloat(3836)))
                elseif scratchValue10 == 0 then
                    scratchValue11 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue11, quest:GetHero())
                    quest:AddLineToConversation(scratchValue11, "TEXT_QST_028_BIRD_KILLER_NONE", me, quest:GetHero(), false)
                    quest:Pause(1.0)
                else
                    scratchValue11 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue11, quest:GetHero())
                    quest:AddLineToConversation(scratchValue11, "TEXT_QST_028_BIRD_KILLER_MORE", me, quest:GetHero(), false)
                    quest:Pause(1.0)
                    quest:GiveHeroGold(math.modf(quest:GetStateInt("CurrentBirdsKilled") * quest:ReadGlobalGameDataFloat(3836)))
                end
                state:SetInt("CurrentBirds", state:GetInt("CurrentBirds") + quest:GetStateInt("CurrentBirdsKilled"))
                if quest:GetStateInt("CurrentBirdsKilled") ~= 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4ef87 end
                    quest:SetStateInt("CurrentBirdsKilled", 0)
                    scratchValue10 = quest:AddNewConversation(me, false, false)
                    if state:GetInt("CurrentBirds") == 7 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4ef87 end
                        scratchValue18 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        quest:GetHealth(resources:ScriptThing(scratchValue21))
                        scratchValue7 = 0.0
                        if 0.0 < fret_01 then
                            me:Speak(quest:GetHero(), "TEXT_QST_028_BIRD_KILLER_DONE", 0, false, true, false)
                            scratchValue5 = me:IsPerformingScriptTask()
                            while scratchValue5 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue18)
                                    goto LAB_00d4ef87
                                end
                                scratchValue5 = me:IsPerformingScriptTask()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:ReleaseResource(scratchValue18)
                                goto LAB_00d4ef87
                            end
                        end
                        quest:GiveHeroGold(math.modf(quest:ReadGlobalGameDataFloat(3840)))
                        state:SetInt("BirdMode", 3)
                        quest:ClearThingHasInformation(me)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue18)
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4ef87 end
                        quest:AddPersonToConversation(scratchValue7, quest:GetHero())
                        quest:AddLineToConversation(false, scratchValue10, me, quest:GetHero())
                        quest:Pause(1.0)
                    end
                end
            end
            scratchValue10 = state:GetInt("BirdMode")
        end
        if not quest:IsActiveThreadTerminating() then
            scratchValue4 = quest:IsActiveThreadTerminating()
            while not scratchValue4 do
                if quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(ctr_90) < 1 then
                    if quest:IsActiveThreadTerminating() then break end
                    scratchValue11 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue11, quest:GetHero())
                    quest:AddLineToConversation(scratchValue11, "TEXT_QST_028_BIRD_KILLER_FINISHED", me, quest:GetHero(), false)
                    quest:SetTimer(ctr_90, 15)
                end
                quest:NewScriptFrame(me)
                scratchValue4 = quest:IsActiveThreadTerminating()
            end
        end
    end
    ::LAB_00d4ef87::
    quest:DeregisterTimer(xStack_90)
    ::LAB_00d4ef90::
    resources:DestroyMovie(scratchValue21)
end

-- BirdKiller.Init (retail 0x00d42ff0)
function Init(quest, me)
    state:SetBool("HaveChatted", false)
    state:SetInt("BirdMode", 0)
    state:SetInt("CurrentBirds", 0)
    quest:SetThingPersistent(me, true)
end

-- BirdKiller.OnPersist (retail 0x00d44c60)
function OnPersist(quest, context)
end

-- BirdKiller.OnPredicateFail (retail 0x00d43010)
function OnPredicateFail(quest, me)
end

