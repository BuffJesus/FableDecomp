-- Readable native conversion: FleshEatingBalverine. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- FleshEatingBalverine.Main (retail 0x00e02470)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, predicateResult8, predicateResult9, balverineDinner
    if not quest:NewScriptFrame(me) then return end
    while not quest:GetStateBool("ShownBalverine") do
        if not quest:NewScriptFrame(me) then return end
    end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00e0285e end
    end
    predicateResult9 = quest:IsActiveThreadTerminating()
    if predicateResult9 then goto LAB_00e0285e end
    balverineDinner = quest:GetThingWithScriptName("BalverineDinner")
    predicateResult8 = predicateResult9
    repeat
        if not quest:NewScriptFrame(me) then goto LAB_00e02855 end
        if not predicateResult9 then
            if not me:IsPerformingScriptTask() then
                me:PlayAnimation("ST_MUNCH", false, false, false, true, true, false, false)
                quest:EntityPlayObjectAnimation(balverineDinner, "EATEN_BY_BALVERINE", false)
            end
            if quest:IsDistanceBetweenThingsUnder(me, quest:GetHero(), 25.0) then
                if quest:IsActiveThreadTerminating() then goto LAB_00e02855 end
                predicateResult9 = true
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00e02855 end
                end
                predicateResult8 = true
            end
        end
        if me:MsgIsHitByHero() then
            goto LAB_00e02760
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e02760 end
            end
            predicateResult = false
        end
        goto FLOW_past_lab_00e02760
        ::LAB_00e02760::
        predicateResult = true
        ::FLOW_past_lab_00e02760::
        if predicateResult then
            if quest:IsActiveThreadTerminating() then goto LAB_00e02855 end
            break
        end
    until predicateResult8 ~= false
    if quest:IsActiveThreadTerminating() then goto LAB_00e02855 end
    me:PlayCombatAnimation("LEAP_STRAIGHT_UP", true, false)
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then goto LAB_00e02855 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e02855 end
    quest:SetStateInt("BalverinesToSurpriseHeroNeeded", quest:GetStateInt("BalverinesToSurpriseHeroNeeded") + 1)
    quest:RemoveThing(me, false, true)
    ::LAB_00e02855::
    ::LAB_00e0285e::
    resources:ReleaseResource(resource)
end

-- FleshEatingBalverine.Init (retail 0x00e02440)
function Init(quest, me)
end

-- FleshEatingBalverine.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- FleshEatingBalverine.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

