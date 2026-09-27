-- Generated native draft: FireHeartHolder. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local resources = quest:RetailResources()
    local bVar1, xStack_10
    local alive = true
    local cVar2 = quest:GetStateBool("LighthouseStarted")
    while true do
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            while not bVar1 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
            end
            return
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then break end
        cVar2 = me:MsgIsUsedByHero()
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            xStack_10 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:SetStateBool("LighthouseStarted", true)
            quest:TakeObjectFromHero("OBJECT_LPDD_FIREHEART_01")
            quest:MiniMapRemoveMarker(me)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_10)
        end
        cVar2 = quest:GetStateBool("LighthouseStarted")
    end
end

function Init(quest, me)
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

