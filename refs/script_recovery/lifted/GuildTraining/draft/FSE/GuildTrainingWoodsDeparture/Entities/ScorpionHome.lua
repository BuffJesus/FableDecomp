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
    local bVar6, iVar7, pCVar8, pPosition, puVar3, puVar4, pu_stk_20, r1, r2, xStack_2c
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
        iVar7 = __ftol2()
        xStack_2c = quest:AddQuestInfoCounter("HUD_BEETLE_ICON", 1.0, nil --[[missing]])
        quest:DisplayQuestInfo(true)
        iVar1 = quest:GetStateInt("DepartureMissionPoint")
        while iVar1 == 1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                return
            end
            pu_stk_20 = 0x0
            r1 = quest:GetAllThingsWithScriptName("GuildScorpions")
            iVar7 = __ftol2()
            quest:UpdateQuestInfoCounter(xStack_2c, -1, pu_stk_20)
            puVar3 = 0x0
            puVar4 = pu_stk_20
            if ((pu_stk_20 - 0x0) / 0xc) < 3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    -- LAB_00d647d1: (native jump target)
                    return
                end
                iVar1 = pu_stk_20 - 0x0 >> 0x1f
                if ((pu_stk_20 - 0x0) / 0xc + iVar1 == iVar1) and (__native_entity_state:GetStateInt("ScorpionsLeft") == 0) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        return
                    end
                    quest:SetStateInt("DepartureMissionPoint", 2)
                    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12", "", "")
                    puVar3 = 0x0
                    puVar4 = pu_stk_20
                else
                    puVar3 = 0x0
                    puVar4 = pu_stk_20
                    if 0 < __native_entity_state:GetStateInt("ScorpionsLeft") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            return
                        end
                        pCVar8 = quest:GetHero()
                        r2 = quest:GetFurthestWithScriptName(pCVar8, "ScorpionSpawn")
                        if r2 == nil then
                        else
                            pPosition = (**(*r2 + 0x18))()
                        end
                        pCVar8 = quest:CreateCreature("CREATURE_GUILD_STAG_BEETLE", nil --[[missing]], "GuildScorpions")
                        -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_18,(int)&*(int *)(pCVar8 + 0x4));
                        pCVar8 = nil
                        if r2 ~= nil then
                            -- TODO(native): (**(code **)(*xStack_18 + 0x118))(0);
                        end
                        quest:EntityAttachToScript(r2, "Q_GuildTrainingWoodsDeparture")
                        __native_entity_state:SetStateInt("ScorpionsLeft", __native_entity_state:GetStateInt("ScorpionsLeft") + -1)
                        r2 = nil
                        puVar3 = 0x0
                        puVar4 = pu_stk_20
                    end
                end
            end
            while puVar5 = pu_stk_20, puVar3 ~= pu_stk_20 do
                pu_stk_20 = puVar4
                -- TODO(native): (**(code **)*puVar3)(0);
                puVar4 = pu_stk_20
                pu_stk_20 = puVar5
                puVar3 = puVar3 + 3
            end
            pu_stk_20 = puVar4
            if nil ~= nil then
                -- TODO(native): free(puStack_24);
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
    local uVar1 = __ftol2()
    __native_entity_state:SetStateInt("ScorpionsLeft", uVar1)
    __native_entity_state:SetStateBool("FlourishHint", false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

