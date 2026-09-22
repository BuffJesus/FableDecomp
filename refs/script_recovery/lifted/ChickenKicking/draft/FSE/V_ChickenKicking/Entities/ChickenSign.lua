-- Generated native draft: ChickenSign. Review coverage report before use.
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
    local bVar3
    local alive = true
    quest:SetThingAsUsable(me, true)
    local pCVar5 = (__native_entity_state:GetStateInt("self_0x1c") + quest:GetStateInt("PrizesWon") * 4)
    local pCVar4 = quest:GetThingWithScriptName("ChickenSign")
    quest:SetReadableObjectText(pCVar4, pCVar5)
    pCVar4 = nil
    local iVar2 = quest:GetStateInt("PrizesWon")
    while true do
        if iVar2 == 7 then
            alive = not quest:IsActiveThreadTerminating()
            return
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then break end
        if iVar2 ~= quest:GetStateInt("PrizesWon") then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            pCVar5 = (__native_entity_state:GetStateInt("self_0x1c") + quest:GetStateInt("PrizesWon") * 4)
            pCVar4 = quest:GetThingWithScriptName("ChickenSign")
            quest:SetReadableObjectText(pCVar4, pCVar5)
            pCVar4 = nil
            iVar2 = quest:GetStateInt("PrizesWon")
        end
    end
end

function Init(quest, me)
    -- TODO(native): this_00 = (vector<std::pair<CCharString,long>,std::allocator<std::pair<CCharString,long>_>_> *) (this + 0x1c);
    -- TODO(native): p1 = CCharString::CCharString(&xStack_4);
    -- TODO(native): std::vector<std::pair<CCharString,long>,std::allocator<std::pair<CCharString,long>_>_>::resize(this_00,8,p1);
    -- TODO(native): CCharString__AssignFromWide(*(void **)this_00,0x12e8044);
    -- TODO(native): CCharString__AssignFromWide((void *)(*(int *)this_00 + 4),0x12e800c);
    -- TODO(native): CCharString__AssignFromWide((void *)(*(int *)this_00 + 8),0x12e7fd8);
    -- TODO(native): CCharString__AssignFromWide((void *)(*(int *)this_00 + 0xc),0x12e7f98);
    -- TODO(native): CCharString__AssignFromWide((void *)(*(int *)this_00 + 0x10),0x12e7f64);
    -- TODO(native): CCharString__AssignFromWide((void *)(*(int *)this_00 + 0x14),0x12e7f24);
    -- TODO(native): CCharString__AssignFromWide((void *)(*(int *)this_00 + 0x18),0x12e7ee8);
    -- TODO(native): CCharString__AssignFromWide((void *)(*(int *)this_00 + 0x1c),0x12e7ea0);
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

