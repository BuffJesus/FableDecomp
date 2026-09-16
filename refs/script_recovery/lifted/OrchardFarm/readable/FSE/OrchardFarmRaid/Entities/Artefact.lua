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
    local predicateResult, isDistanceBetweenThingsUnder, predicateResult2, isBeingCarriedBy
    local predicateResult3, predicateResult4, r1_1, r1_2
    local alive = true
    predicateResult4 = false
    r1_1 = quest:GetThingWithScriptName("BanditTeamCrateDrop")
    alive = not quest:IsActiveThreadTerminating()
    predicateResult = not alive
    repeat
        if predicateResult then
            r1_2 = nil
            -- LAB_00dcddfe: (native jump target)
            return
        end
        ::FLOW_after_lab_00dcddfe::
        isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, r1_1, 3.0)
        if isDistanceBetweenThingsUnder then
            alive = not quest:IsActiveThreadTerminating()
            predicateResult2 = not alive
            if predicateResult2 then
                -- LAB_00dcddcf: (native jump target)
                r1_2 = nil
                do return end
                goto FLOW_after_lab_00dcddfe
            end
            -- TODO(native): IsBeingCarriedBy is not a ForgeFSE binding
            isBeingCarriedBy = me:IsBeingCarriedBy("SCRIPT_NAME_HERO")
            alive = not quest:IsActiveThreadTerminating()
            predicateResult3 = not alive
            if not isBeingCarriedBy then
                if not predicateResult3 then
                    quest:SetStateInt("CrateCount", quest:GetStateInt("CrateCount") + -1)
                    quest:SetMasterGameState("OFBRCratesStolen", true)
                    quest:RemoveThing(me, false, true)
                end
                -- LAB_00dcddbf: (native jump target)
                return
            end
            ::FLOW_after_lab_00dcddbf::
            do return end
            if predicateResult3 then goto FLOW_after_lab_00dcddbf end
            if not predicateResult4 then
                alive = not quest:IsActiveThreadTerminating()
                predicateResult4 = not alive
                do return end
                if predicateResult4 then goto FLOW_after_lab_00dcddbf end
                predicateResult4 = true
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            predicateResult4 = not alive
            if predicateResult4 then return end  -- TODO(native): goto LAB_00dcddcf
            predicateResult4 = false
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        predicateResult = not alive
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

