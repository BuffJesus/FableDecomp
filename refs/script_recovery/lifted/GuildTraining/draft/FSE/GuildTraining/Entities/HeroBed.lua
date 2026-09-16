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
    local iVar2, iVar3, iVar6, puVar1
    local alive = true
    iVar2 = 0
    -- TODO(native): pCStack_40 = this + 8;
    quest:SetThingAsUsable(nil --[[missing]], false)
    iVar6 = 1
    quest:SetThingPersistent(me, true)
    local uVar4 = quest:GetAllThingsWithDefName("OBJECT_GUILD_BED_FLOOR_PALLET_01")
    if 0 < uVar4 then
        iVar3 = 0
        repeat
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                while puVar1 ~= unaff_EDI do
                    -- TODO(native): (**(code **)*puVar1)(0);
                    puVar1 = puVar1 + 3
                end
                if puStack_3c == nil then
                    return
                end
                -- TODO(native): free(puStack_3c);
                return
            end
            quest:SetThingAsUsable(nil --[[missing]], (iVar3 ~= 0))
            quest:SetThingPersistent(nil --[[missing]], (iVar3 ~= 0))
            iVar2 = iVar2 + 1
            iVar3 = iVar3 + 0xc
        until not (iVar2 < uVar4)
    end
    iVar2 = 0
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        uVar4 = quest:GetAllThingsWithDefName("OBJECT_GUILD_BED_APPRENTICE_01")
        if 0 < uVar4 then
            iVar3 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    while puVar1 ~= unaff_EDI do
                        -- TODO(native): (**(code **)*puVar1)(0);
                        puVar1 = puVar1 + 3
                    end
                    if puStack_3c == nil then
                        return
                    end
                    -- TODO(native): free(puStack_3c);
                    return
                end
                quest:SetThingAsUsable(nil --[[missing]], puStack_3c + iVar3)
                quest:SetThingPersistent(nil --[[missing]], iVar6 + iVar3)
                iVar2 = iVar2 + 1
                iVar3 = iVar3 + 0xc
            until not (iVar2 < uVar4)
        end
        iVar2 = 0
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            uVar4 = quest:GetAllThingsWithDefName("OBJECT_BS_SLUM_BED_BROWN_01")
            if 0 < uVar4 then
                iVar6 = 0
                repeat
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        while puVar1 ~= unaff_EDI do
                            -- TODO(native): (**(code **)*puVar1)(0);
                            puVar1 = puVar1 + 3
                        end
                        if puStack_3c == nil then
                            return
                        end
                        -- TODO(native): free(puStack_3c);
                        return
                    end
                    quest:SetThingAsUsable(nil --[[missing]], (iVar6 ~= 0))
                    iVar2 = iVar2 + 1
                    iVar6 = iVar6 + 0xc
                until not (iVar2 < uVar4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                while puVar1 ~= unaff_EDI do
                    -- TODO(native): (**(code **)*puVar1)(0);
                    puVar1 = puVar1 + 3
                end
                if puStack_3c ~= nil then
                    -- TODO(native): free(puStack_3c);
                end
            else
                while puVar1 ~= unaff_EDI do
                    -- TODO(native): (**(code **)*puVar1)(0);
                    puVar1 = puVar1 + 3
                end
                if puStack_3c ~= nil then
                    -- TODO(native): free(puStack_3c);
                end
            end
        else
            while puVar1 ~= unaff_EDI do
                -- TODO(native): (**(code **)*puVar1)(0);
                puVar1 = puVar1 + 3
            end
            if puStack_3c ~= nil then
                -- TODO(native): free(puStack_3c);
            end
        end
    else
        while puVar1 ~= unaff_EDI do
            -- TODO(native): (**(code **)*puVar1)(0);
            puVar1 = puVar1 + 3
        end
        if puStack_3c ~= nil then
            -- TODO(native): free(puStack_3c);
        end
    end
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

