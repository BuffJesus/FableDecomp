-- Generated native draft: CampHostageDoor. Review coverage report before use.
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
    local __native_condition_1, bVar2, bVar3, bVar5, cVar4, dist, p0, pCVar6, r1
    local alive = true
    bVar5 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    r1 = quest:GetThingWithScriptName("CampHostageGuard")
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    repeat
        if bVar2 then
            r1 = nil
            return
        end
        if not quest:GetStateBool("HostagesRescued") then
            bVar5 = true
            bVar2 = me:MsgIsUsedByHero()
            if not bVar2 then goto LAB_00d08c0c end
            bVar2 = true
        else
            goto LAB_00d08c0c
        end
        goto FLOW_past_lab_00d08c0c
        ::LAB_00d08c0c::
        bVar2 = false
        ::FLOW_past_lab_00d08c0c::
        if bVar5 then
            bVar5 = false
        end
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            pCVar6 = quest:GetHero()
            bVar2 = quest:IsObjectInThingsPossession("OBJECT_BANDIT_CAMP_HOSTAGE_KEY", pCVar6)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar2 then
                if bVar3 then goto LAB_00d08e39 end
                __native_condition_1 = (r1 ~= nil and not r1:IsNull())
                if __native_condition_1 then
                    cVar4 = (r1 ~= nil and r1:IsAlive())
                    __native_condition_1 = cVar4
                end
                if __native_condition_1 then
                    dist = 5.0
                    pCVar6 = quest:GetHero()
                    bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar6, r1, dist)
                    if bVar2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00d08e39 end
                        quest:SetStateBool("PlayGuardTooCloseCutscene", true)
                        cVar4 = quest:GetStateBool("PlayGuardTooCloseCutscene")
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00d08e39 end
                            cVar4 = quest:GetStateBool("PlayGuardTooCloseCutscene")
                        end
                        goto LAB_00d08d7c
                    end
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    quest:GiveHeroExperience(quest:ReadGlobalGameData(0x44))
                    quest:SetStateBool("HostagesRescued", true)
                    quest:TakeObjectFromHero("OBJECT_BANDIT_CAMP_HOSTAGE_KEY")
                end
                ::LAB_00d08e39::
                return
            end
            if bVar3 then
                return
            end
            quest:DisplayGameInfo("TEXT_QST_009_NEED_KEY")
            bVar2 = quest:MsgIsGameInfoClickedPast()
            while not bVar2 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:MsgIsGameInfoClickedPast()
            end
            ::LAB_00d08d7c::
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    until false
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

