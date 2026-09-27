-- Readable native conversion: HornetDrone. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local eatNow

-- HornetDrone.Main (retail 0x00e11dc0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    me:MoveToThing(quest:GetNearestWithScriptName(me, "Q_WB_DeadBody"), 0.30000001192092896, ENTITY_MOVE_RUN)
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    me:PlayAnimation("ST_FEED_INTO", false, false, true, false, true, false, false)
    me:PlayLoopingAnimation("ST_FEED_LOOP", -1, false, false, true)
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    while true do
        if me:IsAwareOfHero() then break end
        if me:MsgIsHitByHero() then
            goto LAB_00e1203a
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e1203a end
            end
            predicateResult = false
        end
        goto FLOW_past_lab_00e1203a
        ::LAB_00e1203a::
        predicateResult = true
        ::FLOW_past_lab_00e1203a::
        if predicateResult then goto LAB_00e12130 end
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if not quest:IsActiveThreadTerminating() then
        me:ClearCommands()
        me:ClearAllActionsIncludingLoopingAnimations()
        me:PlayAnimation("ST_FEED_OUTOF", false, false, true, true, true, false, false)
        while me:IsPerformingScriptTask() do
            if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        end
        goto LAB_00e12130
    end
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00e12130::
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    quest:GiveThingBestEnemyTarget(me, quest:GetHero())
    resources:PrepareResource(resource)
    resources:ReleaseResource(resource)
end

-- HornetDrone.Init (retail 0x00e11d80)
function Init(quest, me)
    eatNow = math.random(0, 32767) % 200 + 1
end

-- HornetDrone.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- HornetDrone.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

