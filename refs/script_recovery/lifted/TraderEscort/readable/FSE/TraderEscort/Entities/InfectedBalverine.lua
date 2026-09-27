-- Readable native conversion: InfectedBalverine. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TE_BalverineTimeToGoDistance = 3596,  -- 25
}

-- InfectedBalverine.Main (retail 0x00e02950)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, predicateResult7
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    predicateResult7 = quest:IsActiveThreadTerminating()
    if predicateResult7 then resources:ReleaseResource(resource); return end
    local v_stk_24_2 = quest:ReadGlobalGameData(SCRIPT_DEF.TE_BalverineTimeToGoDistance)
    repeat
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        if not me:IsPerformingScriptTask() then
            me:PlayLoopingAnimation("ST_MUNCH", -1, false, false, false)
        end
        if quest:IsDistanceBetweenThingsUnder(me, quest:GetHero(), v_stk_24_2) then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            me:ClearAllActionsIncludingLoopingAnimations()
            predicateResult7 = true
        end
        if me:MsgIsHitByHero() then
            goto LAB_00e02b66
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e02b66 end
            end
            predicateResult = false
        end
        goto FLOW_past_lab_00e02b66
        ::LAB_00e02b66::
        predicateResult = true
        ::FLOW_past_lab_00e02b66::
        if predicateResult then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            predicateResult7 = true
        end
        if quest:GetStateBool("MissionFailed") then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            break
        end
    until predicateResult7
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    quest:SetStateBool("InfectedTraderCanGetUp", true)
    me:PlayCombatAnimation("LEAP_STRAIGHT_UP", true, false)
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:RemoveThing(me, false, true)
    end
    resources:ReleaseResource(resource)
end

-- InfectedBalverine.Init (retail 0x00e02920)
function Init(quest, me)
end

-- InfectedBalverine.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- InfectedBalverine.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

