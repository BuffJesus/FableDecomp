-- Readable native conversion: Expression_Fish. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Expression_Fish.Main (retail 0x00ee95c0)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, getAllThingsWithDefNameByDistanceFrom, scratchValue4, scratchValue5
    local count, scratchValue, globalFishingCamera1, globalFishingCamera, movie, scratchValue14
    local scratchValue15, msgOnFishingGameFinished
    if not quest:NewScriptFrame() then return end
    scratchValue = 0
    quest:SetQuestAsPersistent(quest:GetActiveQuestName(), false)
    while not quest:IsHeroControlledByPlayer() do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local resource = resources:NewResource()
    scratchValue4 = 4
    while not resources:TryAcquire(resource, hero, scratchValue4) do
        if not quest:NewScriptFrame() then goto LAB_00ee9ee4 end
        scratchValue4 = 4
    end
    quest:SetEnvironmentalEffectsAlwaysUpdate(true)
    movie = resources:StartMovie("")
    quest:SetToDisplayTutorialsDuringCutscenes(true)
    quest:EntityForceToLookAtNothing(hero)
    quest:PauseAllEntities(true)
    quest:FadeScreenOut(0.5, 0.5)
    quest:Pause(1.0)
    quest:SheatheHeroWeapons()
    -- TODO(native): ClearAllActionsIncludingLoopingAnimations: unresolved entity receiver/resource in quest context; arguments: 
    if quest:GetMostRecentValidUsedTargetName() == 0x122d70e then
        if not quest:IsActiveThreadTerminating() then
            if quest:IsObjectInThingsPossession("OBJECT_FISHING_ROD_UPGRADED", hero) then
                if not quest:IsActiveThreadTerminating() then goto LAB_00ee98bc end
            elseif not quest:IsActiveThreadTerminating() then
                goto LAB_00ee98bc
            end
            goto FLOW_past_lab_00ee98bc
            ::LAB_00ee98bc::
            quest:MakeHeroCarryItemInHand(nil --[[missing]], scratchValue4 ~= 0, false)
            goto LAB_00ee98de
            ::FLOW_past_lab_00ee98bc::
        end
    elseif not quest:IsActiveThreadTerminating() then
        quest:MakeHeroCarryItemInHand(nil --[[missing]], nil --[[missing]], nil --[[missing]])
        goto LAB_00ee98de
    end
    goto FLOW_past_lab_00ee98de
    ::LAB_00ee98de::
    -- TODO(native): PlayLoopingAnimation: unresolved entity receiver/resource in quest context; arguments: -1,0,0,0,1,true,0,0
    predicateResult = false
    getAllThingsWithDefNameByDistanceFrom = quest:GetAllThingsWithDefNameByDistanceFrom(hero, "MARKER_FISHING_SPOT")
    count = #getAllThingsWithDefNameByDistanceFrom
    if count < 1 then goto LAB_00ee9adf end
    if not quest:IsActiveThreadTerminating() then
        scratchValue5 = 0
        if 0 < count then
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00ee9ec0 end
                if quest:IsFishingSpotEnabled(getAllThingsWithDefNameByDistanceFrom[scratchValue / 12 + 1]) then
                    -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_6c,iStack_60 + iVar12 * 0xc);
                    break
                end
                scratchValue5 = scratchValue5 + 1
                scratchValue = scratchValue + 12
            until scratchValue5 >= count
        end
        if not quest:IsActiveThreadTerminating() then
            if not (nil ~= nil and (nil):IsAlive()) then
                goto LAB_00ee9adf
            elseif not quest:IsActiveThreadTerminating() then
                -- TODO(native): xStack_30 = (undefined **)puVar7.x;
                -- TODO(native): xStack_2c = puVar7.y;
                -- TODO(native): fret_0 = quest:GetWaterHeightAtPosition(&xStack_30)
                local fret_0 = nil --[[unresolved native value]]
                local position = hero:GetPos()
                local scratchValue2 = position.y - scratchValue14
                local scratchValue3 = position.z - fret_0
                if 100.0 <= (position.x - scratchValue15) * (position.x - scratchValue15) + scratchValue2 * scratchValue2 + scratchValue3 * scratchValue3 then goto LAB_00ee9adf end
                if not quest:IsActiveThreadTerminating() then
                    if not quest:IsActiveThreadTerminating() then quest:EntitySetFacingAngleTowardsThing(hero, nil, true); goto LAB_00ee9adf end
                end
            end
        end
    end
    goto FLOW_past_lab_00ee9adf
    ::LAB_00ee9adf::
    globalFishingCamera1 = quest:GetThingWithScriptName("GlobalFishingCamera1")
    globalFishingCamera = quest:GetThingWithScriptName("GlobalFishingCamera2")
    quest:Pause(1.0)
    quest:CameraUseCameraPoint(globalFishingCamera1, hero, -1.0, 0, -1)
    quest:FadeScreenIn()
    quest:Pause(2.0)
    quest:HeroGoFishing(true)
    msgOnFishingGameFinished = quest:MsgOnFishingGameFinished()
    while msgOnFishingGameFinished == nil do
        if not quest:NewScriptFrame() then goto LAB_00ee9ea5 end
        msgOnFishingGameFinished = quest:MsgOnFishingGameFinished()
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00ee9ea5 end
    quest:CameraUseCameraPoint(globalFishingCamera, hero, -1.0, 0, -1)
    if not (msgOnFishingGameFinished ~= nil and msgOnFishingGameFinished:IsAlive()) then
        if not quest:IsActiveThreadTerminating() then
            -- TODO(native): PlayAnimation: unresolved entity receiver/resource in quest context; arguments: 0,1,0,1,true,0,0
            goto LAB_00ee9cd6
        end
    elseif not quest:IsActiveThreadTerminating() then
        predicateResult = true
        -- TODO(native): PlayAnimation: unresolved entity receiver/resource in quest context; arguments: 0,1,0,1,true,0,0
        quest:Pause(0.2)
        goto LAB_00ee9cd6
    end
    goto FLOW_past_lab_00ee9cd6
    ::LAB_00ee9cd6::
    quest:Pause(3.75)
    quest:FadeScreenOut(0.5, 0.5)
    quest:Pause(1.0)
    if predicateResult then
        if quest:IsActiveThreadTerminating() then goto LAB_00ee9ea5 end
        quest:DisableFishingSpot(nil)
    end
    quest:MakeHeroCarryItemInHand(nil --[[missing]], predicateResult, nil --[[missing]])
    quest:EntityResetForceToLookAt(hero)
    quest:CameraResetToViewBehindHero(0.0)
    quest:CameraDefault()
    quest:Pause(1.0)
    quest:PauseAllEntities(false)
    quest:FadeScreenIn()
    quest:Pause(0.5)
    if predicateResult then
        if quest:IsActiveThreadTerminating() then goto LAB_00ee9ea5 end
        quest:GiveHeroItemsFromContainer(nil, false)
        quest:UpdateFishWeight(msgOnFishingGameFinished)
        quest:RemoveThing(msgOnFishingGameFinished, false, true)
    elseif msgOnFishingGameFinished ~= nil and msgOnFishingGameFinished:IsAlive() then
        if quest:IsActiveThreadTerminating() then goto LAB_00ee9ea5 end
        quest:UpdateFishWeight(msgOnFishingGameFinished)
        quest:GiveHeroItem(msgOnFishingGameFinished)
    end
    quest:SetEnvironmentalEffectsAlwaysUpdate(false)
    quest:SetToDisplayTutorialsDuringCutscenes(false)
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    ::FLOW_past_lab_00ee9cd6::
    ::LAB_00ee9ea5::
    ::FLOW_past_lab_00ee9adf::
    ::LAB_00ee9ec0::
    ::FLOW_past_lab_00ee98de::
    resources:DestroyMovie(movie)
    ::LAB_00ee9ee4::
    resources:ReleaseResource(resource)
end

-- Expression_Fish.Init (retail 0x00ee94c0)
function Init(quest)
    quest:SetStateInt("LastFishingLevel", 1)
end

-- Expression_Fish.OnPersist (retail 0x00ee9590)
function OnPersist(quest, context)
    quest:SetStateInt("LastFishingLevel", quest:PersistTransferInt(context, "LastFishingLevel", quest:GetStateInt("LastFishingLevel") or 0))
end

