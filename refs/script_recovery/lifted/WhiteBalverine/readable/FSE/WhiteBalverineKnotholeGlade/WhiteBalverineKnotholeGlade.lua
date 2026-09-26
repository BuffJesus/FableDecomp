-- Readable native conversion: Q_WhiteBalverineKnotholeGlade. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local TUTORIAL_CATEGORY_AUGMENTATION = 40  -- ETutorialCategory (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    EXPERIENCE_GRANT_SMALL = 60,  -- 10
    WhiteBalvNakedBoastCost = 528,  -- 800
    WhiteBalvNakedBoastReward = 532,  -- 1800
    WhiteBalvNoDamageBoastCost = 536,  -- 500
    WhiteBalvNoDamageBoastReward = 540,  -- 2500
    WBK_RenownAwardForSecondDefence = 4000,  -- 15.0
}

-- Q_WhiteBalverineKnotholeGlade.Main (retail 0x00e13f10)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local resource5, actorMap2, resource8, movie, movie2
    while not quest:IsLevelLoaded("KnotholeGlade") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if quest:GetStateInt("BalverineState") == 0 then
        quest:ActivateQuest("V_KnotholeGladeGates")
    end
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_01", "KnotholeGlade", "KnotholeGlade")
    quest:AddEntityBinding("WB_Villager", "WhiteBalverineKnotholeGlade/Entities/WB_Villager", 1)
    quest:AddEntityBinding("WB_WhiteBalverine", "WhiteBalverineKnotholeGlade/Entities/WB_WhiteBalverine", 1)
    quest:AddEntityBinding("WB_ScaredVillager", "WhiteBalverineKnotholeGlade/Entities/WB_ScaredVillager", 1)
    quest:AddEntityBinding("WB_Guard1", "WhiteBalverineKnotholeGlade/Entities/WB_Guard1", 1)
    quest:AddEntityBinding("WB_Guard2", "WhiteBalverineKnotholeGlade/Entities/WB_Guard2", 1)
    quest:AddEntityBinding("KG_Chief", "WhiteBalverineKnotholeGlade/Entities/KG_Chief", 1)
    quest:FinalizeEntityBindings()
    while not quest:IsLevelLoaded("KnotholeGlade") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local chief = quest:GetThingWithScriptName("KG_Chief")
    quest:SetVillageLimbo(quest:GetThingWithScriptName("VILLAGE_KNOTHOLEGLADE"), true)
    if not quest:GetStateBool("QuestStartScreenShown") then
        if quest:IsActiveThreadTerminating() then goto LAB_00e154cb end
        quest:SetStateBool("QuestStartScreenShown", true)
        quest:KickOffQuestStartScreen("Q_WhiteBalverineKnotholeGlade", true, false)
    end
    quest:CreateThread("WatchForTermination")  -- native thread body CScriptGameResourceObjectScriptedThingBase_HandleWhiteBalverineQuestObjective: lift it as function WatchForTermination(quest)
    if quest:GetStateInt("BalverineState") == 0 or quest:GetStateInt("BalverineState") == 1 then
        if quest:IsActiveThreadTerminating() then goto LAB_00e154cb end
        quest:SetStateInt("BalverineState", 1)
        while quest:IsQuestActive("V_KnotholeGladeGates") do
            if not quest:NewScriptFrame() then goto LAB_00e154cb end
        end
        quest:SetStateInt("BalverineState", 2)
    end
    if quest:GetStateInt("BalverineState") == 2 then
        if quest:IsActiveThreadTerminating() then goto LAB_00e154cb end
        local whiteBalverine = quest:GetThingWithScriptName("WB_WhiteBalverine")
        if not (whiteBalverine ~= nil and whiteBalverine:IsAlive()) then
            quest:CreateCreature("CREATURE_FULLMOON_BALVERINE", quest:GetThingWithScriptName("WB_WhiteBalverineSpawnMarker"):GetPos(), "WB_WhiteBalverine")
        end
        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_02", "KnotholeGlade", "KnotholeGlade")
        while quest:GetStateInt("BalverineState") ~= 3 do
            if not quest:NewScriptFrame() then goto LAB_00e154cb end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e154cb end
        quest:StopOverrideMusic(false)
    end
    if quest:GetStateInt("BalverineState") == 3 then
        if quest:IsActiveThreadTerminating() then goto LAB_00e154cb end
        if not quest:NewScriptFrame() then goto LAB_00e154cb end
        while not quest:IsHeroControlledByPlayer() do
            if not quest:NewScriptFrame() then goto LAB_00e154cb end
        end
        local resource9 = resources:NewResource()
        local resource = resources:NewResource()
        resources:TryAcquire(resource9, chief, 4)
        resources:TryAcquire(resource, hero, 4)
        local actorMap3 = resources:NewActorMap()
        resources:SetActor(actorMap3, "HERO", resource)
        resources:SetActor(actorMap3, "CHIEF", resource9)
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_WBK_CHIEF1", actorMap3, false, true)
        quest:FixMovieSequenceCamera(false)
        quest:SetStateBool("GreenWife", true)
        resources:PrepareResource(resource9)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap3)
        resources:ReleaseResource(resource)
        resources:ReleaseResource(resource9)
        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_03", "KnotholeGlade", "KnotholeGlade")
        while not quest:IsDistanceBetweenThingsUnder(quest:GetThingWithScriptName("WhiteBalverineAmbushMarker"), hero, 10.0) do
            if not quest:NewScriptFrame() then goto LAB_00e154cb end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e154cb end
        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_04", "KnotholeGlade", "KnotholeGlade")
        quest:SetStateInt("BalverineState", 4)
    end
    if quest:GetStateInt("BalverineState") == 4 then
        if quest:IsActiveThreadTerminating() then goto LAB_00e154cb end
        local whiteBalverine2 = quest:GetThingWithScriptName("WB_WhiteBalverine")
        if not (whiteBalverine2 ~= nil and whiteBalverine2:IsAlive()) then
            local position = quest:GetThingWithScriptName("WB_EscapePoint2"):GetPos()
            quest:CreateCreature("CREATURE_FULLMOON_BALVERINE", {x = position.x, y = position.y, z = position.z + 9.0}, "WB_WhiteBalverine")
        end
        while quest:GetStateInt("BalverineState") ~= 5 do
            if not quest:NewScriptFrame() then goto LAB_00e154cb end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e154cb end
        quest:StopOverrideMusic(false)
    end
    if quest:GetStateInt("BalverineState") == 5 then
        if quest:IsActiveThreadTerminating() then goto LAB_00e154cb end
        if not quest:NewScriptFrame() then goto LAB_00e154cb end
        while not quest:IsHeroControlledByPlayer() do
            if not quest:NewScriptFrame() then goto LAB_00e154cb end
        end
        quest:SetStateBool("WifeCutsceneStart", true)
        local resource7 = resources:NewResource()
        chief:AcquireControl(4)
        resources:TryAcquire(resource7, hero, 4)
        local actorMap = resources:NewActorMap()
        resources:SetActor(actorMap, "HERO", resource7)
        resources:SetActor(actorMap, "CHIEF", resource7)
        resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_WBK_CHIEF2", actorMap, false, true)
        quest:FixMovieSequenceCamera(false)
        quest:SetStateBool("WifeCutsceneStop", true)
        resources:PrepareResource(resource7)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource7)
        resources:ReleaseResource(resource7)
        if not quest:NewScriptFrame() then goto LAB_00e154cb end
        if not quest:NewScriptFrame() then goto LAB_00e154cb end
        if not quest:NewScriptFrame() then goto LAB_00e154cb end
        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_05", "KnotholeGlade", "KnotholeGlade")
        quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_SMALL))
        quest:GiveHeroRenownPoints(math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.WBK_RenownAwardForSecondDefence))))
        if not quest:NewScriptFrame() then goto LAB_00e154cb end
        if not quest:NewScriptFrame() then goto LAB_00e154cb end
        if not quest:NewScriptFrame() then goto LAB_00e154cb end
        quest:Pause(2.0)
        if quest:DisplayTutorial(TUTORIAL_CATEGORY_AUGMENTATION) then
            if quest:IsActiveThreadTerminating() then goto LAB_00e154cb end
            while not quest:MsgIsTutorialClickedPast() do
                if not quest:NewScriptFrame() then goto LAB_00e154cb end
            end
        end
        quest:SetStateInt("BalverineState", 6)
    end
    if quest:GetStateInt("BalverineState") == 6 then
        if quest:IsActiveThreadTerminating() then goto LAB_00e154cb end
        local whiteBalverine3 = quest:GetThingWithScriptName("WB_WhiteBalverine")
        if not (whiteBalverine3 ~= nil and whiteBalverine3:IsAlive()) then
            quest:CreateCreature("CREATURE_FULLMOON_BALVERINE", quest:GetThingWithScriptName("WB_BalvEscape2"):GetPos(), "WB_WhiteBalverine")
        end
        quest:CreateCreature("CREATURE_KN_GUARD", quest:GetThingWithScriptName("WB_Guard1Pos"):GetPos(), "WB_Guard1")
        quest:CreateCreature("CREATURE_KN_GUARD", quest:GetThingWithScriptName("WB_Guard2Pos"):GetPos(), "WB_Guard2")
        while quest:GetStateInt("BalverineState") ~= 7 do
            if not quest:NewScriptFrame() then goto LAB_00e154cb end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e154cb end
        quest:StopOverrideMusic(false)
    end
    if not (quest:GetStateInt("BalverineState") == 7 and not quest:IsActiveThreadTerminating()) then goto LAB_00e154cb end
    while not quest:IsHeroControlledByPlayer() do
        if not quest:NewScriptFrame() then goto LAB_00e154cb end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e154cb end
    resource8 = resources:NewResource()
    resource5 = resources:NewResource()
    resources:TryAcquire(resource8, chief, 4)
    resources:TryAcquire(resource5, hero, 4)
    actorMap2 = resources:NewActorMap()
    resources:SetActor(actorMap2, "HERO", resource5)
    resources:SetActor(actorMap2, "CHIEF", resource8)
    movie2 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_WBK_CHIEF3", actorMap2, false, true)
    quest:FixMovieSequenceCamera(false)
    resources:PrepareResource(resource8)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie2)
    resources:DestroyActorMap(actorMap2)
    resources:ReleaseResource(resource5)
    resources:ReleaseResource(resource8)
    quest:SetStateBool("MissionSucceeded", true)
    ::LAB_00e154cb::
