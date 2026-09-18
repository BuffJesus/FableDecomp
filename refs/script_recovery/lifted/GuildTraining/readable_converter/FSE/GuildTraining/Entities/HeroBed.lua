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
    local count, scratchValue, scratchValue3, scratchValue4, count2, count3, scratchValue5
    local scratchValue6, scratchValue7, bsSlumBedBrown01, guildBedApprentice01
    local guildBedFloorPallet01
    scratchValue4 = 0
    quest:SetThingAsUsable(me, false)
    quest:SetThingPersistent(me, true)
    guildBedFloorPallet01 = quest:GetAllThingsWithDefName("OBJECT_GUILD_BED_FLOOR_PALLET_01")
    count = #guildBedFloorPallet01
    if 0 < count then
        scratchValue5 = 0
        repeat
            if quest:IsActiveThreadTerminating() then goto LAB_00d4482d end
            quest:SetThingAsUsable(guildBedFloorPallet01[scratchValue5 + 1], false)
            quest:SetThingPersistent(guildBedFloorPallet01[scratchValue5 + 1], true)
            scratchValue4 = scratchValue4 + 1
            scratchValue5 = scratchValue5 + 1
        until scratchValue4 >= count
    end
    scratchValue = 0
    if not quest:IsActiveThreadTerminating() then
        guildBedApprentice01 = quest:GetAllThingsWithDefName("OBJECT_GUILD_BED_APPRENTICE_01")
        count2 = #guildBedApprentice01
        if 0 < count2 then
            scratchValue6 = 0
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00d449bd end
                quest:SetThingAsUsable(guildBedApprentice01[scratchValue6 + 1], false)
                quest:SetThingPersistent(guildBedApprentice01[scratchValue6 + 1], true)
                scratchValue = scratchValue + 1
                scratchValue6 = scratchValue6 + 1
            until scratchValue >= count2
        end
        scratchValue3 = 0
        if not quest:IsActiveThreadTerminating() then
            bsSlumBedBrown01 = quest:GetAllThingsWithDefName("OBJECT_BS_SLUM_BED_BROWN_01")
            count3 = #bsSlumBedBrown01
            if 0 < count3 then
                scratchValue7 = 0
                repeat
                    if quest:IsActiveThreadTerminating() then goto LAB_00d44b4d end
                    quest:SetThingAsUsable(bsSlumBedBrown01[scratchValue7 + 1], false)
                    quest:SetThingPersistent(bsSlumBedBrown01[scratchValue7 + 1], true)
                    scratchValue3 = scratchValue3 + 1
                    scratchValue7 = scratchValue7 + 1
                until scratchValue3 >= count3
            end
        end
    end
    goto LAB_00d44c44
    ::LAB_00d449bd::
    if #guildBedFloorPallet01 ~= 0 then
        goto LAB_00d44c44
    end
    goto LAB_00d44bc3
    ::LAB_00d44b4d::
    goto LAB_00d44bc3
    ::LAB_00d4482d::
    if #guildBedFloorPallet01 ~= 0 then
        goto LAB_00d44c44
    end
    ::LAB_00d44bc3::
    ::LAB_00d44c44::
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

