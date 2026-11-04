-- Generated native draft: Expression_Fish. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    return quest:WithRetailResources(function(resources)
    local fishingSpotOwner = 0
    local bVar2, bVar3, delay, fVar1, fVar10, fret_0, iStack_60, iVar12, iVar6, iVar9, local_fishingSpot, native_fishingAtSpot, native_fishingWater, native_fishingX, native_fishingY, pCVar4, pCVar5, pCVar8, pThing, pppuVar11, puVar7, r1, r2, r3, r4, r5, string, xStack_10, xStack_20, xStack_54
    local alive = true
    pCVar4 = quest:GetHero()
    quest:RegisterQuestConsciousCondition(pCVar4)
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    iVar9 = 0
    bVar2 = false
    pCVar5 = quest:GetActiveQuestName()
    quest:SetQuestAsPersistent(pCVar5, bVar2)
    bVar2 = quest:IsHeroControlledByPlayer()
    while not bVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        bVar2 = quest:IsHeroControlledByPlayer()
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    xStack_20 = resources:NewResource()
    iVar12 = 4
    pppuVar11 = xStack_20
    pCVar4 = quest:GetHero()
    bVar2 = resources:TryAcquire(pppuVar11, pCVar4, iVar12)
    while not bVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00ee9ee4 end
        iVar12 = 4
        pppuVar11 = xStack_20
        pCVar4 = quest:GetHero()
        bVar2 = resources:TryAcquire(pppuVar11, pCVar4, iVar12)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00ee9ee4 end
    quest:SetEnvironmentalEffectsAlwaysUpdate(true)
    xStack_10 = resources:StartMovie("")
    quest:SetToDisplayTutorialsDuringCutscenes(true)
    pCVar4 = quest:GetHero()
    quest:EntityForceToLookAtNothing(pCVar4)
    quest:PauseAllEntities(true)
    quest:FadeScreenOut(0.5, 0.5)
    quest:Pause(1.0)
    quest:SheatheHeroWeapons()
    resources:ClearAllActionsIncludingLoopingAnimations(xStack_20)
    r1 = quest:GetMostRecentValidUsedTargetName()
    iVar6 = ((r1 ~= "") and 1 or 0)
    if iVar6 == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            pCVar4 = quest:GetHero()
            bVar2 = quest:IsObjectInThingsPossession("OBJECT_FISHING_ROD_UPGRADED", pCVar4)
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    string = "OBJECT_FISHING_ROD_UPGRADED"
                    goto LAB_00ee98bc
                end
            else
                pCVar4 = quest:GetHero()
                r2 = quest:IsObjectInThingsPossession("OBJECT_FISHING_ROD", pCVar4)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    string = "OBJECT_FISHING_ROD"
                    goto LAB_00ee98bc
                end
            end
            goto FLOW_past_lab_00ee98bc
            ::LAB_00ee98bc::
            quest:MakeHeroCarryItemInHand(string)
            goto LAB_00ee98de
            ::FLOW_past_lab_00ee98bc::
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:MakeHeroCarryItemInHand(r1)
            goto LAB_00ee98de
        end
    end
    goto FLOW_past_lab_00ee98de
    ::LAB_00ee98de::
    resources:PlayLoopingAnimation(xStack_20, "ST_FISHING_IDLE", -1, false, false, false, true, true, false, false)
    bVar2 = false
    native_fishingAtSpot = 0
    local_fishingSpot = resources:CopyThingOutput(nil)
    fishingSpotOwner = resources:TrackThingOutput(local_fishingSpot)
    pCVar4 = quest:GetHero()
    iStack_60 = quest:GetAllThingsWithDefNameByDistanceFrom(pCVar4, "MARKER_FISHING_SPOT")
    iVar6 = #iStack_60
    if iVar6 < 1 then goto LAB_00ee9adf end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        iVar12 = 0
        if 0 < iVar6 then
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00ee9ec0 end
                bVar3 = quest:IsFishingSpotEnabled(iStack_60[(iVar9) / 0xc + 1])
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00ee9ec0 end
                    resources:AssignThingOutput(fishingSpotOwner, iStack_60[iVar12 + 1])
                    break
                end
                iVar12 = iVar12 + 1
                iVar9 = iVar9 + 0xc
            until not (iVar12 < iVar6)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            iVar6 = (local_fishingSpot ~= nil and local_fishingSpot:IsAlive())
            if not iVar6 then
                goto LAB_00ee9adf
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    puVar7 = resources:GetThingPosition(local_fishingSpot)
                    native_fishingX = puVar7.x
                    native_fishingY = puVar7.y
                    native_fishingWater = puVar7.z
                    fret_0 = quest:GetWaterHeightAtPosition({x = native_fishingX, y = native_fishingY, z = native_fishingWater})
                    native_fishingWater = fret_0
                    pCVar4 = quest:GetHero()
                    pCVar8 = resources:GetThingPosition(pCVar4)
                    fVar1 = pCVar8.y - native_fishingY
                    fVar10 = pCVar8.z - native_fishingWater
                    if not ((fVar1 * fVar1 + fVar10 * fVar10) + (pCVar8.x - native_fishingX) * (pCVar8.x - native_fishingX) < 100.0) then goto LAB_00ee9adf end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        native_fishingAtSpot = 1
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            bVar3 = true
                            pCVar4 = local_fishingSpot
                            pThing = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(pThing, pCVar4, bVar3)
                            goto LAB_00ee9adf
                        end
                    end
                end
            end
        end
    end
    goto FLOW_past_lab_00ee9adf
    ::LAB_00ee9adf::
    r3 = quest:GetThingWithScriptName("GlobalFishingCamera1")
    r4 = quest:GetThingWithScriptName("GlobalFishingCamera2")
    quest:Pause(1.0)
    iVar12 = -1
    iVar9 = 0
    fVar10 = -1.0
    pCVar4 = quest:GetHero()
    quest:CameraUseCameraPoint(r3, pCVar4, fVar10, iVar9, iVar12)
    quest:FadeScreenIn()
    quest:Pause(2.0)
    xStack_54 = nil
    quest:HeroGoFishing((native_fishingAtSpot ~= 0))
    xStack_54 = quest:MsgOnFishingGameFinished()
    bVar3 = (xStack_54 ~= nil)
    while not bVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00ee9ea5 end
        xStack_54 = quest:MsgOnFishingGameFinished()
        bVar3 = (xStack_54 ~= nil)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        iVar12 = -1
        iVar9 = 0
        fVar10 = -1.0
        pCVar4 = quest:GetHero()
        quest:CameraUseCameraPoint(r4, pCVar4, fVar10, iVar9, iVar12)
        iVar6 = (xStack_54 ~= nil and xStack_54:IsAlive())
        if not iVar6 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                resources:PlayAnimation(xStack_20, "ST_FISHING_FAILURE", false, true, false, true, true, false, false)
                goto LAB_00ee9cd6
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                bVar2 = true
                resources:PlayAnimation(xStack_20, "ST_FISHING_SUCCESS", false, true, false, true, true, false, false)
                quest:Pause(0.2)
                goto LAB_00ee9cd6
            end
        end
        goto FLOW_past_lab_00ee9cd6
        ::LAB_00ee9cd6::
        quest:Pause(3.75)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(1.0)
        if (bVar2) and (native_fishingAtSpot ~= 0) then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00ee9ea5 end
            quest:DisableFishingSpot(local_fishingSpot)
        end
        quest:MakeHeroCarryItemInHand("")
        pCVar4 = quest:GetHero()
        quest:EntityResetForceToLookAt(pCVar4)
        quest:CameraResetToViewBehindHero(0.0)
        quest:CameraDefault()
        quest:Pause(1.0)
        quest:PauseAllEntities(false)
        quest:FadeScreenIn()
        quest:Pause(0.5)
        if (bVar2) and (native_fishingAtSpot ~= 0) then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00ee9ea5 end
            r5 = quest:GiveHeroItemsFromContainer(local_fishingSpot, false)
            quest:UpdateFishWeight(xStack_54)
            quest:RemoveThing(xStack_54, false, true)
        else
            iVar6 = (xStack_54 ~= nil and xStack_54:IsAlive())
            if iVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00ee9ea5 end
                quest:UpdateFishWeight(xStack_54)
                quest:GiveHeroItem(xStack_54)
            end
        end
        quest:SetEnvironmentalEffectsAlwaysUpdate(false)
        quest:SetToDisplayTutorialsDuringCutscenes(false)
        delay = 0
        pCVar5 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pCVar5, delay)
        ::FLOW_past_lab_00ee9cd6::
    end
    ::LAB_00ee9ea5::
    ::FLOW_past_lab_00ee9adf::
    ::LAB_00ee9ec0::
    ::FLOW_past_lab_00ee98de::
    resources:RetireThingOutput(fishingSpotOwner); fishingSpotOwner = 0; resources:DestroyMovie(xStack_10)
    ::LAB_00ee9ee4::
    resources:ReleaseResource(xStack_20)
    end)
end

function Init(quest)
    quest:SetStateInt("LastFishingLevel", 1)
end

function OnPersist(quest, context)
    local lastFishingLevel = quest:GetStateInt("LastFishingLevel") or 0
    lastFishingLevel = quest:PersistTransferInt(context, "LastFishingLevel", lastFishingLevel)
    quest:SetStateInt("LastFishingLevel", lastFishingLevel)
end

