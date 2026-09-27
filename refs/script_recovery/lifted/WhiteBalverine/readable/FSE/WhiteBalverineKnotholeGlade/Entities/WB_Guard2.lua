-- Readable native conversion: WB_Guard2. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_PAUSED = 1  -- ECutsceneBehaviour (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- WB_Guard2.Main (retail 0x00e17320)
function Main(quest, me)
    local resources = quest:RetailResources()
    local getMasterGameState, p0_00
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 3) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    quest:EntitySetCutsceneBehaviour(me, CUTSCENE_BEHAVIOUR_PAUSED)
    local guard2Pos = quest:GetThingWithScriptName("WB_Guard2Pos")
    if guard2Pos == nil then
        p0_00 = {x = 0, y = 0, z = 0}
    else
        p0_00 = guard2Pos:GetPos()
    end
    me:MoveToPosition(p0_00, 3.0, ENTITY_MOVE_RUN, false, true)
    while not quest:IsDistanceBetweenThingsUnder(me, guard2Pos, 5.0) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    getMasterGameState = quest:GetMasterGameState("WhiteBalverineFinished")
    repeat
        if getMasterGameState then
            if not quest:IsActiveThreadTerminating() then
                quest:RemoveThing(me, false, true)
            end
            resources:ReleaseResource(resource)
            return
        end
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        if quest:IsDistanceBetweenThingsUnder(me, quest:GetThingWithScriptName("WB_WhiteBalverine"), 15.0) then
            local position = quest:GetHero():GetPos()
            local scratchValue2 = {x = position.x, y = position.y, z = position.z}
            while true do
                local scratchValue = resources:ScriptThing(resource)
                local outsideDistance = scratchValue ~= nil and scratchValue:IsDistanceFromPositionOver(scratchValue2, 2.0)
                if not outsideDistance then break end
                if not quest:NewScriptFrame(me) then goto LAB_00e17623 end
                me:MoveToPosition(scratchValue2, 0, ENTITY_MOVE_RUN, false, true)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        resources:ReleaseResource(resource)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00e17623 end
            end
            if not quest:IsActiveThreadTerminating() then goto LAB_00e17639 end
            ::LAB_00e17623::
            resources:ReleaseResource(resource)
            return
        end
        ::LAB_00e17639::
        getMasterGameState = quest:GetMasterGameState("WhiteBalverineFinished")
    until false
    resources:ReleaseResource(resource)
end

-- WB_Guard2.Init (retail 0x00e172e0)
function Init(quest, me)
end

-- WB_Guard2.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- WB_Guard2.OnPredicateFail (retail 0x00e172f0)
function OnPredicateFail(quest, me)
end

