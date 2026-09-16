"""Explicit retained Victim and late Hero/Victim control scopes; disabled phases."""
from tools.script_recovery.bully_lifetime_inventory import inventory

SOURCE='''-- DISABLED: these scopes are not a complete Bully Main.
function WithBullyPausedMovie(resources, continuation)
    local movie = resources:StartMovie("")
    resources:Pause(true)
    continuation(movie)
    resources:Pause(false)
    resources:DestroyMovie(movie)
end

function RunBullyRecoveredEntry(quest, me, continuation)
    WithBullyInitialControl(quest, me, function(resources, selfControl)
        WithBullyRetainedVictim(quest, resources, function(victim)
            continuation(resources, selfControl, victim)
        end)
    end)
end

function WithBullyRetainedVictim(quest, resources, continuation)
    if quest:IsActiveThreadTerminating() then return end
    local victim = resources:NewThingFromScriptName("NOVI_Victim")
    if not quest:IsActiveThreadTerminating() then
        continuation(victim)
    end
    resources:DestroyThing(victim)
end

function WithBullyRunoffControls(quest, resources, victim, continuation)
    -- Enter at DBC8FA after the existing self-resource post-acquisition query.
    local heroControl = resources:NewResource()
    resources:PrepareResource(heroControl)
    local function acquireAndRun()
        while not resources:TryAcquire(heroControl, quest:GetHero(), 4) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        local victimControl = resources:NewResource()
        resources:PrepareResource(victimControl)
        local function acquireVictimAndRun()
            while not resources:TryAcquireThing(victimControl, victim, 4) do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            continuation(heroControl, victimControl)
        end
        acquireVictimAndRun()
        resources:ReleaseResource(victimControl)
    end
    acquireAndRun()
    resources:ReleaseResource(heroControl)
end
'''

def recover():
    report,*_=inventory()
    if not report['cfgLifetimeProved']:raise ValueError('Bully retained lifetime proof failed')
    return SOURCE,report
