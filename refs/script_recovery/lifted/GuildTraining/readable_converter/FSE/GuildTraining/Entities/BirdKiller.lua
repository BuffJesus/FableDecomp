-- Readable native conversion: BirdKiller. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_GoldPerBird = 3836,  -- 5.0
    GUI_BirdGoldBonus = 3840,  -- 20.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local birdMode_, haveChatted, currentBirds_

-- BirdKiller.Main (retail 0x00d4dea0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local controlAcquired, birdTimerId, ctr_90, questionAnswer, scratchValue32, movie2, resource4
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
        quest:DeregisterTimer(birdTimerId)
        resources:ReleaseResource(resource4)
    end
    scratchValue32 = 0
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
    me:SetFriendsWithEverythingFlag(true)
    if birdMode_ == 0 then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource4); return end
        local birdMarker = quest:GetAllThingsWithScriptName("BirdMarker")
        if #birdMarker ~= 0 then
            ctr_90 = 0
            repeat
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource4); return end
                local pPosition = birdMarker[ctr_90 / 12 + 1]:GetPos()
                local birdGuildSparrow = quest:CreateCreature("CREATURE_BIRD_GUILD_SPARROW", pPosition, "KillBird", false)
                quest:SetThingPersistent(birdGuildSparrow, true)
                ctr_90 = ctr_90 + 12
                scratchValue32 = scratchValue32 + 1
            until not (scratchValue32 < (#birdMarker))
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource4); return end
        quest:SetStateInt("CurrentBirdsKilled", 0)
        birdMode_ = 2
    end
    birdTimerId = quest:RegisterTimer()
    ctr_90 = birdTimerId
    quest:SetTimer(birdTimerId, 15)
    while birdMode_ == 2 do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
        local hero2 = quest:GetHero()
        if quest:IsDistanceBetweenThingsUnder(hero2, me, 5.5) and quest:GetTimer(birdTimerId) < 1 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
            local conversationId2 = quest:AddNewConversation(me, false, false)
            local hero3 = quest:GetHero()
            quest:AddPersonToConversation(conversationId2, hero3)
            local hero4 = quest:GetHero()
            quest:AddLineToConversation(conversationId2, "TEXT_QST_028_BIRD_KILLER_HELP", me, hero4, false)
            quest:SetTimer(ctr_90, 15)
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
            movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if not haveChatted then
                if not quest:IsActiveThreadTerminating() then
                    haveChatted = true
                    local scratchValue27 = resources:ScriptThing(resource4)
                    if 0.0 < quest:GetHealth(scratchValue27) then
                        local line = "TEXT_QST_028_BIRD_KILLER_GREET"
                        local hero = quest:GetHero()
                        if not me:Speak(hero, line, 0, false, true, false) then
                            ReleaseEverything(); return
                        end
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
                    quest:DeregisterTimer(birdTimerId)
                    resources:ReleaseResource(resource4)
                    do return end
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                end
            end
            if quest:IsActiveThreadTerminating() then goto FLOW_hoist_lab_00d4e5e3_1 end
            if questionAnswer == 1 then
                do
                    birdMode_ = 1
                    local resource3 = resources:NewResource()
                    resources:PrepareResource(resource3)
                    local resource = resource3
                    local hero5 = quest:GetHero()
                    controlAcquired = resources:TryAcquire(resource, hero5, 4)
                    while not controlAcquired do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(resource3)
                            ReleaseEverything()
                            return
                        end
                        local resource2 = resource3
                        local hero6 = quest:GetHero()
                        controlAcquired = resources:TryAcquire(resource2, hero6, 4)
                    end
                    if not quest:IsActiveThreadTerminating() then
                        local actorMap = resources:NewActorMap()
                        resources:SetActor(actorMap, "HERO", resource3)
                        resources:SetActor(actorMap, "ME", resource4)
                        resources:RunMacro("CS_GUILD_GULLS_INTRO", actorMap, false, true)
                        resources:DestroyActorMap(actorMap)
                        resources:ReleaseResource(resource3)
                        goto LAB_00d4e853
                    end
                    resources:ReleaseResource(resource3)
                end
                goto LAB_00d4e978
            end
            do
                local scratchValue33 = resources:ScriptThing(resource4)
                if 0.0 < quest:GetHealth(scratchValue33) then
                    local line2 = "TEXT_QST_028_BIRD_KILLER_REFUSE"
                    local hero7 = quest:GetHero()
                    if not me:Speak(hero7, line2, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(birdTimerId)
                        resources:ReleaseResource(resource4)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            quest:DeregisterTimer(birdTimerId)
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
            ::FLOW_hoist_lab_00d4e5e3_1::
            ::LAB_00d4e9e1::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            ::FLOW_past_lab_00d4e5e3::
            quest:DeregisterTimer(birdTimerId)
            resources:ReleaseResource(resource4)
            return
        end
        ::LAB_00d4e87a::
        birdTimerId = ctr_90
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
    while birdMode_ == 1 do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
        local hero8 = quest:GetHero()
        if quest:IsDistanceBetweenThingsUnder(hero8, me, 5.5) and quest:GetTimer(ctr_90) < 1 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
            if currentBirds_ == 0 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
                local conversationId3 = quest:AddNewConversation(me, false, false)
                local hero9 = quest:GetHero()
                quest:AddPersonToConversation(conversationId3, hero9)
                local hero10 = quest:GetHero()
                quest:AddLineToConversation(conversationId3, "TEXT_QST_028_BIRD_KILLER_ANY", me, hero10, false)
            else
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
                local conversationId4 = quest:AddNewConversation(me, false, false)
                local hero11 = quest:GetHero()
                quest:AddPersonToConversation(conversationId4, hero11)
                local hero12 = quest:GetHero()
                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_BIRD_KILLER_ANY_MORE", me, hero12, false)
            end
            quest:SetTimer(ctr_90, 15)
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
            local currentBirdsKilled = quest:GetStateInt("CurrentBirdsKilled")
            if currentBirdsKilled == 1 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
                local conversationId5 = quest:AddNewConversation(me, false, false)
                local hero13 = quest:GetHero()
                quest:AddPersonToConversation(conversationId5, hero13)
                local hero14 = quest:GetHero()
                quest:AddLineToConversation(conversationId5, "TEXT_QST_028_BIRD_KILLER_ONE", me, hero14, false)
                quest:Pause(1.0)
                local scratchValue = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_GoldPerBird)))
                quest:GiveHeroGold(scratchValue)
            elseif currentBirdsKilled == 0 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
                local conversationId6 = quest:AddNewConversation(me, false, false)
                local hero15 = quest:GetHero()
                quest:AddPersonToConversation(conversationId6, hero15)
                local hero16 = quest:GetHero()
                quest:AddLineToConversation(conversationId6, "TEXT_QST_028_BIRD_KILLER_NONE", me, hero16, false)
                quest:Pause(1.0)
            else
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
                local conversationId7 = quest:AddNewConversation(me, false, false)
                local hero17 = quest:GetHero()
                quest:AddPersonToConversation(conversationId7, hero17)
                local hero18 = quest:GetHero()
                quest:AddLineToConversation(conversationId7, "TEXT_QST_028_BIRD_KILLER_MORE", me, hero18, false)
                quest:Pause(1.0)
                local scratchValue24 = math.tointeger(math.modf(quest:GetStateInt("CurrentBirdsKilled") * quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_GoldPerBird)))
                quest:GiveHeroGold(scratchValue24)
            end
            currentBirds_ = currentBirds_ + quest:GetStateInt("CurrentBirdsKilled")
            if quest:GetStateInt("CurrentBirdsKilled") ~= 0 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
                quest:SetStateInt("CurrentBirdsKilled", 0)
                local conversationId = quest:AddNewConversation(me, false, false)
                if currentBirds_ == 7 then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
                    local movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue34 = resources:ScriptThing(resource4)
                    if 0.0 < quest:GetHealth(scratchValue34) then
                        local line3 = "TEXT_QST_028_BIRD_KILLER_DONE"
                        local hero19 = quest:GetHero()
                        if not me:Speak(hero19, line3, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            quest:DeregisterTimer(birdTimerId)
                            resources:ReleaseResource(resource4)
                            return
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                quest:DeregisterTimer(birdTimerId)
                                resources:ReleaseResource(resource4)
                                do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            quest:DeregisterTimer(birdTimerId)
                            resources:ReleaseResource(resource4)
                            return
                        end
                    end
                    local scratchValue26 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_BirdGoldBonus)))
                    quest:GiveHeroGold(scratchValue26)
                    birdMode_ = 3
                    quest:ClearThingHasInformation(me)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                else
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
                    local hero20 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId, hero20)
                    local hero21 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_BIRD_KILLER_NOT_DONE", me, hero21, false)
                    quest:Pause(1.0)
                end
            end
        end
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(birdTimerId); resources:ReleaseResource(resource4); return end
    while not quest:IsActiveThreadTerminating() do
        local hero22 = quest:GetHero()
        if not (quest:IsDistanceBetweenThingsUnder(hero22, me, 5.5) and quest:GetTimer(ctr_90) < 1) then
            quest:NewScriptFrame(me)
        else
            if quest:IsActiveThreadTerminating() then break end
            local conversationId8 = quest:AddNewConversation(me, false, false)
            local hero23 = quest:GetHero()
            quest:AddPersonToConversation(conversationId8, hero23)
            local hero24 = quest:GetHero()
            quest:AddLineToConversation(conversationId8, "TEXT_QST_028_BIRD_KILLER_FINISHED", me, hero24, false)
            quest:SetTimer(ctr_90, 15)
            quest:NewScriptFrame(me)
        end
    end
    quest:DeregisterTimer(birdTimerId)
    resources:ReleaseResource(resource4)
end

-- BirdKiller.Init (retail 0x00d42ff0)
function Init(quest, me)
    haveChatted = false
    birdMode_ = 0
    currentBirds_ = 0
    quest:SetThingPersistent(me, true)
end

-- BirdKiller.OnPersist (retail 0x00d44c60)
function OnPersist(quest, me, context)
    local birdMode = birdMode_ or 0
    birdMode_ = quest:PersistTransferInt(context, "BirdMode", birdMode)
    local currentBirds = currentBirds_ or 0
    currentBirds_ = quest:PersistTransferInt(context, "CurrentBirds", currentBirds)
end

-- BirdKiller.OnPredicateFail (retail 0x00d43010)
function OnPredicateFail(quest, me)
end

