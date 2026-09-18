-- Readable native conversion: ScorpionHome. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_DepartureBeetles = 3852,  -- 15.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local scorpionsLeft, flourishHint

-- ScorpionHome.Main (retail 0x00d643a0)
function Main(quest, me)
    local count, pPosition, scorpionSpawn, guildStagBeetle, guildScorpions, infoCounter
    while quest:GetStateInt("DepartureMissionPoint") ~= 1 do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    infoCounter = quest:AddQuestInfoCounter("HUD_BEETLE_ICON", math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_DepartureBeetles))), 1.0)
    quest:DisplayQuestInfo(true)
    while quest:GetStateInt("DepartureMissionPoint") == 1 do
        if not quest:NewScriptFrame(me) then return end
        guildScorpions = quest:GetAllThingsWithScriptName("GuildScorpions")
        count = #guildScorpions
        quest:UpdateQuestInfoCounter(infoCounter, math.tointeger(math.modf((quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_DepartureBeetles) - scorpionsLeft) - count)), -1)
        if #guildScorpions < 3 then
            if quest:IsActiveThreadTerminating() then return end
            if #guildScorpions == 0 and scorpionsLeft == 0 then
                quest:SetStateInt("DepartureMissionPoint", 2)
                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12", "", "")
            elseif 0 < scorpionsLeft then
                scorpionSpawn = quest:GetFurthestWithScriptName(quest:GetHero(), "ScorpionSpawn")
                if scorpionSpawn == nil then
                    pPosition = {x = 0, y = 0, z = 0}
                else
                    pPosition = scorpionSpawn:GetPos()
                end
                guildStagBeetle = quest:CreateCreature("CREATURE_GUILD_STAG_BEETLE", pPosition, "GuildScorpions")
                if guildStagBeetle ~= nil then
                    guildStagBeetle:SetToKillOnLevelUnload(0)
                end
                quest:EntityAttachToScript(guildStagBeetle, "Q_GuildTrainingWoodsDeparture")
                scorpionsLeft = scorpionsLeft - 1
            end
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:RemoveQuestInfoElement(infoCounter)
    quest:DisplayQuestInfo(false)
end

-- ScorpionHome.Init (retail 0x00d63d80)
function Init(quest, me)
    scorpionsLeft = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_DepartureBeetles)))
    flourishHint = false
end

-- ScorpionHome.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- ScorpionHome.OnPredicateFail (retail 0x00d63d70)
function OnPredicateFail(quest, me)
end

