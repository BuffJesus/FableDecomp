-- Readable native conversion: ChickenSign. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- per-entity fields (native class members; one Lua state per entity instance)
local self0X1C

-- ChickenSign.Main (retail 0x00e64df0)
function Main(quest, me)
    quest:SetThingAsUsable(me, true)
    quest:SetReadableObjectText(quest:GetThingWithScriptName("ChickenSign"), self0X1C + quest:GetStateInt("PrizesWon") * 4)
    local prizesWon = quest:GetStateInt("PrizesWon")
    while true do
        if prizesWon == 7 then
            return
        end
        if not quest:NewScriptFrame(me) then break end
        if prizesWon ~= quest:GetStateInt("PrizesWon") then
            quest:SetReadableObjectText(quest:GetThingWithScriptName("ChickenSign"), self0X1C + quest:GetStateInt("PrizesWon") * 4)
            prizesWon = quest:GetStateInt("PrizesWon")
        end
    end
end

-- ChickenSign.Init (retail 0x00e68dc0)
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

-- ChickenSign.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- ChickenSign.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

