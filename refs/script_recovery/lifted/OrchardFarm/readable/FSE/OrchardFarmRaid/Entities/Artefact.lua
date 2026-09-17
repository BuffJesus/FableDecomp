-- Readable native conversion: Artefact. Review coverage report before use.
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
    local scratchValue2 = quest:GetThingWithScriptName("BanditTeamCrateDrop")
    repeat
        if quest:IsActiveThreadTerminating() then return end
        if quest:IsDistanceBetweenThingsUnder(me, scratchValue2, 3.0) then
            if not me:IsBeingCarriedBy("SCRIPT_NAME_HERO") then
                quest:SetStateInt("CrateCount", quest:GetStateInt("CrateCount") - 1)
                quest:SetMasterGameState("OFBRCratesStolen", true)
                quest:RemoveThing(me, false, true)
                return
            end
        end
        quest:NewScriptFrame(me)
    until false
end

function Init(quest, me)
    quest:SetStateInt("CrateCount", quest:GetStateInt("CrateCount") + 1)
    quest:StateListPush("CrateList", me)
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
    local p0 = 0
    if 0 ~= quest:GetStateListCount("CrateList") then
        while true do
            if quest:GetStateListAt("CrateList", p0):IsEqualTo(me) then break end
            p0 = p0 + 1
            if p0 == quest:GetStateListCount("CrateList") then
                return
            end
        end
        quest:StateListErase("CrateList", p0)
    end
end

