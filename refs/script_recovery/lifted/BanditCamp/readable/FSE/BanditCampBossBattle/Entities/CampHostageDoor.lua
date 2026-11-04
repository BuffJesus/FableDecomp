-- Readable native conversion: CampHostageDoor. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    EXPERIENCE_GRANT_LARGE = 68,  -- 100
}

-- CampHostageDoor.Main (retail 0x00d08b40)
function Main(quest, me)
    local predicateResult, predicateResult2, isObjectInThingsPossession, predicateResult3, dist
    local hero, campHostageGuard
    if not quest:NewScriptFrame(me) then return end
    campHostageGuard = quest:GetThingWithScriptName("CampHostageGuard")
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            campHostageGuard = nil
            return
        end
        if not (quest:GetStateBool("HostagesRescued")) then
            if not me:MsgIsUsedByHero() then goto LAB_00d08c0c end
            predicateResult2 = true
            goto FLOW_past_lab_00d08c0c
        end
        ::LAB_00d08c0c::
        predicateResult2 = false
        ::FLOW_past_lab_00d08c0c::
        if not predicateResult2 then
            quest:NewScriptFrame(me);
            predicateResult = quest:IsActiveThreadTerminating();
            goto continue_1
        end
        if quest:IsActiveThreadTerminating() then return end
        hero = quest:GetHero()
        isObjectInThingsPossession = quest:IsObjectInThingsPossession("OBJECT_BANDIT_CAMP_HOSTAGE_KEY", hero)
        predicateResult3 = quest:IsActiveThreadTerminating()
        if isObjectInThingsPossession then
            if predicateResult3 then return end
            local scratchValue = (campHostageGuard ~= nil and not campHostageGuard:IsNull()) and (campHostageGuard ~= nil and campHostageGuard:IsAlive())
            if scratchValue then
                dist = 5.0
                local hero2 = quest:GetHero()
                if quest:IsDistanceBetweenThingsUnder(hero2, campHostageGuard, 5.0) then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:SetStateBool("PlayGuardTooCloseCutscene", true)
                    while quest:GetStateBool("PlayGuardTooCloseCutscene") do
                        if not quest:NewScriptFrame(me) then return end
                    end
                    goto LAB_00d08d7c
                end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_LARGE))
            quest:SetStateBool("HostagesRescued", true)
            quest:TakeObjectFromHero("OBJECT_BANDIT_CAMP_HOSTAGE_KEY")
            return
        end
        if predicateResult3 then
            return
        end
        quest:DisplayGameInfo("TEXT_QST_009_NEED_KEY")
        while not quest:MsgIsGameInfoClickedPast() do
            if not quest:NewScriptFrame(me) then return end
        end
        ::LAB_00d08d7c::
        if quest:IsActiveThreadTerminating() then return end
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
        ::continue_1::
    until false
end

-- CampHostageDoor.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- CampHostageDoor.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- CampHostageDoor.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

