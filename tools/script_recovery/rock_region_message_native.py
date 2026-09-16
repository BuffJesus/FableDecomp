"""Fail-closed evidence for one persistent region-message CString scope."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_root_recovery import recover


def verify(data=None):
    data=data or RData()
    _,root=recover(data)
    witness=json.loads(Path(__file__).with_name('rock_region_message_witness.json').read_text())
    for region in witness['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            raise ValueError('Region message native bytes changed: '+region['name'])
    return {'scope':witness,'root':root}
