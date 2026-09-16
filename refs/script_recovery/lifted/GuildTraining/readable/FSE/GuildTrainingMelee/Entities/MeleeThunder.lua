-- Reviewed native slice for Q_GuildTrainingMelee.MeleeThunder.
-- TutorialState transitions 4 -> 6 bracket two control-acquisition phases.

function Init(quest, me)
end

function Main(quest, me)
    local function waitForState(value)
        while quest:GetStateInt("TutorialState") ~= value do
            if not quest:NewScriptFrame(me) then return false end
            if quest:IsActiveThreadTerminating() then return false end
        end
        return true
    end
    local function acquire()
        while not me:AcquireControl(4) do
            if not quest:NewScriptFrame(me) then return false end
            if quest:IsActiveThreadTerminating() then return false end
        end
        return true
    end

    if not waitForState(4) then return end
    if not acquire() then return end
    while quest:GetStateInt("TutorialState") == 4 do
        if not quest:NewScriptFrame(me) then return end
        if quest:IsActiveThreadTerminating() then return end
    end
    if not waitForState(6) then return end
    if not acquire() then return end
    while quest:GetStateInt("TutorialState") == 6 do
        if not quest:NewScriptFrame(me) then return end
        if quest:IsActiveThreadTerminating() then return end
    end
    while quest:NewScriptFrame(me) do
        if quest:IsActiveThreadTerminating() then return end
    end
end
