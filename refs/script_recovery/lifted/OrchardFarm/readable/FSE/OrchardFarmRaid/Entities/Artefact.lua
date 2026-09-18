-- Readable native conversion: Artefact. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- Artefact.Main (retail 0x00dcdc50)
function Main(quest, me)
    local predicateResult, isActiveThreadTerminating
    local function __cleanup_LAB_00dcddcf()
        banditTeamCrateDrop = nil
    end
    isActiveThreadTerminating = false
    local banditTeamCrateDrop = quest:GetThingWithScriptName("BanditTeamCrateDrop")
    local scratchValue = quest:IsActiveThreadTerminating()
    repeat
        if scratchValue then
            return
        end
        if quest:IsDistanceBetweenThingsUnder(me, banditTeamCrateDrop, 3.0) then
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dcddcf(); return end
            predicateResult = quest:IsActiveThreadTerminating()
            if not me:IsBeingCarriedBy("SCRIPT_NAME_HERO") then
                if predicateResult then return end
                quest:SetStateInt("CrateCount", quest:GetStateInt("CrateCount") - 1)
                quest:SetMasterGameState("OFBRCratesStolen", true)
                quest:RemoveThing(me, false, true)
                return
            end
            if predicateResult then
                return
            end
            if isActiveThreadTerminating then quest:NewScriptFrame(me); scratchValue = quest:IsActiveThreadTerminating(); goto continue_1 end
            if quest:IsActiveThreadTerminating() then return end
            isActiveThreadTerminating = true
        else
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00dcddcf(); return end
            isActiveThreadTerminating = false
        end
        quest:NewScriptFrame(me)
        scratchValue = quest:IsActiveThreadTerminating()
        ::continue_1::
    until false
end

-- Artefact.Init (retail 0x00dcfa10)
function Init(quest, me)
    quest:SetStateInt("CrateCount", quest:GetStateInt("CrateCount") + 1)
    quest:StateListPush("CrateList", me)
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
end

-- Artefact.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- Artefact.OnPredicateFail (retail 0x00dcf920)
function OnPredicateFail(quest, me)
    local p0 = 0
    if 0 == quest:GetStateListCount("CrateList") then return end
    while true do
        if quest:GetStateListAt("CrateList", p0):IsEqualTo(me) then break end
        p0 = p0 + 1
        if p0 == quest:GetStateListCount("CrateList") then
            return
        end
    end
    quest:StateListErase("CrateList", p0)
end

