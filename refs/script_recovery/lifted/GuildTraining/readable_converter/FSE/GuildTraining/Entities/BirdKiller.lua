-- Readable native conversion: BirdKiller. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_GoldPerBird = 3836,  -- 5.0
    GUI_BirdGoldBonus = 3840,  -- 20.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local birdMode, haveChatted, currentBirds

-- BirdKiller.Main (retail 0x00d4dea0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local ctr_90, conversationId, questionAnswer, timerId, scratchValue30, movie2, movie3, resource4
    local scratchValue31
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource4)
    end
    scratchValue30 = 0
    resource4 = resources:NewResource()
    resources:PrepareResource(resource4)
    while not resources:TryAcquire(resource4, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource4)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource4); return end
    quest:EntitySetAsKillable(me, false, true)
    quest:SetThingHasInformation(me, false, true, false)
    me:SetFriendsWithEverythingFlag(me)
    if birdMode == 0 then
        if quest:IsActiveThreadTerminating() then goto LAB_00d4ef90 end
        local birdMarker = quest:GetAllThingsWithScriptName("BirdMarker")
        local scratchValue = scratchValue31 - birdMarker >> 31
        if (scratchValue31 - birdMarker) / 12 + scratchValue ~= scratchValue then
            ctr_90 = 0
            repeat
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource4); return end
                quest:SetThingPersistent(quest:CreateCreature("CREATURE_BIRD_GUILD_SPARROW", birdMarker[ctr_90 / 12 + 1]:GetPos(), "KillBird"), true)
                ctr_90 = ctr_90 + 12
                scratchValue30 = scratchValue30 + 1
            until scratchValue30 >= ((scratchValue31 - birdMarker) / 12)
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource4); return end
        quest:SetStateInt("CurrentBirdsKilled", 0)
        birdMode = 2
    end
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 15)
    while birdMode == 2 do
        if not quest:NewScriptFrame(me) then goto LAB_00d4ef87 end
        if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1 then
            local conversationId3 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId3, hero)
            quest:AddLineToConversation(conversationId3, "TEXT_QST_028_BIRD_KILLER_HELP", me, hero, false)
            quest:SetTimer(timerId, 15)
        end
        if me:IsTalkedToByHero() then
            if not quest:IsActiveThreadTerminating() then
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if not haveChatted then
                    if not quest:IsActiveThreadTerminating() then
                        haveChatted = true
                        conversationId = 0.0
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                            me:Speak(hero, "TEXT_QST_028_BIRD_KILLER_GREET", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4e978 end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_BIRD_KILLER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        goto LAB_00d4e5e3
                    end
                    goto LAB_00d4e978
                else
                    if not quest:IsActiveThreadTerminating() then quest:GiveHeroYesNoQuestion("TEXT_QST_028_BIRD_KILLER_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true); goto LAB_00d4e5e3 end
                    goto FLOW_hoist_lab_00d4e5e3_1
                end
                goto FLOW_past_lab_00d4e978
                ::LAB_00d4e978::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                ::FLOW_past_lab_00d4e978::
                goto FLOW_past_lab_00d4e5e3
                ::LAB_00d4e5e3::
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                while questionAnswer < 0 do
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    else
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource4)
                        do return end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                end
                if not quest:IsActiveThreadTerminating() then
                    local predicateResult = quest:IsActiveThreadTerminating()
                    if questionAnswer == 1 then
                        if not predicateResult then
                            birdMode = 1
                            local resource = resources:NewResource()
                            resources:PrepareResource(resource)
                            while not resources:TryAcquire(resource, hero, 4) do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    resources:ReleaseResource(resource)
                                    ReleaseEverything()
                                    do return end
                                end
                            end
                            if not quest:IsActiveThreadTerminating() then
                                local actorMap = resources:NewActorMap()
                                resources:SetActor(actorMap, "HERO", resource)
                                resources:SetActor(actorMap, "ME", resource4)
                                resources:RunMacro("CS_GUILD_GULLS_INTRO", actorMap, false, true)
                                resources:DestroyActorMap(actorMap)
                                resources:ReleaseResource(resource)
                                goto LAB_00d4e853
                            end
                            resources:ReleaseResource(resource)
                        end
                        goto LAB_00d4e978
                    end
                    if not predicateResult then
                        conversationId = 0.0
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                            me:Speak(hero, "TEXT_QST_028_BIRD_KILLER_REFUSE", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    quest:DeregisterTimer(timerId)
                                    resources:ReleaseResource(resource4)
                                    do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4e9e1 end
                        end
                        goto LAB_00d4e853
                    end
                    goto FLOW_past_lab_00d4e853
                    ::LAB_00d4e853::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    goto LAB_00d4e87a
                    ::FLOW_past_lab_00d4e853::
                end
                ::FLOW_hoist_lab_00d4e5e3_1::
                ::LAB_00d4e9e1::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                ::FLOW_past_lab_00d4e5e3::
            end
            goto LAB_00d4ef87
        end
        ::LAB_00d4e87a::
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d4ef87 end
    while birdMode == 1 do
        if not quest:NewScriptFrame(me) then goto LAB_00d4ef87 end
        if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1 then
            if currentBirds == 0 then
                local conversationId4 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId4, hero)
                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_BIRD_KILLER_ANY", me, hero, false)
            else
                local conversationId5 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId5, hero)
                quest:AddLineToConversation(conversationId5, "TEXT_QST_028_BIRD_KILLER_ANY_MORE", me, hero, false)
            end
            quest:SetTimer(timerId, 15)
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d4ef87 end
            local currentBirdsKilled = quest:GetStateInt("CurrentBirdsKilled")
            if currentBirdsKilled == 1 then
                local conversationId6 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId6, hero)
                quest:AddLineToConversation(conversationId6, "TEXT_QST_028_BIRD_KILLER_ONE", me, hero, false)
                quest:Pause(1.0)
                quest:GiveHeroGold(math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_GoldPerBird))))
            elseif currentBirdsKilled == 0 then
                local conversationId7 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId7, hero)
                quest:AddLineToConversation(conversationId7, "TEXT_QST_028_BIRD_KILLER_NONE", me, hero, false)
                quest:Pause(1.0)
            else
                local conversationId8 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId8, hero)
                quest:AddLineToConversation(conversationId8, "TEXT_QST_028_BIRD_KILLER_MORE", me, hero, false)
                quest:Pause(1.0)
                quest:GiveHeroGold(math.tointeger(math.modf(quest:GetStateInt("CurrentBirdsKilled") * quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_GoldPerBird))))
            end
            currentBirds = currentBirds + quest:GetStateInt("CurrentBirdsKilled")
            if quest:GetStateInt("CurrentBirdsKilled") ~= 0 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d4ef87 end
                quest:SetStateInt("CurrentBirdsKilled", 0)
                local conversationId2 = quest:AddNewConversation(me, false, false)
                if currentBirds == 7 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4ef87 end
                    local movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    conversationId = 0.0
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                        me:Speak(hero, "TEXT_QST_028_BIRD_KILLER_DONE", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto LAB_00d4ef87
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00d4ef87
                        end
                    end
                    quest:GiveHeroGold(math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_BirdGoldBonus))))
                    birdMode = 3
                    quest:ClearThingHasInformation(me)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4ef87 end
                    quest:AddPersonToConversation(conversationId, hero)
                    quest:AddLineToConversation(false, conversationId2, me, hero)
                    quest:Pause(1.0)
                end
            end
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d4ef87 end
    while not quest:IsActiveThreadTerminating() do
        if not (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1) then
            quest:NewScriptFrame(me)
        else
            local conversationId9 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId9, hero)
            quest:AddLineToConversation(conversationId9, "TEXT_QST_028_BIRD_KILLER_FINISHED", me, hero, false)
            quest:SetTimer(timerId, 15)
            quest:NewScriptFrame(me)
        end
    end
    ::LAB_00d4ef87::
    quest:DeregisterTimer(timerId)
    ::LAB_00d4ef90::
    resources:ReleaseResource(resource4)
end

-- BirdKiller.Init (retail 0x00d42ff0)
function Init(quest, me)
    haveChatted = false
    birdMode = 0
    currentBirds = 0
    quest:SetThingPersistent(me, true)
end

-- BirdKiller.OnPersist (retail 0x00d44c60)
function OnPersist(quest, me, context)
    quest:SetStateInt("BirdMode", quest:PersistTransferInt(context, "BirdMode", quest:GetStateInt("BirdMode") or 0))
    quest:SetStateInt("CurrentBirds", quest:PersistTransferInt(context, "CurrentBirds", quest:GetStateInt("CurrentBirds") or 0))
end

-- BirdKiller.OnPredicateFail (retail 0x00d43010)
function OnPredicateFail(quest, me)
end

