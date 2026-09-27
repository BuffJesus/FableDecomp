-- Readable native conversion: Expression_Dig. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Expression_Dig.Main (retail 0x00eea7c0)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, duration
    if not quest:NewScriptFrame() then return end
    quest:SetQuestAsPersistent(quest:GetActiveQuestName(), false)
    while not quest:IsHeroControlledByPlayer() do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local resource = resources:NewResource()
    while not resources:TryAcquire(resource, hero, 4) do
        if not quest:NewScriptFrame() then resources:ReleaseResource(resource); return end
    end
    quest:SetCutsceneMode(true, false)
    local movie = resources:StartMovie("")
    quest:PauseAllEntities(true)
    quest:FadeScreenOut(0.25, 0.25)
    quest:Pause(0.5)
    quest:EntityForceToLookAtNothing(hero)
    hero:GetPos()
    hero:GetAngleXY()
    hero:GetPos()
    local isEntityWieldingMeleeWeapon = quest:IsEntityWieldingMeleeWeapon(hero)
    local isEntityWieldingRangedWeapon = quest:IsEntityWieldingRangedWeapon(hero)
    quest:SheatheHeroWeapons()
    resources:ClearAllActionsIncludingLoopingAnimations(resource)
    quest:MakeHeroCarryItemInHand("OBJECT_SPADE")
    quest:FadeScreenIn()
    quest:Pause(0.5)
    resources:PlayLoopingAnimation(resource, "ST_DIGGING_IDLE", 2, false, false, false, true, true, false, false)
    local getNearestEnabledDiggingSpot = quest:GetNearestEnabledDiggingSpot(hero)
    if not (getNearestEnabledDiggingSpot ~= nil and getNearestEnabledDiggingSpot:IsAlive()) then
        goto LAB_00eeaba2
    else
        if not Is2DDistanceBetweenThingsUnder(hero,getNearestEnabledDiggingSpot,3.0) then goto LAB_00eeaba2 end
        predicateResult = true
        if not quest:IsActiveThreadTerminating() then quest:EntitySetFacingAngleTowardsThing(hero, getNearestEnabledDiggingSpot, true); goto LAB_00eeab2b end
    end
    resources:DestroyMovie(movie)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00eeaba2::
    predicateResult = false
    ::LAB_00eeab2b::
    quest:HeroGoDigging()
    quest:Pause(0.25)
    if predicateResult then
        if not quest:IsActiveThreadTerminating() then
            resources:ClearCommands(resource)
            resources:PlayAnimation(resource, "ST_DIGGING_SUCCESS", false, false, false, true, true, false, false)
            duration = 0.55
            goto LAB_00eeabfa
        end
    elseif not quest:IsActiveThreadTerminating() then
        resources:ClearCommands(resource)
        resources:PlayAnimation(resource, "ST_DIGGING_FAILURE", false, false, false, true, true, false, false)
        duration = 1.5
        goto LAB_00eeabfa
    end
    resources:DestroyMovie(movie)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00eeabfa::
    quest:Pause(duration)
    quest:Pause(2.0)
    quest:HeroStopDigging()
    quest:FadeScreenOut(0.25, 0.25)
    quest:Pause(0.25)
    quest:MakeHeroCarryItemInHand("")
    if isEntityWieldingMeleeWeapon then
        if quest:IsActiveThreadTerminating() then resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
        quest:EntityUnsheatheMeleeWeapon(hero, false)
    elseif isEntityWieldingRangedWeapon then
        if quest:IsActiveThreadTerminating() then resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
        quest:EntityUnsheatheRangedWeapon(hero, false)
    end
    quest:EntityResetForceToLookAt(hero)
    quest:Pause(0.25)
    quest:FadeScreenIn()
    quest:SetCutsceneMode(false, true)
    quest:CameraDefault()
    if predicateResult then
        if quest:IsActiveThreadTerminating() then resources:DestroyMovie(movie); resources:ReleaseResource(resource); return end
        quest:GiveHeroItemsFromContainer(getNearestEnabledDiggingSpot, false)
    end
    quest:PauseAllEntities(false)
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    resources:DestroyMovie(movie)
    resources:ReleaseResource(resource)
end

-- Expression_Dig.Init (retail 0x00eea7b0)
function Init(quest)
end

-- Expression_Dig.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

