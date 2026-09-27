-- Readable native conversion: CampHostageDoor. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    EXPERIENCE_GRANT_LARGE = 68,  -- 100
}

-- CampHostageDoor.Main (retail 0x00d08b40)
function Main(quest, me)
    local hero = quest:GetHero()
    if not quest:NewScriptFrame(me) then return end
    local campHostageGuard = quest:GetThingWithScriptName("CampHostageGuard")
    repeat
        if quest:IsActiveThreadTerminating() then return end
        if quest:GetStateBool("HostagesRescued") or not me:MsgIsUsedByHero() then
            quest:NewScriptFrame(me)
        else
            if quest:IsObjectInThingsPossession("OBJECT_BANDIT_CAMP_HOSTAGE_KEY", hero) then
                if ((campHostageGuard ~= nil and not campHostageGuard:IsNull()) and (campHostageGuard ~= nil and campHostageGuard:IsAlive())) and quest:IsDistanceBetweenThingsUnder(hero, campHostageGuard, 5.0) then
                    quest:SetStateBool("PlayGuardTooCloseCutscene", true)
                    while quest:GetStateBool("PlayGuardTooCloseCutscene") do
                        if not quest:NewScriptFrame(me) then return end
                    end
                else
                    quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.EXPERIENCE_GRANT_LARGE))
                    quest:SetStateBool("HostagesRescued", true)
                    quest:TakeObjectFromHero("OBJECT_BANDIT_CAMP_HOSTAGE_KEY")
                    return
                end
            else
                quest:DisplayGameInfo("TEXT_QST_009_NEED_KEY")
                while not quest:MsgIsGameInfoClickedPast() do
                    if not quest:NewScriptFrame(me) then return end
                end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:NewScriptFrame(me)
        end
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

