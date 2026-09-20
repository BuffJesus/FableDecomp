-- Readable native conversion: WillWhisper. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- WillWhisper.Main (retail 0x00d68810)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d68acf end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d68acf end
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAllowBossPhaseChanges(me, false)
    while quest:GetStateBool("BanditsAlive") do
        if not quest:NewScriptFrame(me) then goto LAB_00d68acf end
        if me:MsgIsHitByHero() then
            goto LAB_00d689df
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(me) then goto LAB_00d689df end
            end
            predicateResult = false
        end
        goto FLOW_past_lab_00d689df
        ::LAB_00d689df::
        predicateResult = true
        ::FLOW_past_lab_00d689df::
        if predicateResult then
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
    resources:ReleaseResource(resource)
end

-- WillWhisper.Init (retail 0x00d687d0)
function Init(quest, me)
end

-- WillWhisper.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- WillWhisper.OnPredicateFail (retail 0x00d687e0)
function OnPredicateFail(quest, me)
end

