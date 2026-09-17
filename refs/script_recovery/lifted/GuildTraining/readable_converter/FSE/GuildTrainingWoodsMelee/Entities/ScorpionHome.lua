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
    local predicateResult3, scratchValue, scratchValue3, scratchValue4, hero2, pPosition, pSpeaker
    local scratchValue6, scratchValue7, r1_1, scratchValue8, getAllThingsWithScriptName
    scratchValue8 = 0
    local scratchValue9 = quest:AddQuestInfoCounter("HUD_BEETLE_ICON", math.modf(quest:ReadGlobalGameDataFloat(3856)), 1.0)
    quest:DisplayQuestInfo(true)
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 5)
    local scorpionsAlive = quest:GetStateBool("ScorpionsAlive")
    repeat
        if not scorpionsAlive then
            if not quest:IsActiveThreadTerminating() then
                quest:RemoveQuestInfoElement(scratchValue9)
                quest:DisplayQuestInfo(false)
            end
            quest:DeregisterTimer(timerId)
            return
        end
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); return end
        scratchValue8 = scratchValue8 | 1
        local scratchValue10 = quest:IsPlayerCarryingItemOfType("OBJECT_HERO_STICK") or 0 < quest:GetTimer(timerId)
        predicateResult3 = not scratchValue10
        if scratchValue8 & true then
            scratchValue8 = scratchValue8 & 0xfffffffe
        end
        if predicateResult3 then
            hero2 = quest:GetHero()
            pSpeaker = quest:GetHero()
            quest:AddLineToConversation(quest:AddNewConversation(quest:GetHero(), false, false), "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_REPEAT", pSpeaker, hero2, false)
            quest:SetTimer(timerId, 8)
        end
        getAllThingsWithScriptName = quest:GetAllThingsWithScriptName("GuildScorpions")
        scratchValue4 = (scratchValue7 - getAllThingsWithScriptName) / 12
        scratchValue = scratchValue4
        if scratchValue4 < 0 then
            scratchValue = scratchValue + 4294967296.0
        end
        quest:UpdateQuestInfoCounter(scratchValue9, math.modf((quest:ReadGlobalGameDataFloat(3856) - state:GetInt("ScorpionsLeft")) - scratchValue), -1)
        scratchValue6 = scratchValue7
        if ((scratchValue7 - getAllThingsWithScriptName) / 12) < 3 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            scratchValue3 = scratchValue7 - getAllThingsWithScriptName >> 31
            if ((scratchValue7 - getAllThingsWithScriptName) / 12 + scratchValue3 == scratchValue3) and state:GetInt("ScorpionsLeft") == 0 then
                quest:SetStateBool("ScorpionsAlive", false)
                quest:SetMasterGameState("ScorpionsDestroyed", true)
                scratchValue6 = scratchValue7
            else
                scratchValue6 = scratchValue7
                if 0 < state:GetInt("ScorpionsLeft") then
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
                    quest:EntityAttachToScript(r1_1, "Q_GuildTrainingWoodsMelee")
                    state:SetInt("ScorpionsLeft", state:GetInt("ScorpionsLeft") - 1)
                    scratchValue6 = scratchValue7
                end
            end
        end
        scratchValue7 = scratchValue6
        if getAllThingsWithScriptName ~= nil then
            -- TODO(native): free(xStack_24[0 + 1]);
        end
        scorpionsAlive = quest:GetStateBool("ScorpionsAlive")
        timerId = timerId
    until false
end

-- ScorpionHome.Init (retail 0x00d66c60)
function Init(quest, me)
    state:SetInt("ScorpionsLeft", math.modf(quest:ReadGlobalGameDataFloat(3856)))
    state:SetBool("FlourishHint", false)
end

-- ScorpionHome.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- ScorpionHome.OnPredicateFail (retail 0x00d66c50)
function OnPredicateFail(quest, me)
end

