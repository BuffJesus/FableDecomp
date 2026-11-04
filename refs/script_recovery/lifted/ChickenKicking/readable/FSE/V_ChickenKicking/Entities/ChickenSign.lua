-- Readable native conversion: ChickenSign. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CChickenSign::TextKeys (+0x1c): filled once by native Init from .rdata literals, never written again
local TextKeys = {"TEXT_QST_B17_SIGN_WON_NOTHING", "TEXT_QST_B17_SIGN_WON_EXPR", "TEXT_QST_B17_SIGN_WON_KEY", "TEXT_QST_B17_SIGN_WON_KEY_EXPR", "TEXT_QST_B17_SIGN_WON_HAT", "TEXT_QST_B17_SIGN_WON_HAT_EXPR", "TEXT_QST_B17_SIGN_WON_HAT_KEY", "TEXT_QST_B17_SIGN_WON_HAT_KEY_EXPR"}

-- ChickenSign.Main (retail 0x00e64df0)
function Main(quest, me)
    quest:SetThingAsUsable(me, true)
    local scratchValue = TextKeys[quest:GetStateInt("PrizesWon") + 1]
    local chickenSign = quest:GetThingWithScriptName("ChickenSign")
    quest:SetReadableObjectText(chickenSign, scratchValue)
    chickenSign = nil
    local prizesWon = quest:GetStateInt("PrizesWon")
    while true do
        if prizesWon == 7 then
            quest:IsActiveThreadTerminating()
            return
        end
        if not quest:NewScriptFrame(me) then break end
        if prizesWon ~= quest:GetStateInt("PrizesWon") then
            if quest:IsActiveThreadTerminating() then return end
            scratchValue = TextKeys[quest:GetStateInt("PrizesWon") + 1]
            chickenSign = quest:GetThingWithScriptName("ChickenSign")
            quest:SetReadableObjectText(chickenSign, scratchValue)
            chickenSign = nil
            prizesWon = quest:GetStateInt("PrizesWon")
        end
    end
end

-- ChickenSign.Init (retail 0x00e68dc0)
function Init(quest, me)
end

-- ChickenSign.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- ChickenSign.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

