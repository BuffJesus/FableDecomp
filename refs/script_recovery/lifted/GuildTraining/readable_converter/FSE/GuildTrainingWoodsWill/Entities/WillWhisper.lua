-- Readable native conversion: WillWhisper. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- WillWhisper.Main (retail 0x00d68810)
function Main(quest, me)
    if not quest:NewScriptFrame(me) then return end
    if not me:AcquireControl(4) then goto LAB_00d68acf end
    if quest:IsActiveThreadTerminating() then goto LAB_00d68acf end
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAllowBossPhaseChanges(me, false)
    while quest:GetStateBool("BanditsAlive") do
        if not quest:NewScriptFrame(me) then goto LAB_00d68acf end
        if me:MsgIsHitByHero() or me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(me) then
            quest:EntitySetInFaction(me, "FACTION_HERO")
            me:SetFriendsWithEverythingFlag(me)
        end
        if quest:GetStateBool("WhisperAnimate") then
            if quest:IsActiveThreadTerminating() then goto LAB_00d68acf end
            quest:SetStateBool("WhisperAnimate", false)
            me:PlayAnimation("WILL_CAST_FORCE_SPELL_DELIVER_LEVEL_1", false, false, false, true, true, false, false)
        end
    end
    ::LAB_00d68acf::
    me:ReleaseControl()
end

-- WillWhisper.Init (retail 0x00d687d0)
function Init(quest, me)
end

-- WillWhisper.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- WillWhisper.OnPredicateFail (retail 0x00d687e0)
function OnPredicateFail(quest, me)
end

