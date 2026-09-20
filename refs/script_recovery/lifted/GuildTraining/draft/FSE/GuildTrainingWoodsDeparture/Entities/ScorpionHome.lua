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
    local bVar6, count, fVar9, iVar7, i_stk_28, pCVar8, pPosition, r1, xStack_24, xStack_2c
    local alive = true
    local iVar1 = quest:GetStateInt("DepartureMissionPoint")
    while iVar1 ~= 1 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            return
        end
        iVar1 = quest:GetStateInt("DepartureMissionPoint")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if not bVar6 then
        iVar7 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xf0c)))
        xStack_2c = quest:AddQuestInfoCounter("HUD_BEETLE_ICON", iVar7, 1.0)
        quest:DisplayQuestInfo(true)
        iVar1 = quest:GetStateInt("DepartureMissionPoint")
        while iVar1 == 1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                return
            end
            xStack_24 = quest:GetAllThingsWithScriptName("GuildScorpions")
            i_stk_28 = #xStack_24
            iVar7 = -1
            fVar9 = i_stk_28
            if i_stk_28 < 0 then
                fVar9 = fVar9 + 4294967296.0
            end
            count = math.tointeger(math.modf((quest:ReadGlobalGameDataFloat(0xf0c) - __native_entity_state:GetStateInt("ScorpionsLeft")) - fVar9))
            quest:UpdateQuestInfoCounter(xStack_2c, count, iVar7)
            if (#xStack_24) < 3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    -- LAB_00d647d1: (native jump target)
                    return
                end
                if (#xStack_24 == 0) and (__native_entity_state:GetStateInt("ScorpionsLeft") == 0) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        return
                    end
                    quest:SetStateInt("DepartureMissionPoint", 2)
                    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12", "", "")
                else
                    if 0 < __native_entity_state:GetStateInt("ScorpionsLeft") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            return
                        end
                        pCVar8 = quest:GetHero()
                        r1 = quest:GetFurthestWithScriptName(pCVar8, "ScorpionSpawn")
                        if not (r1 ~= nil and not r1:IsNull()) then
                            pPosition = {x = 0, y = 0, z = 0}
                        else
                            pPosition = r1:GetPos()
                        end
                        pCVar8 = quest:CreateCreature("CREATURE_GUILD_STAG_BEETLE", pPosition, "GuildScorpions")
                        r1 = pCVar8
                        pCVar8 = nil
                        if (r1 ~= nil and not r1:IsNull()) then
                            r1:SetToKillOnLevelUnload(0)
                        end
                        quest:EntityAttachToScript(r1, "Q_GuildTrainingWoodsDeparture")
                        __native_entity_state:SetStateInt("ScorpionsLeft", __native_entity_state:GetStateInt("ScorpionsLeft") + -1)
                        r1 = nil
                        -- TODO(native): xStack_18._4_4_ = (int *)0x0;
                    end
                end
            end
            iVar1 = quest:GetStateInt("DepartureMissionPoint")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if not bVar6 then
            quest:RemoveQuestInfoElement(xStack_2c)
            quest:DisplayQuestInfo(false)
        end
    end
end

function Init(quest, me)
    local iVar1 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xf0c)))
    __native_entity_state:SetStateInt("ScorpionsLeft", iVar1)
    __native_entity_state:SetStateBool("FlourishHint", false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

