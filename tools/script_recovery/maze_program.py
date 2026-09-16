"""Reviewed native control-flow templates; operands are checked by maze_native."""
UNLIMBO = '''    local flags = quest:RetailFlags("MazeResearch")
    while not flags:Get("UNLIMBO") do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local sword = quest:GetRetainedRetailThing("MazeResearch")
    quest:EntitySetInLimbo(sword, false, true)
    quest:EntitySetAlpha(sword, 0.0, true)
'''

EMPTY_INIT = '''    quest:RetailFlags("MazeResearch"):Set("UNLIMBO", false)
'''

EMPTY_MAIN = '''    if quest:GetStateBool("BookRead") then
        if quest:IsActiveThreadTerminating() then return end
        quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_VIGNETTE")
    end
    local sword = quest:GetThingWithScriptName("GoodSword")
    quest:RetainRetailThing("MazeResearch", sword)
    quest:EntitySetInLimbo(sword, true, true)
    if quest:IsActiveThreadTerminating() then return end
    while true do
        if me:MsgIsUsedByHero() then
            if quest:IsActiveThreadTerminating() then return end
            if quest:GetStateBool("BookRead") and not quest:GetStateBool("SwordTaken")
                and quest:GetMasterGameState("PostSavePosition") > 1700
                and quest:GetMasterGameState("JackBossBattleResult") == 2 then
                if quest:IsActiveThreadTerminating() then return end
                local completed = false
                quest:WithRetailResources(function(resources)
                    local hero = resources:NewResource()
                    resources:TryAcquire(hero, quest:GetHero(), 4)
                    local actors = resources:NewActorMap()
                    resources:SetActor(actors, "HERO", hero)
                    local movie = resources:StartMovie("")
                    resources:Pause(true)
                    quest:FixMovieSequenceCamera(true)
                    quest:CreateThread("UnLimboSword", {region = ""})
                    local good = quest:GetMasterGameState("JackBossBattleHeroGoodAtEnd")
                    if quest:IsActiveThreadTerminating() then
                        resources:Pause(false)
                        resources:DestroyMovie(movie)
                        resources:DestroyActorMap(actors)
                        resources:ReleaseResource(hero)
                        return
                    end
                    local macro
                    if good then macro = "CS_GET_SWORD_OF_AEONS_SAINT"
                    else macro = "CS_GET_SWORD_OF_AEONS_REDEEM" end
                    resources:RunMacroWithFlags(macro, actors, quest:RetailFlags("MazeResearch"), false, true)
                    quest:FixMovieSequenceCamera(false)
                    quest:MiniMapRemoveMarker(me)
                    resources:Pause(false)
                    resources:DestroyMovie(movie)
                    resources:DestroyActorMap(actors)
                    resources:ReleaseResource(hero)
                    completed = true
                end)
                if not completed then return end
                quest:SetStateBool("SwordTaken", true)
                quest:MiniMapRemoveMarker(me)
                quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            end
        end
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return end
    end
'''
