"""Structured first/repeat Guard lectures recovered from DACB30..DAD868."""
from tools.script_recovery.guard_health import prove

SOURCE='''function GuardLecture(quest, resources, control)
    local function speak(key)
        if GuardControlledHealthPositive(resources, control) then
            resources:Speak(control, quest:GetHero(), key, 0, false, true, false)
            while resources:IsPerformingScriptTask(control) do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then return false end
            end
            if quest:IsActiveThreadTerminating() then return false end
        end
        return true
    end
    local repeated = quest:GetStateBool("GuardsSpokenOnce")
    if quest:IsActiveThreadTerminating() then return false end
    if repeated then
        if not speak("TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN") then return false end
    else
        if not speak("TEXT_QST_048_GUARD_CAUGHT_YOU_10") then return false end
        if not speak("TEXT_QST_048_GUARD_CAUGHT_YOU_20") then return false end
    end
    local crimes = {"BARREL_BREAKING", "DERELICTION_OF_DUTY", "VIOLENCE", "TEDDY_TO_BULLY", "CONCEALED_AFFAIR"}
    for index, crime in ipairs(crimes) do
        if quest:GetStateBool("WhichBadDeedsPerformed_" .. (index - 1)) then
            if quest:IsActiveThreadTerminating() then return false end
            if not speak("TEXT_QST_048_GUARD_CRIME_" .. crime) then return false end
        end
    end
    if repeated then
        if not speak("TEXT_QST_048_GUARD_AFTER_READ_LIST") then return false end
    else
        for _, suffix in ipairs({30, 40, 50, 60}) do
            if not speak("TEXT_QST_048_GUARD_CAUGHT_YOU_" .. suffix) then return false end
        end
        quest:SetStateBool("GuardsSpokenOnce", true)
    end
    return true
end
'''

def recover(data=None):
    witness=prove(data)
    return SOURCE,{'nativeMainSha256':witness['mainSha256'],'start':0xdacb30,'end':0xdad868,
        'cancellationDestinations':[0xdadd90,0xdadda5,0xdaddb8],
        'scope':'Caller retains resource and movie; all three native cancel destinations unpause and destroy movie before resource cleanup.',
        'limits':['Only lecture phase; actor dispatcher and movie lifecycle must be composed separately.']}
