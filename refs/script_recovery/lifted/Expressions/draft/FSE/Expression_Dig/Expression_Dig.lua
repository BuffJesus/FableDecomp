-- Generated native draft: Expression_Dig. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local bVar2, bVar3, bVar4, bVar5, delay, duration, iVar10, iVar8, pCVar6, pCVar7, pThing, pppuVar9, r1, r2, xStack_20, xStack_30
    local alive = true
    pCVar6 = quest:GetHero()
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    bVar3 = false
    pCVar7 = quest:GetActiveQuestName()
    quest:SetQuestAsPersistent(pCVar7, bVar3)
    bVar3 = quest:IsHeroControlledByPlayer()
    while not bVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        bVar3 = quest:IsHeroControlledByPlayer()
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    xStack_30 = resources:NewResource()
    iVar10 = 4
    pppuVar9 = xStack_30
    pCVar6 = quest:GetHero()
    bVar3 = resources:TryAcquire(pppuVar9, pCVar6, iVar10)
    while not bVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00eead92 end
        iVar10 = 4
        pppuVar9 = xStack_30
        pCVar6 = quest:GetHero()
        bVar3 = resources:TryAcquire(pppuVar9, pCVar6, iVar10)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00eead92 end
    quest:SetCutsceneMode(true, false)
    xStack_20 = resources:StartMovie("")
    quest:PauseAllEntities(true)
    quest:FadeScreenOut(0.25, 0.25)
    quest:Pause(0.5)
    pCVar6 = quest:GetHero()
    quest:EntityForceToLookAtNothing(pCVar6)
    pCVar6 = quest:GetHero()
    pCVar6:GetPos()
    pCVar6 = quest:GetHero()
    pCVar6:GetAngleXY()
    pCVar6 = quest:GetHero()
    pCVar6:GetPos()
    pCVar6 = quest:GetHero()
    bVar3 = quest:IsEntityWieldingMeleeWeapon(pCVar6)
    pCVar6 = quest:GetHero()
    bVar4 = quest:IsEntityWieldingRangedWeapon(pCVar6)
    quest:SheatheHeroWeapons()
    -- TODO(native): ClearAllActionsIncludingLoopingAnimations: unresolved entity receiver/resource in quest context; arguments: 
    quest:MakeHeroCarryItemInHand(nil --[[missing]], (iVar10 ~= 0), nil --[[missing]])
    quest:FadeScreenIn()
    quest:Pause(0.5)
    -- TODO(native): PlayLoopingAnimation: unresolved entity receiver/resource in quest context; arguments: 2,0,0,0,1,true,0,0
    pCVar6 = quest:GetHero()
    r1 = quest:GetNearestEnabledDiggingSpot(pCVar6)
    iVar8 = (r1 ~= nil and r1:IsAlive())
    if not iVar8 then
        goto LAB_00eeaba2
    else
        iVar8 = 3.0
        pCVar6 = quest:GetHero()
        iVar8 = Is2DDistanceBetweenThingsUnder(pCVar6,r1,iVar8)
        if not iVar8 then goto LAB_00eeaba2 end
        bVar2 = true
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            bVar5 = true
            pCVar6 = r1
            pThing = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(pThing, pCVar6, bVar5)
            goto LAB_00eeab2b
        end
    end
    goto FLOW_past_lab_00eeaba2
    ::LAB_00eeaba2::
    bVar2 = false
    ::LAB_00eeab2b::
    quest:HeroGoDigging()
    quest:Pause(0.25)
    if bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            -- TODO(native): ClearCommands: unresolved entity receiver/resource in quest context; arguments: 
            -- TODO(native): PlayAnimation: unresolved entity receiver/resource in quest context; arguments: 0,0,0,1,true,0,0
            duration = 0.55
            goto LAB_00eeabfa
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            -- TODO(native): ClearCommands: unresolved entity receiver/resource in quest context; arguments: 
            -- TODO(native): PlayAnimation: unresolved entity receiver/resource in quest context; arguments: 0,0,0,1,true,0,0
            duration = 1.5
            goto LAB_00eeabfa
        end
    end
    goto FLOW_past_lab_00eeabfa
    ::LAB_00eeabfa::
    quest:Pause(duration)
    quest:Pause(2.0)
    quest:HeroStopDigging()
    quest:FadeScreenOut(0.25, 0.25)
    quest:Pause(0.25)
    quest:MakeHeroCarryItemInHand(r1, bVar2, nil --[[missing]])
    if bVar3 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00eead80 end
        bVar3 = false
        pCVar6 = quest:GetHero()
        quest:EntityUnsheatheMeleeWeapon(pCVar6, bVar3)
    elseif bVar4 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00eead80 end
        bVar3 = false
        pCVar6 = quest:GetHero()
        quest:EntityUnsheatheRangedWeapon(pCVar6, bVar3)
    end
    pCVar6 = quest:GetHero()
    quest:EntityResetForceToLookAt(pCVar6)
    quest:Pause(0.25)
    quest:FadeScreenIn()
    quest:SetCutsceneMode(false, true)
    quest:CameraDefault()
    if bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00eead80 end
        r2 = quest:GiveHeroItemsFromContainer(r1, false)
    end
    quest:PauseAllEntities(false)
    delay = 0
    pCVar7 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(pCVar7, delay)
    ::FLOW_past_lab_00eeabfa::
    ::FLOW_past_lab_00eeaba2::
    ::LAB_00eead80::
    resources:DestroyMovie(xStack_20)
    ::LAB_00eead92::
    resources:ReleaseResource(xStack_30)
end

function Init(quest)
end

function OnPersist(quest, context)
end

