-- Generated native draft: BanditExtra. Review coverage report before use.
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
    local CVar4, bVar1, fVar5, iVar3, pCVar2, pPosition, r1, thing2, xStack_8
    local alive = true
    quest:Pause(0.5)
    if 9 < quest:GetStateListCount("AllCreatures") then
        return
    end
    fVar5 = 14.0
    thing2 = quest:GetHero()
    bVar1 = quest:IsDistanceBetweenThingsOver(me, thing2, fVar5)
    if not bVar1 then
        return
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if bVar1 then
        return
    end
    bVar1 = false
    pPosition = me:GetPos()
    r1 = quest:CreateCreature("CREATURE_BANDIT_GRUNT_LEVEL2", pPosition, "")
    pCVar2 = quest:GetHero()
    fVar5 = (quest:GetDistanceBetweenThings(r1, pCVar2) ^ 2)
    -- TODO(native): xStack_8 = (CCharString)(int)ROUND(fVar5 * _DAT_0126b7dc + 0.5);
    if fVar5 * 0.06666667014360428 == xStack_8 - 1.0 then
        -- TODO(native): xStack_8 = (CCharString)((int)xStack_8 + -1);
    end
    if 3 < xStack_8 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then goto LAB_00dfcbf7 end
        CVar4 = 0x3
    end
    iVar3 = math.random(0, 32767)
    if iVar3 % CVar4 == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then goto LAB_00dfcbf7 end
        pCVar2 = quest:GetHero()
        quest:GiveThingBestEnemyTarget(r1, pCVar2)
    end
    require("TraderConflictGood.native_quest_helpers").UpdateLiveEnemies(quest, me)
    ::LAB_00dfcbf7::
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

