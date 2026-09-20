-- Generated native draft: IsAGuard. Review coverage report before use.
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
    local bVar2, bVar3, bVar4, bVar5, bVar6, bVar7, cVar1, dist, p0, thing2
    local alive = true
    bVar7 = false
    bVar4 = false
    bVar3 = false
    bVar6 = false
    bVar2 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        while not bVar5 do
            cVar1 = quest:GetStateBool("PlayerEngaged")
            while not cVar1 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                bVar5 = me:MsgIsHitBy("")
                if bVar5 then
                    bVar3 = true
                    bVar5 = me:MsgIsHitByHero()
                    if bVar5 then goto LAB_00df983a end
                    bVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar6 then
                        bVar3 = true
                        bVar6 = true
                        bVar2 = true
                        bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar5 then goto LAB_00df983a end
                    end
                    bVar3 = true
                    bVar6 = true
                    bVar5 = true
                else
                    goto LAB_00df983a
                end
                goto FLOW_past_lab_00df983a
                ::LAB_00df983a::
                bVar5 = false
                ::FLOW_past_lab_00df983a::
                if bVar2 then
                    bVar2 = false
                end
                if bVar6 then
                    bVar6 = false
                end
                if bVar3 then
                    bVar3 = false
                end
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        return
                    end
                    quest:ModifyThingHealth(me, 40.0, false)
                else
                    bVar5 = me:MsgIsHitByHero()
                    if bVar5 then
                        goto LAB_00df992a
                    else
                        bVar7 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if bVar7 then
                            bVar7 = true
                            bVar4 = true
                            bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not bVar5 then goto LAB_00df992a end
                        end
                        bVar7 = true
                        bVar5 = false
                    end
                    goto FLOW_past_lab_00df992a
                    ::LAB_00df992a::
                    bVar5 = true
                    ::FLOW_past_lab_00df992a::
                    if bVar4 then
                        bVar4 = false
                    end
                    if bVar7 then
                        bVar7 = false
                    end
                    if bVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            return
                        end
                        quest:SetStateBool("PlayerEngaged", true)
                    end
                end
                dist = 15.0
                thing2 = quest:GetHero()
                bVar5 = quest:IsDistanceBetweenThingsUnder(me, thing2, dist)
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        return
                    end
                    quest:SetStateBool("PlayerEngaged", true)
                end
                cVar1 = quest:GetStateBool("PlayerEngaged")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                return
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
        end
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

