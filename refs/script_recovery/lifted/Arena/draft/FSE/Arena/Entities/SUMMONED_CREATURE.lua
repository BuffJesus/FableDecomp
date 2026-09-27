-- Generated native draft: SUMMONED_CREATURE. Review coverage report before use.
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
    local bVar2, bVar3, bVar4, iVar5, iVar7, p0, pCVar6, pCVar9, r1, r2
    local alive = true
    bVar4 = false
    bVar2 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:SetStateInt("TotalCreatures_0", quest:GetStateInt("TotalCreatures_0") + 1)
        quest:SetStateInt("ExtraCreatures", quest:GetStateInt("ExtraCreatures") + 1)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        while not bVar3 do
            bVar3 = me:MsgIsHitByHeroWithFlourish()
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                iVar5 = quest:GetTimer(quest:GetStateInt("GlobalCrowdTimer"))
                if iVar5 < 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    pCVar9 = quest:GetStateString(("CrowdLoopTags_" .. quest:GetStateInt("NewCrowdBaseLevel") .. "_" .. 3))
                    pCVar6 = quest:GetHero()
                    pCVar6 = quest:GetNearestWithScriptName(pCVar6, "ArenaSpawn")
                    r1 = quest:PlayCriteriaSoundOnThing(pCVar6, pCVar9)
                    quest:SetTimer(quest:GetStateInt("GlobalCrowdTimer"), 10)
                    iVar7 = quest:GetStateInt("NewCrowdPoints") + 5
                    goto LAB_00f1b586
                end
            else
                bVar3 = me:MsgIsHitByHeroWithDecapitate()
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    iVar5 = quest:GetTimer(quest:GetStateInt("GlobalCrowdTimer"))
                    if iVar5 < 9 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        pCVar9 = quest:GetStateString(("CrowdLoopTags_" .. quest:GetStateInt("NewCrowdBaseLevel") .. "_" .. 3))
                        pCVar6 = quest:GetHero()
                        pCVar6 = quest:GetNearestWithScriptName(pCVar6, "ArenaSpawn")
                        r2 = quest:PlayCriteriaSoundOnThing(pCVar6, pCVar9)
                        quest:SetTimer(quest:GetStateInt("GlobalCrowdTimer"), 10)
                        iVar7 = quest:GetStateInt("NewCrowdPoints") + 7
                        goto LAB_00f1b586
                    end
                else
                    bVar3 = me:MsgIsHitByHero()
                    if bVar3 then
                        goto LAB_00f1b523
                    else
                        bVar4 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if bVar4 then
                            bVar4 = true
                            bVar2 = true
                            bVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not bVar3 then goto LAB_00f1b523 end
                        end
                        bVar4 = true
                        bVar3 = false
                    end
                    goto FLOW_past_lab_00f1b523
                    ::LAB_00f1b523::
                    bVar3 = true
                    ::FLOW_past_lab_00f1b523::
                    if bVar2 then
                        bVar2 = false
                    end
                    if bVar4 then
                        bVar4 = false
                    end
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        iVar7 = quest:GetStateInt("NewCrowdPoints") + 2
                        goto LAB_00f1b586
                    end
                end
            end
            goto FLOW_past_lab_00f1b586
            ::LAB_00f1b586::
            quest:SetStateInt("NewCrowdPoints", iVar7)
            ::FLOW_past_lab_00f1b586::
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
        end
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
    quest:SetStateInt("TotalCreatures_0", quest:GetStateInt("TotalCreatures_0") + -1)
    local cVar2 = me:MsgIsKilledBy("SCRIPT_NAME_HERO")
    if cVar2 then
        quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + 8)
    end
end

