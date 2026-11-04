-- Readable native conversion: Expression_Dig. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Expression_Dig.Main (retail 0x00eea7c0)
function Main(quest)
    return quest:WithRetailResources(function(resources)
        local predicateResult, controlAcquired, duration, hero14, getActiveQuestName2
        local hero = quest:GetHero()
        quest:RegisterQuestConsciousCondition(hero)
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
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
        quest:SetCutsceneMode(true, false)
        local movie = resources:StartMovie("")
        quest:PauseAllEntities(true)
        quest:FadeScreenOut(0.25, 0.25)
        quest:Pause(0.5)
        local hero4 = quest:GetHero()
        quest:EntityForceToLookAtNothing(hero4)
        quest:GetHero():GetPos()
        quest:GetHero():GetAngleXY()
        quest:GetHero():GetPos()
        local hero8 = quest:GetHero()
        local isEntityWieldingMeleeWeapon = quest:IsEntityWieldingMeleeWeapon(hero8)
        local hero9 = quest:GetHero()
        local isEntityWieldingRangedWeapon = quest:IsEntityWieldingRangedWeapon(hero9)
        quest:SheatheHeroWeapons()
        resources:ClearAllActionsIncludingLoopingAnimations(resource)
        quest:MakeHeroCarryItemInHand("OBJECT_SPADE")
        quest:FadeScreenIn()
        quest:Pause(0.5)
        resources:PlayLoopingAnimation(resource, "ST_DIGGING_IDLE", 2, false, false, false, true, true, false, false)
        local hero10 = quest:GetHero()
        local getNearestEnabledDiggingSpot = quest:GetNearestEnabledDiggingSpot(hero10)
        local digSpotOwner = resources:TrackThingOutput(getNearestEnabledDiggingSpot)
        if not (getNearestEnabledDiggingSpot ~= nil and getNearestEnabledDiggingSpot:IsAlive()) then
            goto LAB_00eeaba2
        else
            local hero11 = quest:GetHero()
            if not resources:IsHorizontalDistanceBetweenThingsUnder(hero11, getNearestEnabledDiggingSpot, 3.0) then goto LAB_00eeaba2 end
            predicateResult = true
            if not quest:IsActiveThreadTerminating() then
                local pThing = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(pThing, getNearestEnabledDiggingSpot, true)
                goto LAB_00eeab2b
            end
        end
        goto FLOW_past_lab_00eeaba2
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
        goto FLOW_past_lab_00eeabfa
        ::LAB_00eeabfa::
        quest:Pause(duration)
        quest:Pause(2.0)
        quest:HeroStopDigging()
        quest:FadeScreenOut(0.25, 0.25)
        quest:Pause(0.25)
        quest:MakeHeroCarryItemInHand("")
        if isEntityWieldingMeleeWeapon then
            if quest:IsActiveThreadTerminating() then goto LAB_00eead80 end
            local hero12 = quest:GetHero()
            quest:EntityUnsheatheMeleeWeapon(hero12, false)
        elseif isEntityWieldingRangedWeapon then
            if quest:IsActiveThreadTerminating() then goto LAB_00eead80 end
            local hero13 = quest:GetHero()
            quest:EntityUnsheatheRangedWeapon(hero13, false)
        end
        hero14 = quest:GetHero()
        quest:EntityResetForceToLookAt(hero14)
        quest:Pause(0.25)
        quest:FadeScreenIn()
        quest:SetCutsceneMode(false, true)
        quest:CameraDefault()
        if predicateResult then
            if quest:IsActiveThreadTerminating() then goto LAB_00eead80 end
            quest:GiveHeroItemsFromContainer(getNearestEnabledDiggingSpot, false)
        end
        quest:PauseAllEntities(false)
        getActiveQuestName2 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(getActiveQuestName2, 0)
        ::FLOW_past_lab_00eeabfa::
        ::FLOW_past_lab_00eeaba2::
        ::LAB_00eead80::
        resources:RetireThingOutput(digSpotOwner); digSpotOwner = 0
        resources:DestroyMovie(movie)
        resources:ReleaseResource(resource)
    end)
end

-- Expression_Dig.Init (retail 0x00eea7b0)
function Init(quest)
end

-- Expression_Dig.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

