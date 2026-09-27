-- Readable native conversion: FireHeartHolder. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- FireHeartHolder.Main (retail 0x00df2d30)
function Main(quest, me)
    local resources = quest:RetailResources()
    local lighthouseStarted = quest:GetStateBool("LighthouseStarted")
    while true do
        if lighthouseStarted then
            while not quest:IsActiveThreadTerminating() do
                quest:NewScriptFrame(me)
            end
            return
        end
        if not quest:NewScriptFrame(me) then break end
        if not me:MsgIsUsedByHero() then
            lighthouseStarted = quest:GetStateBool("LighthouseStarted")
        else
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:SetStateBool("LighthouseStarted", true)
            quest:TakeObjectFromHero("OBJECT_LPDD_FIREHEART_01")
            quest:MiniMapRemoveMarker(me)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            lighthouseStarted = quest:GetStateBool("LighthouseStarted")
        end
    end
end

-- FireHeartHolder.Init (retail 0x00df2cd0)
function Init(quest, me)
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
end

-- FireHeartHolder.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- FireHeartHolder.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

