-- Readable native conversion: FleeingWoman. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    WB_ScreamingVillagerDistance = 3652,  -- 30
    WB_ScreamingVillagerScreamsDistance = 3656,  -- 10
    WB_ScreamingVillagerFadeOutTime = 3660,  -- 2.5
}

-- per-entity fields (native class members; one Lua state per entity instance)
local screamedAtHero

-- FleeingWoman.Main (retail 0x00e0f4e0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, p0_00, fleeingWomanEscapePos
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00e0f616 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e0f616 end
    while not quest:IsDistanceBetweenThingsUnder(me, hero, quest:ReadGlobalGameData(SCRIPT_DEF.WB_ScreamingVillagerDistance)) do
        if not quest:NewScriptFrame(me) then goto LAB_00e0f616 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e0f616 end
    fleeingWomanEscapePos = quest:GetThingWithScriptName("FleeingWomanEscapePos")
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            resources:ReleaseResource(resource)
            return
        end
        if not screamedAtHero then
            if quest:IsDistanceBetweenThingsUnder(me, hero, quest:ReadGlobalGameData(SCRIPT_DEF.WB_ScreamingVillagerScreamsDistance)) then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                screamedAtHero = true
                local conversationID = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationID, hero)
                quest:AddLineToConversation(conversationID, "TEXT_QST_072_WOMAN_FLEES_10", me, hero, false)
            end
        end
        if not me:IsPerformingScriptTask() then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            if not (fleeingWomanEscapePos ~= nil and not fleeingWomanEscapePos:IsNull()) then
                p0_00 = {x = 0, y = 0, z = 0}
            else
                p0_00 = fleeingWomanEscapePos:GetPos()
            end
            me:MoveToPosition(p0_00, 1.0, ENTITY_MOVE_RUN, false, true)
        end
        if not quest:IsDistanceBetweenThingsUnder(me, fleeingWomanEscapePos, 2.0) then quest:NewScriptFrame(me); predicateResult = quest:IsActiveThreadTerminating(); goto continue_1 end
        if not quest:IsActiveThreadTerminating() then
            quest:FadeOutAndKillEntity(me, true, quest:ReadGlobalGameDataFloat(SCRIPT_DEF.WB_ScreamingVillagerFadeOutTime), true)
        end
        resources:ReleaseResource(resource)
        do return end
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
        ::continue_1::
    until false
    ::LAB_00e0f616::
    resources:ReleaseResource(resource)
end

-- FleeingWoman.Init (retail 0x00e0f4b0)
function Init(quest, me)
    screamedAtHero = false
end

-- FleeingWoman.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- FleeingWoman.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

