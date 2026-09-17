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

-- ScorpionHome.Main (retail 0x00d67270)
function Main(quest, me)
    local scratchValue, scratchValue3, hero2, pPosition, pSpeaker, r1_1, r1_2
    local getAllThingsWithScriptName
    local scratchValue5 = quest:AddQuestInfoCounter("HUD_BEETLE_ICON", math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(3856))), 1.0)
    quest:DisplayQuestInfo(true)
    local timerId = quest:RegisterTimer()
    local scratchValue6 = timerId
    quest:SetTimer(timerId, 5)
    local scorpionsAlive = quest:GetStateBool("ScorpionsAlive")
    repeat
        if not scorpionsAlive then
            if not quest:IsActiveThreadTerminating() then
                quest:RemoveQuestInfoElement(scratchValue5)
                quest:DisplayQuestInfo(false)
            end
            quest:DeregisterTimer(timerId)
            return
        end
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); return end
        local scratchValue7 = quest:IsPlayerCarryingItemOfType("OBJECT_HERO_STICK") or 0 < quest:GetTimer(timerId)
        if not scratchValue7 then
            hero2 = quest:GetHero()
            pSpeaker = quest:GetHero()
            quest:AddLineToConversation(quest:AddNewConversation(quest:GetHero(), false, false), "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_REPEAT", pSpeaker, hero2, false)
            quest:SetTimer(scratchValue6, 8)
        end
        getAllThingsWithScriptName = quest:GetAllThingsWithScriptName("GuildScorpions")
        scratchValue3 = #getAllThingsWithScriptName
        scratchValue = scratchValue3
        if scratchValue3 < 0 then
            scratchValue = scratchValue + 4294967296.0
        end
        quest:UpdateQuestInfoCounter(scratchValue5, math.tointeger(math.modf((quest:ReadGlobalGameDataFloat(3856) - state:GetInt("ScorpionsLeft")) - scratchValue)), -1)
        if #getAllThingsWithScriptName < 3 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue6); return end
            if #getAllThingsWithScriptName == 0 and state:GetInt("ScorpionsLeft") == 0 then
                quest:SetStateBool("ScorpionsAlive", false)
                quest:SetMasterGameState("ScorpionsDestroyed", true)
            elseif 0 < state:GetInt("ScorpionsLeft") then
                r1_1 = quest:GetFurthestWithScriptName(quest:GetHero(), "ScorpionSpawn")
                if not (r1_1 ~= nil and not r1_1:IsNull()) then
                    pPosition = {x = 0, y = 0, z = 0}
                else
                    pPosition = r1_1:GetPos()
                end
                r1_2 = quest:CreateCreature("CREATURE_GUILD_STAG_BEETLE", pPosition, "GuildScorpions")
                if r1_2 ~= nil and not r1_2:IsNull() then
                    r1_2:SetToKillOnLevelUnload(0)
                end
                quest:EntityAttachToScript(r1_2, "Q_GuildTrainingWoodsMelee")
                state:SetInt("ScorpionsLeft", state:GetInt("ScorpionsLeft") - 1)
            end
        end
        scorpionsAlive = quest:GetStateBool("ScorpionsAlive")
        timerId = scratchValue6
    until false
end

-- ScorpionHome.Init (retail 0x00d66c60)
function Init(quest, me)
    state:SetInt("ScorpionsLeft", math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(3856))))
    state:SetBool("FlourishHint", false)
end

-- ScorpionHome.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- ScorpionHome.OnPredicateFail (retail 0x00d66c50)
function OnPredicateFail(quest, me)
end

