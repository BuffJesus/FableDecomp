"""Native binding/vtable/source/byte proof for the complete Sparrow Main."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.rock_sparrow_program import BODY


def recover(data=None):
    data=data or RData()
    witness=json.loads(Path(__file__).with_name('rock_sparrow_witness.json').read_text())
    for region in witness['regions']:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:raise ValueError('Sparrow native operands changed')
    for item in witness['sources']:
        if hashlib.sha256((ROOT/item['path']).read_bytes()).hexdigest()!=item['sha256']:raise ValueError('Sparrow source correspondence changed')
    if data.string_at(0x12f165c)!='RTFE_Sparrow':raise ValueError('Sparrow binding name changed')
    return BODY,witness
