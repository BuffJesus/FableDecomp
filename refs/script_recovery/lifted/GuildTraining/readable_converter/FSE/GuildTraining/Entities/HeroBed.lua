-- Readable native conversion: HeroBed. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- HeroBed.Main (retail 0x00d446d0)
function Main(quest, me)
    local scratchValue, scratchValue3, scratchValue4, scratchValue5, scratchValue6, scratchValue7
    scratchValue4 = 0
    quest:SetThingAsUsable(me, false)
    quest:SetThingPersistent(me, true)
    local guildBedFloorPallet01 = quest:GetAllThingsWithDefName("OBJECT_GUILD_BED_FLOOR_PALLET_01")
    local count = #guildBedFloorPallet01
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
        local guildBedApprentice01 = quest:GetAllThingsWithDefName("OBJECT_GUILD_BED_APPRENTICE_01")
        local count2 = #guildBedApprentice01
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
            local bsSlumBedBrown01 = quest:GetAllThingsWithDefName("OBJECT_BS_SLUM_BED_BROWN_01")
            local count3 = #bsSlumBedBrown01
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
    do return end
    ::LAB_00d449bd::
    if #guildBedFloorPallet01 ~= 0 then
        return
    end
    do return end
    ::LAB_00d44b4d::
    do return end
    ::LAB_00d4482d::
    if #guildBedFloorPallet01 ~= 0 then
        return
    end
end

-- HeroBed.Init (retail 0x00d40be0)
function Init(quest, me)
end

-- HeroBed.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- HeroBed.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

