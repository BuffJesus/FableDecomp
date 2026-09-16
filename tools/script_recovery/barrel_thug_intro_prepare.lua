-- Native DB6CFB..DB6E20. Main owns control; the intro movie begins next.
local function prepareBarrelThugIntroduction(quest, me, resources, control)
    if quest:IsActiveThreadTerminating() then return false end
    resources:PrepareResource(control)
    while not resources:TryAcquire(control, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return false end
    end
    if quest:IsActiveThreadTerminating() then return false end
    while not quest:GetStateBool("BarrelManLeftHeroInCharge") do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return false end
    end
    if quest:IsActiveThreadTerminating() then return false end
    resources:PlaceBarrelThugAtStart(me)
    quest:Pause(3.0)
    return true
end

return prepareBarrelThugIntroduction
