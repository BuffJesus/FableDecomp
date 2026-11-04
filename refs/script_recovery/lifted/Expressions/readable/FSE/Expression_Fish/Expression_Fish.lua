-- Readable native conversion: Expression_Fish. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Expression_Fish.Main (retail 0x00ee95c0)
function Main(quest)
    return quest:WithRetailResources(function(resources)
        local fishingSpotOwner = 0
        local controlAcquired, predicateResult, scratchValue4, scratchValue9, native_fishingAtSpot
        local native_fishingWater, string, msgOnFishingGameFinished
        local hero = quest:GetHero()
        quest:RegisterQuestConsciousCondition(hero)
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
        scratchValue9 = 0
        local getActiveQuestName = quest:GetActiveQuestName()
        quest:SetQuestAsPersistent(getActiveQuestName, false)
        while not quest:IsHeroControlledByPlayer() do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        local resource = resources:NewResource()
        local hero2 = quest:GetHero()
        controlAcquired = resources:TryAcquire(resource, hero2, 4)
        while not controlAcquired do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            local hero3 = quest:GetHero()
            controlAcquired = resources:TryAcquire(resource, hero3, 4)
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        quest:SetEnvironmentalEffectsAlwaysUpdate(true)
        local movie = resources:StartMovie("")
        quest:SetToDisplayTutorialsDuringCutscenes(true)
        local hero4 = quest:GetHero()
        quest:EntityForceToLookAtNothing(hero4)
        quest:PauseAllEntities(true)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(1.0)
        quest:SheatheHeroWeapons()
        resources:ClearAllActionsIncludingLoopingAnimations(resource)
        local getMostRecentValidUsedTargetName = quest:GetMostRecentValidUsedTargetName()
        if getMostRecentValidUsedTargetName == "" then
            if not quest:IsActiveThreadTerminating() then
                local hero5 = quest:GetHero()
                if quest:IsObjectInThingsPossession("OBJECT_FISHING_ROD_UPGRADED", hero5) then
                    if not quest:IsActiveThreadTerminating() then string = "OBJECT_FISHING_ROD_UPGRADED"; goto LAB_00ee98bc end
                else
                    quest:GetHero()
                    if not quest:IsActiveThreadTerminating() then string = "OBJECT_FISHING_ROD"; goto LAB_00ee98bc end
                end
                goto FLOW_past_lab_00ee98bc
                ::LAB_00ee98bc::
                quest:MakeHeroCarryItemInHand(string)
                goto LAB_00ee98de
                ::FLOW_past_lab_00ee98bc::
            end
        elseif not quest:IsActiveThreadTerminating() then
            quest:MakeHeroCarryItemInHand(getMostRecentValidUsedTargetName)
            goto LAB_00ee98de
        end
        resources:RetireThingOutput(fishingSpotOwner); fishingSpotOwner = 0; resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
        do return end
        ::LAB_00ee98de::
        resources:PlayLoopingAnimation(resource, "ST_FISHING_IDLE", -1, false, false, false, true, true, false, false)
        predicateResult = false
        native_fishingAtSpot = 0
        local local_fishingSpot = resources:CopyThingOutput(nil)
        fishingSpotOwner = resources:TrackThingOutput(local_fishingSpot)
        local hero7 = quest:GetHero()
        local getAllThingsWithDefNameByDistanceFrom = quest:GetAllThingsWithDefNameByDistanceFrom(hero7, "MARKER_FISHING_SPOT")
        local count = #getAllThingsWithDefNameByDistanceFrom
        if count < 1 then goto LAB_00ee9adf end
        if not quest:IsActiveThreadTerminating() then
            scratchValue4 = 0
            if 0 < count then
                repeat
                    if quest:IsActiveThreadTerminating() then resources:RetireThingOutput(fishingSpotOwner); fishingSpotOwner = 0; resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
                    if quest:IsFishingSpotEnabled(getAllThingsWithDefNameByDistanceFrom[scratchValue9 + 1]) then
                        if quest:IsActiveThreadTerminating() then resources:RetireThingOutput(fishingSpotOwner); fishingSpotOwner = 0; resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
                        resources:AssignThingOutput(fishingSpotOwner, getAllThingsWithDefNameByDistanceFrom[scratchValue4 + 1])
                        break
                    end
                    scratchValue4 = scratchValue4 + 1
                    scratchValue9 = scratchValue9 + 1
                until not (scratchValue4 < count)
            end
            if not quest:IsActiveThreadTerminating() then
                if not (local_fishingSpot ~= nil and local_fishingSpot:IsAlive()) then
                    goto LAB_00ee9adf
                elseif not quest:IsActiveThreadTerminating() then
                    local getThingPosition2 = resources:GetThingPosition(local_fishingSpot)
                    local native_fishingX = getThingPosition2.x
                    local native_fishingY = getThingPosition2.y
                    native_fishingWater = getThingPosition2.z
                    native_fishingWater = quest:GetWaterHeightAtPosition({x = native_fishingX, y = native_fishingY, z = native_fishingWater})
                    local hero8 = quest:GetHero()
                    local getThingPosition = resources:GetThingPosition(hero8)
                    local scratchValue = getThingPosition.y - native_fishingY
                    local scratchValue3 = getThingPosition.z - native_fishingWater
                    if not ((scratchValue * scratchValue + scratchValue3 * scratchValue3) + (getThingPosition.x - native_fishingX) * (getThingPosition.x - native_fishingX) < 100.0) then goto LAB_00ee9adf end
                    if not quest:IsActiveThreadTerminating() then
                        native_fishingAtSpot = 1
                        if not quest:IsActiveThreadTerminating() then
                            local pThing = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(pThing, local_fishingSpot, true)
                            goto LAB_00ee9adf
                        end
                    end
                end
            end
        end
        resources:RetireThingOutput(fishingSpotOwner); fishingSpotOwner = 0; resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
        do return end
        ::LAB_00ee9adf::
        local globalFishingCamera1 = quest:GetThingWithScriptName("GlobalFishingCamera1")
        local globalFishingCamera = quest:GetThingWithScriptName("GlobalFishingCamera2")
        quest:Pause(1.0)
        local hero9 = quest:GetHero()
        quest:CameraUseCameraPoint(globalFishingCamera1, hero9, -1.0, 0, -1)
        quest:FadeScreenIn()
        quest:Pause(2.0)
        quest:HeroGoFishing(native_fishingAtSpot ~= 0)
        msgOnFishingGameFinished = quest:MsgOnFishingGameFinished()
        while msgOnFishingGameFinished == nil do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then resources:RetireThingOutput(fishingSpotOwner); fishingSpotOwner = 0; resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
            msgOnFishingGameFinished = quest:MsgOnFishingGameFinished()
        end
        if quest:IsActiveThreadTerminating() then resources:RetireThingOutput(fishingSpotOwner); fishingSpotOwner = 0; resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
        local hero10 = quest:GetHero()
        quest:CameraUseCameraPoint(globalFishingCamera, hero10, -1.0, 0, -1)
        if not (msgOnFishingGameFinished ~= nil and msgOnFishingGameFinished:IsAlive()) then
            if not quest:IsActiveThreadTerminating() then resources:PlayAnimation(resource, "ST_FISHING_FAILURE", false, true, false, true, true, false, false); goto LAB_00ee9cd6 end
        elseif not quest:IsActiveThreadTerminating() then
            predicateResult = true
            resources:PlayAnimation(resource, "ST_FISHING_SUCCESS", false, true, false, true, true, false, false)
            quest:Pause(0.2)
            goto LAB_00ee9cd6
        end
        resources:RetireThingOutput(fishingSpotOwner); fishingSpotOwner = 0; resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
        do return end
        ::LAB_00ee9cd6::
        quest:Pause(3.75)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(1.0)
        if predicateResult and native_fishingAtSpot ~= 0 then
            if quest:IsActiveThreadTerminating() then resources:RetireThingOutput(fishingSpotOwner); fishingSpotOwner = 0; resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
            quest:DisableFishingSpot(local_fishingSpot)
        end
        quest:MakeHeroCarryItemInHand("")
        local hero11 = quest:GetHero()
        quest:EntityResetForceToLookAt(hero11)
        quest:CameraResetToViewBehindHero(0.0)
        quest:CameraDefault()
        quest:Pause(1.0)
        quest:PauseAllEntities(false)
        quest:FadeScreenIn()
        quest:Pause(0.5)
        if predicateResult and native_fishingAtSpot ~= 0 then
            if quest:IsActiveThreadTerminating() then resources:RetireThingOutput(fishingSpotOwner); fishingSpotOwner = 0; resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
            quest:GiveHeroItemsFromContainer(local_fishingSpot, false)
            quest:UpdateFishWeight(msgOnFishingGameFinished)
            quest:RemoveThing(msgOnFishingGameFinished, false, true)
        elseif msgOnFishingGameFinished ~= nil and msgOnFishingGameFinished:IsAlive() then
            if quest:IsActiveThreadTerminating() then resources:RetireThingOutput(fishingSpotOwner); fishingSpotOwner = 0; resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
            quest:UpdateFishWeight(msgOnFishingGameFinished)
            quest:GiveHeroItem(msgOnFishingGameFinished)
        end
        quest:SetEnvironmentalEffectsAlwaysUpdate(false)
        quest:SetToDisplayTutorialsDuringCutscenes(false)
        local getActiveQuestName2 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(getActiveQuestName2, 0)
        resources:RetireThingOutput(fishingSpotOwner); fishingSpotOwner = 0; resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
    end)
end

-- Expression_Fish.Init (retail 0x00ee94c0)
function Init(quest)
    quest:SetStateInt("LastFishingLevel", 1)
end

-- Expression_Fish.OnPersist (retail 0x00ee9590)
function OnPersist(quest, context)
    local lastFishingLevel = quest:GetStateInt("LastFishingLevel") or 0
    lastFishingLevel = quest:PersistTransferInt(context, "LastFishingLevel", lastFishingLevel)
    quest:SetStateInt("LastFishingLevel", lastFishingLevel)
end

