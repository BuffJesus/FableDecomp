-- Readable native conversion: TraderComment. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TE_TraderCommentDistance = 3600,  -- 8
}

local helpers = require("TraderEscort.native_quest_helpers")

-- TraderComment.Main (retail 0x00e05360)
function Main(quest, me)
    local predicateResult, ctr_28, scratchValue3
    if not quest:NewScriptFrame(me) then return end
    local darkwoodTrader = quest:GetAllThingsWithScriptName("DarkwoodTrader")
    local scratchValue4 = quest:ReadGlobalGameData(SCRIPT_DEF.TE_TraderCommentDistance)
    predicateResult = quest:IsActiveThreadTerminating()
    while true do
        if predicateResult then break end
        ctr_28 = 0
        if #darkwoodTrader ~= 0 then
            scratchValue3 = 0
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00e05536 end
                local scratchValue = darkwoodTrader[scratchValue3 + 1]:IsAlive()
                local sequence = scratchValue and quest:IsDistanceBetweenThingsUnder(me, darkwoodTrader[scratchValue3 + 1], scratchValue4)
                if not sequence then ctr_28 = ctr_28 + 1; scratchValue3 = scratchValue3 + 1; goto continue_1 end
                if quest:IsActiveThreadTerminating() then return end
                if helpers.MakeTraderComment(quest, me, me:GetDataString(), darkwoodTrader[scratchValue3 + 1], 1) then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:RemoveThing(me, false, true)
                    return
                end
                ctr_28 = ctr_28 + 1
                scratchValue3 = scratchValue3 + 1
                ::continue_1::
            until ctr_28 >= #darkwoodTrader
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e0557f end
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
    end
    ::LAB_00e05596::
    do return end
    ::LAB_00e05536::
    goto LAB_00e05596
    ::LAB_00e0557f::
    goto LAB_00e05596
end

-- TraderComment.Init (retail 0x00e02d40)
function Init(quest, me)
end

-- TraderComment.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TraderComment.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

