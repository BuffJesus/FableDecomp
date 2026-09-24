-- Readable native conversion: RockTrollTrigger. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TE_EarthTrollTriggerDistance = 3612,  -- 7
}

-- per-entity fields (native class members; one Lua state per entity instance)
local rockTrollSpawned

-- RockTrollTrigger.Main (retail 0x00e0ad40)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue
    if not quest:NewScriptFrame(me) then return end
    quest:ReadGlobalGameData(SCRIPT_DEF.TE_EarthTrollTriggerDistance)
    -- TODO(native): xStack_34 = CVar2;
    while not quest:IsDistanceBetweenThingsUnder(me, hero, scratchValue) do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if rockTrollSpawned then quest:RemoveThing(me, false, true); return end
    if quest:IsActiveThreadTerminating() then return end
    rockTrollSpawned = true
    if quest:GetStateBool("ShownEarthTroll") then quest:RemoveThing(me, false, true); return end
    if quest:IsActiveThreadTerminating() then return end
    local resource = resources:NewResource()
    resources:TryAcquire(resource, hero, 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    resources:RunMacro("CS_DARKWOOD_TRADER_TROLL", actorMap, false, true)
    quest:SetStateBool("ShownEarthTroll", true)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
    quest:RemoveThing(me, false, true)
end

-- RockTrollTrigger.Init (retail 0x00e03a20)
function Init(quest, me)
    rockTrollSpawned = false
end

-- RockTrollTrigger.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- RockTrollTrigger.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

