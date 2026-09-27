-- Generated native draft: SummonerAttacker. Review coverage report before use.
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
    local resources = quest:RetailResources()
    local bVar1, bVar2, bVar3, cVar4, fVar9, iVar6, p0, pCVar5, pCVar7, r1, r2, xStack_10, xStack_44, xStack_50
    local alive = true
    bVar3 = false
    bVar1 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:EntitySetAlpha(me, 0.0, true)
        quest:EntitySetInLimbo(me, true, true)
        while (not quest:GetStateBool("SummonerAttacksStarted") or (quest:GetStateInt("CurrentAttackWave") ~= __native_entity_state:GetStateInt("WaveID"))) do
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
            quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
            quest:EntitySetInLimbo(me, false, true)
            -- TODO(native): FadeInThing(p0,0x40000000);
            if __native_entity_state:GetStateInt("WaveID") == 1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                r1 = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_SUMMONER_BLUE", 1.0)
            end
            xStack_10 = resources:NewResource()
            r2 = quest:GetThingWithScriptName("FireHeart")
            xStack_44 = nil
            xStack_50 = nil
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                while true do
                    resources:PrepareResource(xStack_10)
                    bVar2 = resources:TryAcquire(xStack_10, me, 4)
                    while not bVar2 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00df292a end
                        bVar2 = resources:TryAcquire(xStack_10, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                    fVar9 = 15.0
                    pCVar5 = quest:GetHero()
                    bVar2 = quest:IsDistanceBetweenThingsOver(me, pCVar5, fVar9)
                    if bVar2 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00df292a end
                            iVar6 = me:IsPerformingScriptTask()
                            if not iVar6 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00df292a end
                                me:SummonerLightningOrbAttackTarget(pCVar5)
                            end
                            bVar2 = me:MsgIsHitByHero()
                            if bVar2 then
                                goto LAB_00df26bd
                            else
                                bVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                                if bVar3 then
                                    bVar3 = true
                                    bVar1 = true
                                    bVar2 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                    if not bVar2 then goto LAB_00df26bd end
                                end
                                bVar3 = true
                                bVar2 = false
                            end
                            goto FLOW_past_lab_00df26bd
                            ::LAB_00df26bd::
                            bVar2 = true
                            ::FLOW_past_lab_00df26bd::
                            if bVar1 then
                                bVar1 = false
                            end
                            if bVar3 then
                                bVar3 = false
                            end
                            if bVar2 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00df292a end
                                cVar4 = (xStack_44 ~= nil and xStack_44:IsAlive())
                                if not cVar4 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00df292a end
                                    pCVar5 = quest:GetHero()
                                    bVar2 = true
                                    fVar9 = 15.0
                                    pCVar7 = pCVar5:GetPos()
                                    pCVar5 = quest:CreateCreatureNearby("CREATURE_MINION_WARDOG", pCVar7, fVar9, "SummonerMinion")
                                    xStack_44 = pCVar5
                                end
                                cVar4 = (xStack_50 ~= nil and xStack_50:IsAlive())
                                if not cVar4 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00df292a end
                                    pCVar5 = quest:GetHero()
                                    bVar2 = true
                                    fVar9 = 15.0
                                    pCVar7 = pCVar5:GetPos()
                                    pCVar5 = quest:CreateCreatureNearby("CREATURE_MINION_WARDOG", pCVar7, fVar9, "SummonerMinion")
                                    xStack_50 = pCVar5
                                end
                            end
                            fVar9 = 15.0
                            pCVar5 = quest:GetHero()
                            bVar2 = quest:IsDistanceBetweenThingsOver(me, pCVar5, fVar9)
                        until not (bVar2)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                    resources:PrepareResource(xStack_10)
                    quest:MiniMapRemoveMarker(me)
                    fVar9 = 15.0
                    pCVar5 = quest:GetHero()
                    bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar5, fVar9)
                    if bVar2 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00df292a end
                            fVar9 = 15.0
                            pCVar5 = quest:GetHero()
                            bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar5, fVar9)
                        until not (bVar2)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                end
            end
            ::LAB_00df292a::
            resources:ReleaseResource(xStack_10)
        end
    end
end

function Init(quest, me)
    quest:SetStateInt("SummonersAlive", quest:GetStateInt("SummonersAlive") + 1)
    local pCVar1 = me:GetDataString()
    local iVar2 = ((pCVar1 == "WAVE2") and 0 or 1)
    local cVar5 = not (iVar2 ~= 0)
    __native_entity_state:SetStateInt("WaveID", ((cVar5 ~= false and cVar5 ~= nil and cVar5 ~= 0) and 1 or 0) + 1)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
    quest:SetStateInt("SummonersAlive", quest:GetStateInt("SummonersAlive") + -1)
    local cVar2 = me:MsgIsKilledBy("")
    if cVar2 then
        quest:SetStateInt("CurrentAttackWave", quest:GetStateInt("CurrentAttackWave") + 1)
    end
end

