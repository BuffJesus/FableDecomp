-- Generated native draft: HeroBed. Review coverage report before use.
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
    local bVar3, iVar4, iVar5, iVar7, puStack_18, puStack_24, puStack_c, puVar6, pu_stk_14
    local alive = true
    iVar5 = 0
    quest:SetThingAsUsable(me, false)
    quest:SetThingPersistent(me, true)
    puStack_c = quest:GetAllThingsWithDefName("OBJECT_GUILD_BED_FLOOR_PALLET_01")
    iVar4 = #puStack_c
    if 0 < iVar4 then
        iVar7 = 0
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            puVar6 = pu_stk_20
            if bVar3 then goto LAB_00d4482d end
            quest:SetThingAsUsable(puStack_c[(iVar7) / 0xc + 1], false)
            quest:SetThingPersistent(puStack_c[(iVar7) / 0xc + 1], true)
            iVar5 = iVar5 + 1
            iVar7 = iVar7 + 0xc
        until not (iVar5 < iVar4)
    end
    iVar4 = 0
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    puVar6 = pu_stk_20
    if bVar3 then
        puVar6 = pu_stk_14
        pu_stk_14 = puVar6
    else
        puStack_24 = quest:GetAllThingsWithDefName("OBJECT_GUILD_BED_APPRENTICE_01")
        iVar5 = #puStack_24
        if 0 < iVar5 then
            iVar7 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                puVar6 = pu_stk_20
                if bVar3 then goto LAB_00d449bd end
                quest:SetThingAsUsable(puStack_24[(iVar7) / 0xc + 1], false)
                quest:SetThingPersistent(puStack_24[(iVar7) / 0xc + 1], true)
                iVar4 = iVar4 + 1
                iVar7 = iVar7 + 0xc
            until not (iVar4 < iVar5)
        end
        iVar4 = 0
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        puVar6 = pu_stk_20
        if bVar3 then
            puVar6 = pu_stk_14
            pu_stk_14 = puVar6
        else
            puStack_18 = quest:GetAllThingsWithDefName("OBJECT_BS_SLUM_BED_BROWN_01")
            iVar5 = #puStack_18
            if 0 < iVar5 then
                iVar7 = 0
                repeat
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    puVar6 = pu_stk_20
                    if bVar3 then goto LAB_00d44b4d end
                    quest:SetThingAsUsable(puStack_18[(iVar7) / 0xc + 1], false)
                    quest:SetThingPersistent(puStack_18[(iVar7) / 0xc + 1], true)
                    iVar4 = iVar4 + 1
                    iVar7 = iVar7 + 0xc
                until not (iVar4 < iVar5)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            puVar6 = pu_stk_20
            if bVar3 then
                puVar6 = pu_stk_14
                pu_stk_14 = puVar6
            else
                puVar6 = pu_stk_14
                pu_stk_14 = puVar6
            end
        end
    end
    bVar3 = puStack_c == nil
    goto LAB_00d44c44
    ::LAB_00d449bd::
    puVar6 = pu_stk_14
    pu_stk_14 = puVar6
    puVar6 = puStack_c
    if puStack_c ~= pu_stk_8 then
        bVar3 = puStack_c == nil
        goto LAB_00d44c44
    end
    goto LAB_00d44bc3
    ::LAB_00d44b4d::
    puVar6 = pu_stk_14
    pu_stk_14 = puVar6
    goto LAB_00d44bc3
    ::LAB_00d4482d::
    puVar6 = pu_stk_14
    pu_stk_14 = puVar6
    puVar6 = puStack_c
    if puStack_c ~= pu_stk_8 then
        bVar3 = puStack_c == nil
        goto LAB_00d44c44
    end
    ::LAB_00d44bc3::
    bVar3 = puStack_c == nil
    ::LAB_00d44c44::
    if not bVar3 then
    end
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

