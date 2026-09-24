-- Generated native draft: BanditCronies. Review coverage report before use.
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
    local fVar6, iVar1, iVar2, pCVar4, r1, uVar5
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    local bVar3 = not alive
    if not bVar3 then
        fVar6 = 13.0
        pCVar4 = quest:GetHero()
        bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar4, fVar6)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            fVar6 = 13.0
            pCVar4 = quest:GetHero()
            bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar4, fVar6)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            iVar1 = quest:ReadGlobalGameData(0x3b0)
            iVar2 = quest:ReadGlobalGameData(0x3ac)
            uVar5 = math.random(0, 32767)
            -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(quest:ReadGlobalGameData(0x3ac) + (uVar5 % (uint)(iVar1 - iVar2 >> 2)) * 4),(int)&xStack_18);
            -- TODO(native): CCharString::CCharString(&xStack_14,&xStack_18);
            r1 = quest:PlaySoundOnThing(me, nil --[[missing]])
            pCVar4 = quest:GetHero()
            quest:GiveThingBestEnemyTarget(me, pCVar4)
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
            until not (not bVar3)
        end
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

