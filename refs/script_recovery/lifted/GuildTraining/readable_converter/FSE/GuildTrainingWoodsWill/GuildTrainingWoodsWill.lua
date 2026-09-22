-- Readable native conversion: Q_GuildTrainingWoodsWill. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_MinHealth = 3800,  -- 6.0
}

-- Q_GuildTrainingWoodsWill.Main (retail 0x00d67890)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, ctr_84, ctr_88, scratchValue2, addNewConversation, scratchValue3, getHero
    local pppuVar, scratchValue4, resource, movie, scratchValue5, actorMap, resource2, willBandit
    scratchValue2 = 0
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionOver", false)
    quest:SetStateBool("WhisperAnimate", false)
    quest:SetStateBool("BanditsAlive", true)
    quest:AddEntityBinding("WillWhisper", "GuildTrainingWoodsWill/Entities/WillWhisper", 1)
    quest:FinalizeEntityBindings()
    scratchValue = quest:IsLevelLoaded("GuildWoods")
    while not scratchValue do
        quest:NewScriptFrame()
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then
            return
        end
        scratchValue = quest:IsLevelLoaded("GuildWoods")
    end
    scratchValue = quest:IsActiveThreadTerminating()
    if scratchValue then return end
    quest:SetQuestCardObjective("Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_02", "", "")
    quest:CreateThread("WatchForTermination")  -- native thread body Quest_GuildTrainingWoods_Will_Init: lift it as function WatchForTermination(quest)
    quest:CreateThread("DoMission")  -- native thread body 0x00D68AE0: lift it as function DoMission(quest)
    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", false)
    local willWhisper = quest:GetThingWithScriptName("WillWhisper")
    local resource3 = resources:NewResource()
    resources:PrepareResource(resource3)
    scratchValue = resources:TryAcquire(resource3, willWhisper, 4)
    while not scratchValue do
        quest:NewScriptFrame()
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then goto LAB_00d685d5 end
        scratchValue = resources:TryAcquire(resource3, willWhisper, 4)
    end
    scratchValue = quest:IsActiveThreadTerminating()
    if scratchValue then goto LAB_00d685d5 end
    willBandit = quest:GetAllThingsWithScriptName("WillBandit")
    scratchValue3 = #willBandit * 12
    resource = resources:NewResource()
    scratchValue5 = (function(n) local t = {} for i = 1, n do t[i] = resources:NewResource() end return t end)(scratchValue3 / 12)
    resources:ReleaseResource(resource)
    scratchValue4 = 0
    if #willBandit ~= 0 then
        ctr_84 = 0
        repeat
            resources:TryAcquire(scratchValue5[scratchValue2 / 16 + 1], willBandit[ctr_84 + 1], 4)
            ctr_84 = ctr_84 + 1
            scratchValue4 = scratchValue4 + 1
            scratchValue2 = scratchValue2 + 16
        until scratchValue4 >= #willBandit
    end
    scratchValue2 = 0
    resource2 = resources:NewResource()
    resources:PrepareResource(resource2)
    scratchValue3 = 4
    pppuVar = resource2
    getHero = hero
    scratchValue = resources:TryAcquire(pppuVar, getHero, scratchValue3)
    while not scratchValue do
        quest:NewScriptFrame()
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then goto LAB_00d67db1 end
        scratchValue3 = 4
        pppuVar = resource2
        getHero = hero
        scratchValue = resources:TryAcquire(pppuVar, getHero, scratchValue3)
    end
    scratchValue = quest:IsActiveThreadTerminating()
    if scratchValue then
        goto LAB_00d67db1
    else
        actorMap = resources:NewActorMap()
        resources:SetActor(actorMap, "HERO", resource2)
        resources:SetActor(actorMap, "BAN1", scratchValue5[0 + 1])
        resources:SetActor(actorMap, "BAN2", scratchValue5[1 + 1])
        resources:SetActor(actorMap, "BAN3", scratchValue5[2 + 1])
        resources:SetActor(actorMap, "WHISPER", resource3)
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_GUILD_WILL_WOODS_INTRO", actorMap, false, true)
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource2)
        for _, r in ipairs(scratchValue5) do resources:ReleaseResource(r) end
        ctr_88 = 0
        if #willBandit ~= 0 then
            repeat
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then goto LAB_00d685cc end
                getHero = willBandit[scratchValue2 / 12 + 1]
                local pTarget = hero
                quest:GiveThingBestEnemyTarget(getHero, pTarget)
                getHero = willBandit[scratchValue2 / 12 + 1]
                scratchValue = false
                local fret_0 = quest:GetHealth(getHero)
                quest:ModifyThingHealth(getHero, 15.0 - fret_0, scratchValue)
                willBandit[scratchValue2 / 12 + 1]:SetToKillOnLevelUnload(false)
                ctr_88 = ctr_88 + 1
                scratchValue2 = scratchValue2 + 12
            until ctr_88 >= #willBandit
        end
        scratchValue = quest:IsActiveThreadTerminating()
        if not scratchValue then
            resources:PrepareResource(resource3)
            while quest:GetStateBool("BanditsAlive") do
                quest:NewScriptFrame()
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then goto LAB_00d685cc end
                scratchValue = quest:IsLevelLoaded("GuildWoods")
                if not scratchValue then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00d685cc end
                    scratchValue = quest:IsLevelLoaded("GuildWoods")
                    while not scratchValue do
                        quest:NewScriptFrame()
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00d685cc end
                        scratchValue = quest:IsLevelLoaded("GuildWoods")
                    end
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00d685cc end
                end
                willBandit = quest:GetAllThingsWithScriptName("WillBandit")
                if #willBandit == 0 then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00d685cc end
                    quest:SetStateBool("BanditsAlive", false)
                end
                getHero = hero
                local fret_00 = quest:GetHealth(getHero)
                if fret_00 < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00d685cc end
                    addNewConversation = quest:AddNewConversation(willWhisper, false, false)
                    getHero = hero
                    quest:AddPersonToConversation(addNewConversation, getHero)
                    getHero = hero
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_TEEN_WHISPER_SKILL_WOODS_HEALTH", willWhisper, getHero, false)
                    scratchValue = quest:IsXbox()
                    if scratchValue then
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00d685cc end
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_STRAFE_HELP")
                        scratchValue = quest:MsgIsGameInfoClickedPast()
                        while not scratchValue do
                            quest:NewScriptFrame()
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then goto LAB_00d685cc end
                            scratchValue = quest:MsgIsGameInfoClickedPast()
                        end
                    else
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00d685cc end
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_STRAFE_HELP_PC")
                        scratchValue = quest:MsgIsGameInfoClickedPast()
                        while not scratchValue do
                            quest:NewScriptFrame()
                            scratchValue = quest:IsActiveThreadTerminating()
                            if scratchValue then goto LAB_00d685cc end
                            scratchValue = quest:MsgIsGameInfoClickedPast()
                        end
                    end
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00d685cc end
                    quest:SetStateBool("WhisperAnimate", true)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                end
            end
            scratchValue = quest:IsActiveThreadTerminating()
            if not scratchValue then
                resources:PrepareResource(resource3)
                scratchValue = resources:TryAcquire(resource3, willWhisper, 4)
                while not scratchValue do
                    quest:NewScriptFrame()
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then goto LAB_00d685cc end
                    scratchValue = resources:TryAcquire(resource3, willWhisper, 4)
                end
                scratchValue = quest:IsActiveThreadTerminating()
                if not scratchValue then
                    resource2 = resources:NewResource()
                    resources:PrepareResource(resource2)
                    addNewConversation = 4
                    pppuVar = resource2
                    getHero = hero
                    scratchValue = resources:TryAcquire(pppuVar, getHero, addNewConversation)
                    while not scratchValue do
                        quest:NewScriptFrame()
                        scratchValue = quest:IsActiveThreadTerminating()
                        if scratchValue then goto LAB_00d6849b end
                        addNewConversation = 4
                        pppuVar = resource2
                        getHero = hero
                        scratchValue = resources:TryAcquire(pppuVar, getHero, addNewConversation)
                    end
                    scratchValue = quest:IsActiveThreadTerminating()
                    if scratchValue then
                        goto LAB_00d6849b
                    else
                        actorMap = resources:NewActorMap()
                        resources:SetActor(actorMap, "HERO", resource2)
                        resources:SetActor(actorMap, "WHISPER", resource3)
                        movie = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GUILD_WILL_WOODS_OUTRO", actorMap, false, true)
                        quest:FixMovieSequenceCamera(false)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        resources:DestroyActorMap(actorMap)
                        resources:ReleaseResource(resource2)
                        quest:SetStateBool("MissionSucceeded", true)
                    end
                    goto FLOW_past_lab_00d6849b
                    ::LAB_00d6849b::
                    resources:ReleaseResource(resource2)
                    ::FLOW_past_lab_00d6849b::
                end
            end
        end
    end
    goto FLOW_past_lab_00d67db1
    ::LAB_00d67db1::
    resources:ReleaseResource(resource2)
    for _, r in ipairs(scratchValue5) do resources:ReleaseResource(r) end
    ::FLOW_past_lab_00d67db1::
    ::LAB_00d685cc::
    ::LAB_00d685d5::
    resources:ReleaseResource(resource3)
