-- Generated native draft: ScorpionHome. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local bVar6, count, fVar10, iVar7, iVar8, i_stk_28, pCVar9, pPosition, pSpeaker, r1, v_stk_4c, xStack_18, xStack_24
    local alive = true
    v_stk_4c = 0
    iVar7 = (math.modf(quest:ReadGlobalGameDataFloat(0xf10)))
    local xStack_2c = quest:AddQuestInfoCounter("HUD_BEETLE_ICON", iVar7, 1.0)
    quest:DisplayQuestInfo(true)
    local timerId = quest:RegisterTimer()
    local xStack_50 = timerId
    quest:SetTimer(timerId, 5)
    local cVar1 = quest:GetStateBool("ScorpionsAlive")
    repeat
        if not cVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if not bVar6 then
                quest:RemoveQuestInfoElement(xStack_2c)
                quest:DisplayQuestInfo(false)
            end
            -- LAB_00d676fb: (native jump target)
            quest:DeregisterTimer(timerId)
            return
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            quest:DeregisterTimer(timerId)
            return
        end
        v_stk_4c = v_stk_4c | 1
        bVar6 = quest:IsPlayerCarryingItemOfType("OBJECT_HERO_STICK")
        local __native_condition_1 = bVar6
        if not __native_condition_1 then
            iVar8 = quest:GetTimer(timerId)
            __native_condition_1 = 0 < iVar8
        end
        if __native_condition_1 then
            bVar6 = false
        else
            bVar6 = true
        end
        if (v_stk_4c & 1) ~= 0 then
            v_stk_4c = v_stk_4c & 0xfffffffe
        end
        if bVar6 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                quest:DeregisterTimer(timerId)
                return
            end
            bVar6 = false
            pCVar9 = quest:GetHero()
            iVar7 = quest:AddNewConversation(pCVar9, bVar6, false)
            pCVar9 = quest:GetHero()
            pSpeaker = quest:GetHero()
            quest:AddLineToConversation(iVar7, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_REPEAT", pSpeaker, pCVar9, false)
            quest:SetTimer(xStack_50, 8)
        end
        xStack_24 = quest:GetAllThingsWithScriptName("GuildScorpions")
        i_stk_28 = (pu_stk_20 - xStack_24) / 0xc
        iVar7 = -1
        fVar10 = i_stk_28
        if i_stk_28 < 0 then
            fVar10 = fVar10 + 4294967296.0
        end
        count = (math.modf((quest:ReadGlobalGameDataFloat(0xf10) - __native_entity_state:GetStateInt("ScorpionsLeft")) - fVar10))
        quest:UpdateQuestInfoCounter(xStack_2c, count, iVar7)
        if ((pu_stk_20 - xStack_24) / 0xc) < 3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                -- LAB_00d67728: (native jump target)
                timerId = xStack_50
                quest:DeregisterTimer(timerId)
                return
            end
            iVar8 = pu_stk_20 - xStack_24 >> 0x1f
            if ((pu_stk_20 - xStack_24) / 0xc + iVar8 == iVar8) and (__native_entity_state:GetStateInt("ScorpionsLeft") == 0) then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    timerId = xStack_50
                    quest:DeregisterTimer(timerId)
                    return
                end
                quest:SetStateBool("ScorpionsAlive", false)
                quest:SetMasterGameState("ScorpionsDestroyed", true)
            else
                if 0 < __native_entity_state:GetStateInt("ScorpionsLeft") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        timerId = xStack_50
                        quest:DeregisterTimer(timerId)
                        return
                    end
                    pCVar9 = quest:GetHero()
                    r1 = quest:GetFurthestWithScriptName(pCVar9, "ScorpionSpawn")
                    if not (r1 ~= nil and not r1:IsNull()) then
                        pPosition = {x = 0, y = 0, z = 0}
                    else
                        pPosition = r1:GetPos()
                    end
                    pCVar9 = quest:CreateCreature("CREATURE_GUILD_STAG_BEETLE", pPosition, "GuildScorpions")
                    xStack_18 = pCVar9
                    pCVar9 = nil
                    if (r1 ~= nil and not r1:IsNull()) then
                        r1:SetToKillOnLevelUnload(0)
                    end
                    quest:EntityAttachToScript(r1, "Q_GuildTrainingWoodsMelee")
                    __native_entity_state:SetStateInt("ScorpionsLeft", __native_entity_state:GetStateInt("ScorpionsLeft") + -1)
                    r1 = nil
                end
            end
        end
        cVar1 = quest:GetStateBool("ScorpionsAlive")
        timerId = xStack_50
    until false
end

function Init(quest, me)
    local iVar1 = (math.modf(quest:ReadGlobalGameDataFloat(0xf10)))
    __native_entity_state:SetStateInt("ScorpionsLeft", iVar1)
    __native_entity_state:SetStateBool("FlourishHint", false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

