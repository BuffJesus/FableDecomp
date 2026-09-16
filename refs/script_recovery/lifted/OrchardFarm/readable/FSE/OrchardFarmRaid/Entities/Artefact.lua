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
    local predicateResult, predicateResult2, predicateResult3
    local alive = true
    predicateResult2 = false
    local banditTeamCrateDrop = quest:GetThingWithScriptName("BanditTeamCrateDrop")
    alive = not quest:IsActiveThreadTerminating()
    local scratchValue = not alive
    repeat
        if scratchValue then
            return
        end
        scratchValue = quest:IsDistanceBetweenThingsUnder(me, banditTeamCrateDrop, 3.0)
        if scratchValue then
            alive = not quest:IsActiveThreadTerminating()
            scratchValue = not alive
            if scratchValue then
                -- LAB_00dcddcf: (native jump target)
                return
            end
            -- TODO(native): IsBeingCarriedBy is not a ForgeFSE binding
            scratchValue = me:IsBeingCarriedBy("SCRIPT_NAME_HERO")
            alive = not quest:IsActiveThreadTerminating()
            predicateResult = not alive
            if not scratchValue then
                if not predicateResult then
                    quest:SetStateInt("CrateCount", quest:GetStateInt("CrateCount") + -1)
                    quest:SetMasterGameState("OFBRCratesStolen", true)
                    quest:RemoveThing(me, false, true)
                end
                return
            end
            if predicateResult then return end
            if not predicateResult2 then
                if quest:IsActiveThreadTerminating() then return end
                predicateResult2 = true
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            predicateResult3 = not alive
            if predicateResult3 then return end  -- TODO(native): goto LAB_00dcddcf
            predicateResult2 = false
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        scratchValue = not alive
    until false
end

function Init(quest, me)
    quest:SetStateInt("CrateCount", quest:GetStateInt("CrateCount") + 1)
    -- TODO(native): CStack_4 = (CCharString)this;
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
            -- TODO(native): IsEqualTo is not a ForgeFSE binding
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

