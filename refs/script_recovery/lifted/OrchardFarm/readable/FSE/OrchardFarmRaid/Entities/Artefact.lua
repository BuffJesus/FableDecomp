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
    local predicateResult, scratchValue
    local alive = true
    local function __cleanup_LAB_00dcddcf()
        scratchValue2 = nil
    end
    scratchValue = false
    local scratchValue2 = quest:GetThingWithScriptName("BanditTeamCrateDrop")
    alive = not quest:IsActiveThreadTerminating()
    local scratchValue3 = not alive
    repeat
        if scratchValue3 then
            scratchValue2 = nil
            -- LAB_00dcddfe: (native jump target)
            return
        end
        scratchValue3 = quest:IsDistanceBetweenThingsUnder(me, scratchValue2, 3.0)
        if scratchValue3 then
            alive = not quest:IsActiveThreadTerminating()
            scratchValue3 = not alive
            if scratchValue3 then
                __cleanup_LAB_00dcddcf()
                return
            end
            scratchValue3 = me:IsBeingCarriedBy("SCRIPT_NAME_HERO")
            alive = not quest:IsActiveThreadTerminating()
            predicateResult = not alive
            if not scratchValue3 then
                if not predicateResult then
                    quest:SetStateInt("CrateCount", quest:GetStateInt("CrateCount") + -1)
                    quest:SetMasterGameState("OFBRCratesStolen", true)
                    quest:RemoveThing(me, false, true)
                end
                -- LAB_00dcddbf: (native jump target)
                return
            end
            if predicateResult then
                return
            end
            if not scratchValue then
                scratchValue = quest:IsActiveThreadTerminating()
                if scratchValue then
                    return
                end
                scratchValue = true
            end
        else
            scratchValue = quest:IsActiveThreadTerminating()
            if scratchValue then __cleanup_LAB_00dcddcf(); return end
            scratchValue = false
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        scratchValue3 = not alive
    until false
end

function Init(quest, me)
    quest:SetStateInt("CrateCount", quest:GetStateInt("CrateCount") + 1)
    -- TODO(native): xStack_4 = (CCharString)this;
    quest:StateListPush("CrateList", me)
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
    local getStateListAt, p0
    p0 = 0
    if p0 ~= quest:GetStateListCount("CrateList") then
        while true do
            getStateListAt = quest:GetStateListAt("CrateList", p0):IsEqualTo(me)
            if getStateListAt then break end
            p0 = p0 + 1
            if p0 == quest:GetStateListCount("CrateList") then
                return
            end
        end
        quest:StateListErase("CrateList", p0)
    end
end

