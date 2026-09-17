-- Generated native draft: TC_GuardSpawnPoint. Review coverage report before use.
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
    local bVar2, iVar4, iVar6, iVar7, native_arg_sequence_1, pCVar3, pCVar5, pScriptName, r1
    local alive = true
    local cVar1 = quest:GetStateBool("QuestStartScreened")
    while not cVar1 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        cVar1 = quest:GetStateBool("QuestStartScreened")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    native_arg_sequence_1 = false
    if not bVar2 then
        native_arg_sequence_1 = true
    else
        native_arg_sequence_1 = false
    end
    if native_arg_sequence_1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
    end
    if native_arg_sequence_1 then
        while true do
            if not (0x19 - quest:GetStateInt("InitialNumberInRegion") ~= quest:GetStateInt("NumberSpawned")) then break end
            -- TODO(native): if ((((*(__native_entity_state:GetStateInt("self_0x14") + 0x4c) - *(__native_entity_state:GetStateInt("self_0x14") + 0x48)) / 0xc - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 0x19 < quest:GetStateInt("NextTimeToSpawnGuards")) or (((*(__native_entity_state:GetStateInt("self_0x14") + 0x4c) - *(__native_entity_state:GetStateInt("self_0x14") + 0x48)) / 0xc) < 7) then
            if false then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                pCVar3 = me:GetPos()
                bVar2 = quest:IsCameraPosOnScreen(pCVar3)
                if not bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                    iVar7 = 0
                    repeat
                        iVar4 = quest:GetStateInt("NumberSpawned")
                        iVar6 = 0x19 - quest:GetStateInt("InitialNumberInRegion")
                        if iVar6 == iVar4 or iVar6 - iVar4 < 0 then break end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            return
                        end
                        if quest:ReadGlobalGameData(0xf88) < (0x19 - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                return
                            end
                        elseif quest:ReadGlobalGameData(0xf8c) < (0x19 - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                return
                            end
                        elseif quest:ReadGlobalGameData(0xf90) < (0x19 - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                return
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                return
                            end
                        end
                        if iVar4 % 500 == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                -- LAB_00df7f78: (native jump target)
                                return
                            end
                            -- TODO(native): CCharString::operator=((CCharString *)xStack_1c,"CREATURE_BS_SHERIFF");
                        end
                        bVar2 = false
                        pScriptName = ("CREATURE_BS_GUARD_BLACK" + 4)
                        pCVar3 = me:GetPos()
                        pCVar5 = "CREATURE_BS_GUARD_BLACK"
                        r1 = quest:CreateCreature(pCVar5, pCVar3, pScriptName)
                        quest:SetCombatNearbyBreakOffRange(r1, pThing)
                        quest:EntitySetInFaction(r1, "FACTION_MONSTERS")
                        quest:MiniMapAddMarker(r1, "HUD_ORB_RED_SMALL")
                        pCVar5 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(r1, pCVar5)
                        quest:SetStateInt("NumberSpawned", quest:GetStateInt("NumberSpawned") + 1)
                        iVar7 = iVar7 + 1
                    until not (iVar7 < 2)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                end
                quest:SetStateInt("NextTimeToSpawnGuards", ((((quest:GetStateListCount("AllCreatures") * 0xc)) / 0xc - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 0x18)
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            return
        end
    end
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

