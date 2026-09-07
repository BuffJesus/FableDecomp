Quest = nil

local QUEST_NAME = "TrophyDealer"

function HilightGuildTeleporter(quest)
    quest:Log("TrophyDealer: HilightGuildTeleporter thread started.")

    -- Wait until HeroGuildComplexInside is loaded
    while not quest:IsRegionLoaded("HeroGuildComplexInside") do
        if not quest:NewScriptFrame() then
            return
        end
    end

    local teleporter = quest:GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER")
    if teleporter then
        quest:MiniMapAddMarker(teleporter, "HUD_ORB_QUEST_CORE")
        quest:Log("TrophyDealer: Added minimap marker to HERO_GUILD_TELEPORT_MARKER.")
    else
        quest:Log("!!! WARNING: TrophyDealer - HERO_GUILD_TELEPORT_MARKER not found.")
    end

    -- Wait until Witchwood1 is loaded
    while not quest:IsRegionLoaded("Witchwood1") do
        if not quest:NewScriptFrame() then
            return
        end
    end

    if teleporter then
        quest:MiniMapRemoveMarker(teleporter)
        quest:Log("TrophyDealer: Removed minimap marker from HERO_GUILD_TELEPORT_MARKER.")
    end

    quest:Log("TrophyDealer: HilightGuildTeleporter thread completed.")
end

function Init(questObject)
    Quest = questObject
    Quest:Log("TrophyDealer: Init phase started.")

    local activeQuestName = Quest:GetActiveQuestName()
    Quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_01", "Witchwood2", "")
    Quest:SetStateBool("StartScreenShown", false)
    Quest:Log("TrophyDealer: Objective 01 set.")
end

function Main(questObject)
    Quest = questObject
    Quest:Log("TrophyDealer: Main() started.")

    -- Disable creature generators in Witchwood2
    Quest:SetCreatureGeneratorsEnabled("Witchwood2", false)

    -- Register scripted entity bindings
    Quest:AddEntityBinding("TrophyDealerInCave", "TrophyDealer/Entities/TrophyDealerInCave")
    Quest:AddEntityBinding("DemonDoorFace", "TrophyDealer/Entities/DemonDoorFace")
    Quest:FinalizeEntityBindings()
    Quest:Log("TrophyDealer: Entity bindings finalized.")

    -- Activate the Singing Stones quest without preloading resources
    Quest:ActivateQuestWithoutLoadingResources("V_SingingStones")
    Quest:Log("TrophyDealer: Activated V_SingingStones without loading resources.")

    -- Spawn microthread to mark Guild teleporter
    Quest:CreateThread("HilightGuildTeleporter")

    -- Wait until Witchwood1 is loaded
    while not Quest:IsRegionLoaded("Witchwood1") do
        if not Quest:NewScriptFrame() then
            return
        end
    end

    -- Set objective 02 and kick off quest start screen
    local activeQuestName = Quest:GetActiveQuestName()
    Quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_02", "Witchwood2", "")
    Quest:KickOffQuestStartScreen("V_TrophyDealer", true, false)
    Quest:SetStateBool("StartScreenShown", true)
    Quest:Log("TrophyDealer: Objective 02 set and start screen displayed.")
end

function OnPersist(questObject, context)
    Quest = questObject
    local startScreenShown = Quest:GetStateBool("StartScreenShown") or false
    startScreenShown = Quest:PersistTransferBool(context, "StartScreenShown", startScreenShown)
    Quest:SetStateBool("StartScreenShown", startScreenShown)
end
