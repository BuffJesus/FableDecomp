-- Readable native conversion: Global_OpenChest. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_DEFAULT = 0  -- ECutsceneBehaviour (Ego_r.pdb)
local CUTSCENE_BEHAVIOUR_NOT_PAUSED = 2  -- ECutsceneBehaviour (Ego_r.pdb)

-- Global_OpenChest.Main (retail 0x00eec890)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local msgOnChestOpeningCancelled, getNumberOfKeysNeededToUnlockChest, scratchValue7
    local getMostRecentValidUsedTarget
    if not quest:NewScriptFrame() then return end
    quest:SetQuestAsPersistent(quest:GetActiveQuestName(), false)
    if not quest:IsHeroControlledByPlayer() then goto LAB_00eece68 end
    if quest:IsActiveThreadTerminating() then return end
    getMostRecentValidUsedTarget = quest:GetMostRecentValidUsedTarget()
    if not ((getMostRecentValidUsedTarget ~= nil) and (getMostRecentValidUsedTarget ~= nil and getMostRecentValidUsedTarget:IsAlive())) then goto LAB_00eece5f end
    getNumberOfKeysNeededToUnlockChest = quest:GetNumberOfKeysNeededToUnlockChest(getMostRecentValidUsedTarget)
    if getNumberOfKeysNeededToUnlockChest < 1 then
        goto LAB_00eec9da
    elseif not quest:IsActiveThreadTerminating() then
        if getNumberOfKeysNeededToUnlockChest <= quest:GetNumberOfItemsOfTypeInInventory("") then goto LAB_00eec9da end
        quest:PlayCriteriaSoundOnThing(getMostRecentValidUsedTarget, "CHEST_OPEN_FAIL")
        quest:DisplayLockedChestMessage(getMostRecentValidUsedTarget)
        goto LAB_00eece56
    end
    goto FLOW_past_lab_00eec9da
    ::LAB_00eec9da::
    if not quest:IsActiveThreadTerminating() then
        if not hero:AcquireControl(4) then goto LAB_00eecdb1 end
        if not quest:IsActiveThreadTerminating() then
            local isEntityWieldingMeleeWeapon = quest:IsEntityWieldingMeleeWeapon(hero)
            local isEntityWieldingRangedWeapon = quest:IsEntityWieldingRangedWeapon(hero)
            quest:SetToKeepHeroAbilitiesDuringCutscenes(true)
            quest:SetToDisplayTutorialsDuringCutscenes(true)
            -- TODO(native): ClearAllActionsIncludingLoopingAnimations: unresolved entity receiver/resource in quest context; arguments: 
            local movie = resources:StartMovie("")
            quest:SetCutsceneMode(true, false)
            quest:EntitySetCutsceneBehaviour(getMostRecentValidUsedTarget, CUTSCENE_BEHAVIOUR_NOT_PAUSED)
            quest:PauseAllEntities(true)
            local getItemDefNamesFromContainer = quest:GetItemDefNamesFromContainer(getMostRecentValidUsedTarget) == scratchValue7
            msgOnChestOpeningCancelled = true
            if quest:OpenChest(getMostRecentValidUsedTarget, true) then
                if not quest:IsActiveThreadTerminating() then
                    msgOnChestOpeningCancelled = quest:MsgOnChestOpeningCancelled()
                    while not quest:IsChestOpen(getMostRecentValidUsedTarget) and msgOnChestOpeningCancelled == false do
                        if not quest:NewScriptFrame() then goto LAB_00eecd9f end
                        msgOnChestOpeningCancelled = quest:MsgOnChestOpeningCancelled()
                        if msgOnChestOpeningCancelled then goto continue_1 end
                        if hero ~= nil and hero:MsgIsHitBy("") then
                            goto LAB_00eecc47
                        else
                            if hero ~= nil and hero:MsgIsHitByAnySpecialAbilityFrom("") then goto LAB_00eecc47 end
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
            if getItemDefNamesFromContainer or msgOnChestOpeningCancelled ~= false then
                goto LAB_00eecd4f
            end
            goto FLOW_past_lab_00eecd4f
            ::LAB_00eecd4f::
            quest:SetToKeepHeroAbilitiesDuringCutscenes(false)
            quest:SetToDisplayTutorialsDuringCutscenes(false)
            if isEntityWieldingMeleeWeapon then
                if quest:IsActiveThreadTerminating() then goto LAB_00eecd9f end
                quest:EntityUnsheatheMeleeWeapon(hero, false)
            elseif isEntityWieldingRangedWeapon then
                if quest:IsActiveThreadTerminating() then goto LAB_00eecd9f end
                quest:EntityUnsheatheRangedWeapon(hero, false)
            end
            -- TODO(native): CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)&iStack_2c);
            resources:DestroyMovie(movie)
            hero:ReleaseControl()
            goto LAB_00eece56
            ::FLOW_past_lab_00eecd4f::
            if not quest:IsActiveThreadTerminating() then
                while not quest:MsgOnHeroRewardedWithItemsFrom() do
                    if not quest:NewScriptFrame() then goto LAB_00eecd9a end
                end
                if not quest:IsActiveThreadTerminating() then goto LAB_00eecd4f end
                ::LAB_00eecd9a::
            end
            ::FLOW_past_lab_00eecca6::
            ::LAB_00eecd9f::
            -- TODO(native): CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)&iStack_2c);
            resources:DestroyMovie(movie)
        end
        ::LAB_00eecdb1::
        hero:ReleaseControl()
    end
    ::FLOW_past_lab_00eec9da::
    goto FLOW_past_lab_00eece56
    ::LAB_00eece56::
    goto LAB_00eece5f
    ::FLOW_past_lab_00eece56::
    ::LAB_00eec9c0::
    ::LAB_00eec9c9::
    do return end
    ::LAB_00eece5f::
    ::LAB_00eece68::
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Global_OpenChest.Init (retail 0x00eec7c0)
function Init(quest)
end

-- Global_OpenChest.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

