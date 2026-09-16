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
    local pCVar10, piVar1, piVar2, puVar5, r1, r2, uVar4, uVar7
    local alive = true
    local iVar6 = quest:GetStateInt("DepartureMissionPoint")
    while iVar6 ~= 1 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        iVar6 = quest:GetStateInt("DepartureMissionPoint")
    end
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        -- TODO(native): __ftol2();
        pCVar10 = "HUD_BEETLE_ICON"
        uVar7 = quest:AddQuestInfoCounter("HUD_BEETLE_ICON", nil --[[missing]], nil --[[missing]])
        quest:DisplayQuestInfo(true)
        iVar6 = quest:GetStateInt("DepartureMissionPoint")
        while iVar6 == 1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                return
            end
            r1 = quest:GetAllThingsWithScriptName("GuildScorpions")
            -- TODO(native): iStack_40 = (iStack_38 - (int)ppuStack_3c) / 0xc;
            uVar4 = __ftol2(0xffffffff)
            quest:UpdateQuestInfoCounter(piStack_44, 0, 0x0)
            piVar1 = 0x0
            if ((piStack_30 - 0x0) / 0xc) < 3 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- LAB_00d647d1: (native jump target)
                    return
                end
                iVar6 = piStack_30 - 0x0 >> 0x1f
                if ((piStack_30 - 0x0) / 0xc + iVar6 == iVar6) and (__native_entity_state:GetStateInt("ScorpionsLeft") == 0) then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d647d1
                    quest:SetStateInt("DepartureMissionPoint", 2)
                    quest:SetQuestCardObjective("", "Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_12", r1)
                    piVar1 = 0x0
                else
                    piVar1 = 0x0
                    if 0 < __native_entity_state:GetStateInt("ScorpionsLeft") then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d647d1
                        uVar4 = quest:GetHero()
                        r2 = quest:GetFurthestWithScriptName(uVar4, "ScorpionSpawn")
                        if piStack_30 == nil then
                        else
                            puVar5 = (**(*piStack_30 + 0x18))()
                        end
                        iVar6 = quest:CreateCreature("CREATURE_GUILD_STAG_BEETLE", nil --[[missing]], "GuildScorpions")
                        -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)&piStack_44, (CCountedPointer<class_CDiskFileWin32> *)(iVar6 + 4));
                        if piStack_44 ~= nil then
                            -- TODO(native): (**(code **)(*piStack_44 + 0x118))(0);
                        end
                        quest:EntityAttachToScript(iVar6, "Q_GuildTrainingWoodsDeparture")
                        __native_entity_state:SetStateInt("ScorpionsLeft", __native_entity_state:GetStateInt("ScorpionsLeft") + -1)
                        piVar1 = 0x0
                    end
                end
            end
            while piVar3 = piStack_30, piVar1 ~= piStack_30 do
                -- TODO(native): (**(code **)*piVar1)(0);
                piVar2 = piVar2
                -- TODO(native): piStack_30 = piVar3;
                piVar1 = piVar1 + 3
            end
            if nil ~= nil then
                -- TODO(native): free(piStack_34);
            end
            iVar6 = quest:GetStateInt("DepartureMissionPoint")
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            quest:RemoveQuestInfoElement(ppuStack_3c)
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

