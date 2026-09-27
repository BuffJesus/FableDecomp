-- Readable native conversion: Cham. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- Cham.Main (retail 0x00f14770)
function Main(quest, me)
    local resources = quest:RetailResources()
    local resource = resources:NewResource()
    quest:EntityUnsetAsOpinionSource(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    while not quest:GetStateBool("ChamLeaving") do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    me:MoveToPosition(quest:GetThingWithScriptName("GuardingDoorMarkerLeft"):GetPos(), 1.0, ENTITY_MOVE_RUN, false, true)
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:RemoveThing(me, false, true)
    end
    resources:ReleaseResource(resource)
end

-- Cham.Init (retail 0x00f14720)
function Init(quest, me)
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

-- Cham.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- Cham.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

