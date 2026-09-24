-- Readable native conversion: DarkwoodRockTroll. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- per-entity fields (native class members; one Lua state per entity instance)
local healthBarID

-- DarkwoodRockTroll.Main (retail 0x00e03d30)
function Main(quest, me)
    if not quest:NewScriptFrame(me) then return end
    quest:GiveThingBestEnemyTarget(me, quest:GetHero())
    while 0.0 < quest:GetHealth(me) do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields")
    quest:SetStateBool("TradersShouldBeScared", false)
end

-- DarkwoodRockTroll.Init (retail 0x00e03bf0)
function Init(quest, me)
    quest:SetStateBool("TradersShouldBeScared", true)
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_04", "BarrowFields", "BarrowFields")
    quest:DisplayQuestInfo(true)
    healthBarID = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_EARTH_TROLL", 1.0)
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
end

-- DarkwoodRockTroll.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- DarkwoodRockTroll.OnPredicateFail (retail 0x00e03b00)
function OnPredicateFail(quest, me)
    if not me:MsgIsKilledBy("") or not quest:GetStateBool("TradersShouldBeScared") then return end
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields")
    quest:SetStateBool("TradersShouldBeScared", false)
end

