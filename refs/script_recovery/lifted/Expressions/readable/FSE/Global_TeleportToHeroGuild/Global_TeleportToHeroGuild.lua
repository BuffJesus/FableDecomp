-- Readable native conversion: Global_TeleportToHeroGuild. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Global_TeleportToHeroGuild.Main (retail 0x00cdd6b0)
function Main(quest)
    local resources = quest:RetailResources()
    local getGuildSealRecallAngleXY, scratchValue5, hero, hero6, hero7, getGuildSealRecallPos
    local getGuildSealRecallPos2, scratchValue6
    quest:GetHero()
    if not quest:NewScriptFrame() then return end
    if not quest:IsTeleportingActive() then quest:DeactivateQuestLater("Global_TeleportToHeroGuild", 0); return end
    if quest:IsActiveThreadTerminating() then return end
    local heroGuildTeleportMarker = quest:GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER")
    scratchValue5 = 0
    if not (not ((heroGuildTeleportMarker ~= nil and not heroGuildTeleportMarker:IsNull()) and (heroGuildTeleportMarker ~= nil and heroGuildTeleportMarker:IsAlive()))) then
        if quest:IsActiveThreadTerminating() then return end
        hero = quest:GetHero()
        if quest:IsDistanceBetweenThingsUnder(heroGuildTeleportMarker, hero, 5.0) then
            if quest:IsActiveThreadTerminating() then return end
        else
            if quest:IsActiveThreadTerminating() then return end
            local isRegionLoaded = quest:IsRegionLoaded("HeroGuildComplexInside")
            local predicateResult = quest:IsActiveThreadTerminating()
            if not isRegionLoaded then
                if predicateResult then
                    return
                end
                local hero3 = quest:GetHero()
                local followers = quest:GetFollowingEntityList(hero3)
                local resource = resources:NewResource()
                local hero4 = quest:GetHero()
                resources:TryAcquire(resource, hero4, 4)
                scratchValue6 = 0
                if #followers ~= 0 then
                    repeat
                        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                        resources:PerformExpression(resource, followers[scratchValue5 + 1], "EXPRESSION_WAIT")
                        scratchValue6 = scratchValue6 + 1
                        scratchValue5 = scratchValue5 + 1
                    until not (scratchValue6 < (#followers))
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                local hero5 = quest:GetHero()
                local hero8 = quest:GetHero()
                local getAngleXY = hero5:GetAngleXY()
                local position = hero8:GetPos()
                quest:SetGuildSealRecallLocation(position, getAngleXY)
                local hero9 = quest:GetHero()
                quest:EntityTeleportToThing(hero9, heroGuildTeleportMarker, true)
                resources:ReleaseResource(resource)
                goto LAB_00cdda68
            end
            if predicateResult then
                return
            end
        end
        if quest:IsActiveThreadTerminating() then return end
        getGuildSealRecallPos = quest:GetGuildSealRecallPos()
        if not (0.0001 * 0.0001 < getGuildSealRecallPos.z * getGuildSealRecallPos.z + getGuildSealRecallPos.y * getGuildSealRecallPos.y + getGuildSealRecallPos.x * getGuildSealRecallPos.x) then quest:SetGuildSealRecallLocation({x = 0, y = 0, z = 0}, 0.0); goto LAB_00cdda68 end
        if quest:IsActiveThreadTerminating() then return end
        getGuildSealRecallPos2 = quest:GetGuildSealRecallPos()
        hero6 = quest:GetHero()
        quest:EntityTeleportToPosition(hero6, getGuildSealRecallPos2, 0.0, true, true)
        getGuildSealRecallAngleXY = quest:GetGuildSealRecallAngleXY()
        hero7 = quest:GetHero()
        quest:EntitySetFacingAngle(hero7, getGuildSealRecallAngleXY, true)
        quest:SetGuildSealRecallLocation({x = 0, y = 0, z = 0}, 0.0)
    end
    ::LAB_00cdda68::
    quest:DeactivateQuestLater("Global_TeleportToHeroGuild", 0)
end

-- Global_TeleportToHeroGuild.Init (retail 0x00cdd5e0)
function Init(quest)
end

-- Global_TeleportToHeroGuild.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

