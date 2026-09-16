"""Pinned retail Init and empty predicate hook; no invented parent defaults."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData

SOURCE='''function Init(quest, me)
    quest:InitializeRockTrollMarker(me)
end

function OnPredicateFail(quest, me)
end
'''

def recover(data=None):
    data=data or RData()
    witness=json.loads(Path(__file__).with_name('rock_lifecycle_witness.json').read_text())
    for span in witness['spans']:
        raw=data.bytes_at(span['address'],span['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=span['sha256']:
            raise ValueError('Rock lifecycle native operands changed')
    if data.string_at(witness['iconAddress'])!=witness['icon']:
        raise ValueError('Rock marker literal changed')
    return SOURCE,witness