end

-- Q_GuildTrainingWoodsWill.WatchForTermination (retail 0x00d68600)
function WatchForTermination(quest)
    while not quest:GetStateBool("MissionFailed") and not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if not quest:GetStateBool("MissionFailed") then
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, true, false)
    else
        quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, "", true)
    end
    quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
end

-- Q_GuildTrainingWoodsWill.DoMission (retail 0x00d68ae0)
function DoMission(quest)
    quest:GiveHeroNewQuestObjective("first objective", 1)
    local isLevelLoaded = quest:IsLevelLoaded("GuildWoods")
    while true do
        if isLevelLoaded then
            if quest:IsActiveThreadTerminating() then return end
            quest:CreateThread("WatchForLeaving")  -- native thread body NScript::CGlobal_WatchForHeroDeathScript::WatchForHeroDeath: lift it as function WatchForLeaving(quest)
            quest:NewScriptFrame()
            if not quest:IsActiveThreadTerminating() and not quest:GetStateBool("MissionFailed") then
                EndMission(quest)
            end
            return
        end
        if not quest:NewScriptFrame() then break end
        isLevelLoaded = quest:IsLevelLoaded("GuildWoods")
    end
end

-- Q_GuildTrainingWoodsWill.WatchForLeaving (retail 0x00d68c50)
function WatchForLeaving(quest)
    local hero = quest:GetHero()
    while hero ~= nil and hero:IsAlive() do
        if quest:GetStateBool("MissionFailed") or quest:GetStateBool("MissionSucceeded") then break end
        if not quest:NewScriptFrame() then return end
    end
    if not quest:IsActiveThreadTerminating() and not quest:GetStateBool("MissionSucceeded") then
        quest:SetStateBool("MissionFailed", true)
    end
end

-- Q_GuildTrainingWoodsWill.EndMission (retail 0x00d68cd0)
function EndMission(quest)
    local missionOver = quest:GetStateBool("MissionOver")
    while true do
        if missionOver then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("MissionSucceeded", true)
            return
        end
        if not quest:NewScriptFrame() then break end
        missionOver = quest:GetStateBool("MissionOver")
    end
end

