"""Retail EC4130 helper: fresh region queries and explicit cancellation checks."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.rock_root_recovery import recover as root_recover

BODY='''    while quest:IsRegionLoaded("Witchwood1") do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetCreatureGeneratorsEnabled("Witchwood1", true)
'''


def recover(data=None):
    data=data or RData();_,registration=root_recover(data)
    witness=json.loads(Path(__file__).with_name('rock_region_exit_witness.json').read_text())
    raw=data.bytes_at(0xec4130,176)
    if raw is None or hashlib.sha256(raw).hexdigest()!=witness['sha256']:
        raise ValueError('Rock region-exit native operand changed')
    if data.string_at(0x12cc268)!='Witchwood1':raise ValueError('Rock region-exit name changed')
    return BODY,witness
