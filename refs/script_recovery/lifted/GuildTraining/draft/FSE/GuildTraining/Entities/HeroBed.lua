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
    local bVar3, iVar4, iVar5, iVar7, puVar1, puVar2, puVar6, pu_stk_14, pu_stk_20, pu_stk_8
    local alive = true
    iVar5 = 0
    quest:SetThingAsUsable(me, false)
    quest:SetThingPersistent(me, true)
    pu_stk_8 = 0x0
    pu_stk_14 = 0x0
    pu_stk_20 = 0x0
    iVar4 = quest:GetAllThingsWithDefName("OBJECT_GUILD_BED_FLOOR_PALLET_01")
    if 0 < iVar4 then
        iVar7 = 0
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            puVar6 = pu_stk_20
            puVar1 = 0x0
            -- TODO(native): if (bVar3) goto joined_r0x00d4482d;
            quest:SetThingAsUsable(nil --[[missing]], (0x0 + iVar7))
            quest:SetThingPersistent(nil --[[missing]], (0x0 + iVar7))
            iVar5 = iVar5 + 1
            iVar7 = iVar7 + 0xc
        until not (iVar5 < iVar4)
    end
    iVar4 = 0
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    puVar6 = pu_stk_20
    puVar1 = 0x0
    if bVar3 then
        while puVar1 ~= puVar6 do
            -- TODO(native): (**(code **)*puVar1)(0);
            puVar1 = puVar1 + 3
        end
        puVar1 = 0x0
        puVar6 = pu_stk_14
        if nil ~= nil then
            -- TODO(native): free(puStack_24);
            puVar1 = 0x0
            puVar6 = pu_stk_14
        end
        while puVar2 = pu_stk_14, puVar1 ~= pu_stk_14 do
            pu_stk_14 = puVar6
            -- TODO(native): (**(code **)*puVar1)(0);
            puVar6 = pu_stk_14
            pu_stk_14 = puVar2
            puVar1 = puVar1 + 3
        end
        pu_stk_14 = puVar6
        puVar1 = 0x0
        puVar2 = pu_stk_8
        if nil ~= nil then
            -- TODO(native): free(puStack_18);
            puVar1 = 0x0
            puVar2 = pu_stk_8
        end
        while puVar6 = pu_stk_8, bVar3 = puVar1 ~= pu_stk_8, pu_stk_8 = puVar2, bVar3 do
            -- TODO(native): (**(code **)*puVar1)(0);
            puVar2 = pu_stk_8
            pu_stk_8 = puVar6
            puVar1 = puVar1 + 3
        end
    else
        iVar5 = quest:GetAllThingsWithDefName("OBJECT_GUILD_BED_APPRENTICE_01")
        if 0 < iVar5 then
            iVar7 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                puVar6 = pu_stk_20
                puVar1 = 0x0
                -- TODO(native): if (bVar3) goto joined_r0x00d449bd;
                quest:SetThingAsUsable(nil --[[missing]], (0x0 + iVar7))
                quest:SetThingPersistent(nil --[[missing]], (0x0 + iVar7))
                iVar4 = iVar4 + 1
                iVar7 = iVar7 + 0xc
            until not (iVar4 < iVar5)
        end
        iVar4 = 0
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        puVar6 = pu_stk_20
        puVar1 = 0x0
        if bVar3 then
            while puVar1 ~= puVar6 do
                -- TODO(native): (**(code **)*puVar1)(0);
                puVar1 = puVar1 + 3
            end
            puVar1 = 0x0
            puVar6 = pu_stk_14
            if nil ~= nil then
                -- TODO(native): free(puStack_24);
                puVar1 = 0x0
                puVar6 = pu_stk_14
            end
            while puVar2 = pu_stk_14, puVar1 ~= pu_stk_14 do
                pu_stk_14 = puVar6
                -- TODO(native): (**(code **)*puVar1)(0);
                puVar6 = pu_stk_14
                pu_stk_14 = puVar2
                puVar1 = puVar1 + 3
            end
            pu_stk_14 = puVar6
            puVar1 = 0x0
            puVar2 = pu_stk_8
            if nil ~= nil then
                -- TODO(native): free(puStack_18);
                puVar1 = 0x0
                puVar2 = pu_stk_8
            end
            while puVar6 = pu_stk_8, bVar3 = puVar1 ~= pu_stk_8, pu_stk_8 = puVar2, bVar3 do
                -- TODO(native): (**(code **)*puVar1)(0);
                puVar2 = pu_stk_8
                pu_stk_8 = puVar6
                puVar1 = puVar1 + 3
            end
        else
            iVar5 = quest:GetAllThingsWithDefName("OBJECT_BS_SLUM_BED_BROWN_01")
            if 0 < iVar5 then
                iVar7 = 0
                repeat
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    puVar6 = pu_stk_20
                    puVar1 = 0x0
                    -- TODO(native): if (bVar3) goto joined_r0x00d44b4d;
                    quest:SetThingAsUsable(nil --[[missing]], (0x0 + iVar7))
                    quest:SetThingPersistent(nil --[[missing]], (0x0 + iVar7))
                    iVar4 = iVar4 + 1
                    iVar7 = iVar7 + 0xc
                until not (iVar4 < iVar5)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            puVar6 = pu_stk_20
            puVar1 = 0x0
            if bVar3 then
                while puVar1 ~= puVar6 do
                    -- TODO(native): (**(code **)*puVar1)(0);
                    puVar1 = puVar1 + 3
                end
                puVar1 = 0x0
                puVar6 = pu_stk_14
                if nil ~= nil then
                    -- TODO(native): free(puStack_24);
                    puVar1 = 0x0
                    puVar6 = pu_stk_14
                end
                while puVar2 = pu_stk_14, puVar1 ~= pu_stk_14 do
                    pu_stk_14 = puVar6
                    -- TODO(native): (**(code **)*puVar1)(0);
                    puVar6 = pu_stk_14
                    pu_stk_14 = puVar2
                    puVar1 = puVar1 + 3
                end
                pu_stk_14 = puVar6
                puVar1 = 0x0
                puVar2 = pu_stk_8
                if nil ~= nil then
                    -- TODO(native): free(puStack_18);
                    puVar1 = 0x0
                    puVar2 = pu_stk_8
                end
                while puVar6 = pu_stk_8, bVar3 = puVar1 ~= pu_stk_8, pu_stk_8 = puVar2, bVar3 do
                    -- TODO(native): (**(code **)*puVar1)(0);
                    puVar2 = pu_stk_8
                    pu_stk_8 = puVar6
                    puVar1 = puVar1 + 3
                end
            else
                while puVar1 ~= puVar6 do
                    -- TODO(native): (**(code **)*puVar1)(0);
                    puVar1 = puVar1 + 3
                end
                puVar1 = 0x0
                puVar6 = pu_stk_14
                if nil ~= nil then
                    -- TODO(native): free(puStack_24);
                    puVar1 = 0x0
                    puVar6 = pu_stk_14
                end
                while puVar2 = pu_stk_14, puVar1 ~= pu_stk_14 do
                    pu_stk_14 = puVar6
                    -- TODO(native): (**(code **)*puVar1)(0);
                    puVar6 = pu_stk_14
                    pu_stk_14 = puVar2
                    puVar1 = puVar1 + 3
                end
                pu_stk_14 = puVar6
                puVar1 = 0x0
                puVar2 = pu_stk_8
                if nil ~= nil then
                    -- TODO(native): free(puStack_18);
                    puVar1 = 0x0
                    puVar2 = pu_stk_8
                end
                while puVar6 = pu_stk_8, bVar3 = puVar1 ~= pu_stk_8, pu_stk_8 = puVar2, bVar3 do
                    -- TODO(native): (**(code **)*puVar1)(0);
                    puVar2 = pu_stk_8
                    pu_stk_8 = puVar6
                    puVar1 = puVar1 + 3
                end
            end
        end
    end
    bVar3 = nil == nil
    goto LAB_00d44c44
    -- TODO(native): joined_r0x00d449bd:
    while puVar1 ~= puVar6 do
        -- TODO(native): (**(code **)*puVar1)(0);
        puVar1 = puVar1 + 3
    end
    puVar1 = 0x0
    puVar6 = pu_stk_14
    if nil ~= nil then
        -- TODO(native): free(puStack_24);
        puVar1 = 0x0
        puVar6 = pu_stk_14
    end
    while puVar2 = pu_stk_14, puVar1 ~= pu_stk_14 do
        pu_stk_14 = puVar6
        -- TODO(native): (**(code **)*puVar1)(0);
        puVar6 = pu_stk_14
        pu_stk_14 = puVar2
        puVar1 = puVar1 + 3
    end
    pu_stk_14 = puVar6
    if nil ~= nil then
        -- TODO(native): free(puStack_18);
    end
    puVar1 = pu_stk_8
    puVar6 = 0x0
    if 0x0 ~= pu_stk_8 then
        repeat
            -- TODO(native): (**(code **)*puVar6)(0);
            puVar6 = puVar6 + 3
        until not (puVar6 ~= puVar1)
        bVar3 = nil == nil
        goto LAB_00d44c44
    end
    goto LAB_00d44bc3
    -- TODO(native): joined_r0x00d44b4d:
    while puVar1 ~= puVar6 do
        -- TODO(native): (**(code **)*puVar1)(0);
        puVar1 = puVar1 + 3
    end
    puVar1 = 0x0
    puVar6 = pu_stk_14
    if nil ~= nil then
        -- TODO(native): free(puStack_24);
        puVar1 = 0x0
        puVar6 = pu_stk_14
    end
    while puVar2 = pu_stk_14, puVar1 ~= pu_stk_14 do
        pu_stk_14 = puVar6
        -- TODO(native): (**(code **)*puVar1)(0);
        puVar6 = pu_stk_14
        pu_stk_14 = puVar2
        puVar1 = puVar1 + 3
    end
    pu_stk_14 = puVar6
    puVar1 = 0x0
    puVar2 = pu_stk_8
    if nil ~= nil then
        -- TODO(native): free(puStack_18);
        puVar1 = 0x0
        puVar2 = pu_stk_8
    end
    while puVar6 = pu_stk_8, bVar3 = puVar1 ~= pu_stk_8, pu_stk_8 = puVar2, bVar3 do
        -- TODO(native): (**(code **)*puVar1)(0);
        puVar2 = pu_stk_8
        pu_stk_8 = puVar6
        puVar1 = puVar1 + 3
    end
    goto LAB_00d44bc3
    -- TODO(native): joined_r0x00d4482d:
    while puVar1 ~= puVar6 do
        -- TODO(native): (**(code **)*puVar1)(0);
        puVar1 = puVar1 + 3
    end
    puVar1 = 0x0
    puVar6 = pu_stk_14
    if nil ~= nil then
        -- TODO(native): free(puStack_24);
        puVar1 = 0x0
        puVar6 = pu_stk_14
    end
    while puVar2 = pu_stk_14, puVar1 ~= pu_stk_14 do
        pu_stk_14 = puVar6
        -- TODO(native): (**(code **)*puVar1)(0);
        puVar6 = pu_stk_14
        pu_stk_14 = puVar2
        puVar1 = puVar1 + 3
    end
    pu_stk_14 = puVar6
    if nil ~= nil then
        -- TODO(native): free(puStack_18);
    end
    puVar1 = pu_stk_8
    puVar6 = 0x0
    if 0x0 ~= pu_stk_8 then
        repeat
            -- TODO(native): (**(code **)*puVar6)(0);
            puVar6 = puVar6 + 3
        until not (puVar6 ~= puVar1)
        bVar3 = nil == nil
        goto LAB_00d44c44
    end
    ::LAB_00d44bc3::
    bVar3 = nil == nil
    ::LAB_00d44c44::
    if not bVar3 then
        -- TODO(native): free(puStack_c);
    end
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

