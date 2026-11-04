-- Readable native conversion: Global_OpenChest. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_DEFAULT = 0  -- ECutsceneBehaviour (Ego_r.pdb)
local CUTSCENE_BEHAVIOUR_NOT_PAUSED = 2  -- ECutsceneBehaviour (Ego_r.pdb)
-- Global_OpenChest.Main (retail 0x00eec890)
function Main(quest)
    return quest:WithRetailResources(function(resources)
        local chestTargetOwner
        local controlAcquired, msgOnChestOpeningCancelled, scratchValue, native_keyQueryResult
        local hero6
        local getMostRecentValidUsedTarget, scratchValue9
        local hero = quest:GetHero()
        quest:RegisterQuestConsciousCondition(hero)
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
        local getActiveQuestName = quest:GetActiveQuestName()
        quest:SetQuestAsPersistent(getActiveQuestName, false)
        if not quest:IsHeroControlledByPlayer() then goto LAB_00eece68 end
        if quest:IsActiveThreadTerminating() then return end
        getMostRecentValidUsedTarget = quest:GetMostRecentValidUsedTarget()
        chestTargetOwner = resources:TrackThingOutput(getMostRecentValidUsedTarget)
        if (getMostRecentValidUsedTarget ~= nil and not getMostRecentValidUsedTarget:IsNull()) and (getMostRecentValidUsedTarget ~= nil and getMostRecentValidUsedTarget:IsAlive()) then
            if quest:IsActiveThreadTerminating() then goto LAB_00eec9c9 end
            native_keyQueryResult = quest:GetNumberOfKeysNeededToUnlockChest(getMostRecentValidUsedTarget)
            scratchValue = native_keyQueryResult.count
            scratchValue9 = native_keyQueryResult.keyName or ""
            if scratchValue < 1 then
                goto LAB_00eec9da
            elseif not quest:IsActiveThreadTerminating() then
                if scratchValue <= quest:GetNumberOfItemsOfTypeInInventory(scratchValue9) then goto LAB_00eec9da end
                if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then goto LAB_00eec9c0 end
                quest:PlayCriteriaSoundOnThing(getMostRecentValidUsedTarget, "CHEST_OPEN_FAIL")
                quest:DisplayLockedChestMessage(getMostRecentValidUsedTarget)
                goto LAB_00eece56
            end
            goto FLOW_past_lab_00eec9da
            ::LAB_00eec9da::
            if not quest:IsActiveThreadTerminating() then
                local resource = resources:NewResource()
                local hero2 = quest:GetHero()
                controlAcquired = resources:TryAcquire(resource, hero2, 4)
                while not controlAcquired do
                    quest:NewScriptFrame()
                    if quest:IsActiveThreadTerminating() then goto LAB_00eecdb1 end
                    local hero3 = quest:GetHero()
                    controlAcquired = resources:TryAcquire(resource, hero3, 4)
                end
                if not quest:IsActiveThreadTerminating() then
                    local hero4 = quest:GetHero()
                    local isEntityWieldingMeleeWeapon = quest:IsEntityWieldingMeleeWeapon(hero4)
                    local hero5 = quest:GetHero()
                    local isEntityWieldingRangedWeapon = quest:IsEntityWieldingRangedWeapon(hero5)
                    quest:SetToKeepHeroAbilitiesDuringCutscenes(true)
                    quest:SetToDisplayTutorialsDuringCutscenes(true)
                    resources:ClearAllActionsIncludingLoopingAnimations(resource)
                    local movie = resources:StartMovie("")
                    quest:SetCutsceneMode(true, false)
                    quest:EntitySetCutsceneBehaviour(getMostRecentValidUsedTarget, CUTSCENE_BEHAVIOUR_NOT_PAUSED)
                    quest:PauseAllEntities(true)
                    local getContainerItemDefCount = quest:GetContainerItemDefCount(getMostRecentValidUsedTarget) == 0
                    msgOnChestOpeningCancelled = true
                    if quest:OpenChest(getMostRecentValidUsedTarget, true) then
                        if not quest:IsActiveThreadTerminating() then
                            msgOnChestOpeningCancelled = quest:MsgOnChestOpeningCancelled()
                            while not quest:IsChestOpen(getMostRecentValidUsedTarget) and msgOnChestOpeningCancelled == false do
                                quest:NewScriptFrame()
                                if quest:IsActiveThreadTerminating() then goto LAB_00eecd9f end
                                msgOnChestOpeningCancelled = quest:MsgOnChestOpeningCancelled()
                                if msgOnChestOpeningCancelled then goto continue_1 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00eecd9f end
                                hero6 = quest:GetHero()
                                if hero6 ~= nil and hero6:MsgIsHitBy("") then
                                    goto LAB_00eecc47
                                else
                                    local hero7 = quest:GetHero()
                                    if hero7 ~= nil and hero7:MsgIsHitByAnySpecialAbilityFrom("") then goto LAB_00eecc47 end
                                end
                                goto FLOW_past_lab_00eecc47
                                ::LAB_00eecc47::
                                msgOnChestOpeningCancelled = true
                                ::FLOW_past_lab_00eecc47::
                                ::continue_1::
                            end
                            if not quest:IsActiveThreadTerminating() then goto LAB_00eecca6 end
                        end
                    else
                        goto LAB_00eecca6
                    end
                    goto FLOW_past_lab_00eecca6
                    ::LAB_00eecca6::
                    quest:PauseAllEntities(false)
                    quest:EntitySetCutsceneBehaviour(getMostRecentValidUsedTarget, CUTSCENE_BEHAVIOUR_DEFAULT)
                    quest:SetCutsceneMode(false, true)
                    quest:CameraDefault()
                    if getContainerItemDefCount or msgOnChestOpeningCancelled ~= false then
                        goto LAB_00eecd4f
                    end
                    goto FLOW_past_lab_00eecd4f
                    ::LAB_00eecd4f::
                    quest:SetToKeepHeroAbilitiesDuringCutscenes(false)
                    quest:SetToDisplayTutorialsDuringCutscenes(false)
                    if isEntityWieldingMeleeWeapon then
                        if quest:IsActiveThreadTerminating() then goto LAB_00eecd9f end
                        local hero8 = quest:GetHero()
                        quest:EntityUnsheatheMeleeWeapon(hero8, false)
                    elseif isEntityWieldingRangedWeapon then
                        if quest:IsActiveThreadTerminating() then goto LAB_00eecd9f end
                        local hero9 = quest:GetHero()
                        quest:EntityUnsheatheRangedWeapon(hero9, false)
                    end
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(resource)
                    goto LAB_00eece56
                    ::FLOW_past_lab_00eecd4f::
                    if not quest:IsActiveThreadTerminating() then
                        while not quest:MsgOnHeroRewardedWithItemsFrom() do
                            quest:NewScriptFrame()
                            if quest:IsActiveThreadTerminating() then goto LAB_00eecd9a end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00eecd4f end
                        ::LAB_00eecd9a::
                    end
                    ::FLOW_past_lab_00eecca6::
                    ::LAB_00eecd9f::
                    resources:DestroyMovie(movie)
                end
                ::LAB_00eecdb1::
                resources:ReleaseResource(resource)
            end
            ::FLOW_past_lab_00eec9da::
            goto FLOW_past_lab_00eece56
            ::LAB_00eece56::
            goto LAB_00eece5f
            ::FLOW_past_lab_00eece56::
            ::LAB_00eec9c0::
            ::LAB_00eec9c9::
            resources:RetireThingOutput(chestTargetOwner); chestTargetOwner = 0
            return
        end
        ::LAB_00eece5f::
        resources:RetireThingOutput(chestTargetOwner); chestTargetOwner = 0
        ::LAB_00eece68::
        local getActiveQuestName2 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(getActiveQuestName2, 0)
    end)
end

-- Global_OpenChest.Init (retail 0x00eec7c0)
function Init(quest)
end

-- Global_OpenChest.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

