-- Generated native draft: Artefact. Review coverage report before use.
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
    local bVar3, bVar4
    local alive = true
    bVar4 = false
    local r1 = quest:GetThingWithScriptName("BanditTeamCrateDrop")
    alive = not quest:IsActiveThreadTerminating()
    local bVar2 = not alive
    repeat
        if bVar2 then
            r1 = nil
            return
        end
        bVar2 = quest:IsDistanceBetweenThingsUnder(me, r1, 3.0)
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                -- LAB_00dcddcf: (native jump target)
                r1 = nil
                return
            end
            -- TODO(native): IsBeingCarriedBy is not a ForgeFSE binding
            bVar2 = me:IsBeingCarriedBy("SCRIPT_NAME_HERO")
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar2 then
                if not bVar3 then
                    quest:SetStateInt("CrateCount", quest:GetStateInt("CrateCount") + -1)
                    quest:SetMasterGameState("OFBRCratesStolen", true)
                    quest:RemoveThing(me, false, true)
                end
                return
            end
            if bVar3 then return end
            if not bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end
                bVar4 = true
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then return end  -- TODO(native): goto LAB_00dcddcf
            bVar4 = false
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
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
    local bVar1, p0, v_stk_c
    p0 = 0
    if p0 ~= (quest:GetStateListCount("CrateList") * 0xc) then
        while true do
            v_stk_c = quest:GetStateListAt("CrateList", (p0) / 0xc)
            -- TODO(native): IsEqualTo is not a ForgeFSE binding
            bVar1 = quest:GetStateListAt("CrateList", (p0) / 0xc):IsEqualTo(me)
            if bVar1 then break end
            v_stk_c = nil
            p0 = p0 + 0xc
            if p0 == (quest:GetStateListCount("CrateList") * 0xc) then
                return
            end
        end
        quest:StateListErase("CrateList", (p0) / 0xc)
        v_stk_c = nil
    end
end

