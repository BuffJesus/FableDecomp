-- Readable native conversion: HeroBed. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- HeroBed.Main (retail 0x00d446d0)
function Main(quest, me)
    local scratchValue, scratchValue2, scratchValue3, scratchValue4, scratchValue5, scratchValue6
    local scratchValue7, scratchValue8, scratchValue9, scratchValue10, getAllThingsWithDefName
    local getAllThingsWithDefName2, getAllThingsWithDefName3
    scratchValue5 = 0
    quest:SetThingAsUsable(me, false)
    quest:SetThingPersistent(me, true)
    getAllThingsWithDefName3 = quest:GetAllThingsWithDefName("OBJECT_GUILD_BED_FLOOR_PALLET_01")
    scratchValue2 = LOCALLIST_Count(getAllThingsWithDefName3[0 + 1])
    if 0 < scratchValue2 then
        scratchValue8 = 0
        repeat
            if quest:IsActiveThreadTerminating() then goto LAB_00d4482d end
            quest:SetThingAsUsable(getAllThingsWithDefName3[0 + 1][scratchValue8 + 1], false)
            quest:SetThingPersistent(getAllThingsWithDefName3[0 + 1][scratchValue8 + 1], true)
            scratchValue5 = scratchValue5 + 1
            scratchValue8 = scratchValue8 + 1
        until scratchValue5 >= scratchValue2
    end
    scratchValue3 = 0
    if quest:IsActiveThreadTerminating() then
        if getAllThingsWithDefName2 ~= nil then
            -- TODO(native): free(xStack_24[0 + 1]);
        end
        if getAllThingsWithDefName ~= nil then
            -- TODO(native): free(xStack_18[0 + 1]);
        end
    else
        getAllThingsWithDefName2 = quest:GetAllThingsWithDefName("OBJECT_GUILD_BED_APPRENTICE_01")
        scratchValue6 = LOCALLIST_Count(getAllThingsWithDefName2[0 + 1])
        if 0 < scratchValue6 then
            scratchValue9 = 0
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00d449bd end
                quest:SetThingAsUsable(getAllThingsWithDefName2[0 + 1][scratchValue9 + 1], false)
                quest:SetThingPersistent(getAllThingsWithDefName2[0 + 1][scratchValue9 + 1], true)
                scratchValue3 = scratchValue3 + 1
                scratchValue9 = scratchValue9 + 1
            until scratchValue3 >= scratchValue6
        end
        scratchValue4 = 0
        if quest:IsActiveThreadTerminating() then
            if getAllThingsWithDefName2 ~= nil then
                -- TODO(native): free(xStack_24[0 + 1]);
            end
            if getAllThingsWithDefName ~= nil then
                -- TODO(native): free(xStack_18[0 + 1]);
            end
        else
            getAllThingsWithDefName = quest:GetAllThingsWithDefName("OBJECT_BS_SLUM_BED_BROWN_01")
            scratchValue7 = LOCALLIST_Count(getAllThingsWithDefName[0 + 1])
            if 0 < scratchValue7 then
                scratchValue10 = 0
                repeat
                    if quest:IsActiveThreadTerminating() then goto LAB_00d44b4d end
                    quest:SetThingAsUsable(getAllThingsWithDefName[0 + 1][scratchValue10 + 1], false)
                    quest:SetThingPersistent(getAllThingsWithDefName[0 + 1][scratchValue10 + 1], true)
                    scratchValue4 = scratchValue4 + 1
                    scratchValue10 = scratchValue10 + 1
                until scratchValue4 >= scratchValue7
            end
            if quest:IsActiveThreadTerminating() then
                if getAllThingsWithDefName2 ~= nil then
                    -- TODO(native): free(xStack_24[0 + 1]);
                end
                if getAllThingsWithDefName ~= nil then
                    -- TODO(native): free(xStack_18[0 + 1]);
                end
            else
                if getAllThingsWithDefName2 ~= nil then
                    -- TODO(native): free(xStack_24[0 + 1]);
                end
                if getAllThingsWithDefName ~= nil then
                    -- TODO(native): free(xStack_18[0 + 1]);
                end
            end
        end
    end
    scratchValue = getAllThingsWithDefName3 == nil
    goto LAB_00d44c44
    ::LAB_00d449bd::
    if getAllThingsWithDefName2 ~= nil then
        -- TODO(native): free(xStack_24[0 + 1]);
    end
    if getAllThingsWithDefName ~= nil then
        -- TODO(native): free(xStack_18[0 + 1]);
    end
    if getAllThingsWithDefName3 ~= pu_stk_8 then
        scratchValue = getAllThingsWithDefName3 == nil
        goto LAB_00d44c44
    end
    goto LAB_00d44bc3
    ::LAB_00d44b4d::
    if getAllThingsWithDefName2 ~= nil then
        -- TODO(native): free(xStack_24[0 + 1]);
    end
    if getAllThingsWithDefName ~= nil then
        -- TODO(native): free(xStack_18[0 + 1]);
    end
    goto LAB_00d44bc3
    ::LAB_00d4482d::
    if getAllThingsWithDefName2 ~= nil then
        -- TODO(native): free(xStack_24[0 + 1]);
    end
    if getAllThingsWithDefName ~= nil then
        -- TODO(native): free(xStack_18[0 + 1]);
    end
    if getAllThingsWithDefName3 ~= pu_stk_8 then
        scratchValue = getAllThingsWithDefName3 == nil
        goto LAB_00d44c44
    end
    ::LAB_00d44bc3::
    scratchValue = getAllThingsWithDefName3 == nil
    ::LAB_00d44c44::
    if not scratchValue then
        -- TODO(native): free(xStack_c[0 + 1]);
    end
end

-- HeroBed.Init (retail 0x00d40be0)
function Init(quest, me)
end

-- HeroBed.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- HeroBed.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

