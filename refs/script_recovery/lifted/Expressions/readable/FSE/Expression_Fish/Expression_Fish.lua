-- Readable native conversion: Expression_Fish. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Expression_Fish.Main (retail 0x00ee95c0)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, scratchValue4, scratchValue9, string, scratchValue13, scratchValue14
    local msgOnFishingGameFinished
    if not quest:NewScriptFrame() then return end
    scratchValue9 = 0
    quest:SetQuestAsPersistent(quest:GetActiveQuestName(), false)
    while not quest:IsHeroControlledByPlayer() do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local resource = resources:NewResource()
    while not resources:TryAcquire(resource, hero, 4) do
        if not quest:NewScriptFrame() then resources:ReleaseResource(resource); return end
    end
    quest:SetEnvironmentalEffectsAlwaysUpdate(true)
    local movie = resources:StartMovie("")
    quest:SetToDisplayTutorialsDuringCutscenes(true)
    quest:EntityForceToLookAtNothing(hero)
    quest:PauseAllEntities(true)
    quest:FadeScreenOut(0.5, 0.5)
    quest:Pause(1.0)
    quest:SheatheHeroWeapons()
    resources:ClearAllActionsIncludingLoopingAnimations(resource)
    local getMostRecentValidUsedTargetName = quest:GetMostRecentValidUsedTargetName()
    if getMostRecentValidUsedTargetName == "" then
        if not quest:IsActiveThreadTerminating() then
            if quest:IsObjectInThingsPossession("OBJECT_FISHING_ROD_UPGRADED", hero) then
                if not quest:IsActiveThreadTerminating() then string = "OBJECT_FISHING_ROD_UPGRADED"; goto LAB_00ee98bc end
            elseif not quest:IsActiveThreadTerminating() then
                string = "OBJECT_FISHING_ROD"
                goto LAB_00ee98bc
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
    resources:DestroyMovie(movie)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00ee98de::
    resources:PlayLoopingAnimation(resource, "ST_FISHING_IDLE", -1, false, false, false, true, true, false, false)
    predicateResult = false
    local getAllThingsWithDefNameByDistanceFrom = quest:GetAllThingsWithDefNameByDistanceFrom(hero, "MARKER_FISHING_SPOT")
    local count = #getAllThingsWithDefNameByDistanceFrom
    if count < 1 then goto LAB_00ee9adf end
    if not quest:IsActiveThreadTerminating() then
        scratchValue4 = 0
        if 0 < count then
            repeat
                if quest:IsActiveThreadTerminating() then resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
                if quest:IsFishingSpotEnabled(getAllThingsWithDefNameByDistanceFrom[scratchValue9 + 1]) then
                    if quest:IsActiveThreadTerminating() then resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
                    -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_6c,iStack_60 + iVar12 * 0xc);
                    break
                end
                scratchValue4 = scratchValue4 + 1
                scratchValue9 = scratchValue9 + 1
            until scratchValue4 >= count
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
                local scratchValue = position.y - scratchValue13
                local scratchValue3 = position.z - fret_0
                if 100.0 <= (position.x - scratchValue14) * (position.x - scratchValue14) + scratchValue * scratchValue + scratchValue3 * scratchValue3 then goto LAB_00ee9adf end
                if not quest:IsActiveThreadTerminating() then
                    if not quest:IsActiveThreadTerminating() then quest:EntitySetFacingAngleTowardsThing(hero, nil, true); goto LAB_00ee9adf end
                end
            end
        end
    end
    resources:DestroyMovie(movie)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00ee9adf::
    local globalFishingCamera1 = quest:GetThingWithScriptName("GlobalFishingCamera1")
    local globalFishingCamera = quest:GetThingWithScriptName("GlobalFishingCamera2")
    quest:Pause(1.0)
    quest:CameraUseCameraPoint(globalFishingCamera1, hero, -1.0, 0, -1)
    quest:FadeScreenIn()
    quest:Pause(2.0)
    quest:HeroGoFishing(true)
    msgOnFishingGameFinished = quest:MsgOnFishingGameFinished()
    while msgOnFishingGameFinished == nil do
        if not quest:NewScriptFrame() then resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
        msgOnFishingGameFinished = quest:MsgOnFishingGameFinished()
    end
    if quest:IsActiveThreadTerminating() then resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
    quest:CameraUseCameraPoint(globalFishingCamera, hero, -1.0, 0, -1)
    if not (msgOnFishingGameFinished ~= nil and msgOnFishingGameFinished:IsAlive()) then
        if not quest:IsActiveThreadTerminating() then resources:PlayAnimation(resource, "ST_FISHING_FAILURE", false, true, false, true, true, false, false); goto LAB_00ee9cd6 end
    elseif not quest:IsActiveThreadTerminating() then
        predicateResult = true
        resources:PlayAnimation(resource, "ST_FISHING_SUCCESS", false, true, false, true, true, false, false)
        quest:Pause(0.2)
        goto LAB_00ee9cd6
    end
    resources:DestroyMovie(movie)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00ee9cd6::
    quest:Pause(3.75)
    quest:FadeScreenOut(0.5, 0.5)
    quest:Pause(1.0)
    if predicateResult then
        if quest:IsActiveThreadTerminating() then resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
        quest:DisableFishingSpot(nil)
    end
    quest:MakeHeroCarryItemInHand("")
    quest:EntityResetForceToLookAt(hero)
    quest:CameraResetToViewBehindHero(0.0)
    quest:CameraDefault()
    quest:Pause(1.0)
    quest:PauseAllEntities(false)
    quest:FadeScreenIn()
    quest:Pause(0.5)
    if predicateResult then
        if quest:IsActiveThreadTerminating() then resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
        quest:GiveHeroItemsFromContainer(nil, false)
        quest:UpdateFishWeight(msgOnFishingGameFinished)
        quest:RemoveThing(msgOnFishingGameFinished, false, true)
    elseif msgOnFishingGameFinished ~= nil and msgOnFishingGameFinished:IsAlive() then
        if quest:IsActiveThreadTerminating() then resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
        quest:UpdateFishWeight(msgOnFishingGameFinished)
        quest:GiveHeroItem(msgOnFishingGameFinished)
    end
    quest:SetEnvironmentalEffectsAlwaysUpdate(false)
    quest:SetToDisplayTutorialsDuringCutscenes(false)
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    resources:DestroyMovie(movie)
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