end

-- Q_WhiteBalverineKnotholeGlade.Init (retail 0x00e13c60)
function Init(quest)
    quest:AddQuestRegion("Q_WhiteBalverineKnotholeGlade", "KnotholeGlade")
    quest:AddQuestRegion("Q_WhiteBalverineKnotholeGlade", "Witchwood4")
    quest:AddQuestRegion("Q_WhiteBalverineKnotholeGlade", "DemonDoor_KnotholeGlade")
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(SCRIPT_DEF.WhiteBalvNakedBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.WhiteBalvNakedBoastReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(SCRIPT_DEF.WhiteBalvNoDamageBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.WhiteBalvNoDamageBoastReward), false, "", 0)
    quest:SetStateInt("BalverineState", 0)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("WifeCutsceneStart", false)
    quest:SetStateBool("WifeCutsceneStop", false)
    quest:SetStateBool("GreenWife", false)
    quest:SetStateBool("QuestStartScreenShown", false)
end

-- Q_WhiteBalverineKnotholeGlade.OnPersist (retail 0x00e13ee0)
function OnPersist(quest, context)
    quest:SetStateBool("QuestStartScreenShown", quest:PersistTransferBool(context, "QuestStartScreenShown", quest:GetStateBool("QuestStartScreenShown")))
end

-- Q_WhiteBalverineKnotholeGlade.WatchForTermination (retail 0x00e15500)
function WatchForTermination(quest)
    local missionFailed = quest:GetStateBool("MissionFailed")
    while not missionFailed and not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if missionFailed then
        quest:SetQuestAsFailed(quest:GetActiveQuestName(), false, "", true)
    end
    quest:ActivateQuest("Q_WhiteBalverineWW")
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_06", "Witchwood4", "KnotholeGlade")
    quest:SetVillageLimbo(quest:GetThingWithScriptName("VILLAGE_KNOTHOLEGLADE"), false)
    while not quest:GetMasterGameState("WhiteBalverineFinished") do
        if not quest:NewScriptFrame() then goto LAB_00e15740 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e15740 end
    quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, false, false)
    quest:Pause(1.0)
    quest:FadeScreenIn()
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    ::LAB_00e15740::
end

