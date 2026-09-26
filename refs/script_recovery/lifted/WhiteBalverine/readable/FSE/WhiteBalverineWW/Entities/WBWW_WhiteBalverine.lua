-- Readable native conversion: WBWW_WhiteBalverine. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- WBWW_WhiteBalverine.Main (retail 0x00e191a0)
function Main(quest, me)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
end

-- WBWW_WhiteBalverine.Init (retail 0x00e19150)
function Init(quest, me)
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
end

-- WBWW_WhiteBalverine.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- WBWW_WhiteBalverine.OnPredicateFail (retail 0x00e19190)
function OnPredicateFail(quest, me)
end

