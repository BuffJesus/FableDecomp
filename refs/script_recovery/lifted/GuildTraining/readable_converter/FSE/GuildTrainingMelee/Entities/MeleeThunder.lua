-- Readable native conversion: MeleeThunder. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- MeleeThunder.Main (retail 0x00d58080)
function Main(quest, me)
    while quest:GetStateInt("TutorialState") ~= 4 do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            me:ReleaseControl()
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d582ed end
    if not me:AcquireControl(4) then goto LAB_00d582ed end
    if quest:IsActiveThreadTerminating() then goto LAB_00d582ed end
    while quest:GetStateInt("TutorialState") == 4 do
        if not quest:NewScriptFrame(me) then goto LAB_00d582ed end
    end
    while quest:GetStateInt("TutorialState") ~= 6 do
        if not quest:NewScriptFrame(me) then goto LAB_00d58297 end
    end
    if not quest:IsActiveThreadTerminating() then
        if not me:AcquireControl(4) then goto LAB_00d58297 end
        if not quest:IsActiveThreadTerminating() then
            while quest:GetStateInt("TutorialState") == 6 do
                if not quest:NewScriptFrame(me) then goto LAB_00d58297 end
            end
            if not quest:IsActiveThreadTerminating() then
                repeat
                    quest:NewScriptFrame(me)
                until quest:IsActiveThreadTerminating()
            end
        end
    end
    ::LAB_00d58297::
    me:ReleaseControl()
    do return end
    ::LAB_00d582ed::
    me:ReleaseControl()
end

-- MeleeThunder.Init (retail 0x00d58050)
function Init(quest, me)
end

-- MeleeThunder.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- MeleeThunder.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

