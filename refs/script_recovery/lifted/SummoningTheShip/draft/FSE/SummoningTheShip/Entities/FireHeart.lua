-- Generated native draft: FireHeart. Review coverage report before use.
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
    local bVar2, bVar3, bVar4, bVar5, cVar1, fret_0, fret_00, fret_01, fret_02, iVar6, i_stk_30, pThing, r1, r2, r3, r4, r5, timerId, xStack_2c
    local alive = true
    bVar2 = false
    bVar4 = false
    bVar5 = false
    quest:EntitySetAlpha(me, 0.0, true)
    cVar1 = quest:GetStateBool("LighthouseStarted")
    while not cVar1 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        cVar1 = quest:GetStateBool("LighthouseStarted")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:MiniMapRemoveMarker(me)
        cVar1 = quest:GetStateBool("SummonerAttacksStarted")
        while not cVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            cVar1 = quest:GetStateBool("SummonerAttacksStarted")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            quest:EntitySetAsDamageable(me, true)
            quest:DisplayQuestInfo(true)
            quest:EntitySetAlpha(me, 1.0, true)
            i_stk_30 = quest:RegisterTimer()
            r1 = quest:GetThingWithScriptName("FireHeart")
            -- TODO(native): r2 = quest:AddQuestInfoBarHealth(r1, &0xff00ff00, "HUD_ICON_FIRE_HEART", 1.0)
            r2 = nil --[[unresolved native value]]
            timerId = quest:RegisterTimer()
            xStack_2c = timerId
            quest:SetTimer(timerId, 3)
            fret_0 = quest:GetHealth(me)
            if 1.0 < fret_0 then
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00df3431 end
                    iVar6 = quest:GetTimer(timerId)
                    if iVar6 < 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00df3431 end
                        fret_00 = quest:GetHealth(me)
                        if 750.0 < fret_00 then
                            fret_01 = quest:GetHealth(me)
                            if 1500.0 < fret_01 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00df3431 end
                                r3 = quest:Play2DSound("SND_MM_FIREHEART_BEAT_SLOW")
                                quest:SetTimer(timerId, 3)
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00df3431 end
                                r4 = quest:Play2DSound("SND_MM_FIREHEART_BEAT_MEDIUM")
                                quest:SetTimer(timerId, 2)
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00df3431 end
                            r5 = quest:Play2DSound("SND_MM_FIREHEART_BEAT_FAST")
                            quest:SetTimer(timerId, 1)
                        end
                    end
                    bVar3 = me:MsgIsHitBy("SummonerAttacker")
                    if bVar3 then
                        goto LAB_00df3310
                    else
                        bVar4 = me:MsgIsHitByAnySpecialAbilityFrom("SummonerAttacker")
                        if bVar4 then
                            bVar4 = true
                            bVar5 = true
                            bVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not bVar3 then goto LAB_00df3310 end
                        end
                        bVar4 = true
                        bVar3 = false
                    end
                    goto FLOW_past_lab_00df3310
                    ::LAB_00df3310::
                    bVar3 = true
                    ::FLOW_past_lab_00df3310::
                    if bVar5 then
                        bVar5 = false
                    end
                    if bVar4 then
                        bVar4 = false
                    end
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00df3431 end
                        iVar6 = quest:GetTimer(i_stk_30)
                        if iVar6 == 0 then
                            bVar2 = true
                            bVar3 = require("SummoningTheShip.native_quest_helpers").MakeBriarRoseComment(quest, me, "FIREHEART")
                            if not bVar3 then goto LAB_00df33c1 end
                            bVar3 = true
                        else
                            goto LAB_00df33c1
                        end
                        goto FLOW_past_lab_00df33c1
                        ::LAB_00df33c1::
                        bVar3 = false
                        ::FLOW_past_lab_00df33c1::
                        if bVar2 then
                            bVar2 = false
                        end
                        if bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00df3431 end
                            quest:SetTimer(i_stk_30, 0x14)
                        end
                    end
                    fret_02 = quest:GetHealth(me)
                until not (1.0 < fret_02)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if not bVar5 then
                quest:SetStateBool("MissionFailed", true)
            end
            ::LAB_00df3431::
            quest:DeregisterTimer(xStack_2c)
            quest:DeregisterTimer(i_stk_30)
        end
    end
end

function Init(quest, me)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAsDamageable(me, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

