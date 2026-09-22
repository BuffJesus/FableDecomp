-- Readable native conversion: GratefulVillagerSpawn. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- GratefulVillagerSpawn.Main (retail 0x00e10a90)
function Main(quest, me)
    local panickedVillagersScene = quest:GetStateBool("PanickedVillagersScene")
    while true do
        if panickedVillagersScene then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetCategoryActivity("Post waspboss killed", true)
            return
        end
        if not quest:NewScriptFrame(me) then break end
        panickedVillagersScene = quest:GetStateBool("PanickedVillagersScene")
    end
end

-- GratefulVillagerSpawn.Init (retail 0x00e10a80)
function Init(quest, me)
end

-- GratefulVillagerSpawn.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- GratefulVillagerSpawn.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

