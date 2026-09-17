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
    local bVar6, iVar8, pCVar9, pPosition, pSpeaker, puVar3, puVar4, pu_stk_20, r1, r2, v_stk_4c
    local alive = true
    v_stk_4c = 0
    local iVar7 = __ftol2()
    local xStack_2c = quest:AddQuestInfoCounter("HUD_BEETLE_ICON", 1.0, v_stk_4c)
    quest:DisplayQuestInfo(true)
    local timerId = quest:RegisterTimer()
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
            quest:SetTimer(timerId, 8)
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
                -- LAB_00d67728: (native jump target)
                timerId = timerId
                quest:DeregisterTimer(timerId)
                return
            end
            iVar8 = pu_stk_20 - 0x0 >> 0x1f
            if ((pu_stk_20 - 0x0) / 0xc + iVar8 == iVar8) and (__native_entity_state:GetStateInt("ScorpionsLeft") == 0) then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    timerId = timerId
                    quest:DeregisterTimer(timerId)
                    return
                end
                quest:SetStateBool("ScorpionsAlive", false)
                quest:SetMasterGameState("ScorpionsDestroyed", true)
                puVar3 = 0x0
                puVar4 = pu_stk_20
            else
                puVar3 = 0x0
                puVar4 = pu_stk_20
                if 0 < __native_entity_state:GetStateInt("ScorpionsLeft") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        timerId = timerId
                        quest:DeregisterTimer(timerId)
                        return
                    end
                    pCVar9 = quest:GetHero()
                    r2 = quest:GetFurthestWithScriptName(pCVar9, "ScorpionSpawn")
                    if r2 == nil then
                    else
                        pPosition = (**(*r2 + 0x18))()
                    end
                    pCVar9 = quest:CreateCreature("CREATURE_GUILD_STAG_BEETLE", nil --[[missing]], "GuildScorpions")
                    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_18,(int)&*(int *)(pCVar9 + 0x4));
                    pCVar9 = nil
                    if r2 ~= nil then
                        -- TODO(native): (**(code **)(*xStack_18 + 0x118))(0);
                    end
                    quest:EntityAttachToScript(r2, "Q_GuildTrainingWoodsMelee")
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
        cVar1 = quest:GetStateBool("ScorpionsAlive")
        timerId = timerId
    until false
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

