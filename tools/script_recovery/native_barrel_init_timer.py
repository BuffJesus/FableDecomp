"""Restore Init timer global receiver after native operand recovery."""
import hashlib
import json
from pathlib import Path

def recover(source,data):
    witness=json.loads(Path(__file__).with_name('native_barrel_init_timer_witness.json').read_text())
    raw=data.bytes_at(witness['address'],witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=witness['sha256']:
        raise ValueError('Barrel Init native instructions changed')
    old='    quest:SetTimer(native_arg_barrel_watch_timer, 0)'
    if source.count(old)!=1:raise ValueError('Barrel Init timer correspondence changed')
    new='    quest:WithRetailResources(function(resources)\n        resources:ResetBarrelWatchTimer(native_arg_barrel_watch_timer)\n    end)'
    return source.replace(old,new,1),dict(witness,status='global-timer-restored')
