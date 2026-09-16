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
            -- LAB_00dcddfe: (native jump target)
            -- TODO(native): auStack_1c._4_4_ = 0;
            return
        end
        bVar2 = quest:IsDistanceBetweenThingsUnder(me, r1, 3.0)
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                -- LAB_00dcddcf: (native jump target)
                if true then return end  -- TODO(native): goto LAB_00dcddfe
                -- TODO(native): goto LAB_00dcddf6
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
                -- LAB_00dcddbf: (native jump target)
                return
            end
            if bVar3 then return end  -- TODO(native): goto LAB_00dcddbf
            if not bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00dcddbf
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
    local cVar1
    local p0 = *QUESTLIST_Begin("CrateList")
    if p0 ~= QUESTLIST_End("CrateList") then
        while true do
            -- TODO(native): local_4 = *(int **)(p0 + 8);
            -- TODO(native): local_8 = *(int **)(p0 + 4);
            if local_4 ~= nil then
                -- TODO(native): *local_4 = *local_4 + 1;
            end
            cVar1 = quest:GetFurthestWithScriptName(nil --[[missing]], nil --[[missing]])
            if cVar1 ~= 0 then break end
            p0 = p0 + 0xc
            if p0 == QUESTLIST_End("CrateList") then
                return
            end
        end
        -- TODO(native): std__vector__pop_back((void *)(*(int *)(this + 0x14) + 0x54),p0);
    end
end

