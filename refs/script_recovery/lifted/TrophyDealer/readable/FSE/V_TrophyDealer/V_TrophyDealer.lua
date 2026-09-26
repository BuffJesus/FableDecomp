-- Readable native conversion: V_TrophyDealer. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- V_TrophyDealer.Main (retail 0x00ee6e90)
function Main(quest)
    quest:SetCreatureGeneratorsEnabled("Witchwood2", false)
    quest:AddEntityBinding("TrophyDealerInCave", "V_TrophyDealer/Entities/TrophyDealerInCave")
    quest:AddEntityBinding("DemonDoorFace", "V_TrophyDealer/Entities/DemonDoorFace")
    quest:FinalizeEntityBindings()
    quest:ActivateQuestWithoutLoadingResources("V_SingingStones")
    quest:CreateThread("HilightGuildTeleporter")  -- native thread body NScript::CV_TrophyDealerScript::HilightGuildTeleporter: lift it as function HilightGuildTeleporter(quest)
    local isRegionLoaded = quest:IsRegionLoaded("Witchwood1")
    while true do
        if isRegionLoaded then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_02", "Witchwood2", "")
            quest:KickOffQuestStartScreen("V_TrophyDealer", true, false)
            return
        end
        if not quest:NewScriptFrame() then break end
        isRegionLoaded = quest:IsRegionLoaded("Witchwood1")
    end
end

-- V_TrophyDealer.Init (retail 0x00ee6d60)
function Init(quest)
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_01", "Witchwood2", "")
end

-- V_TrophyDealer.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

-- V_TrophyDealer.HilightGuildTeleporter (retail 0x00ee71a0)
function HilightGuildTeleporter(quest)
    while not quest:IsRegionLoaded("HeroGuildComplexInside") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local heroGuildTeleportMarker = quest:GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER")
    quest:MiniMapAddMarker(heroGuildTeleportMarker, "HUD_ORB_QUEST_CORE")
    while not quest:IsRegionLoaded("Witchwood1") do
        if not quest:NewScriptFrame() then goto LAB_00ee7326 end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:MiniMapRemoveMarker(heroGuildTeleportMarker)
    end
    ::LAB_00ee7326::
end

