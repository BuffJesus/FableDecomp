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
    local bVar2, iVar12, piVar1, piVar3, ppVar9, puVar11, r1, r2, r3, uVar10, uVar6, uVar8
    local alive = true
    uVar6 = __ftol2()
    local uVar14 = quest:AddQuestInfoCounter("HUD_BEETLE_ICON", 0x0, nil --[[missing]])
    quest:DisplayQuestInfo(true)
    local ppVar7 = quest:RegisterTimer()
    quest:SetTimer(ppVar7, nil --[[missing]])
    local cVar5 = quest:GetStateBool("ScorpionsAlive")
    repeat
        if not cVar5 then
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                quest:RemoveQuestInfoElement(nil --[[missing]])
                quest:DisplayQuestInfo(nil --[[missing]])
            end
            -- LAB_00d676fb: (native jump target)
            quest:DeregisterTimer(ppVar7)
            return
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end  -- TODO(native): goto LAB_00d676fb
        cVar5 = quest:IsPlayerCarryingItemOfType("OBJECT_HERO_STICK")
        if not cVar5 then
            uVar14 = quest:GetTimer(ppVar7)
            -- TODO(native): ppVar13 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar14 >> 0x20);
            if 0 < uVar14 then return end  -- TODO(native): goto LAB_00d67361
            bVar2 = true
        else
            -- LAB_00d67361: (native jump target)
            bVar2 = false
        end
        if (uVar6 & 1) ~= 0 then
            uVar6 = 0
        end
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                quest:DeregisterTimer(ppVar7)
                return
            end
            uVar8 = quest:GetHero()
            ppVar9 = quest:AddNewConversation(uVar8, (uVar6 ~= 0), bVar2)
            iVar12 = *piVar1
            uVar8 = quest:GetHero()
            uVar10 = quest:GetHero()
            quest:AddLineToConversation(ppVar9, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_REPEAT", uVar10, uVar8, false)
            quest:SetTimer(ppVar7, 8)
        end
        r1 = quest:GetAllThingsWithScriptName("GuildScorpions")
        -- TODO(native): local_4c = (int *)(((int)ppuStack_44 - iStack_48) / 0xc);
        uVar8 = __ftol2(0xffffffff)
        quest:UpdateQuestInfoCounter(0x0, 0x0, 0)
        if ((piStack_38 - piStack_3c) / 0xc) < 3 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d67728: (native jump target)
                -- TODO(native): goto LAB_00d676fb
            end
            iVar12 = piStack_38 - piStack_3c >> 0x1f
            if ((piStack_38 - piStack_3c) / 0xc + iVar12 == iVar12) and (__native_entity_state:GetStateInt("ScorpionsLeft") == 0) then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d67728
                quest:SetStateBool("ScorpionsAlive", false)
                quest:SetMasterGameState("ScorpionsDestroyed", true)
            else
                if 0 < __native_entity_state:GetStateInt("ScorpionsLeft") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d67728
                    r2 = quest:GetHero()
                    r3 = quest:GetFurthestWithScriptName(r2, "ScorpionSpawn")
                    if piStack_38 == nil then
                    else
                        puVar11 = (**(*piStack_38 + 0x18))()
                    end
                    iVar12 = quest:CreateCreature(ppVar9, nil --[[missing]], "CREATURE_GUILD_STAG_BEETLE")
                    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)&local_4c, (CCountedPointer<class_CDiskFileWin32> *)(iVar12 + 4));
                    if local_4c ~= nil then
                        -- TODO(native): (**(code **)(*local_4c + 0x118))(0);
                    end
                    quest:EntityAttachToScript(iVar12, "Q_GuildTrainingWoodsMelee")
                    __native_entity_state:SetStateInt("ScorpionsLeft", __native_entity_state:GetStateInt("ScorpionsLeft") + -1)
                    piVar1 = 0x0
                end
            end
        end
        while piVar4 = piStack_38, piVar1 ~= piStack_38 do
            -- TODO(native): (**(code **)*piVar1)();
            piVar3 = piVar3
            -- TODO(native): piStack_38 = piVar4;
            piVar1 = piVar1 + 3
        end
        if nil ~= nil then
            -- TODO(native): free(piStack_3c);
        end
        cVar5 = quest:GetStateBool("ScorpionsAlive")
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

