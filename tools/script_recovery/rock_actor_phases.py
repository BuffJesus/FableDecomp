"""Explicit continuation phases, not a replacement or stub for actor Main."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData

SOURCE='''-- Partial native phases only. No Main is defined or registered here.
-- continuation must contain the omitted native body while these locals live.
function WithRockTrollSelfPhase(quest, me, continuation)
    quest:CreateRetainedThingThread("WatchForRockTrollKilled", me, false, "")
    quest:WithRetailResources(function(resources)
        local seh_me = resources:NewResource()
        resources:PrepareResource(seh_me)
        while not resources:TryAcquire(seh_me, me, 4) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        continuation(resources, seh_me)
    end)
end

-- Called only from the native not-PlayedExhumeCutScene branch, after item work.
-- resources is the outer self phase's owner; errors unwind that outer scope.
function WithRockTrollHeroPhase(quest, resources, continuation)
    if quest:IsActiveThreadTerminating() then return end
    local seh_hero = resources:NewResource()
    resources:PrepareResource(seh_hero)
    while not resources:TryAcquire(seh_hero, quest:GetHero(), 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(seh_hero)
            return
        end
    end
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(seh_hero)
        return
    end
    continuation(seh_hero)
    resources:ReleaseResource(seh_hero)
end
'''


def recover(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('rock_actor_phases_witness.json').read_text())
    for region in w['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:raise ValueError('Rock actor phase operands changed')
    return SOURCE,w
