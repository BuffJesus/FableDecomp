-- Readable native conversion: BirdKiller. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_GoldPerBird = 3836,  -- 5.0
    GUI_BirdGoldBonus = 3840,  -- 20.0
}

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
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue4, scratchValue5, ctr_90, scratchValue7, scratchValue, addNewConversation
    local scratchValue13, timerId, scratchValue14, movie, movie2, resource, scratchValue15
    local function __region_LAB_00d4e853_c2()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
    end
    local function __cleanup_LAB_00d4e91f()
        quest:PauseAllNonScriptedEntities(false)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    scratchValue14 = 0
    resource = resources:NewResource()
    scratchValue4 = resources:TryAcquire(resource, me, 4)
    while not scratchValue4 do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        scratchValue4 = resources:TryAcquire(resource, me, 4)
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    quest:EntitySetAsKillable(me, false, true)
    quest:SetThingHasInformation(me, false, true, false)
    me:SetFriendsWithEverythingFlag(me)
    if state:GetInt("BirdMode") == 0 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d4ef90 end
        scratchValue15 = quest:GetAllThingsWithScriptName("BirdMarker")
        if #scratchValue15 ~= 0 then
            ctr_90 = 0
            repeat
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                quest:SetThingPersistent(quest:CreateCreature("CREATURE_BIRD_GUILD_SPARROW", scratchValue15[ctr_90 / 12 + 1]:GetPos(), "KillBird"), true)
                ctr_90 = ctr_90 + 12
                scratchValue14 = scratchValue14 + 1
            until scratchValue14 >= #scratchValue15
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        quest:SetStateInt("CurrentBirdsKilled", 0)
        state:SetInt("BirdMode", 2)
    end
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 15)
    scratchValue = state:GetInt("BirdMode")
    while scratchValue == 2 do
        if not quest:NewScriptFrame(me) then goto LAB_00d4ef87 end
        if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1 then
            addNewConversation = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(addNewConversation, hero)
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_BIRD_KILLER_HELP", me, hero, false)
            quest:SetTimer(timerId, 15)
        end
        if me:IsTalkedToByHero() then
            if not quest:IsActiveThreadTerminating() then
                movie2 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if not state:GetBool("HaveChatted") then
                    if not quest:IsActiveThreadTerminating() then
                        state:SetBool("HaveChatted", true)
                        scratchValue7 = 0.0
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            me:Speak(hero, "TEXT_QST_028_BIRD_KILLER_GREET", 0, false, true, false)
                            scratchValue5 = me:IsPerformingScriptTask()
                            while scratchValue5 do
                                if not quest:NewScriptFrame(me) then __cleanup_LAB_00d4e91f(); return end
                                scratchValue5 = me:IsPerformingScriptTask()
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4e978 end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_BIRD_KILLER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
                        while scratchValue < 0 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:DeregisterTimer(timerId)
                                return
                            end
                            scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if not quest:IsActiveThreadTerminating() then
                            scratchValue4 = quest:IsActiveThreadTerminating()
                            if scratchValue == 1 then
                                if not scratchValue4 then
                                    state:SetInt("BirdMode", 1)
                                    scratchValue4 = resources:TryAcquire(0, hero, 4)
                                    while not scratchValue4 do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            resources:ReleaseResource(0)
                                            quest:PauseAllNonScriptedEntities(false)
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(resource)
                                            return
                                        end
                                        scratchValue4 = resources:TryAcquire(0, hero, 4)
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        scratchValue13 = resources:NewActorMap()
                                        resources:SetActor(scratchValue13, "HERO", 0)
                                        resources:SetActor(scratchValue13, "ME", resource)
                                        resources:RunMacro("CS_GUILD_GULLS_INTRO", scratchValue13, false, true)
                                        resources:DestroyActorMap(scratchValue13)
                                        resources:ReleaseResource(0)
                                        __region_LAB_00d4e853_c2(); goto LAB_00d4e87a
                                    end
                                    resources:ReleaseResource(0)
                                end
                                goto LAB_00d4e978
                            end
                            if not scratchValue4 then
                                scratchValue7 = 0.0
                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                    me:Speak(hero, "TEXT_QST_028_BIRD_KILLER_REFUSE", 0, false, true, false)
                                    scratchValue5 = me:IsPerformingScriptTask()
                                    while scratchValue5 do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(resource)
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
                    resources:DestroyMovie(movie2)
                else
                    if not quest:IsActiveThreadTerminating() then
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_BIRD_KILLER_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
                        while scratchValue < 0 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:DeregisterTimer(timerId)
                                return
                            end
                            scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if not quest:IsActiveThreadTerminating() then
                            scratchValue4 = quest:IsActiveThreadTerminating()
                            if scratchValue == 1 then
                                if not scratchValue4 then
                                    state:SetInt("BirdMode", 1)
                                    scratchValue4 = resources:TryAcquire(0, hero, 4)
                                    while not scratchValue4 do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            resources:ReleaseResource(0)
                                            __cleanup_LAB_00d4e91f()
                                            return
                                        end
                                        scratchValue4 = resources:TryAcquire(0, hero, 4)
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        scratchValue13 = resources:NewActorMap()
                                        resources:SetActor(scratchValue13, "HERO", 0)
                                        resources:SetActor(scratchValue13, "ME", resource)
                                        resources:RunMacro("CS_GUILD_GULLS_INTRO", scratchValue13, false, true)
                                        resources:DestroyActorMap(scratchValue13)
                                        resources:ReleaseResource(0)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie2)
                                        goto LAB_00d4e87a
                                    end
                                    resources:ReleaseResource(0)
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                goto FLOW_after_lab_00d4e5e3
                            end
                            if not scratchValue4 then
                                scratchValue7 = 0.0
                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                    me:Speak(hero, "TEXT_QST_028_BIRD_KILLER_REFUSE", 0, false, true, false)
                                    scratchValue5 = me:IsPerformingScriptTask()
                                    while scratchValue5 do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(resource)
                                            return
                                        end
                                        scratchValue5 = me:IsPerformingScriptTask()
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d4e9e1 end
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
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
        scratchValue = state:GetInt("BirdMode")
    end
    if not quest:IsActiveThreadTerminating() then
        scratchValue = state:GetInt("BirdMode")
        while scratchValue == 1 do
            if not quest:NewScriptFrame(me) then goto LAB_00d4ef87 end
            if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1 then
                if state:GetInt("CurrentBirds") == 0 then
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_BIRD_KILLER_ANY", me, hero, false)
                else
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_BIRD_KILLER_ANY_MORE", me, hero, false)
                end
                quest:SetTimer(timerId, 15)
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d4ef87 end
                scratchValue = quest:GetStateInt("CurrentBirdsKilled")
                if scratchValue == 1 then
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_BIRD_KILLER_ONE", me, hero, false)
                    quest:Pause(1.0)
                    quest:GiveHeroGold(math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_GoldPerBird))))
                elseif scratchValue == 0 then
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_BIRD_KILLER_NONE", me, hero, false)
                    quest:Pause(1.0)
                else
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_BIRD_KILLER_MORE", me, hero, false)
                    quest:Pause(1.0)
                    quest:GiveHeroGold(math.tointeger(math.modf(quest:GetStateInt("CurrentBirdsKilled") * quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_GoldPerBird))))
                end
                state:SetInt("CurrentBirds", state:GetInt("CurrentBirds") + quest:GetStateInt("CurrentBirdsKilled"))
                if quest:GetStateInt("CurrentBirdsKilled") ~= 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4ef87 end
                    quest:SetStateInt("CurrentBirdsKilled", 0)
                    scratchValue = quest:AddNewConversation(me, false, false)
                    if state:GetInt("CurrentBirds") == 7 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4ef87 end
                        movie = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        scratchValue7 = 0.0
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            me:Speak(hero, "TEXT_QST_028_BIRD_KILLER_DONE", 0, false, true, false)
                            scratchValue5 = me:IsPerformingScriptTask()
                            while scratchValue5 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie)
                                    goto LAB_00d4ef87
                                end
                                scratchValue5 = me:IsPerformingScriptTask()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto LAB_00d4ef87
                            end
                        end
                        quest:GiveHeroGold(math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_BirdGoldBonus))))
                        state:SetInt("BirdMode", 3)
                        quest:ClearThingHasInformation(me)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4ef87 end
                        quest:AddPersonToConversation(scratchValue7, hero)
                        quest:AddLineToConversation(false, scratchValue, me, hero)
                        quest:Pause(1.0)
                    end
                end
            end
            scratchValue = state:GetInt("BirdMode")
        end
        if not quest:IsActiveThreadTerminating() then
            scratchValue4 = quest:IsActiveThreadTerminating()
            while not scratchValue4 do
                if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1 then
                    if quest:IsActiveThreadTerminating() then break end
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_BIRD_KILLER_FINISHED", me, hero, false)
                    quest:SetTimer(timerId, 15)
                end
                quest:NewScriptFrame(me)
                scratchValue4 = quest:IsActiveThreadTerminating()
            end
        end
    end
    ::LAB_00d4ef87::
    quest:DeregisterTimer(timerId)
    ::LAB_00d4ef90::
    resources:ReleaseResource(resource)
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

