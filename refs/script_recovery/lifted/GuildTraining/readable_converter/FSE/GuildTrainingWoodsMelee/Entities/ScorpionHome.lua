-- Readable native conversion: ScorpionHome. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_MeleeBeetles = 3856,  -- 10.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local scorpionsLeft, flourishHint

-- ScorpionHome.Main (retail 0x00d67270)
function Main(quest, me)
    local count, pPosition, scorpionSpawn, guildStagBeetle, guildScorpions
    local hero = quest:GetHero()
    local infoCounter = quest:AddQuestInfoCounter("HUD_BEETLE_ICON", math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MeleeBeetles))), 1.0)
    quest:DisplayQuestInfo(true)
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 5)
    local scorpionsAlive = quest:GetStateBool("ScorpionsAlive")
    repeat
        if not scorpionsAlive then
            if not quest:IsActiveThreadTerminating() then
                quest:RemoveQuestInfoElement(infoCounter)
                quest:DisplayQuestInfo(false)
            end
            quest:DeregisterTimer(timerId)
            return
        end
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); return end
        local isPlayerCarryingItemOfType = quest:IsPlayerCarryingItemOfType("OBJECT_HERO_STICK") or 0 < quest:GetTimer(timerId)
        if not isPlayerCarryingItemOfType then
            quest:AddLineToConversation(quest:AddNewConversation(hero, false, false), "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_REPEAT", hero, hero, false)
            quest:SetTimer(timerId, 8)
        end
        guildScorpions = quest:GetAllThingsWithScriptName("GuildScorpions")
        count = #guildScorpions
        quest:UpdateQuestInfoCounter(infoCounter, math.tointeger(math.modf((quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MeleeBeetles) - scorpionsLeft) - count)), -1)
        if #guildScorpions >= 3 then scorpionsAlive = quest:GetStateBool("ScorpionsAlive"); goto continue_1 end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        if #guildScorpions == 0 and scorpionsLeft == 0 then
            quest:SetStateBool("ScorpionsAlive", false)
            quest:SetMasterGameState("ScorpionsDestroyed", true)
        elseif 0 < scorpionsLeft then
            scorpionSpawn = quest:GetFurthestWithScriptName(hero, "ScorpionSpawn")
            if scorpionSpawn == nil then
                pPosition = {x = 0, y = 0, z = 0}
            else
                pPosition = scorpionSpawn:GetPos()
            end
            guildStagBeetle = quest:CreateCreature("CREATURE_GUILD_STAG_BEETLE", pPosition, "GuildScorpions")
            if guildStagBeetle ~= nil then
                guildStagBeetle:SetToKillOnLevelUnload(0)
            end
            quest:EntityAttachToScript(guildStagBeetle, "Q_GuildTrainingWoodsMelee")
            scorpionsLeft = scorpionsLeft - 1
        end
        scorpionsAlive = quest:GetStateBool("ScorpionsAlive")
        ::continue_1::
    until false
end

-- ScorpionHome.Init (retail 0x00d66c60)
function Init(quest, me)
    scorpionsLeft = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MeleeBeetles)))
    flourishHint = false
end

-- ScorpionHome.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- ScorpionHome.OnPredicateFail (retail 0x00d66c50)
function OnPredicateFail(quest, me)
end

