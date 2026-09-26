-- Generated native draft: V_TrophyDealer. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local pQuestName
    local alive = true
    quest:SetCreatureGeneratorsEnabled("Witchwood2", false)
    quest:AddEntityBinding("TrophyDealerInCave", "V_TrophyDealer/Entities/TrophyDealerInCave")
    quest:AddEntityBinding("DemonDoorFace", "V_TrophyDealer/Entities/DemonDoorFace")
    quest:FinalizeEntityBindings()
    quest:ActivateQuestWithoutLoadingResources("V_SingingStones")
    quest:CreateThread("HilightGuildTeleporter")  -- native thread body NScript::CV_TrophyDealerScript::HilightGuildTeleporter: lift it as function HilightGuildTeleporter(quest)
    if not bVar3 then
    end
    local bVar3 = quest:IsRegionLoaded("Witchwood1")
    while true do
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                pQuestName = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_02", "Witchwood2", "")
                quest:KickOffQuestStartScreen("V_TrophyDealer", true, false)
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then break end
        bVar3 = quest:IsRegionLoaded("Witchwood1")
    end
end

function Init(quest)
    local pQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_01", "Witchwood2", "")
end

function OnPersist(quest, context)
end

function HilightGuildTeleporter(quest)
    local bVar1, r1
    local alive = true
    bVar1 = quest:IsRegionLoaded("HeroGuildComplexInside")
    while not bVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        bVar1 = quest:IsRegionLoaded("HeroGuildComplexInside")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        r1 = quest:GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER")
        quest:MiniMapAddMarker(r1, "HUD_ORB_QUEST_CORE")
        bVar1 = quest:IsRegionLoaded("Witchwood1")
        while not bVar1 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then goto LAB_00ee7326 end
            bVar1 = quest:IsRegionLoaded("Witchwood1")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            quest:MiniMapRemoveMarker(r1)
        end
        ::LAB_00ee7326::
    end
end

