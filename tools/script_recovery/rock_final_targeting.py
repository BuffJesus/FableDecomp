"""Final targeting on bound Troll; fresh borrowed Hero output for each consumer."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData

SOURCE='''function RockTrollFinalTargetingPhase(quest, me)
    quest:TargetRockTrollAtHero(me)
    repeat
        quest:NewScriptFrame()
    until quest:IsActiveThreadTerminating()
end
'''


def recover(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('rock_final_targeting_witness.json').read_text())
    raw=data.bytes_at(0xec4fa9,91)
    if raw is None or hashlib.sha256(raw).hexdigest()!=w['sha256']:raise ValueError('Rock final targeting operands changed')
    return SOURCE,w
