-- Readable native conversion: Global_TeleportToHeroGuild. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Global_TeleportToHeroGuild.Main (retail 0x00cdd6b0)
function Main(quest)
    local hero = quest:GetHero()
    local getGuildSealRecallPos, heroGuildTeleportMarker, scratchValue8, scratchValue9
    if not quest:NewScriptFrame() then return end
    if not quest:IsTeleportingActive() then goto LAB_00cdda71 end
    heroGuildTeleportMarker = quest:GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER")
    if not ((heroGuildTeleportMarker ~= nil) and (heroGuildTeleportMarker ~= nil and heroGuildTeleportMarker:IsAlive())) then goto LAB_00cdda68 end
    if not quest:IsDistanceBetweenThingsUnder(heroGuildTeleportMarker, hero, 5.0) then
        if not quest:IsRegionLoaded("HeroGuildComplexInside") then
            hero:AcquireControl(4)
            local scratchValue = (scratchValue9._4_4_ - scratchValue9._0_4_) >> 31
            scratchValue8 = 0
            if (scratchValue9._4_4_ - scratchValue9._0_4_) / 12 + scratchValue ~= scratchValue then
                repeat
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue8 = scratchValue8 + 1
                    else
                        hero:ReleaseControl()
                        do return end
                        scratchValue8 = scratchValue8 + 1
                    end
                until scratchValue8 >= ((scratchValue9._4_4_ - scratchValue9._0_4_) / 12)
            end
            if quest:IsActiveThreadTerminating() then hero:ReleaseControl(); return end
            quest:SetGuildSealRecallLocation(hero:GetAngleXY(), 4)
            quest:EntityTeleportToThing(hero, heroGuildTeleportMarker, true)
            hero:ReleaseControl()
            goto LAB_00cdda68
        end
    end
    getGuildSealRecallPos = quest:GetGuildSealRecallPos()
    if not (DAT_0129ba3c * DAT_0129ba3c < getGuildSealRecallPos.z * getGuildSealRecallPos.z + getGuildSealRecallPos.y * getGuildSealRecallPos.y + getGuildSealRecallPos.x * getGuildSealRecallPos.x) then quest:SetGuildSealRecallLocation(0.0, nil --[[missing]]); goto LAB_00cdda68 end
    if quest:IsActiveThreadTerminating() then return end
    quest:EntityTeleportToPosition(hero, nil --[[missing]], 0.0, true, true)
    quest:EntitySetFacingAngle(hero, quest:GetGuildSealRecallAngleXY(), true)
    quest:SetGuildSealRecallLocation(0.0, nil --[[missing]])
    ::LAB_00cdda68::
    ::LAB_00cdda71::
    quest:DeactivateQuestLater("Global_TeleportToHeroGuild", 0)
end

-- Global_TeleportToHeroGuild.Init (retail 0x00cdd5e0)
function Init(quest)
end

-- Global_TeleportToHeroGuild.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

