-- Readable native conversion: Expression_Dig. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Expression_Dig.Main (retail 0x00eea7c0)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, isEntityWieldingMeleeWeapon, isEntityWieldingRangedWeapon, duration
    local scratchValue, getNearestEnabledDiggingSpot, movie
    if not quest:NewScriptFrame() then return end
    quest:SetQuestAsPersistent(quest:GetActiveQuestName(), false)
    while not quest:IsHeroControlledByPlayer() do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local resource = resources:NewResource()
    scratchValue = 4
    while not resources:TryAcquire(resource, hero, scratchValue) do
        if not quest:NewScriptFrame() then goto LAB_00eead92 end
        scratchValue = 4
    end
    quest:SetCutsceneMode(true, false)
    movie = resources:StartMovie("")
    quest:PauseAllEntities(true)
    quest:FadeScreenOut(0.25, 0.25)
    quest:Pause(0.5)
    quest:EntityForceToLookAtNothing(hero)
    hero:GetPos()
    hero:GetAngleXY()
    hero:GetPos()
    isEntityWieldingMeleeWeapon = quest:IsEntityWieldingMeleeWeapon(hero)
    isEntityWieldingRangedWeapon = quest:IsEntityWieldingRangedWeapon(hero)
    quest:SheatheHeroWeapons()
    -- TODO(native): ClearAllActionsIncludingLoopingAnimations: unresolved entity receiver/resource in quest context; arguments: 
    quest:MakeHeroCarryItemInHand(nil --[[missing]], scratchValue ~= 0, nil --[[missing]])
    quest:FadeScreenIn()
    quest:Pause(0.5)
    -- TODO(native): PlayLoopingAnimation: unresolved entity receiver/resource in quest context; arguments: 2,0,0,0,1,true,0,0
    getNearestEnabledDiggingSpot = quest:GetNearestEnabledDiggingSpot(hero)
    if not (getNearestEnabledDiggingSpot ~= nil and getNearestEnabledDiggingSpot:IsAlive()) then
        goto LAB_00eeaba2
    else
        if not Is2DDistanceBetweenThingsUnder(hero,getNearestEnabledDiggingSpot,3.0) then goto LAB_00eeaba2 end
        predicateResult = true
        if not quest:IsActiveThreadTerminating() then quest:EntitySetFacingAngleTowardsThing(hero, getNearestEnabledDiggingSpot, true); goto LAB_00eeab2b end
    end
    goto FLOW_past_lab_00eeaba2
    ::LAB_00eeaba2::
    predicateResult = false
    ::LAB_00eeab2b::
    quest:HeroGoDigging()
    quest:Pause(0.25)
    if predicateResult then
        if not quest:IsActiveThreadTerminating() then
            -- TODO(native): ClearCommands: unresolved entity receiver/resource in quest context; arguments: 
            -- TODO(native): PlayAnimation: unresolved entity receiver/resource in quest context; arguments: 0,0,0,1,true,0,0
            duration = 0.55
            goto LAB_00eeabfa
        end
    elseif not quest:IsActiveThreadTerminating() then
        -- TODO(native): ClearCommands: unresolved entity receiver/resource in quest context; arguments: 
        -- TODO(native): PlayAnimation: unresolved entity receiver/resource in quest context; arguments: 0,0,0,1,true,0,0
        duration = 1.5
        goto LAB_00eeabfa
    end
    goto FLOW_past_lab_00eeabfa
    ::LAB_00eeabfa::
    quest:Pause(duration)
    quest:Pause(2.0)
    quest:HeroStopDigging()
    quest:FadeScreenOut(0.25, 0.25)
    quest:Pause(0.25)
    quest:MakeHeroCarryItemInHand(getNearestEnabledDiggingSpot, predicateResult, nil --[[missing]])
    if isEntityWieldingMeleeWeapon then
        if quest:IsActiveThreadTerminating() then goto LAB_00eead80 end
        quest:EntityUnsheatheMeleeWeapon(hero, false)
    elseif isEntityWieldingRangedWeapon then
        if quest:IsActiveThreadTerminating() then goto LAB_00eead80 end
        quest:EntityUnsheatheRangedWeapon(hero, false)
    end
    quest:EntityResetForceToLookAt(hero)
    quest:Pause(0.25)
    quest:FadeScreenIn()
    quest:SetCutsceneMode(false, true)
    quest:CameraDefault()
    if predicateResult then
        if quest:IsActiveThreadTerminating() then goto LAB_00eead80 end
        quest:GiveHeroItemsFromContainer(getNearestEnabledDiggingSpot, false)
    end
    quest:PauseAllEntities(false)
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    ::FLOW_past_lab_00eeabfa::
    ::FLOW_past_lab_00eeaba2::
    ::LAB_00eead80::
    resources:DestroyMovie(movie)
    ::LAB_00eead92::
    resources:ReleaseResource(resource)
end

-- Expression_Dig.Init (retail 0x00eea7b0)
function Init(quest)
end

-- Expression_Dig.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

