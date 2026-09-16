"""Post-exhumation animations/task wait; actor Main remains incomplete."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData

SOURCE='''-- Called after exhumation/Hero cleanup, while the self resource object lives.
function WithRockTrollAnimationPhase(quest, resources, seh_me, continuation)
    resources:PlayRockTrollAnimation(seh_me, "SPECIAL_BOAST")
    resources:PlayRockTrollAnimation(seh_me, "SPECIAL_IDLE")
    while resources:IsPerformingScriptTask(seh_me) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    resources:PrepareResource(seh_me)
    continuation()
end
'''


def recover(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('rock_animation_witness.json').read_text())
    for region in w['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:raise ValueError('Rock animation native operands changed')
    for address,value in w['strings'].items():
        if data.string_at(int(address,16))!=value:raise ValueError('Rock animation literal changed')
    return SOURCE,w
