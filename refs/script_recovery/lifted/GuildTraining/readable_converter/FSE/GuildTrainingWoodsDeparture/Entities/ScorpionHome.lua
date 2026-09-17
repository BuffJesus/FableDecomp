-- Readable native conversion: ScorpionHome. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- ScorpionHome.Main (retail 0x00d643a0)
function Main(quest, me)
    local scratchValue, scratchValue3, pPosition, r1_1, getAllThingsWithScriptName, scratchValue6
    local departureMissionPoint = quest:GetStateInt("DepartureMissionPoint")
    while departureMissionPoint ~= 1 do
        if not quest:NewScriptFrame(me) then return end
        departureMissionPoint = quest:GetStateInt("DepartureMissionPoint")
    end
    if quest:IsActiveThreadTerminating() then return end
    scratchValue6 = quest:AddQuestInfoCounter("HUD_BEETLE_ICON", math.modf(quest:ReadGlobalGameDataFloat(3852)), 1.0)
    quest:DisplayQuestInfo(true)
    departureMissionPoint = quest:GetStateInt("DepartureMissionPoint")
    while departureMissionPoint == 1 do
        if not quest:NewScriptFrame(me) then return end
        getAllThingsWithScriptName = quest:GetAllThingsWithScriptName("GuildScorpions")
        scratchValue3 = (#getAllThingsWithScriptName * 12) / 12
        scratchValue = scratchValue3
        if scratchValue3 < 0 then
            scratchValue = scratchValue + 4294967296.0
        end
        quest:UpdateQuestInfoCounter(scratchValue6, math.modf((quest:ReadGlobalGameDataFloat(3852) - state:GetInt("ScorpionsLeft")) - scratchValue), -1)
        if ((#getAllThingsWithScriptName * 12) / 12) < 3 then
            if quest:IsActiveThreadTerminating() then return end
            if #getAllThingsWithScriptName == 0 and state:GetInt("ScorpionsLeft") == 0 then
                quest:SetStateInt("DepartureMissionPoint", 2)
                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12", "", "")
            elseif 0 < state:GetInt("ScorpionsLeft") then
                r1_1 = quest:GetFurthestWithScriptName(quest:GetHero(), "ScorpionSpawn")
                if not (r1_1 ~= nil and not r1_1:IsNull()) then
                    pPosition = {x = 0, y = 0, z = 0}
                else
                    pPosition = r1_1:GetPos()
                end
                quest:CreateCreature("CREATURE_GUILD_STAG_BEETLE", pPosition, "GuildScorpions")
                if r1_1 ~= nil and not r1_1:IsNull() then
                    r1_1:SetToKillOnLevelUnload(0)
                end
                quest:EntityAttachToScript(r1_1, "Q_GuildTrainingWoodsDeparture")
                state:SetInt("ScorpionsLeft", state:GetInt("ScorpionsLeft") - 1)
            end
        end
        departureMissionPoint = quest:GetStateInt("DepartureMissionPoint")
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:RemoveQuestInfoElement(scratchValue6)
    quest:DisplayQuestInfo(false)
end

-- ScorpionHome.Init (retail 0x00d63d80)
function Init(quest, me)
    state:SetInt("ScorpionsLeft", math.modf(quest:ReadGlobalGameDataFloat(3852)))
    state:SetBool("FlourishHint", false)
end

-- ScorpionHome.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- ScorpionHome.OnPredicateFail (retail 0x00d63d70)
function OnPredicateFail(quest, me)
end

