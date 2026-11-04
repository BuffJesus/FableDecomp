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
    local pPosition, guildStagBeetle
    local scratchValue2 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MeleeBeetles)))
    local infoCounter = quest:AddQuestInfoCounter("HUD_BEETLE_ICON", scratchValue2, 1.0)
    quest:DisplayQuestInfo(true)
    local timerId = quest:RegisterTimer()
    local timerId2 = timerId
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
        local predicateResult = not isPlayerCarryingItemOfType
        if predicateResult then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            local hero = quest:GetHero()
            local conversationId = quest:AddNewConversation(hero, false, false)
            local hero2 = quest:GetHero()
            local pSpeaker = quest:GetHero()
            quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_REPEAT", pSpeaker, hero2, false)
            quest:SetTimer(timerId2, 8)
        end
        local guildScorpions = quest:GetAllThingsWithScriptName("GuildScorpions")
        local count2 = #guildScorpions
        local scratchValue = count2
        local count = math.tointeger(math.modf((quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MeleeBeetles) - scorpionsLeft) - scratchValue))
        quest:UpdateQuestInfoCounter(infoCounter, count, -1)
        if not ((#guildScorpions) < 3) then scorpionsAlive = quest:GetStateBool("ScorpionsAlive"); timerId = timerId2; goto continue_1 end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); return end
        if (#guildScorpions == 0) and scorpionsLeft == 0 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); return end
            quest:SetStateBool("ScorpionsAlive", false)
            quest:SetMasterGameState("ScorpionsDestroyed", true)
        elseif 0 < scorpionsLeft then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); return end
            local hero3 = quest:GetHero()
            local scorpionSpawn = quest:GetFurthestWithScriptName(hero3, "ScorpionSpawn")
            if scorpionSpawn == nil then
                pPosition = {x = 0, y = 0, z = 0}
            else
                pPosition = scorpionSpawn:GetPos()
            end
            guildStagBeetle = quest:CreateCreature("CREATURE_GUILD_STAG_BEETLE", pPosition, "GuildScorpions", false)
            if guildStagBeetle ~= nil then
                guildStagBeetle:SetToKillOnLevelUnload(false)
            end
            quest:EntityAttachToScript(guildStagBeetle, "Q_GuildTrainingWoodsMelee")
            scorpionsLeft = scorpionsLeft - 1
            guildStagBeetle = nil
        end
        scorpionsAlive = quest:GetStateBool("ScorpionsAlive")
        timerId = timerId2
        ::continue_1::
    until false
end

-- ScorpionHome.Init (retail 0x00d66c60)
function Init(quest, me)
    scorpionsLeft = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MeleeBeetles)))
    flourishHint = false
end

-- ScorpionHome.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- ScorpionHome.OnPredicateFail (retail 0x00d66c50)
function OnPredicateFail(quest, me)
end

