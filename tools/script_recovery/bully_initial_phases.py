"""Native-backed initial home and nine controlled-health consumers; phase candidate."""
import hashlib
import json
import struct
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData,ROOT

SOURCE='''-- DISABLED PHASE CANDIDATE: not registered as actor Main.
-- Caller must own the native self-resource across this phase and continuation.
function WithBullyInitialControl(quest, me, continuation)
    quest:RegisterBoundConsciousCondition()
    quest:NewScriptFrame()
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        local control = resources:NewResource()
        resources:PrepareResource(control)
        while not resources:TryAcquire(control, me, 4) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        if not BullyReturnHomePhase(quest, me, resources, control) then return end
        -- Native DBB53C continues with a termination query and retained Victim.
        -- Caller must implement that next phase; this is not complete Main.
        continuation(resources, control)
    end)
end

function BullyReturnHomePhase(quest, me, resources, control)
    local home = me:GetHomePos()
    while true do
        local actor = resources:NewThingFromResource(control)
        local outside = resources:ThingIsDistanceFromPositionOver(actor, home, 2.0)
        resources:DestroyThing(actor)
        if not outside then return true end
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
        resources:MoveToPosition(control, home, 0.0, 0, false, true)
        while resources:IsPerformingScriptTask(control) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
    end
end

function BullyControlledHealthAboveThreshold(resources, control)
    local actor = resources:NewThingFromResource(control)
    local health = resources:ThingHealth(actor)
    local positive = health > __THRESHOLD__
    resources:DestroyThing(actor)
    return positive
end
'''

def recover(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('bully_initial_witness.json').read_text())
    raw=data.bytes_at(w['mainAddress'],w['mainSize'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=w['mainSha256']:raise ValueError('Bully native Main changed')
    for span in w['helperSpans']:
        raw=data.bytes_at(span['address'],span['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=span['sha256']:raise ValueError('Bully native helper changed')
    threshold=data.bytes_at(w['thresholdAddress'],4)
    if threshold.hex()!=w['thresholdHex']:raise ValueError('Bully health threshold changed')
    return SOURCE.replace('__THRESHOLD__',repr(struct.unpack('<f',threshold)[0])),w

if __name__=='__main__':
    source,w=recover();out=ROOT/'work/bully_converter';out.mkdir(exist_ok=True)
    (out/'INITIAL_PHASES.lua').write_text(source)
    (out/'INITIAL_EVIDENCE.json').write_text(json.dumps(w,indent=2)+'\n')
