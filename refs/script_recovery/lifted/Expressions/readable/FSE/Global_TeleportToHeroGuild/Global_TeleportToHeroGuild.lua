-- Readable native conversion: Global_TeleportToHeroGuild. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Global_TeleportToHeroGuild.Main (retail 0x00cdd6b0)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, getGuildSealRecallPos, scratchValue6
    if not quest:NewScriptFrame() then return end
    if not quest:IsTeleportingActive() then quest:DeactivateQuestLater("Global_TeleportToHeroGuild", 0); return end
    if quest:IsActiveThreadTerminating() then return end
    local heroGuildTeleportMarker = quest:GetThingWithScriptName("HERO_GUILD_TELEPORT_MARKER")
    scratchValue = 0
    if not ((heroGuildTeleportMarker ~= nil and not heroGuildTeleportMarker:IsNull()) and (heroGuildTeleportMarker ~= nil and heroGuildTeleportMarker:IsAlive())) then goto LAB_00cdda68 end
    if not quest:IsDistanceBetweenThingsUnder(heroGuildTeleportMarker, hero, 5.0) then
        if not quest:IsRegionLoaded("HeroGuildComplexInside") then
            local followers = quest:GetFollowingEntityList(hero)
            local resource = resources:NewResource()
            resources:TryAcquire(resource, hero, 4)
            scratchValue6 = 0
            if #followers ~= 0 then
                repeat
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                    resources:PerformExpression(resource, followers[scratchValue + 1], "EXPRESSION_WAIT")
                    scratchValue6 = scratchValue6 + 1
                    scratchValue = scratchValue + 1
                until scratchValue6 >= #followers
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            quest:SetGuildSealRecallLocation(hero:GetPos(), hero:GetAngleXY())
            quest:EntityTeleportToThing(hero, heroGuildTeleportMarker, true)
            resources:ReleaseResource(resource)
            goto LAB_00cdda68
        end
    end
    getGuildSealRecallPos = quest:GetGuildSealRecallPos()
    if not (0.0001 * 0.0001 < getGuildSealRecallPos.z * getGuildSealRecallPos.z + getGuildSealRecallPos.y * getGuildSealRecallPos.y + getGuildSealRecallPos.x * getGuildSealRecallPos.x) then quest:SetGuildSealRecallLocation({x = 0, y = 0, z = 0}, 0.0); goto LAB_00cdda68 end
    if quest:IsActiveThreadTerminating() then return end
    quest:EntityTeleportToPosition(hero, quest:GetGuildSealRecallPos(), 0.0, true, true)
    quest:EntitySetFacingAngle(hero, quest:GetGuildSealRecallAngleXY(), true)
    quest:SetGuildSealRecallLocation({x = 0, y = 0, z = 0}, 0.0)
    ::LAB_00cdda68::
    quest:DeactivateQuestLater("Global_TeleportToHeroGuild", 0)
end

-- Global_TeleportToHeroGuild.Init (retail 0x00cdd5e0)
function Init(quest)
end

-- Global_TeleportToHeroGuild.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

