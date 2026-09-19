-- Readable native conversion: CampHostageDoor. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CampHostageDoor.Main (retail 0x00dfc050)
function Main(quest, me)
    quest:CloseDoor(me)
    if quest:IsActiveThreadTerminating() then return end
    while true do
        if me:MsgIsUsedByHero() then break end
        if not quest:NewScriptFrame(me) then return end
    end
    quest:OpenDoor(me)
    quest:SetStateBool("OpenedCage", true)
end

-- CampHostageDoor.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- CampHostageDoor.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- CampHostageDoor.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

